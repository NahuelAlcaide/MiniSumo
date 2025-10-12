#pragma once
#include <Arduino.h>

#include "Motors/MotorController/MotorController.h"
#include "Strategies/IStrategy.h"

// Devuelve un pointer a la estrategía que está activa actualmente.
IStrategy* getActiveStrategy();

void battleExec();

void initializeBattleEngine();

void setBattleDebugMode(bool enabled);

void setActiveStrategy(uint8_t index);

IMotorController* getActiveMotorController();