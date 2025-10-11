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
};