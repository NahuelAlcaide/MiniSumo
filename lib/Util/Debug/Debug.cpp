#include <Arduino.h>
#include "Config.h"
#include "Sensors/Sensors.h"
#include "Debug.h"
#include "Motors/MotorController/MotorController.h"
#include "Battle/BattleEngine/BattleEngine.h"
#include "Util/Util.h"

//================================================================================
// Global Variables
//================================================================================

// Toggles for enabling/disabling specific debug functionalities via serial commands.
bool g_enableSensorReadout = false;
bool g_enableControlTest = false;
bool g_enableStrategyDebug = false;

//================================================================================
// Debug Functions
//================================================================================

void controlTest(IMotorController* motors){
    SensorData data = readAllSensors();
    if (data.center < 70){
        int diff = static_cast<int>((data.right - data.left) * (90.0 / 1000.0));
        motors->oppositeDirection(diff);
    } else {
        motors->brake();
    }
}

void controlTestStop(IMotorController* motors) {
    motors->limpMotors();
}

void invertLineSensors() {
    INVERT_LINE = !INVERT_LINE;
}

/**
 * @brief Processes incoming remote commands to control the robot for debugging.
 *
 * This function toggles debug modes based on remote control input.
 *
 * Supported Remote Commands:
 * - REMOTE_CMD_SENSOR_READOUT : Toggles the continuous sensor data readout.
 * - REMOTE_CMD_CONTROL_TEST   : Toggles the control system test.
 */
static void processRemoteCommands(uint16_t remoteData, IMotorController* motors) {
    if (remoteData == REMOTE_CMD_SENSOR_READOUT) {
        g_enableSensorReadout = !g_enableSensorReadout;
        Serial.print("Sensor readout is now ");
        Serial.println(g_enableSensorReadout ? "ON" : "OFF");
        // else: can handle other non-debug actions here if needed
    }
    if (remoteData == REMOTE_CMD_CONTROL_TEST) {
        bool prevState = g_enableControlTest;
        g_enableControlTest = !g_enableControlTest;
        Serial.print("Control test is now ");
        Serial.println(g_enableControlTest ? "ON" : "OFF");
        if (prevState && !g_enableControlTest) {
            // Control test was ON and is now OFF, stop motors
            controlTestStop(motors);
        }
        // else: can handle other non-debug actions here if needed
    }
    if (remoteData == REMOTE_CMD_INVERT_LINE) {
        invertLineSensors();
        Serial.print("Line sensors inversion is now ");
        Serial.println(INVERT_LINE ? "ON" : "OFF");
    }
    if (remoteData == REMOTE_CMD_STRATEGY_DEBUG)
    {
        if (g_enableStrategyDebug)
        {
            setBattleDebugMode(false);
            Serial.println("Strategy debug mode is now OFF");
        } else
        {
            setBattleDebugMode(true);
            Serial.println("Strategy debug mode is now ON");
        }
    }
}

/**
 * @brief Main debug loop function, to be called repeatedly in loop().
 *
 * This function processes incoming remote commands and runs any active
 * debug routines (like sensor readouts) based on the toggle flags.
 */
void debug(uint16_t remoteCommand, IMotorController* motors) {
    processRemoteCommands(remoteCommand, motors);

    if (g_enableSensorReadout) {
        printSensorReadout(readAllSensors());
    }

    if (g_enableControlTest) {
        controlTest(motors);
    }
}

void clearDebugFlags() {
    g_enableSensorReadout = false;
    g_enableControlTest = false;
    Serial.println("Debug flags cleared.");
}
