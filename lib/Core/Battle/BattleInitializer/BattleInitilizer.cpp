#include "BattleInitilizer.h"
#include <Arduino.h>

#include "Config.h"
#include "Battle/InitialMove/InitialMove.h"
#include "Led/Led.h"
#include "Sensors/Sensors.h"

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
            if(data.left > F_SEARCH_THRESHOLD || data.center > F_SEARCH_THRESHOLD || data.right > F_SEARCH_THRESHOLD){
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
