
#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "motors.h"
#include "battle.h"

void seekLoop(SensorData data) {
    static int lastDir = 0; // -1 for left, 1 for right, 0 for none
    static unsigned long lastSeenTime = 0;
    const int threshold = SEEK_THRESHOLD;
    const unsigned long holdTimeout = 600; // ms to keep turning after losing signal

    // Always set state hold at start
    g_battleStateHold = BATTLE_STATE_HOLD_SEEK;

    // Check sensors
    if (data.left > threshold) {
        lastDir = -1;
        lastSeenTime = millis();
    } else if (data.right > threshold) {
        lastDir = 1;
        lastSeenTime = millis();
    }

    // Continue turning in last direction if still seeing or within timeout
    if (lastDir != 0 && (millis() - lastSeenTime < holdTimeout)) {
        float turn = 0.2f * lastDir;
        sameDirection(40, turn);
    } else {
        g_battleStateHold = BATTLE_STATE_HOLD_NONE;
    }
}