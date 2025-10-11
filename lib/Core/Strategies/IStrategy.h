#pragma once

#include "Sensors/sensors.h"

class IStrategy {
public:
    virtual ~IStrategy() = default;

    virtual const char* getName() const = 0;

    virtual void init() = 0;

    virtual void execute(const SensorData& data) = 0;
};
