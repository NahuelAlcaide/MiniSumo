#include "DefensiveBlindSearch.h"

#include <Arduino.h>

#include "Config.h"

DefensiveBlindSearch::DefensiveBlindSearch(IMotorController* motorController) :
    BlindSearch(motorController)
{}

DefensiveBlindSearch::Status DefensiveBlindSearch::execute(const SensorData data) {
    enum State {
        BRAKE,
        TURNING,
        WAITING
    };

    static State currentState = WAITING;
    static unsigned long lastStateChange = millis();
    static int turnDir = 1; // 1 derecha, -1 izquierda.

    unsigned long currentTime = millis();

    switch (currentState) {
        case WAITING:
            m_motorController->limpMotors();
            if (currentTime - lastStateChange > DEFENSIVE_SEARCH_WAIT_TIME) {
                turnDir *= -1; // Cambiar dirección
                currentState = TURNING;
                lastStateChange = currentTime;
            }
            break;

        case TURNING:
            m_motorController->oppositeDirection(255 * turnDir);
            if (currentTime - lastStateChange > DEFENSIVE_SEARCH_TURN_TIME) {
                currentState = BRAKE;
                lastStateChange = currentTime;
            }
            break;

        case BRAKE:
            m_motorController->brake();
            if (currentTime - lastStateChange > DEFENSIVE_SEARCH_BRAKE_TIME) {
                currentState = WAITING;
                lastStateChange = currentTime;
            }
            break;
    }

    return COMPLETED;
}