#include "SystemState.h"

#include <arduino.h>

// Include the headers for each module that needs resetting.
#include "Battle/BattleMenu/battleMenu.h"
#include "Battle/BattleInitializer/battleInitilizer.h"
#include "Battle/BattleEngine/battleEngine.h" // To get the active strategy
#include "Battle/InitialMove/initialMove.h"

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
