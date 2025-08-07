#include <Arduino.h>

#define DECODE_RC6
#include <IRremote.hpp>

#include "config.h"

#include "debug.h"
#include "sensors.h"
#include "motors.h"
#include "remote.h"
#include "led.h"


//================================================================================
// Global Variables
//================================================================================

bool g_enableDebugMode = false;
bool g_emergencyStop = false;

void resetEmergencyStop() {
  g_emergencyStop = false;
  g_enableDebugMode = false;
  clearDebugFlags();
  ledMainMenu();
}

void setup() {
  Serial.begin(115200);

  ledSetup();
  sensorSetup();
  motorSetup();
  remoteSetup(); 
}

void processRemoteCommands(uint16_t remoteData) {
  switch(remoteData){
    case E_STOP:
      ledBlinkQuick();
      g_emergencyStop = true;
      limpMotors();
      return;
    case REMOTE_CMD_DEBUG_MODE:
      clearDebugFlags();
      g_enableDebugMode = !g_enableDebugMode;
      if (g_enableDebugMode) {
        ledBlinkDebugMode();
      } else {
        ledMainMenu();
      }
      Serial.print("Debug mode is now ");
      Serial.println(g_enableDebugMode ? "ON" : "OFF");
      break;
  }
}

void loop() {  
  if(!g_emergencyStop) {
    uint16_t remoteCommand = remoteHandler();
    ledUpdate();
    processRemoteCommands(remoteCommand);
    
    // Trigger de E_STOP
    if (g_emergencyStop) return;

    if (g_enableDebugMode){
      debug(remoteCommand);
    }
  } else {
    uint16_t remoteCommand = remoteHandler();
    if (remoteCommand == E_STOP_OFF) {
      resetEmergencyStop();
    }
  }
}