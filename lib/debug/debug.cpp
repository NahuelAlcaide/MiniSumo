#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "debug.h"
#include "motors.h"
#include "search.h"

//================================================================================
// Global Variables
//================================================================================

// Toggles for enabling/disabling specific debug functionalities via serial commands.
bool g_enableSensorReadout = false;
bool g_enableControlTest = false;

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

/**
 * @brief Reads all sensor values and prints them to the serial monitor in a formatted table.
 * This function provides a detailed, human-readable output of all sensor states,
 * including proximity sensors, line-following sensors, and raw analog noise levels.
 * The output is padded to ensure consistent alignment.
 */
void printSensorReadout() {
    // Read all sensor data into a struct
    SensorData readings = readAllSensors();

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
 * @brief Processes incoming serial commands to control the robot for debugging.
 *
 * This function acts as a simple command-line interface over the serial port.
 * It allows for direct motor control and toggling of debug printouts.
 *
 * Supported Commands:
 * - "SD,<power>,<turn>" : Drive motors in the Same Direction.
 * - <power>: Integer, motor power (e.g., 0-255).
 * - <turn>: Float, turn factor (e.g., -1.0 to 1.0).
 * - "OD,<power>"        : Drive motors in the Opposite Direction (for turning in place).
 * - <power>: Integer, motor power.
 * - "DB"                : Toggles the continuous sensor data readout.
 * - "CT"                : Toggles the control system test.
 * - "STOP"              : Stops all motor movement immediately.
 */
void processSerialCommands() {
    // Proceed only if there is data available to read from the serial buffer
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove any leading/trailing whitespace

        // --- Command: Same Direction Drive ---
        if (input.startsWith("SD,")) {
            String params = input.substring(3); // Get the part of the string after "SD,"
            int commaIndex = params.indexOf(',');

            if (commaIndex != -1) {
                String powerStr = params.substring(0, commaIndex);
                String turnStr = params.substring(commaIndex + 1);
                
                int power = powerStr.toInt();
                float turn = turnStr.toFloat();
                
                Serial.print("Executing SameDirection: Power=");
                Serial.print(power);
                Serial.print(", Turn=");
                Serial.println(turn);
                
                sameDirection(power, turn);
            } else {
                Serial.println("Error: Invalid format for SD. Use: SD,<power>,<turn>");
            }
        } 
        // --- Command: Opposite Direction Drive ---
        else if (input.startsWith("OD,")) {
            String param = input.substring(3); // Get the part of the string after "OD,"
            int power = param.toInt();

            Serial.print("Executing OppositeDirection: Power=");
            Serial.println(power);

            oppositeDirection(power);
        }
        // --- Command: Toggle Debug Readout ---
        else if (input.equalsIgnoreCase("DB")) {
            g_enableSensorReadout = !g_enableSensorReadout;
            Serial.print("Sensor readout is now ");
            Serial.println(g_enableSensorReadout ? "ON" : "OFF");
        }
        // --- Command: Toggle Control Test ---
        else if (input.equalsIgnoreCase("CT")) {
            g_enableControlTest = !g_enableControlTest;
            Serial.print("Control test is now ");
            Serial.println(g_enableControlTest ? "ON" : "OFF");
        }
        // --- Unknown Command ---
        else {
            Serial.println("Error: Unknown command. Use: SD, OD, DB, CT, STOP");
        }
    }
}

/**
 * @brief Main debug loop function, to be called repeatedly in loop().
 *
 * This function processes incoming serial commands and runs any active
 * debug routines (like sensor readouts) based on the toggle flags.
 */
void debug() {
    processSerialCommands();

    if (g_enableSensorReadout) {
        printSensorReadout();
    }

    if (g_enableControlTest) {
        controlTest();
    }
}
