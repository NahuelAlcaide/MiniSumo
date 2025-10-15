#pragma once
#include <Arduino.h>

#include "Motors/IMotorController.h"

void debug(uint16_t remoteCommand, IMotorController* motors);
void clearDebugFlags();