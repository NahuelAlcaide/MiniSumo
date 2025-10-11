#include "BehaviorFactory.h"
#include <arduino.h>

#include "Behaviors/AttackBehaviors/Attack/standardAttack.h"
#include "Behaviors/AttackBehaviors/Attack/ChargeAttack/chargeAttack.h"
#include "Behaviors/LineEvadeBehaviors/StandardLineEvade/StandardLineEvade.h"
#include "Behaviors/SearchBehaviors/BlindSearch/BlindSearch.h"
#include "Behaviors/SearchBehaviors/FocalizedSearch/FocalizedSearch.h"
#include "Strategies/DefaultStrategy/DefaultStrategy.h"

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

    // --- Create NEW instances of behaviors with the correct motors ---
    g_blind_search_behavior = new blindSearch(motorController);
    g_focalized_search_behavior = new focalizedSearch(motorController);
    g_standard_attack_behavior = new standardAttack(motorController);
    g_charge_attack_behavior = new chargeAttack(motorController);
    g_standard_line_evade_behavior = new standardLineEvade(motorController);

    // --- Create NEW instances of the strategy with the new behaviors ---
    g_default_strategy = new DefaultStrategy(
        g_blind_search_behavior,
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