#include <Arduino.h>

#define DECODE_RC6
#include <IRremote.hpp>

#include "config.h"

// Librerías "low" level
#include "sensors.h"
#include "motors.h"
#include "remote.h"
#include "led.h"

// Estados
#include "battle.h"
#include "debug.h"


//================================================================================
// Global Variables
//================================================================================

bool g_emergencyStop = false;
uint16_t g_remoteCommand = 0;

void setup() {
  Serial.begin(115200);

  ledSetup();
  sensorSetup();
  motorSetup();
  remoteSetup(); 
}

void resetEmergencyStop() {
  g_emergencyStop = false;
  clearDebugFlags();
  ledMainMenu();
  clearBattleFlags();
}

enum mainMenuState {
  STATE_DEFAULT,
  STATE_BATTLE_MENU,
  STATE_DEBUG_MENU
};

static mainMenuState g_mainMenuState = STATE_DEFAULT;

static void processRemoteCommands() {
  if (g_remoteCommand == E_STOP){
    g_mainMenuState = STATE_DEFAULT;
    Serial.print("State changed: DEFAULT (E_STOP)\n");
    ledBlinkQuick();
    g_emergencyStop = true;
    limpMotors();
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
        }
    }
  }
}

void mainMenu() {
  switch(g_mainMenuState) {
    case STATE_DEFAULT:
      break;
    case STATE_BATTLE_MENU:
      battleModeLoop(g_remoteCommand);
      break;
    case STATE_DEBUG_MENU:
      debug(g_remoteCommand);
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
      battleModeLoop(g_remoteCommand);
    }
  } else {
    uint16_t remoteCommand = remoteHandler();
    if (remoteCommand == E_STOP_OFF) {
      resetEmergencyStop();
    }
  }
}