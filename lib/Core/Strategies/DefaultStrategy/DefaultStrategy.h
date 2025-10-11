#pragma once
#include <stdint.h>

#include "Behaviors/LineEvadeBehaviors/ILineEvadeBehavior.h"
#include "Behaviors/SearchBehaviors/ISearchBehavior.h"
#include "Strategies/IStrategy.h"

// Forward declarations
enum class DefaultStrategyState : uint8_t;
class IBehavior;
class IAttackBehavior;
class ISearchBehavior;
class ILineEvadeBehavior;

class DefaultStrategy : public IStrategy {
public:
    DefaultStrategy(
        ISearchBehavior* blindSearch,
        ISearchBehavior* focalizedSearch,
        IAttackBehavior* standardAttack,
        IAttackBehavior* chargeAttack,
        ILineEvadeBehavior* standardLineEvade
    );

    // Implementing the IStrategy interface remains the same
    const char* getName() const override;
    void init() override;
    void execute(const SensorData& data) override;

private:
    // This array will hold our behavior pointers. Its size is determined at compile time.
    // The implementation details are now hidden in the .cpp file.
    IBehavior* m_behaviorTable[5]{}; // The size must match the number of states

    DefaultStrategyState m_currentState;
};