#include "config.h"
#include <Arduino.h>
#include <QTRSensors.h>

#include "sensors.h"

// --- Constants ---
// Define the minimum time in microseconds that must pass before reading the sensors again.
// This replaces the blocking delay and ensures the IR LED duty cycle is respected.
#define SENSOR_COOLDOWN_US ((IR_MICROS_DELAY / IR_DUTY_CYCLE) - (IR_MICROS_DELAY * IR_LED_COUNT))

// --- Global Variables ---
QTRSensors linea;
uint16_t valoresLinea[2];

/**
 * @brief Pulses an IR LED and reads the corresponding phototransistor.
 *
 * @param sensor The analog pin of the phototransistor.
 * @param led The digital pin of the IR LED.
 * @return The denoised sensor reading.
 */
int pulse(int sensor, int led) {
    // Read ambient noise before turning on the LED
    int noise = analogRead(sensor);
    
    // Turn on the LED, wait for the phototransistor to react
    digitalWrite(led, HIGH);
    delayMicroseconds(IR_MICROS_DELAY);
    
    // Take the reading with the LED on
    int reading = analogRead(sensor);
    
    // Turn the LED off
    digitalWrite(led, LOW);
    
    // Subtract the ambient noise from the reading
    int denoised = reading - noise;
    return denoised;
}

/**
 * @brief Sets up the sensor pins and the QTR library.
 */
void sensorSetup() {
    // Set pin modes for the front-facing proximity sensors
    pinMode(RIGHT_SENSOR_PIN, INPUT);
    pinMode(CENTER_SENSOR_PIN, INPUT);
    pinMode(LEFT_SENSOR_PIN, INPUT);

    // Set pin modes for the IR LEDs
    pinMode(RIGHT_LED_PIN, OUTPUT);
    pinMode(CENTER_LED_PIN, OUTPUT);
    pinMode(LEFT_LED_PIN, OUTPUT);

    // Configure the QTR library for the line following sensors
    linea.setTypeRC();
    linea.setSensorPins((const uint8_t[]){LEFT_LINE_PIN, RIGHT_LINE_PIN}, 2);
}

/**
 * @brief Reads all robot sensors using a non-blocking cooldown.
 *
 * If called before the SENSOR_COOLDOWN_US period has passed, this function
 * will return the previously cached sensor data. Otherwise, it will
 * perform a new read and update the cache.
 *
 * @return A SensorData struct containing all the latest sensor values.
 */
SensorData readAllSensors() {
    // Static variables persist their values between function calls.
    // We use them to store the last read time and the last read values.
    static unsigned long lastReadTimestamp = 0;
    static SensorData lastValues;

    unsigned long currentTime = micros();

    // Check if the cooldown period has passed since the last successful read.
    // The check also handles the initial run where lastReadTimestamp is 0.
    if (currentTime - lastReadTimestamp >= SENSOR_COOLDOWN_US) {
        // Enough time has passed, so perform a new sensor read.
        
        // Update the timestamp to the current time.
        lastReadTimestamp = currentTime;

        // Read the front-facing proximity sensors.
        lastValues.right = pulse(RIGHT_SENSOR_PIN, RIGHT_LED_PIN);
        lastValues.center = pulse(CENTER_SENSOR_PIN, CENTER_LED_PIN);
        lastValues.left = pulse(LEFT_SENSOR_PIN, LEFT_LED_PIN);

        // Read the line-following sensors.
        linea.read(valoresLinea);
        lastValues.lineLeft = valoresLinea[0];
        lastValues.lineRight = valoresLinea[1];
    }

    // Return the last-read values (either the new ones or the cached ones).
    return lastValues;
}
