#pragma once

#include "Behaviors/LineEvadeBehaviors/ILineEvadeBehavior.h"
#include "Motors/IMotorController.h"

class standardLineEvade : public ILineEvadeBehavior {
public:

    enum EvadeState {
        IDLE,
        INITIAL_BRAKING,
        TURNING,
        NORMAL_REVERSING,
        PANIC_REVERSING,
        END_BRAKING
    };

    /**
    * @brief Loop de evasión de linea.
    *
    * Función que implementa un comportamiento de evasiónd de linea.
    *
    * @param data Struct de tipo SensorData con las lecturas de los sensores.
    *
    */
    Status execute(SensorData data) override;

    explicit standardLineEvade(IMotorController* motorController);
private:
    IMotorController* m_motorController; // Contiene un pointer a la capa de abstracción de los motores// Contiene un pointer a la capa de abstracción de los motores
};
