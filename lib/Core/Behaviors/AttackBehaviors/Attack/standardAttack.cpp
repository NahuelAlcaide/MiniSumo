#include "standardAttack.h"
#include "config.h"

standardAttack::standardAttack(IMotorController* motorController) :
    m_motorController(motorController)
{}

standardAttack::Status standardAttack::execute(const SensorData data) {

    int power = static_cast<int>((data.center - ATTACK_THRESHOLD) * (255 - ATTACK_POWER_SCALE / 1000.0 - ATTACK_THRESHOLD) + ATTACK_POWER_SCALE);
    float diff = static_cast<int>((data.right - data.left) * (1 / 1000.0));

    m_motorController->sameDirection(power, diff);

    return COMPLETED;
}
