#include "Debug.h"
#include <Arduino.h>

// Include dependencies for the private methods
#include "Config.h"
#include "Sensors/Sensors.h"
#include "Battle/BattleEngine/BattleEngine.h" // For setBattleDebugMode
#include "Util/Util.h" // For INVERT_LINE



// Constructor to initialize state
Debug::Debug() :
    m_sensorReadoutEnabled(false),
    m_controlTestEnabled(false),
    m_cleanWheelsEnabled(false){
}

void Debug::update(uint16_t remoteCommand, IMotorController* motors) {
    // 1. Process commands to update state
    processRemoteCommands(remoteCommand, motors);

    // 2. Run active debug routines based on state
    if (m_sensorReadoutEnabled) {
        printSensorReadout(readAllSensors()); // Assumes printSensorReadout is available
    }

    if (m_controlTestEnabled) {
        controlTest(motors);
    }

    if (m_cleanWheelsEnabled) {
        cleanWheels(motors);
    }
}

void Debug::reset() {
    m_sensorReadoutEnabled = false;
    m_controlTestEnabled = false;
    m_cleanWheelsEnabled = false;
    Serial.println("Debug flags cleared.");
}

// --- Private Methods ---

void Debug::processRemoteCommands(uint16_t remoteCommand, IMotorController* motors) {
    switch (remoteCommand)
    {
        case REMOTE_CMD_SENSOR_READOUT:
            {
                m_sensorReadoutEnabled = !m_sensorReadoutEnabled;
                Serial.print("Sensor readout is now ");
                Serial.println(m_sensorReadoutEnabled ? "ON" : "OFF");
                break;
            }
        case REMOTE_CMD_CONTROL_TEST:
            {
                bool prevState = m_controlTestEnabled;
                m_controlTestEnabled = !m_controlTestEnabled;
                Serial.print("Control test is now ");
                Serial.println(m_controlTestEnabled ? "ON" : "OFF");

                if (prevState && !m_controlTestEnabled) {
                    // Was ON, is now OFF. Stop motors.
                    stopMotors(motors);
                }
                break;
            }
        case REMOTE_CMD_INVERT_LINE:
            {
                invertLineSensors();
                Serial.print("Line sensors inversion is now ");
                Serial.println(INVERT_LINE ? "ON" : "OFF");
                break;
            }
        case REMOTE_CMD_STRATEGY_DEBUG:
            {
                bool newState = !getBattleDebugMode();
                setBattleDebugMode(newState);
                Serial.print("Strategy debug mode is now ");
                Serial.println(newState ? "ON" : "OFF");
                break;
            }
        case REMOTE_CMD_CLEAN_WHEELS:
            {
                bool prevState = m_cleanWheelsEnabled;
                m_cleanWheelsEnabled = !m_cleanWheelsEnabled;
                Serial.print("Clean wheels is now ");
                Serial.println(m_cleanWheelsEnabled ? "ON" : "OFF");

                if (prevState && !m_cleanWheelsEnabled) {
                    // Was ON, is now OFF. Stop motors.
                    stopMotors(motors);
                }
                break;
            }
    }
}

void Debug::controlTest(IMotorController* motors) {
    SensorData data = readAllSensors();
    if (data.center < 70) {
        int diff = static_cast<int>((data.right - data.left) * (90.0 / 1000.0));
        motors->oppositeDirection(diff);
    } else {
        motors->brake();
    }
}

void Debug::stopMotors(IMotorController* motors) {
    motors->limpMotors();
}

void Debug::invertLineSensors() {
    INVERT_LINE = !INVERT_LINE; // Assumes INVERT_LINE is a global config
}

void Debug::cleanWheels(IMotorController* motors)
{
    motors->sameDirection(50, 0);
}