#pragma once
#include "Sensors/Sensors.h"

enum battleInitState {
    BATTLE_INIT_STATE_IDLE,
    BATTLE_INIT_STATE_COUNTDOWN,
    BATTLE_INIT_STATE_INITIAL_MOVE,
    BATTLE_INIT_STATE_BATTLE
};

void resetBattleInitState();

bool execBattleInit(SensorData data);