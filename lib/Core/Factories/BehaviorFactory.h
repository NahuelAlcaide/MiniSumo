#pragma once
#include "Motors/IMotorController.h"

// This function will create all our strategy and behavior objects.
// It takes a flag to decide which dependencies to inject.
IMotorController* initializeBehaviorsAndStrategy(bool useDummyMotors);