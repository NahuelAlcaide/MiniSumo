#pragma once
#include "Motors/IMotorController.h"

class MotorController : public IMotorController
{
public:

    void motorSetup() override;

    /**
     *@brief Hace girar los dos motores en la misma dirección.
     *        dirPow: [-255, 255], turn: [-1, 1]
     *
     * @param dirPow Factor de potencia de los motores. Valores positivos para ir hacia adelante, negativos para ir hacia atrás.
     * @param turn Factor de giro. Valores positivos para girar a la derecha, negativos para girar a la izquierda.
     * Si es 0, los motores giran a la misma velocidad.
     */
    void sameDirection(int dirPow, float turn) override;

    void oppositeDirection(int dirPow) override;

    void brake() override;

    void limpMotors() override;

    const MotorDebugInfo& getDebugInfo() const override;

private:
    void updateDebugValues(MotorCommand command, int dirPow, float turn, uint8_t leftSpeed, uint8_t rightSpeed);
    MotorDebugInfo m_debugInfo;
};