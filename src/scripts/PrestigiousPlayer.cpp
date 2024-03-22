#include "PrestigiousPlayer.h"

void PrestigiousPlayerScript::OnLevelChanged(Player* player, uint8 /*oldLevel*/)
{
    if (!player)
    {
        return;
    }

    if (player->GetLevel() >= sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
    {
        LOG_INFO("module", "Player hit max level.");
    }
}
