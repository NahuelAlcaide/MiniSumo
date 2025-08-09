#include "motors.h"
#include "sensors.h"
#include "battle.h"
#include "config.h"

void attackLoop(SensorData data) {
    int power = static_cast<int>((data.center - ATTACK_THRESHOLD) * (255 - SEEK_SPEED / 1000.0 - ATTACK_THRESHOLD) + 60);
    float diff = static_cast<int>((data.right - data.left) * (1 / 1000.0));
    sameDirection(power, diff);
}