#pragma once
#include "Sensors/sensors.h"

class IBehavior {
public:

    enum Status {
        RUNNING,   // El comportamiento quiere mantenerse activo.
        COMPLETED  // El comportamiento no requiere mantenerse activoW.
    };

    virtual ~IBehavior() = default;
    virtual Status execute(SensorData data) = 0;
};