#include "EStop.h"

#include "Battle/BattleEngine/BattleEngine.h"
#include "Battle/BattleInitializer/BattleInitilizer.h"
#include "Debug/Debug.h"
#include "Led/Led.h"
#include "MainMenu/MainMenu.h"
#include "SystemState/SystemState.h"

static bool g_emergencyStop = false;

void resetEmergencyStop() {
    g_emergencyStop = false;
    g_debugModule.reset();
    ledMainMenu();
    resetSystemState();
    resetBattleInitState();
}

bool getEmergencyStopState() {
    return g_emergencyStop;
}

void emergencyStop() {
    g_emergencyStop = true;
    setMainMenuState(STATE_DEFAULT);
    Serial.print("State changed: DEFAULT (E_STOP)\n");
    ledBlinkQuick();

    IMotorController* motors = getActiveMotorController();
    if (motors) {
        motors->limpMotors();
    }
}