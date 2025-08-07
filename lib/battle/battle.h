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

#endif // BATTLE_H