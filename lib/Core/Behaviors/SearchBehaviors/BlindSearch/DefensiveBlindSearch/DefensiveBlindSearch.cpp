#include "DefensiveBlindSearch.h"

#include <Arduino.h>

#include "Config.h"

DefensiveBlindSearch::DefensiveBlindSearch(IMotorController* motorController) :
    BlindSearch(motorController)
{}

DefensiveBlindSearch::Status DefensiveBlindSearch::execute(const SensorData data) {
    static unsigned long lastSwitch = millis();
    static int turnDir = 1; // 1 derecha, -1 izquierda.

    if (millis() - lastSwitch > SEARCH_TURN_INTERVAL) { // ms, duración de cada dirección
        turnDir *= -1;
        lastSwitch = millis();
    }

    float turn = SEARCH_TURN_FACTOR * turnDir;
    m_motorController->oppositeDirection(SEARCH_SPEED, turn);

    return COMPLETED;
}