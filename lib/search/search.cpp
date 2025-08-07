#include "sensors.h"
#include "motors.h"
#include <Arduino.h> // For millis(), sin(), pow()

void searchLoop() {
    SensorData data = readAllSensors();
    if (data.lineLeft > 100 || data.lineRight > 100) {
        brake();
    } else {
        // Skewed S-curve motion using sine wave
        static unsigned long startTime = millis();
        float elapsed = (millis() - startTime) / 1000.0; // seconds
        float frequency = 0.2; // Hz
        float raw = sin(2 * PI * frequency * elapsed); // [-1, 1]
        float skew = 3.0; // Higher value = more time at extremes
        float turn = pow(abs(raw), skew) * (raw >= 0 ? 1 : -1); // Skewed to extremes
        sameDirection(80, turn); // 80 is a slowish speed
    }
}