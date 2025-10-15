#include <Arduino.h>

#define DECODE_RC6
#include <IRremote.hpp>

#include "Config.h"

// Librerías "low" level
#include "Sensors/Sensors.h"
#include "Remote/Remote.h"
#include "Led/Led.h"

// System
#include "Battle/BattleEngine/BattleEngine.h"
#include "Battle/BattleMenu/BattleMenu.h"
#include "ESTOP/EStop.h"
#include "MainMenu/MainMenu.h"

//================================================================================
// Global Variables
//================================================================================

uint16_t g_remoteCommand = 0;
bool g_emergencyStop = false;

void setup() {
  Serial.begin(115200);

  ledSetup();
  sensorSetup();
  initializeBattleEngine();
  remoteSetup();
  battleEngineSetup();
}

void loop() {
  if(!g_emergencyStop) {
    g_remoteCommand = remoteHandler();

    if (g_remoteCommand == E_STOP_OFF) {
      resetEmergencyStop();
    }
    if (g_remoteCommand == E_STOP) {
      emergencyStop();
    }

    // Trigger de E_STOP
    if (getEmergencyStopState()) return;

    ledUpdate();

    if(getBattleMenuState() != BATTLE_MENU_START_STATE) {
      mainMenu(g_remoteCommand);
    } else {
      battleExec();
    }
  }
}