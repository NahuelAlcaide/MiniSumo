#pragma once
#include "Behaviors/AttackBehaviors/Attack/standardAttack.h"

class chargeAttack : public standardAttack {
public:
    /**
    * @brief Loop de ataque.
    *
    * Función simple que mueve el robot hacia adelante a la <b>potencia máxima</b>
    * y ajusta la dirección basándose en la diferencia entre los sensores derecho e izquierdo.
    *
    * @param data Struct de tipo SensorData con las lecturas de los sensores.
    *
    */
    Status execute(SensorData data) override;
    explicit chargeAttack(IMotorController* motorController);
};