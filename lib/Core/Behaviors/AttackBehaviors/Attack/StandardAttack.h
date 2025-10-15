#pragma once

#include "Sensors/Sensors.h"
#include "Behaviors/AttackBehaviors/IAttackBehavior.h"
#include "Motors/IMotorController.h"

class StandardAttack : public IAttackBehavior {
public:
    /**
    * @brief Loop de ataque.
    *
    * Función simple que mueve el robot hacia adelante con una potencia proporcional a la lectura del sensor central
    * y ajusta la dirección basándose en la diferencia entre los sensores derecho e izquierdo.
    *
    * @param data Struct de tipo SensorData con las lecturas de los sensores.
    *
    */
    Status execute(SensorData data) override;

    explicit StandardAttack(IMotorController* motorController);
protected:
    IMotorController* m_motorController; // Contiene un pointer a la capa de abstracción de los motores
};
