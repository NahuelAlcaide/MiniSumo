#include "Strategy_2.h"

#include <stddef.h>
#include <Arduino.h>

#include "Config.h"

#include "Behaviors/IBheavior.h"
#include "Behaviors/AttackBehaviors/Attack/StandardAttack.h"
#include "Behaviors/AttackBehaviors/Attack/ChargeAttack/ChargeAttack.h"
#include "Behaviors/SearchBehaviors/FocalizedSearch/FocalizedSearch.h"
#include "Behaviors/SearchBehaviors/BlindSearch/BlindSearch.h"
#include "Behaviors/LineEvadeBehaviors/StandardLineEvade/StandardLineEvade.h"

#include "Debug/debug.h"

// The state enum's definition
enum class Strategy_2State : uint8_t {
    CHARGING,
    ATTACKING,
    EVADING_LINE,
    FOCALIZED_SEARCHING,
    BlIND_SEARCHING,
    COUNT
};

static const char* g_stateNames[] = {
    "CHARGING_2",
    "ATTACKING_2",
    "EVADING_LINE_2",
    "FOCALIZED_SEARCHING_2",
    "BLIND_SEARCHING_2"
};

// The constructor now works because the compiler can see that ISeekBehavior and
// IAttackBehavior are both valid types of IBehavior.
Strategy_2::Strategy_2(
    ISearchBehavior* blindSearch,
    ISearchBehavior* focalizedSearch,
    IAttackBehavior* standardAttack,
    IAttackBehavior* chargeAttack,
    ILineEvadeBehavior* standardLineEvade
    )
{
    m_currentState = Strategy_2State::BlIND_SEARCHING;
    m_behaviorTable[static_cast<size_t>(Strategy_2State::BlIND_SEARCHING)]     = blindSearch;
    m_behaviorTable[static_cast<size_t>(Strategy_2State::FOCALIZED_SEARCHING)] = focalizedSearch;
    m_behaviorTable[static_cast<size_t>(Strategy_2State::ATTACKING)]           = standardAttack;
    m_behaviorTable[static_cast<size_t>(Strategy_2State::CHARGING)]            = chargeAttack;
    m_behaviorTable[static_cast<size_t>(Strategy_2State::EVADING_LINE)]        = standardLineEvade;
}


static unsigned long g_attackStartTime;

void Strategy_2::init() {
    m_currentState = Strategy_2State::BlIND_SEARCHING;
    g_attackStartTime = 0;
}

const char* Strategy_2::getName() const {
    return "Default Array Strategy";
}

void Strategy_2::execute(const SensorData& data) {

    Strategy_2State suggestedState;

    // 1. STRATEGY'S JOB: Determine the highest-priority suggested state.
    if((millis() - g_attackStartTime > 2000 && data.center > ATTACK_THRESHOLD && g_attackStartTime != 0)){
        g_attackStartTime = 0;
        suggestedState = Strategy_2State::CHARGING;
    }else if(data.center > ATTACK_THRESHOLD) {
        if(g_attackStartTime == 0) {
            g_attackStartTime = millis();
        }
        suggestedState = Strategy_2State::ATTACKING;
    } else if(data.lineLeft < LINE_EVADE_THRESHOLD || data.lineRight < LINE_EVADE_THRESHOLD) {
        suggestedState = Strategy_2State::EVADING_LINE;
    } else if (data.left > SEEK_THRESHOLD || data.center > SEEK_THRESHOLD || data.right > SEEK_THRESHOLD) {
        suggestedState = Strategy_2State::FOCALIZED_SEARCHING;
    } else {
        suggestedState = Strategy_2State::BlIND_SEARCHING;
    }

    // 2. BEHAVIOR'S JOB: Execute and report its status.
    IBehavior::Status status = m_behaviorTable[static_cast<size_t>(m_currentState)]->execute(data);

    // 3. FINAL DECISION: Check the status AND the priority.
    // The enum's integer values define the priority (lower value = higher priority).
    bool isPriorityEscalation = (suggestedState < m_currentState);

    // A state transition happens if:
    // A) The current behavior completes its task, OR
    // B) A higher-priority state is suggested.
    if (status == IBehavior::COMPLETED || isPriorityEscalation) {
        // If we are escalating priority, reset the timer for the new state.
        if (isPriorityEscalation && m_currentState != Strategy_2State::ATTACKING) {
            g_attackStartTime = 0;
        }
        m_currentState = suggestedState;
    }
    // If neither condition is met, the current behavior holds its state.

    Serial.print("Current State: ");
    Serial.print(g_stateNames[static_cast<size_t>(m_currentState)]);
    printSensorReadout(data);
}

