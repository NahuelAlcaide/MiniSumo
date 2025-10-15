#pragma once
#include "Behaviors/AttackBehaviors/Attack/StandardAttack.h"
#include "Behaviors/AttackBehaviors/Attack/ChargeAttack/ChargeAttack.h"
#include "Behaviors/LineEvadeBehaviors/StandardLineEvade/StandardLineEvade.h"
#include "Behaviors/SearchBehaviors/BlindSearch/BlindSearch.h"
#include "Behaviors/SearchBehaviors/BlindSearch/DefensiveBlindSearch/DefensiveBlindSearch.h"
#include "Behaviors/SearchBehaviors/FocalizedSearch/FocalizedSearch.h"
#include "Motors/DummyMotorController/DummyMotorController.h"
#include "Motors/MotorController/MotorController.h"
#include "Strategies/DefaultStrategy/DefaultStrategy.h"
#include "Strategies/DefensiveStrategy/DefensiveStrategy.h"

extern MotorController  g_real_motors;
extern DummyMotorController g_dummy_motors;

extern DefaultStrategy* g_default_strategy;

extern BlindSearch* g_blind_search_behavior;
extern FocalizedSearch*   g_focalized_search_behavior;
extern StandardAttack* g_standard_attack_behavior;
extern ChargeAttack*   g_charge_attack_behavior;
extern StandardLineEvade* g_standard_line_evade_behavior;

extern DefensiveStrategy* g_defensive_strategy;

extern DefensiveBlindSearch* g_defensive_blind_search_behavior;