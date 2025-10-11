#include "battleInitilizer.h"
#include <arduino.h>

#include "config.h"
#include "Battle/InitialMove/initialMove.h"
#include "led/led.h"
#include "Sensors/sensors.h"

static battleInitState g_battleInitState = BATTLE_INIT_STATE_IDLE;

void resetBattleInitState() {
    g_battleInitState = BATTLE_INIT_STATE_IDLE;
}

bool execBattleInit(SensorData data)
{
    switch(g_battleInitState) {
        case BATTLE_INIT_STATE_IDLE:
            Serial.println("Battle mode.");
            g_battleInitState = BATTLE_INIT_STATE_COUNTDOWN;
            return false;
        case BATTLE_INIT_STATE_COUNTDOWN:
            Serial.println("Countdown start.");
            ledBlinkQuick();
            delay(5000);
            g_battleInitState = BATTLE_INIT_STATE_INITIAL_MOVE;
            static unsigned long startTime = millis();
        return false;
        case BATTLE_INIT_STATE_INITIAL_MOVE:
            static unsigned int turnTime = execInitialMove();
            if(data.left > SEEK_THRESHOLD || data.center > SEEK_THRESHOLD || data.right > SEEK_THRESHOLD){
                g_battleInitState = BATTLE_INIT_STATE_BATTLE;
                return false;
            }else if (millis() - startTime  >= turnTime) {
                g_battleInitState = BATTLE_INIT_STATE_BATTLE;
                return false;
            } else {
                return false;
            }
        case BATTLE_INIT_STATE_BATTLE:
            return true;
    }
    return false;
}
