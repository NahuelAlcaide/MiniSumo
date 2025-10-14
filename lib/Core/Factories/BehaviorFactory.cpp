#include "BehaviorFactory.h"
#include <Arduino.h>

#include "Strategies/DefaultStrategy/DefaultStrategy.h"

#include "Behaviors/AttackBehaviors/Attack/StandardAttack.h"
#include "Behaviors/AttackBehaviors/Attack/ChargeAttack/ChargeAttack.h"
#include "Behaviors/LineEvadeBehaviors/StandardLineEvade/StandardLineEvade.h"
#include "Behaviors/SearchBehaviors/BlindSearch/BlindSearch.h"
#include "Behaviors/SearchBehaviors/FocalizedSearch/FocalizedSearch.h"

#include "Strategies/DefensiveStrategy/DefensiveStrategy.h"

#include "Behaviors/SearchBehaviors/BlindSearch/DefensiveBlindSearch/DefensiveBlindSearch.h"

#include "GlobalState.h"

IMotorController* initializeBehaviorsAndStrategy(bool useDummyMotors) {
    // Determine which motor controller to use
    IMotorController* motorController = useDummyMotors ? static_cast<IMotorController*>(&g_dummy_motors) : static_cast<IMotorController*>(&g_real_motors);

    // --- Clean up old objects if they exist ---
    delete g_blind_search_behavior;
    delete g_focalized_search_behavior;
    delete g_standard_attack_behavior;
    delete g_charge_attack_behavior;
    delete g_standard_line_evade_behavior;
    delete g_default_strategy;
    delete g_defensive_strategy;

    delete g_defensive_blind_search_behavior;

    // --- Create NEW instances of behaviors with the correct motors ---
    g_blind_search_behavior = new BlindSearch(motorController);
    g_focalized_search_behavior = new FocalizedSearch(motorController);
    g_standard_attack_behavior = new StandardAttack(motorController);
    g_charge_attack_behavior = new ChargeAttack(motorController);
    g_standard_line_evade_behavior = new StandardLineEvade(motorController);

    g_defensive_blind_search_behavior = new DefensiveBlindSearch(motorController);

    // --- Create NEW instances of the strategy with the new behaviors ---
    g_default_strategy = new DefaultStrategy(
        g_blind_search_behavior,
        g_focalized_search_behavior,
        g_standard_attack_behavior,
        g_charge_attack_behavior,
        g_standard_line_evade_behavior
    );

    g_defensive_strategy = new DefensiveStrategy(
        g_defensive_blind_search_behavior,
        g_focalized_search_behavior,
        g_standard_attack_behavior,
        g_charge_attack_behavior,
        g_standard_line_evade_behavior
    );

    if (useDummyMotors) {
        Serial.println("FACTORY: Initialized with DUMMY motors.");
    } else {
        Serial.println("FACTORY: Initialized with REAL motors.");
    }

    return motorController;
}