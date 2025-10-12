#include "StandardLineEvade.h"
#include <Arduino.h>

#include "../Hardware/Sensors/Sensors.h"
#include "Config.h"

// --- State-Tracking Variables ---
static unsigned long phaseStartTime = 0;
static standardLineEvade::EvadeState evadeState = standardLineEvade::IDLE;
// Stores the direction of the initial hit (-1 left, 1 right, 0 both)
static int saved_direction = 0;
static standardLineEvade::Status g_status = standardLineEvade::COMPLETED;

standardLineEvade::standardLineEvade(IMotorController* motorController) :
    m_motorController(motorController) // Initialize the member variable
{}

standardLineEvade::Status standardLineEvade::execute(SensorData data) {

    g_status = RUNNING;

    // --- 1. Trigger Detection (Only when IDLE) ---
    if (evadeState == IDLE) {
        int current_direction = 0;
        if (data.lineLeft < LINE_EVADE_THRESHOLD) { current_direction -= 1; }
        if (data.lineRight < LINE_EVADE_THRESHOLD) { current_direction += 1; }

        if (current_direction != 0 || (data.lineLeft < LINE_EVADE_THRESHOLD && data.lineRight < LINE_EVADE_THRESHOLD)) {
            saved_direction = current_direction;
            evadeState = INITIAL_BRAKING;
            phaseStartTime = millis(); // Start timer for the first phase
        }
    }

    // --- Action Sequence based on State ---

    // 2. Initial Brake (Fixed Duration)
    if (evadeState == INITIAL_BRAKING) {
        m_motorController->brake();
        if (millis() - phaseStartTime > LINE_EVADE_START_BRAKE_TIME) {
            evadeState = TURNING;       // Transition to turning
            phaseStartTime = millis();  // Reset timer for the turning phase
        }
    }
    // 3. Turning (With Panic Timeout)
    else if (evadeState == TURNING) {
        // First, check for panic condition: stuck turning for too long
        if (millis() - phaseStartTime > LINE_EVADE_PANIC_TIMER) {
            evadeState = PANIC_REVERSING; // <-- Trigger the panic state
            phaseStartTime = millis();    // Reset timer for the panic reverse
            return g_status; // Exit and start panic reverse on the next loop
        }

        // Read sensors to see if we're still on the line
        int current_direction = 0;
        if (data.lineLeft < LINE_EVADE_THRESHOLD) { current_direction -= 1; }
        if (data.lineRight < LINE_EVADE_THRESHOLD) { current_direction += 1; }

        if (current_direction != 0) { // Still on the line, so keep turning
            float turnValue = -current_direction * LINE_EVADE_TURN_RATE;
            m_motorController->sameDirection(-LINE_EVADE_TURN_SPEED, -turnValue);
        } else { // Successfully got off the line
            evadeState = NORMAL_REVERSING; // Proceed to normal reverse
            phaseStartTime = millis();     // Reset timer for the reverse phase
        }
    }
    // 4. Normal Reverse (After a successful turn)
    else if (evadeState == NORMAL_REVERSING) {
        float reverseTurnValue = -saved_direction * LINE_EVADE_REVERSE_TURN_RATE;
        m_motorController->sameDirection(-LINE_EVADE_REVERSE_SPEED, -reverseTurnValue);

        if (millis() - phaseStartTime > LINE_EVADE_REVERSE_TIME) {
            evadeState = END_BRAKING;   // Proceed to the final brake
            phaseStartTime = millis();  // Reset timer for the brake
        }
    }
    // 5. Panic Reverse (NEW STATE)
    else if (evadeState == PANIC_REVERSING) {
        m_motorController->sameDirection(-255, 0); // Full blast reverse

        // Check if we are finally off the line
        bool onLine = (data.lineLeft < LINE_EVADE_THRESHOLD || data.lineRight < LINE_EVADE_THRESHOLD);
        if (onLine) {
            // As long as we see the line, keep resetting the timer for the *next* phase
            phaseStartTime = millis();
        }

        // Once we are off the line, the timer starts counting.
        // We end the maneuver after reversing for the specified panic time.
        if (!onLine && (millis() - phaseStartTime > LINE_EVADE_PANIC_REVERSE_TIME)) {
            evadeState = IDLE; // Panic maneuver is over, go back to idle
            g_status = COMPLETED;
        }
    }
    // 6. Final Brake (Only after a normal reverse)
    else if (evadeState == END_BRAKING) {
        m_motorController->brake();
        if (millis() - phaseStartTime > LINE_EVADE_END_BRAKE_TIME) {
            evadeState = IDLE; // Maneuver complete, reset to idle
            g_status = COMPLETED;
        }
    }
    // 7. Idle
    else {
        g_status = COMPLETED;
    }

    return g_status;
}