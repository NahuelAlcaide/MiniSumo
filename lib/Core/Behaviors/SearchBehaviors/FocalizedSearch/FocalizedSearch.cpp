#include "FocalizedSearch.h"

#include <Arduino.h>
#include "Config.h"
#include "../Hardware/Sensors/Sensors.h"

static FocalizedSearch::Status g_status = FocalizedSearch::COMPLETED;

static FocalizedSearch::lastDirection lastDir = FocalizedSearch::NONE;


FocalizedSearch::FocalizedSearch(IMotorController* motorController) :
    m_motorController(motorController)
{}

FocalizedSearch::Status FocalizedSearch::execute(const SensorData data) {
    static unsigned long lastSeenTime = 0;

    g_status = RUNNING;

    // Chekear sensores
    if (data.center > F_SEARCH_THRESHOLD) {
        lastDir = CENTER;
        lastSeenTime = millis();
    }else if (data.left > F_SEARCH_THRESHOLD) {
        lastDir = LEFT;
        lastSeenTime = millis();
    } else if (data.right > F_SEARCH_THRESHOLD) {
        lastDir = RIGHT;
        lastSeenTime = millis();
    }

    // continuar girando en la última dirección que leyó por encima del threshold mientras el timeout no se acabe
    if (lastDir != NONE && (millis() - lastSeenTime < F_SEARCH_TURN_TIMEOUT))
    {
        int motorSpeed = F_SEARCH_SPEED;
        switch(lastDir) {
        case RIGHT:
            m_motorController->sameDirection(motorSpeed, F_SEARCH_TURN_RATE);
            break;
        case CENTER:
            m_motorController->sameDirection(motorSpeed, 0);
            break;
        case LEFT:
            m_motorController->sameDirection(motorSpeed, -F_SEARCH_TURN_RATE);
            break;
        }
    } else {
        lastDir = NONE;
        g_status = COMPLETED;
    }
    return g_status;
}