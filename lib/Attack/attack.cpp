#include "motors.h"
#include "sensors.h"
#include "battle.h"
#include "config.h"

/**
 * @brief Loop de ataque.
 * 
 * Función simple que mueve el robot hacia adelante con una potencia proporcional a la lectura del sensor central
 * y ajusta la dirección basándose en la diferencia entre los sensores derecho e izquierdo.
 *
 * @param data Struct de tipo @ref SensorData con las lecturas de los sensores.
 * @param charge Si es true la potencia se maxea independientemente de la lectura del sensor central.
 * 
 */
void attackLoop(SensorData data, bool charge = false) {

    int power = static_cast<int>((data.center - ATTACK_THRESHOLD) * (255 - ATTACK_POWER_SCALE / 1000.0 - ATTACK_THRESHOLD) + ATTACK_POWER_SCALE);
    float diff = static_cast<int>((data.right - data.left) * (1 / 1000.0));

    if(charge && data.center > ATTACK_THRESHOLD) {
        g_battleStateHold = BATTLE_STATE_HOLD_ATTACK_CHARGE;
        power = 255;
    } else {
        g_battleStateHold = BATTLE_STATE_HOLD_NONE;
    }

    sameDirection(power, diff);

}