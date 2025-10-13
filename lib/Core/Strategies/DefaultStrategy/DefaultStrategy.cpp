#include "DefaultStrategy.h"

#include <Arduino.h>

#include "Config.h"

#include "Behaviors/IBheavior.h"
#include "Behaviors/AttackBehaviors/Attack/StandardAttack.h"
#include "Behaviors/AttackBehaviors/Attack/ChargeAttack/ChargeAttack.h"
#include "Behaviors/SearchBehaviors/FocalizedSearch/FocalizedSearch.h"
#include "Behaviors/SearchBehaviors/BlindSearch/BlindSearch.h"
#include "Behaviors/LineEvadeBehaviors/StandardLineEvade/StandardLineEvade.h"

#include "../Util/Util/Util.h"

// The state enum's definition
enum class DefaultStrategyState : uint8_t {
    CHARGING,
    ATTACKING,
    EVADING_LINE,
    FOCALIZED_SEARCHING,
    BlIND_SEARCHING,
    COUNT
};

static const char* g_stateNames[] = {
    "CHARGING",
    "ATTACKING",
    "EVADING_LINE",
    "FOCALIZED_SEARCHING",
    "BLIND_SEARCHING"
};

// The constructor now works because the compiler can see that ISeekBehavior and
// IAttackBehavior are both valid types of IBehavior.
DefaultStrategy::DefaultStrategy(
    ISearchBehavior* blindSearch,
    ISearchBehavior* focalizedSearch,
    IAttackBehavior* standardAttack,
    IAttackBehavior* chargeAttack,
    ILineEvadeBehavior* standardLineEvade
    )
{
    m_currentState = DefaultStrategyState::BlIND_SEARCHING;
    m_behaviorTable[static_cast<size_t>(DefaultStrategyState::BlIND_SEARCHING)]     = blindSearch;
    m_behaviorTable[static_cast<size_t>(DefaultStrategyState::FOCALIZED_SEARCHING)] = focalizedSearch;
    m_behaviorTable[static_cast<size_t>(DefaultStrategyState::ATTACKING)]           = standardAttack;
    m_behaviorTable[static_cast<size_t>(DefaultStrategyState::CHARGING)]            = chargeAttack;
    m_behaviorTable[static_cast<size_t>(DefaultStrategyState::EVADING_LINE)]        = standardLineEvade;
}


static unsigned long g_attackStartTime;

void DefaultStrategy::init() {
    m_currentState = DefaultStrategyState::BlIND_SEARCHING;
    g_attackStartTime = 0;
}

const char* DefaultStrategy::getName() const {
    return "Default Array Strategy";
}

void DefaultStrategy::execute(const SensorData& data) {

    DefaultStrategyState suggestedState;

    // 1. STRATEGY'S JOB: Determine the highest-priority suggested state.
    if((millis() - g_attackStartTime > 2000 && data.center > ATTACK_THRESHOLD && g_attackStartTime != 0)){
        g_attackStartTime = 0;
        suggestedState = DefaultStrategyState::CHARGING;
    }else if(data.center > ATTACK_THRESHOLD) {
        if(g_attackStartTime == 0) {
            g_attackStartTime = millis();
        }
        suggestedState = DefaultStrategyState::ATTACKING;
    } else if(data.lineLeft < LINE_EVADE_THRESHOLD || data.lineRight < LINE_EVADE_THRESHOLD) {
        suggestedState = DefaultStrategyState::EVADING_LINE;
    } else if (data.left > SEEK_THRESHOLD || data.center > SEEK_THRESHOLD || data.right > SEEK_THRESHOLD) {
        suggestedState = DefaultStrategyState::FOCALIZED_SEARCHING;
    } else {
        suggestedState = DefaultStrategyState::BlIND_SEARCHING;
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
        if (isPriorityEscalation && m_currentState != DefaultStrategyState::ATTACKING) {
            g_attackStartTime = 0;
        }
        m_currentState = suggestedState;
    }
    // If neither condition is met, the current behavior holds its state.

    Serial.print("Current State: ");
    Serial.print(g_stateNames[static_cast<size_t>(m_currentState)]);
    printSensorReadout(data);
}

