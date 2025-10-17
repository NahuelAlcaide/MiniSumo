#pragma once
#include "Motors/IMotorController.h"

class DummyMotorController : public IMotorController
{
public:
    void motorSetup() override;
    void sameDirection(int dirPow, float turn) override;
    void oppositeDirection(int dirPow) override;
    void brake() override;
    void limpMotors() override;
    const MotorDebugInfo& getDebugInfo() const override;
private:
    void updateDebugValues(MotorCommand command, int dirPow, float turn, uint8_t leftSpeed, uint8_t rightSpeed);
    MotorDebugInfo m_debugInfo;
};