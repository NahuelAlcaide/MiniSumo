#pragma once
#include <Arduino.h>

enum mainMenuState {
    STATE_DEFAULT,
    STATE_BATTLE_MENU,
    STATE_DEBUG_MENU,
    STATE_STRATEGY_MENU
};

void mainMenu(uint16_t remoteCommand);

void setMainMenuState(mainMenuState state);