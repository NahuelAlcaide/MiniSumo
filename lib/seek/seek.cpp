
#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "motors.h"
#include "battle.h"

enum lastDirection {
    NONE,
    RIGHT,
    CENTER,
    LEFT,
};

static lastDirection lastDir = NONE;

void seekLoop(SensorData data) {
    static unsigned long lastSeenTime = 0;

    // Always set state hold at start
    g_battleStateHold = BATTLE_STATE_HOLD_SEEK;

    // Check sensors
    if (data.center > SEEK_THRESHOLD) {
        lastDir = CENTER;
        lastSeenTime = millis();
    }else if (data.left > SEEK_THRESHOLD) {
        lastDir = LEFT;
        lastSeenTime = millis();
    } else if (data.right > SEEK_THRESHOLD) {
        lastDir = RIGHT;
        lastSeenTime = millis();
    }

    // Continue turning in last direction if still seeing or within timeout
    if (lastDir != NONE && (millis() - lastSeenTime < SEEK_TURN_TIMEOUT)) {
        switch(lastDir) {
            case RIGHT:
                sameDirection(SEEK_SPEED, SEEK_TURN_RATE);
                break;
            case CENTER:
                sameDirection(SEEK_SPEED, 0);
                break;
            case LEFT:
                sameDirection(SEEK_SPEED, -SEEK_TURN_RATE);
                break;
        }
    } else {
        lastDir = NONE; // Reset last direction if no signal
        g_battleStateHold = BATTLE_STATE_HOLD_NONE;
    }
}