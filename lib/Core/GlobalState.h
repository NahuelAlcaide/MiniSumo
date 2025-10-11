#pragma once
#include "Behaviors/AttackBehaviors/Attack/standardAttack.h"
#include "Behaviors/AttackBehaviors/Attack/ChargeAttack/chargeAttack.h"
#include "Behaviors/LineEvadeBehaviors/StandardLineEvade/StandardLineEvade.h"
#include "Behaviors/SearchBehaviors/BlindSearch/BlindSearch.h"
#include "Behaviors/SearchBehaviors/FocalizedSearch/FocalizedSearch.h"
#include "Motors/DummyMotorController/DummyMotorController.h"
#include "Motors/MotorController/MotorController.h"
#include "Strategies/DefaultStrategy/DefaultStrategy.h"

extern MotorController  g_real_motors;
extern DummyMotorController g_dummy_motors;

extern blindSearch* g_blind_search_behavior;
extern focalizedSearch*   g_focalized_search_behavior;
extern standardAttack* g_standard_attack_behavior;
extern chargeAttack*   g_charge_attack_behavior;
extern standardLineEvade* g_standard_line_evade_behavior;

extern DefaultStrategy* g_default_strategy;