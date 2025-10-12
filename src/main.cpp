#include <Arduino.h>

#define DECODE_RC6
#include <IRremote.hpp>

#include "Config.h"

// Librerías "low" level
#include "Sensors/Sensors.h"
#include "Motors/MotorController/MotorController.h"
#include "Remote/Remote.h"
#include "Led/Led.h"

// Util
#include "Debug/Debug.h"

// System
#include "Battle/BattleEngine/BattleEngine.h"
#include "SystemState/SystemState.h"
#include "Battle/BattleMenu/BattleMenu.h"
#include "Battle/BattleEngine/BattleEngine.h"
#include "Strategies/StrategyMenu/StrategyMenu.h"


//================================================================================
// Global Variables
//================================================================================

bool g_emergencyStop = false;
uint16_t g_remoteCommand = 0;

void setup() {
  Serial.begin(115200);

  ledSetup();
  sensorSetup();
  initializeBattleEngine();
  remoteSetup(); 
}

void resetEmergencyStop() {
  g_emergencyStop = false;
  clearDebugFlags();
  ledMainMenu();
  resetSystemState();
}

enum mainMenuState {
  STATE_DEFAULT,
  STATE_BATTLE_MENU,
  STATE_DEBUG_MENU,
  STATE_STRATEGY_MENU
};

static mainMenuState g_mainMenuState = STATE_DEFAULT;

static void processRemoteCommands() {
  if (g_remoteCommand == E_STOP){
    g_mainMenuState = STATE_DEFAULT;
    Serial.print("State changed: DEFAULT (E_STOP)\n");
    ledBlinkQuick();
    g_emergencyStop = true;

    IMotorController* motors = getActiveMotorController();
    if (motors) {
      motors->limpMotors();
    }
    return;
  } else if(getBattleMenuState() != BATTLE_MENU_START_STATE) {
    switch(g_remoteCommand){
      case REMOTE_CMD_DEBUG_MODE:
        if (g_mainMenuState != STATE_DEBUG_MENU){
          g_mainMenuState = STATE_DEBUG_MENU;
          Serial.print("State changed: DEBUG_MENU\n");
          clearDebugFlags();
          ledBlinkDebugMode();
          Serial.print("Debug mode is now ON");
          break;
        } else {
          ledMainMenu();
          g_mainMenuState = STATE_DEFAULT;
          Serial.print("State changed: DEFAULT (Debug OFF)\n");
          Serial.print("Debug mode is now OFF");
          break;
        }
      case REMOTE_CMD_BATTLE_MODE:
        if (g_mainMenuState != STATE_BATTLE_MENU){
            g_mainMenuState = STATE_BATTLE_MENU;
            Serial.print("State changed: BATTLE_MENU\n");
            ledBattleMenu();
            break;
        } else {
          ledMainMenu();
          g_mainMenuState = STATE_DEFAULT;
          Serial.print("State changed: DEFAULT (Battle OFF)\n");
          break;
        }
      case REMOTE_CMD_STRATEGY_MENU:
        if (g_mainMenuState != STATE_STRATEGY_MENU){
          g_mainMenuState = STATE_STRATEGY_MENU;
          Serial.print("State changed: STRATEGY_MENU\n");
          ledStrategyMenu();
          break;
        } else {
          ledMainMenu();
          g_mainMenuState = STATE_DEFAULT;
          Serial.print("State changed: DEFAULT (Strategy OFF)\n");
          break;
        }
    }
  }
}

void mainMenu() {
  switch(g_mainMenuState) {
    case STATE_DEFAULT:
      break;
    case STATE_BATTLE_MENU:
      execBattleMenu(g_remoteCommand);
      break;
  case STATE_DEBUG_MENU: {
        IMotorController* motors = getActiveMotorController();
        debug(g_remoteCommand, motors);
        break;
      }
    case STATE_STRATEGY_MENU:
      strategyMenu(g_remoteCommand);
      break;
  }
}

void loop() {  
  if(!g_emergencyStop) {
    g_remoteCommand = remoteHandler();
    ledUpdate();
    processRemoteCommands();
    
    // Trigger de E_STOP
    if (g_emergencyStop) return;

    if(getBattleMenuState() != BATTLE_MENU_START_STATE) {
      mainMenu();
    } else {
      battleExec();
    }
  } else {
    uint16_t remoteCommand = remoteHandler();
    if (remoteCommand == E_STOP_OFF) {
      resetEmergencyStop();
    }
  }
}