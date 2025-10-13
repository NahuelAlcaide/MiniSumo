#include "SystemState.h"

#include <Arduino.h>

// Include the headers for each module that needs resetting.
#include "Battle/BattleMenu/BattleMenu.h"
#include "Battle/BattleInitializer/BattleInitilizer.h"
#include "Battle/BattleEngine/BattleEngine.h" // To get the active strategy
#include "Battle/InitialMove/InitialMove.h"

void resetSystemState() {
    // Call the reset function for each module.
    resetBattleMenuState();
    resetBattleInitState();
    resetInitialMove();

    // Reset the active strategy.
    IStrategy* activeStrategy = getActiveStrategy();
    if (activeStrategy) {
        activeStrategy->init();
    }

    Serial.println("System state has been reset.");
}
