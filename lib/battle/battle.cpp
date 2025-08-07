#include <Arduino.h>
#include "config.h"
#include "search.h"
#include "led.h"
#include "battle.h"

// Estados de batalla
#include "search.h"

static uint16_t g_remoteCommand = 0;

static battleMenuState g_battleMenuState = BATTLE_MENU_DEFAULT_STATE;

battleMenuState getBattleMenuState() {
    return g_battleMenuState;
}

enum battleInitState {
  BATTLE_STATE_IDLE,
  BATTLE_STATE_BATTLE_CONFIG,
  BATTLE_STATE_COUNTDOWN,
  BATTLE_STATE_BATTLE
};

static battleInitState g_battleInitState = BATTLE_STATE_IDLE;

enum battleState {
    BATTLE_STATE_BLIND_SEARCH,
    BATTLE_STATE_SEEK,
    //BATTLE_STATE_SIDE_CRASH,
    BATTLE_STATE_LINE_EVADE,
    BATTLE_STATE_ATTACK,
    //BATTLE_STATE_PANIC
};

static battleState g_battleState = BATTLE_STATE_BLIND_SEARCH;

void clearBattleFlags() {
    g_battleMenuState = BATTLE_MENU_DEFAULT_STATE;
    g_battleInitState = BATTLE_STATE_IDLE;
    g_battleState = BATTLE_STATE_BLIND_SEARCH;
    Serial.println("Battle flags cleared.");
}

static void processRemoteCommands() {
  switch(g_remoteCommand) {
    case REMOTE_CMD_BATTLE_START:
        g_battleMenuState = BATTLE_MENU_START_STATE;
        break;
  }
}

void battleStateManger() {

}

void battleExec() {
    switch(g_battleState) {
        case BATTLE_STATE_BLIND_SEARCH:
            searchLoop();
            break;
        case BATTLE_STATE_SEEK:
            //seekLoop();
            break;
        case BATTLE_STATE_LINE_EVADE:
            //lineEvadeLoop();
            break;
        case BATTLE_STATE_ATTACK:
            //attackLoop();
            break;
    }
}

void battleModeLoop(uint16_t remoteCommand){
    g_remoteCommand = remoteCommand;
    if (g_battleMenuState != BATTLE_MENU_START_STATE) {
        processRemoteCommands();
    } else {
        switch(g_battleInitState) {
            case BATTLE_STATE_IDLE:
                Serial.println("Battle mode.");
                g_battleInitState = BATTLE_STATE_COUNTDOWN;
                break;
            case BATTLE_STATE_COUNTDOWN:
                Serial.println("Countdown start.");
                ledBlinkQuick();
                delay(5000);
                g_battleInitState = BATTLE_STATE_BATTLE;
                break;
            case BATTLE_STATE_BATTLE:
                battleStateManger();
                battleExec();
                break;
        }
    }
}