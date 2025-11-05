#include "InitialMove.h"

#include "Config.h"
#include "../../../Hardware/Motors/MotorController/MotorController.h"
#include "Battle/BattleEngine/BattleEngine.h"

static initialMoveState initialMove = initialMoveState::INITIAL_MOVE_RIGHT; // Movimiento inicial default

struct InitialMoveInfo {
    int motorSpeed;
    int moveTime;
};

static const InitialMoveInfo moveMap[] = {
    /* [INITIAL_MOVE_RIGHT] */            {255,  INIT_TURN_TIME},
    /* [INITIAL_MOVE_RIGHT_FORWARD] */    {255,  INIT_FORWARD_TURN_TIME},
    /* [INITIAL_MOVE_RIGHT_BACKWARD] */   {255,  INIT_BACKWARD_TURN_TIME},
    /* [INITIAL_MOVE_LEFT] */             {-255, INIT_TURN_TIME},
    /* [INITIAL_MOVE_LEFT_FORWARD] */     {-255, INIT_FORWARD_TURN_TIME},
    /* [INITIAL_MOVE_LEFT_BACKWARD] */    {-255, INIT_BACKWARD_TURN_TIME}
};

static_assert(
    (sizeof(moveMap) / sizeof(moveMap[0])) == static_cast<size_t>(initialMoveState::_Count),
    "initialMoveState enum and moveMap array are out of sync!"
);

static inline InitialMoveInfo getInfoForCurrentMove() {
    return moveMap[static_cast<size_t>(initialMove)];
}

void resetInitialMove(){
    initialMove = initialMoveState::INITIAL_MOVE_RIGHT;
}

int getInitialMoveTime()
{
    InitialMoveInfo info = getInfoForCurrentMove();
    return info.moveTime;
}

void setInitialMove(uint16_t remoteCommand) {
    switch (remoteCommand) {
        case REMOTE_CMD_RIGHT:
            initialMove = initialMoveState::INITIAL_MOVE_RIGHT;
            break;
        case REMOTE_CMD_UP_RIGHT:
            initialMove = initialMoveState::INITIAL_MOVE_RIGHT_FORWARD;
            break;
        case REMOTE_CMD_DOWN_RIGHT:
            initialMove = initialMoveState::INITIAL_MOVE_RIGHT_BACKWARD;
            break;
        case REMOTE_CMD_LEFT:
            initialMove = initialMoveState::INITIAL_MOVE_LEFT;
            break;
        case REMOTE_CMD_UP_LEFT:
            initialMove = initialMoveState::INITIAL_MOVE_LEFT_FORWARD;
            break;
        case REMOTE_CMD_DOWN_LEFT:
            initialMove = initialMoveState::INITIAL_MOVE_LEFT_BACKWARD;
            break;
    }
}

void execInitialMove()
{
    IMotorController* motors = getActiveMotorController();
    if (motors) {
        InitialMoveInfo info = getInfoForCurrentMove();
        motors->oppositeDirection(info.motorSpeed);
    }
}
