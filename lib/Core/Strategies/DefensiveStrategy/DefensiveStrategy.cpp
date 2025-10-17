#include "DefensiveStrategy.h"

#include <Arduino.h>

#include "Config.h"

#include "Behaviors/IBheavior.h"
#include "Behaviors/AttackBehaviors/Attack/StandardAttack.h"
#include "Behaviors/AttackBehaviors/Attack/ChargeAttack/ChargeAttack.h"
#include "Behaviors/SearchBehaviors/FocalizedSearch/FocalizedSearch.h"
#include "Behaviors/SearchBehaviors/BlindSearch/DefensiveBlindSearch/DefensiveBlindSearch.h"
#include "Behaviors/LineEvadeBehaviors/StandardLineEvade/StandardLineEvade.h"

#include "../Util/Util/Util.h"

// The state enum's definition
enum class DefensiveStrategyState : uint8_t {
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
    "DEFENSIVE_BLIND_SEARCHING"
};

DefensiveStrategy::DefensiveStrategy(
    ISearchBehavior* DefensiveBlindSearch,
    ISearchBehavior* FocalizedSearch,
    IAttackBehavior* StandardAttack,
    IAttackBehavior* ChargeAttack,
    ILineEvadeBehavior* StandardLineEvade
    )
{
    m_currentState = DefensiveStrategyState::BlIND_SEARCHING;
    m_behaviorTable[static_cast<size_t>(DefensiveStrategyState::BlIND_SEARCHING)]     = DefensiveBlindSearch;
    m_behaviorTable[static_cast<size_t>(DefensiveStrategyState::FOCALIZED_SEARCHING)] = FocalizedSearch;
    m_behaviorTable[static_cast<size_t>(DefensiveStrategyState::ATTACKING)]           = StandardAttack;
    m_behaviorTable[static_cast<size_t>(DefensiveStrategyState::CHARGING)]            = ChargeAttack;
    m_behaviorTable[static_cast<size_t>(DefensiveStrategyState::EVADING_LINE)]        = StandardLineEvade;
}


static unsigned long g_attackStartTime;

void DefensiveStrategy::init() {
    m_currentState = DefensiveStrategyState::BlIND_SEARCHING;
    g_attackStartTime = 0;
}

const char* DefensiveStrategy::getName() const {
    return "Default Array Strategy";
}

void DefensiveStrategy::execute(const SensorData& data) {

    DefensiveStrategyState suggestedState;

    // 1. STRATEGY'S JOB: Determine the highest-priority suggested state.
    if((millis() - g_attackStartTime > 2000 && data.center > ATTACK_THRESHOLD && g_attackStartTime != 0)){
        g_attackStartTime = 0;
        suggestedState = DefensiveStrategyState::CHARGING;
    }else if(data.center > ATTACK_THRESHOLD) {
        if(g_attackStartTime == 0) {
            g_attackStartTime = millis();
        }
        suggestedState = DefensiveStrategyState::ATTACKING;
    } else if(data.lineLeft < LINE_EVADE_THRESHOLD || data.lineRight < LINE_EVADE_THRESHOLD) {
        suggestedState = DefensiveStrategyState::EVADING_LINE;
    } else if (data.left > F_SEARCH_THRESHOLD || data.center > F_SEARCH_THRESHOLD || data.right > F_SEARCH_THRESHOLD) {
        suggestedState = DefensiveStrategyState::FOCALIZED_SEARCHING;
    } else {
        suggestedState = DefensiveStrategyState::BlIND_SEARCHING;
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
        if (isPriorityEscalation && m_currentState != DefensiveStrategyState::ATTACKING) {
            g_attackStartTime = 0;
        }
        m_currentState = suggestedState;
    }
    // If neither condition is met, the current behavior holds its state.
}

const char* DefensiveStrategy::getCurrentStateName() const {
    auto stateIndex = static_cast<size_t>(m_currentState);
    if (stateIndex < static_cast<size_t>(DefensiveStrategyState::COUNT)) {
        return g_stateNames[stateIndex];
    }
    return "UNKNOWN";
}

uint8_t DefensiveStrategy::getCurrentStateId() const {
    return static_cast<uint8_t>(m_currentState);
}

