#include "Battle/BattleEngine/BattleEngine.h"
#include "Sensors/Sensors.h"
#include "Strategies/IStrategy.h"

// 1. Include ALL concrete classes that will be used
#include "Strategies/DefaultStrategy/DefaultStrategy.h"

#include "Battle/BattleInitializer/BattleInitilizer.h"
#include "Behaviors/AttackBehaviors/Attack/StandardAttack.h"
#include "Behaviors/SearchBehaviors/FocalizedSearch/FocalizedSearch.h"
#include "Behaviors/SearchBehaviors/BlindSearch/BlindSearch.h"
#include "Behaviors/LineEvadeBehaviors/StandardLineEvade/StandardLineEvade.h"
#include "Behaviors/AttackBehaviors/Attack/ChargeAttack/ChargeAttack.h"
#include "Factories/BehaviorFactory.h"

#include "Strategies/DefensiveStrategy/DefensiveStrategy.h"

#include "Behaviors/SearchBehaviors/BlindSearch/DefensiveBlindSearch/DefensiveBlindSearch.h"

#include "Motors/MotorController/MotorController.h"
#include "Motors/DummyMotorController/DummyMotorController.h"

#include "GlobalState.h"

#include <EEPROM.h>

#include "Debug/StrategyDebug/StrategyDebug.h"

MotorController g_real_motors;
DummyMotorController g_dummy_motors;

DefaultStrategy* g_default_strategy = nullptr;

BlindSearch* g_blind_search_behavior = nullptr;
FocalizedSearch*   g_focalized_search_behavior = nullptr;
StandardAttack* g_standard_attack_behavior= nullptr;
ChargeAttack*   g_charge_attack_behavior = nullptr;
StandardLineEvade* g_standard_line_evade_behavior = nullptr;

DefensiveStrategy* g_defensive_strategy = nullptr;

DefensiveBlindSearch* g_defensive_blind_search_behavior = nullptr;

static bool g_debug_mode_active = false;

static IStrategy* g_strategies[5];

static IMotorController* activeController;

static uint8_t g_current_strategy_index;
static IStrategy* g_active_strategy = nullptr;

constexpr int STRATEGY_EEPROM_ADDR = 0;

void battleEngineSetup() {
    // Load the last saved strategy index from EEPROM
    EEPROM.get(STRATEGY_EEPROM_ADDR, g_current_strategy_index);

    // Validate the index to prevent out-of-bounds access
    if (g_current_strategy_index >= sizeof(g_strategies) / sizeof(g_strategies[0]) || g_strategies[g_current_strategy_index] == nullptr) {
        g_current_strategy_index = 0; // Default to the first strategy if invalid
    }
}

void initializeBattleEngine() {
    activeController = initializeBehaviorsAndStrategy(g_debug_mode_active);

    g_strategies[0] = g_default_strategy;
    g_strategies[1] = g_defensive_strategy;
}

void setBattleDebugMode(bool enabled) {
    if (g_debug_mode_active != enabled) {
        g_debug_mode_active = enabled;
        initializeBattleEngine();
    }
}

bool getBattleDebugMode() {
    return g_debug_mode_active;
}

IMotorController* getActiveMotorController()
{
    return activeController;
};

void setActiveStrategy(uint8_t index) {
    if (index < sizeof(g_strategies) / sizeof(g_strategies[0]) && g_current_strategy_index != index) {
        g_current_strategy_index = index;
        g_active_strategy = g_strategies[g_current_strategy_index];

        // Save the new strategy index to EEPROM
        EEPROM.put(STRATEGY_EEPROM_ADDR, g_current_strategy_index);

        Serial.println("Strategy changed to index: " + String(g_current_strategy_index));

        if (g_active_strategy) {
            g_active_strategy->init();
        }
    }
}

IStrategy* getActiveStrategy() {
    return g_active_strategy;
}

// Loop principal de ejecución de la pelea
void battleExec() {
    if (!g_active_strategy) {
        g_active_strategy = g_strategies[g_current_strategy_index];
        g_active_strategy->init();

        Serial.println("Initialized battle engine with strategy: " + String(g_current_strategy_index) + "");
    }

    SensorData data = readAllSensors();
    if (execBattleInit(data))
    {
        g_active_strategy->execute(data);
    }

    if (g_debug_mode_active) {
        sendRawDebugPacket(g_current_strategy_index, g_active_strategy, data, activeController);
    }
}