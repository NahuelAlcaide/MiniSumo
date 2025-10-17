#pragma once

#include "Sensors/Sensors.h"

class IStrategy {
public:
    virtual ~IStrategy() = default;

    virtual const char* getName() const = 0;

    virtual void init() = 0;

    virtual void execute(const SensorData& data) = 0;

    virtual const char* getCurrentStateName() const = 0;
    virtual uint8_t getCurrentStateId() const = 0;
};
