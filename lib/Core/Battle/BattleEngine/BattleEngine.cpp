#include "Battle/BattleEngine/BattleEngine.h"
#include "Sensors/sensors.h"
#include "Strategies/IStrategy.h"

// 1. Include ALL concrete classes that will be used
#include "Battle/BattleInitializer/battleInitilizer.h"
#include "Behaviors/AttackBehaviors/Attack/standardAttack.h"
#include "Behaviors/SearchBehaviors/FocalizedSearch/FocalizedSearch.h"
#include "Behaviors/SearchBehaviors/BlindSearch/BlindSearch.h"
#include "Behaviors/LineEvadeBehaviors/StandardLineEvade/StandardLineEvade.h"
#include "Behaviors/AttackBehaviors/Attack/ChargeAttack/ChargeAttack.h"
#include "Factories/BehaviorFactory.h"
#include "Strategies/DefaultStrategy/DefaultStrategy.h"

#include "Motors/MotorController/MotorController.h"
#include "Motors/DummyMotorController/DummyMotorController.h"

#include "GlobalState.h"

MotorController g_real_motors;
DummyMotorController g_dummy_motors;

blindSearch* g_blind_search_behavior = nullptr;
focalizedSearch*   g_focalized_search_behavior = nullptr;
standardAttack* g_standard_attack_behavior= nullptr;
chargeAttack*   g_charge_attack_behavior = nullptr;
standardLineEvade* g_standard_line_evade_behavior = nullptr;

DefaultStrategy* g_default_strategy = nullptr;

static bool g_debug_mode_active = false;

static IStrategy* g_strategies[5];

static IMotorController* activeController;

void initializeBattleEngine() {
    activeController = initializeBehaviorsAndStrategy(g_debug_mode_active);

    g_strategies[0] = g_default_strategy;
}

void setBattleDebugMode(bool enabled) {
    if (g_debug_mode_active != enabled) {
        g_debug_mode_active = enabled;
        initializeBattleEngine();
    }
}

IMotorController* getActiveMotorController()
{
    return activeController;
};

static uint8_t g_current_strategy_index = 0;
static IStrategy* g_active_strategy = nullptr;

IStrategy* getActiveStrategy() {
    return g_active_strategy;
}

// Loop principal de ejecución de la pelea
void battleExec() {
    if (!g_active_strategy) {
        g_active_strategy = g_strategies[g_current_strategy_index];
        g_active_strategy->init();
    }

    SensorData data = readAllSensors();
    if (execBattleInit(data))
    {
        g_active_strategy->execute(data);
    }
}