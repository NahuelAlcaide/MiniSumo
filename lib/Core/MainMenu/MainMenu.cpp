#include "MainMenu.h"
#include <Arduino.h>

#include "Config.h"
#include "Battle/BattleEngine/BattleEngine.h"
#include "Battle/BattleMenu/BattleMenu.h"
#include "Debug/Debug.h"
#include "ESTOP/EStop.h"
#include "Led/Led.h"
#include "Strategies/StrategyMenu/StrategyMenu.h"

class IMotorController;

static mainMenuState g_mainMenuState = STATE_DEFAULT;

void setMainMenuState(mainMenuState state) {
    g_mainMenuState = state;
}

static void processRemoteCommands(uint16_t remoteCommand) {
    switch(remoteCommand){
        case REMOTE_CMD_DEBUG_MODE:
            if (g_mainMenuState != STATE_DEBUG_MENU){
                g_mainMenuState = STATE_DEBUG_MENU;
                Serial.print("State changed: DEBUG_MENU\n");
                clearDebugFlags();
                ledBlinkDebugMode();
                Serial.println("Debug mode is now ON");
                break;
            } else {
                ledMainMenu();
                g_mainMenuState = STATE_DEFAULT;
                Serial.print("State changed: DEFAULT (Debug OFF)\n");
                Serial.println("Debug mode is now OFF");
                break;
            }
        case REMOTE_CMD_BATTLE_MODE:
            if (g_mainMenuState != STATE_BATTLE_MENU){
                g_mainMenuState = STATE_BATTLE_MENU;
                Serial.println("State changed: BATTLE_MENU");
                ledBattleMenu();
                break;
            } else {
                ledMainMenu();
                g_mainMenuState = STATE_DEFAULT;
                Serial.println("State changed: DEFAULT (Battle OFF)");
                break;
            }
        case REMOTE_CMD_STRATEGY_MENU:
            if (g_mainMenuState != STATE_STRATEGY_MENU){
                g_mainMenuState = STATE_STRATEGY_MENU;
                Serial.println("State changed: STRATEGY_MENU");
                ledStrategyMenu();
                break;
            } else {
                ledMainMenu();
                g_mainMenuState = STATE_DEFAULT;
                Serial.println("State changed: DEFAULT (Strategy OFF)");
                break;
            }
    }
}

void mainMenu(uint16_t remoteCommand) {
    processRemoteCommands(remoteCommand);
    switch(g_mainMenuState) {
        case STATE_DEFAULT:
            break;
        case STATE_BATTLE_MENU:
            execBattleMenu(remoteCommand);
            break;
        case STATE_DEBUG_MENU: {
            IMotorController* motors = getActiveMotorController();
            debug(remoteCommand, motors);
            break;
        }
        case STATE_STRATEGY_MENU:
            strategyMenu(remoteCommand);
            break;
    }
}

