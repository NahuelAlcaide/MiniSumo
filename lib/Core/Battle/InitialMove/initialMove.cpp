#include <Arduino.h>
#include "config.h"
#include "../../../Hardware/Motors/MotorController/MotorController.h"
#include "Battle/BattleEngine/BattleEngine.h"

enum initialMoveState {
    INITIAL_MOVE_RIGHT,
    INITIAL_MOVE_RIGHT_FORWARD,
    INITIAL_MOVE_RIGHT_BACKWARD,
    INITIAL_MOVE_LEFT,
    INITIAL_MOVE_LEFT_FORWARD,
    INITIAL_MOVE_LEFT_BACKWARD,
};

static initialMoveState initialMove = INITIAL_MOVE_RIGHT; // Movimiento inicial default

void resetInitialMove()
{
    initialMove = INITIAL_MOVE_RIGHT;
}

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

int execInitialMove()
{
    IMotorController* motors = getActiveMotorController();
    if (motors) {
        switch (initialMove) {
        case INITIAL_MOVE_RIGHT:
            motors->oppositeDirection(255); // Giro de 90ª hacia la derecha
            return INIT_TURN_TIME;
        case INITIAL_MOVE_RIGHT_FORWARD:
            motors->oppositeDirection(255); // Giro de 45ª hacia la derecha
            return INIT_FORWARD_TURN_TIME;
        case INITIAL_MOVE_RIGHT_BACKWARD:
            motors->oppositeDirection(255); // Giro de 135ª hacia la derecha
            return INIT_BACKWARD_TURN_TIME;
        case INITIAL_MOVE_LEFT:
            motors->oppositeDirection(-255); // Giro de 90ª hacia la izquierda
            return INIT_TURN_TIME;
        case INITIAL_MOVE_LEFT_FORWARD:
            motors->oppositeDirection(-255); // Giro de 45ª hacia la izquierda
            return INIT_FORWARD_TURN_TIME;
        case INITIAL_MOVE_LEFT_BACKWARD:
            motors->oppositeDirection(-255); // Giro de 135ª hacia la izquierda
            return INIT_BACKWARD_TURN_TIME;
        }
    }
}
