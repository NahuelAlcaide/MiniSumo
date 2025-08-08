#include "sensors.h"
#include "motors.h"
#include <Arduino.h> // For millis()

void searchLoop(SensorData data) {
    if (data.lineLeft > 100 || data.lineRight > 100) {
        brake();
    } else {
        // Fixed-rate zig-zag motion
        static unsigned long lastSwitch = millis();
        static int turnDir = 1; // 1 for right, -1 for left
        unsigned long interval = 800; // ms, duration for each side
        if (millis() - lastSwitch > interval) {
            turnDir *= -1;
            lastSwitch = millis();
        }
        float turn = 0.1f * turnDir; // Fixed turn rate
        sameDirection(40, turn);
    }
}