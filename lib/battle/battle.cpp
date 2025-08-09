#include <Arduino.h>
#include "config.h"
#include "search.h"
#include "led.h"
#include "battle.h"
#include "sensors.h"

// Estados de batalla
#include "search.h"
#include "seek.h"
#include "attack.h"
#include "line.h"

static uint16_t g_remoteCommand = 0;

static battleMenuState g_battleMenuState = BATTLE_MENU_DEFAULT_STATE;

static SensorData g_sensorData;

battleMenuState getBattleMenuState() {
    return g_battleMenuState;
}

enum battleInitState {
  BATTLE_STATE_IDLE,
  BATTLE_STATE_BATTLE_CONFIG,
  BATTLE_STATE_COUNTDOWN,
  BATTLE_STATE_INITIAL_MOVE,
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

battleStateHold g_battleStateHold = BATTLE_STATE_HOLD_NONE;

void battleStateManger() {
    g_sensorData = readAllSensors();
    if(g_sensorData.center > ATTACK_THRESHOLD || g_battleStateHold == BATTLE_STATE_HOLD_ATTACK) {
        g_battleState = BATTLE_STATE_ATTACK;
    } else if(g_sensorData.lineLeft > LINE_EVADE_THRESHOLD || g_sensorData.lineRight > LINE_EVADE_THRESHOLD || g_battleStateHold == BATTLE_STATE_HOLD_LINE_EVADE) {
        g_battleState = BATTLE_STATE_LINE_EVADE;
    } else if (g_sensorData.left > SEEK_THRESHOLD || g_sensorData.center > SEEK_THRESHOLD || g_sensorData.right > SEEK_THRESHOLD || g_battleStateHold == BATTLE_STATE_HOLD_SEEK) {
        g_battleState = BATTLE_STATE_SEEK;
    } else {
        g_battleState = BATTLE_STATE_BLIND_SEARCH;
    }
}

void battleExec() {
    switch(g_battleState) {
        case BATTLE_STATE_BLIND_SEARCH:
            searchLoop(g_sensorData);
            break;
        case BATTLE_STATE_SEEK:
            seekLoop(g_sensorData);
            break;
        case BATTLE_STATE_LINE_EVADE:
            lineEvadeLoop(g_sensorData);
            break;
        case BATTLE_STATE_ATTACK:
            attackLoop(g_sensorData);
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
            case BATTLE_STATE_INITIAL_MOVE:
                g_battleInitState = BATTLE_STATE_BATTLE;
                break;
            case BATTLE_STATE_BATTLE:
                battleStateManger();
                battleExec();
                break;
        }
    }
}