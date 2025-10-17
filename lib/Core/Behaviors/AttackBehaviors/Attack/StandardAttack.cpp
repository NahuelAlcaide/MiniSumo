#include "StandardAttack.h"
#include "Config.h"

StandardAttack::StandardAttack(IMotorController* motorController) :
    m_motorController(motorController)
{}

StandardAttack::Status StandardAttack::execute(const SensorData data) {

    float power_f = static_cast<float>(data.center - ATTACK_THRESHOLD) * (255 - ATTACK_POWER_SCALE) / (999 - ATTACK_THRESHOLD) + ATTACK_POWER_SCALE;

    int power = static_cast<int>(power_f);

    if (power > 255) {
        power = 255;
    } else if (power < 0) {
        power = 0;
    }

    float diff = (data.right - data.left) * (1 / 1000.0);

    m_motorController->sameDirection(power, diff);

    return COMPLETED;
}
