#pragma once

#include "Behaviors/LineEvadeBehaviors/ILineEvadeBehavior.h"
#include "Motors/IMotorController.h"

class StandardLineEvade : public ILineEvadeBehavior {
public:

    enum EvadeState {
        IDLE,
        TURNING,
        NORMAL_REVERSING,
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

    explicit StandardLineEvade(IMotorController* motorController);
private:
    IMotorController* m_motorController; // Contiene un pointer a la capa de abstracción de los motores// Contiene un pointer a la capa de abstracción de los motores
};
