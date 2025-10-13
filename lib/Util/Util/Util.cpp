#include "Util.h"
#include <Arduino.h>

#include "Config.h"
#include "Sensors/Sensors.h"

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

/**
 * @brief Reads all sensor values and prints them to the serial monitor in a formatted table.
 * This function provides a detailed, human-readable output of all sensor states,
 * including proximity sensors, line-following sensors, and raw analog noise levels.
 * The output is padded to ensure consistent alignment.
 */
void printSensorReadout(const SensorData &readings) {

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