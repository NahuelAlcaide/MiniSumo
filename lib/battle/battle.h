#ifndef BATTLE_H
#define BATTLE_H
#include <Arduino.h>

enum battleMenuState {
    BATTLE_MENU_DEFAULT_STATE,
    BATTLE_MENU_CONFIG_STATE,
    BATTLE_MENU_START_STATE,
};

void battleModeLoop(uint16_t remoteCommand);
void clearBattleFlags();
battleMenuState getBattleMenuState();

enum battleStateHold {
    BATTLE_STATE_HOLD_NONE,
    //BATTLE_STATE_HOLD_BLIND_SEARCH,
    BATTLE_STATE_HOLD_SEEK,
    BATTLE_STATE_HOLD_LINE_EVADE,
    BATTLE_STATE_HOLD_ATTACK
};

extern battleStateHold g_battleStateHold;

#endif // BATTLE_H