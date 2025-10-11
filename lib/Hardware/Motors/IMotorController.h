#pragma once

class IMotorController {
public:
    virtual ~IMotorController() = default;

    virtual void motorSetup();
    virtual void sameDirection(int power, float diff) = 0;
    virtual void oppositeDirection(int power) = 0;
    virtual void brake();
    virtual void limpMotors();
};