#include <Arduino.h>
#include "sensors.h"
#include "motors.h"
#include "config.h"

void searchLoop(SensorData data) {
    static unsigned long lastSwitch = millis();
    static int turnDir = 1; // 1 for right, -1 for left
    if (millis() - lastSwitch > SEARCH_TURN_INTERVAL) { // ms, duration for each side
        turnDir *= -1;
        lastSwitch = millis();
    }
    float turn = SEARCH_TURN_FACTOR * turnDir; // Fixed turn rate
    sameDirection(SEARCH_SPEED, turn);
}