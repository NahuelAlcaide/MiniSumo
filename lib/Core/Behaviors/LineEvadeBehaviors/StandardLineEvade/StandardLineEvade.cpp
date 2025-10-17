#include "StandardLineEvade.h"
#include <Arduino.h>

#include "../Hardware/Sensors/Sensors.h"
#include "Config.h"

// --- State-Tracking Variables ---
static unsigned long phaseStartTime = 0;
static StandardLineEvade::EvadeState evadeState = StandardLineEvade::IDLE;
// Stores the direction of the initial hit (-1 left, 1 right, 0 both)
static int saved_direction = 0;
static StandardLineEvade::Status g_status = StandardLineEvade::COMPLETED;

StandardLineEvade::StandardLineEvade(IMotorController* motorController) :
    m_motorController(motorController) // Initialize the member variable
{}

StandardLineEvade::Status StandardLineEvade::execute(SensorData data) {

    switch (evadeState) {
        case IDLE: {
            int current_direction = 0;
            if (data.lineLeft < LINE_EVADE_THRESHOLD) { current_direction -= 1; }
            if (data.lineRight < LINE_EVADE_THRESHOLD) { current_direction += 1; }

            // If a line is detected, start the maneuver and set status to RUNNING
            if (data.lineLeft < LINE_EVADE_THRESHOLD || data.lineRight < LINE_EVADE_THRESHOLD) {
                saved_direction = current_direction;
                evadeState = NORMAL_REVERSING;
                phaseStartTime = millis();
                g_status = RUNNING;
            } else {
                g_status = COMPLETED; // Not in a maneuver, so status is COMPLETED
            }
            break;
        }

        case NORMAL_REVERSING: {
            // Reverse straight for a fixed duration
            m_motorController->sameDirection(-LINE_EVADE_REVERSE_SPEED, 0);

            if (millis() - phaseStartTime > LINE_EVADE_REVERSE_TIME) {
                evadeState = TURNING;
                phaseStartTime = millis();
            }
            // g_status remains RUNNING
            break;
        }

        case TURNING: {
            // Turn away from the initially detected line for a fixed duration
            int turnValue = -saved_direction * 255;
            m_motorController->oppositeDirection(turnValue);

            if (millis() - phaseStartTime > LINE_EVADE_TURN_TIME) {
                m_motorController->limpMotors(); // Stop motors before completing
                evadeState = IDLE;
                g_status = COMPLETED; // Now the maneuver is fully complete
            }
            // g_status remains RUNNING
            break;
        }

        default: {
            // Fallback to a safe state
            m_motorController->limpMotors();
            evadeState = IDLE;
            g_status = COMPLETED;
            break;
        }
    }

    return g_status;
}

