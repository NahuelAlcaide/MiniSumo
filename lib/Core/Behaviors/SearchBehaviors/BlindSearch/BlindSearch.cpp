#include "BlindSearch.h"

#include <Arduino.h>

#include "Config.h"

BlindSearch::BlindSearch(IMotorController* motorController) :
    m_motorController(motorController)
{}

BlindSearch::Status BlindSearch::execute(SensorData data) {
    static unsigned long lastSwitch = millis();
    static int turnDir = 1; // 1 derecha, -1 izquierda.

    if (millis() - lastSwitch > SEARCH_TURN_INTERVAL) { // ms, duración de cada dirección
        turnDir *= -1;
        lastSwitch = millis();
    }

    float turn = SEARCH_TURN_FACTOR * turnDir;
    m_motorController->sameDirection(SEARCH_SPEED, turn);

    return COMPLETED;
}
