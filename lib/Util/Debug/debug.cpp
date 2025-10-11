#include <Arduino.h>
#include "config.h"
#include "Sensors/sensors.h"
#include "debug.h"
#include "Motors/MotorController/MotorController.h"
#include "../Core/Battle/BattleEngine/BattleEngine.h"

//================================================================================
// Global Variables
//================================================================================

// Toggles for enabling/disabling specific debug functionalities via serial commands.
bool g_enableSensorReadout = false;
bool g_enableControlTest = false;
bool g_enableStrategyDebug = false;

//================================================================================
// Helper Functions
//================================================================================

/**
 * @brief Prints a value to the serial monitor, padded with leading spaces.
 * This ensures that the output columns are aligned, regardless of the number
 * of digits in the value.
 * @param value The integer value to print.
 * @param width The total width of the field.
 */
void printPaddedValue(int value, int width) {
    int temp = value;
    int digits = 1;
    if (temp < 0) {
        temp = -temp;
        digits++; // Account for the '-' sign
    }
    while (temp >= 10) {
        temp /= 10;
        digits++;
    }

    // Print leading spaces
    for (int i = 0; i < (width - digits); i++) {
        Serial.print(" ");
    }
    
    Serial.print(value);
}

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
 * @brief Reads all sensor values and prints them to the serial monitor in a formatted table.
 * This function provides a detailed, human-readable output of all sensor states,
 * including proximity sensors, line-following sensors, and raw analog noise levels.
 * The output is padded to ensure consistent alignment.
 */
void printSensorReadout(SensorData readings) {

    // Print formatted sensor data to the serial monitor
    Serial.print("Sensores: L["); printPaddedValue(readings.left, 3);   Serial.print("] ");
    Serial.print("C[");           printPaddedValue(readings.center, 3); Serial.print("] ");
    Serial.print("R[");           printPaddedValue(readings.right, 3);  Serial.print("] | ");
    
    Serial.print("Linea: L["); printPaddedValue(readings.lineLeft, 4);  Serial.print("] ");
    Serial.print("R[");        printPaddedValue(readings.lineRight, 4); Serial.print("] | ");

    // Raw analog values can be useful for calibrating and checking for noise
    Serial.print("Ruido: L["); printPaddedValue(analogRead(LEFT_SENSOR_PIN), 3);   Serial.print("] ");
    Serial.print("C[");        printPaddedValue(analogRead(CENTER_SENSOR_PIN), 3); Serial.print("] ");
    Serial.print("R[");        printPaddedValue(analogRead(RIGHT_SENSOR_PIN), 3);  Serial.println("]");
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
