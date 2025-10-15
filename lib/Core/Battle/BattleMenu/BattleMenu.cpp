#include "BattleMenu.h"
#include <stdint.h>
#include "Config.h"
#include "Battle/InitialMove/InitialMove.h"
#include "Led/Led.h"

static battleMenuState g_battleMenuState = BATTLE_MENU_DEFAULT_STATE;

battleMenuState getBattleMenuState() {
    return g_battleMenuState;
}

void resetBattleMenuState() {
    g_battleMenuState = BATTLE_MENU_DEFAULT_STATE;
}

static void processRemoteCommands(uint16_t remoteCommand) {
    switch(remoteCommand) {
        case REMOTE_CMD_BATTLE_START:
            g_battleMenuState = BATTLE_MENU_START_STATE;
            break;
        case REMOTE_CMD_BATTLE_CONFIG:
            ledBlinkQuick(3);
            g_battleMenuState = BATTLE_MENU_CONFIG_STATE;
            break;
    }
}

void execBattleMenu(uint16_t remoteCommand) {
    processRemoteCommands(remoteCommand);
    if (g_battleMenuState == BATTLE_MENU_CONFIG_STATE) {
        if (remoteCommand == REMOTE_CMD_OK) {
            ledBlinkQuick(2);
            g_battleMenuState = BATTLE_MENU_DEFAULT_STATE;
        } else if(remoteCommand != 0) {
            setInitialMove(remoteCommand);
        }
    }
}