#include "BattleInitilizer.h"
#include <Arduino.h>

#include "Config.h"
#include "Battle/BattleEngine/BattleEngine.h"
#include "Battle/InitialMove/InitialMove.h"
#include "Led/Led.h"
#include "Sensors/Sensors.h"

static battleInitState g_battleInitState = BATTLE_INIT_STATE_IDLE;

static int g_initialMoveTime = 0;
static unsigned long g_initialMoveStartTime = getInitialMoveTime();

void resetBattleInitState() {
    resetInitialMove();
    g_battleInitState = BATTLE_INIT_STATE_IDLE;
    g_initialMoveStartTime = 0;
}

bool execBattleInit(SensorData data)
{
    switch(g_battleInitState) {
        case BATTLE_INIT_STATE_IDLE:
            Serial.println("Battle mode.");
            g_initialMoveTime = getInitialMoveTime();
            g_battleInitState = BATTLE_INIT_STATE_COUNTDOWN;
            return false;
        case BATTLE_INIT_STATE_COUNTDOWN:
            Serial.println("Countdown start.");
            ledBlinkQuick();
            delay(5000);
            g_battleInitState = BATTLE_INIT_STATE_INITIAL_MOVE;
            g_initialMoveStartTime = millis();
            return false;
        case BATTLE_INIT_STATE_INITIAL_MOVE:
            execInitialMove();
            if(data.left > F_SEARCH_THRESHOLD || data.center > F_SEARCH_THRESHOLD || data.right > F_SEARCH_THRESHOLD){
                g_battleInitState = BATTLE_INIT_STATE_BATTLE;
            }
            if (millis() - g_initialMoveStartTime  >= g_initialMoveTime) {
                g_battleInitState = BATTLE_INIT_STATE_BATTLE;
            }
            return false;
        case BATTLE_INIT_STATE_BATTLE:
            return true;
    }
    return false;
}
