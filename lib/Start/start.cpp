#include <Arduino.h>
#include "config.h"
#include "motors.h"

enum initialMoveState {
    INITIAL_MOVE_RIGHT,
    INITIAL_MOVE_RIGHT_FORWARD,
    INITIAL_MOVE_RIGHT_BACKWARD,
    INITIAL_MOVE_LEFT,
    INITIAL_MOVE_LEFT_FORWARD,
    INITIAL_MOVE_LEFT_BACKWARD,
};

static initialMoveState initialMove = INITIAL_MOVE_RIGHT; // Default initial move

void setInitialMove(uint16_t remoteCommand) {
    switch (remoteCommand) {
        case REMOTE_CMD_RIGHT:
            initialMove = INITIAL_MOVE_RIGHT;
            break;
        case REMOTE_CMD_UP_RIGHT:
            initialMove = INITIAL_MOVE_RIGHT_FORWARD;
            break;
        case REMOTE_CMD_DOWN_RIGHT:
            initialMove = INITIAL_MOVE_RIGHT_BACKWARD;
            break;
        case REMOTE_CMD_LEFT:
            initialMove = INITIAL_MOVE_LEFT;
            break;
        case REMOTE_CMD_UP_LEFT:
            initialMove = INITIAL_MOVE_LEFT_FORWARD;
            break;
        case REMOTE_CMD_DOWN_LEFT:
            initialMove = INITIAL_MOVE_LEFT_BACKWARD;
            break;
    }
}

int execInitialMove() {
    switch (initialMove) {
        case INITIAL_MOVE_RIGHT:
            oppositeDirection(255); // Move right
            return INIT_TURN_TIME;
        case INITIAL_MOVE_RIGHT_FORWARD:
            oppositeDirection(255); // Move right and forward
            return INIT_FORWARD_TURN_TIME;
        case INITIAL_MOVE_RIGHT_BACKWARD:
            oppositeDirection(255); // Move right and backward
            return INIT_BACKWARD_TURN_TIME;
        case INITIAL_MOVE_LEFT:
            oppositeDirection(-255); // Move left
            return INIT_TURN_TIME;
        case INITIAL_MOVE_LEFT_FORWARD:
            oppositeDirection(-255); // Move left and forward
            return INIT_FORWARD_TURN_TIME;
        case INITIAL_MOVE_LEFT_BACKWARD:
            oppositeDirection(-255); // Move left and backward
            return INIT_BACKWARD_TURN_TIME;
    }
}
