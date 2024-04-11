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

bool PrestigiousPlayerScript::CanEquipItem(Player* player, uint8 /*slot*/, uint16& /*dest*/, Item* pItem, bool /*swap*/, bool /*not_loading*/)
{
    if (!player || !pItem)
    {
        return true;
    }

    if (pItem->HasFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_UNK26))
    {
        player->SendSystemMessage("You cannot equip this item since you acquired it from a previous playthrough. Reach max level to unlock the item.");
        return false;
    }

    return true;
}
