#pragma once
#include "Motors/IMotorController.h"

void debug(uint16_t remoteCommand, IMotorController* motors);
void clearDebugFlags();
void printSensorReadout(SensorData readings);