#include "StrategyMenu.h"

#include "Config.h"
#include "Battle/BattleEngine/BattleEngine.h"

void strategyMenu(uint16_t remoteCommand)
{
    switch (remoteCommand)
    {
    case REMOTE_CMD_STRATEGY_1:
        setActiveStrategy(0);
        break;
    case REMOTE_CMD_STRATEGY_2:
        setActiveStrategy(1);
        break;
    }
}