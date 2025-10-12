#pragma once
#include <arduino.h>

enum battleMenuState {
    BATTLE_MENU_DEFAULT_STATE,
    BATTLE_MENU_CONFIG_STATE,
    BATTLE_MENU_START_STATE,
};

battleMenuState getBattleMenuState();

void execBattleMenu(uint16_t remoteCommand);
void resetBattleMenuState();