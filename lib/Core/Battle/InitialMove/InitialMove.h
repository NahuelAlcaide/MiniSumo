#pragma once

#include <Arduino.h>
#include "Config.h"

enum class initialMoveState {
    INITIAL_MOVE_RIGHT,
    INITIAL_MOVE_RIGHT_FORWARD,
    INITIAL_MOVE_RIGHT_BACKWARD,
    INITIAL_MOVE_LEFT,
    INITIAL_MOVE_LEFT_FORWARD,
    INITIAL_MOVE_LEFT_BACKWARD,

    _Count
};

void execInitialMove();

int getInitialMoveTime();

void setInitialMove(uint16_t remoteCommand);

void resetInitialMove();
