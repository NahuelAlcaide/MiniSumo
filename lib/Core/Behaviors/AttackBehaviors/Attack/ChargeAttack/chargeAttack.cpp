#include "chargeAttack.h"

#include "config.h"

chargeAttack::chargeAttack(IMotorController* motorController) :
    standardAttack(motorController)
{}

chargeAttack::Status chargeAttack::execute(const SensorData data) {

    float diff = static_cast<int>((data.right - data.left) * (1 / 1000.0));

    m_motorController->sameDirection(255, diff);

    if (data.center > ATTACK_THRESHOLD)
    {
        return RUNNING;
    } else
    {
        return COMPLETED;
    }

}
