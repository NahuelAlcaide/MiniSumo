#pragma once
#include <Arduino.h>

#include "Motors/MotorController/MotorController.h"
#include "Strategies/IStrategy.h"

void battleEngineSetup();

// Devuelve un pointer a la estrategía que está activa actualmente.
IStrategy* getActiveStrategy();

void battleExec();

void initializeBattleEngine();

void setBattleDebugMode(bool enabled);

bool getBattleDebugMode();

void setActiveStrategy(uint8_t index);

IMotorController* getActiveMotorController();