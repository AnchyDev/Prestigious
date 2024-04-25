#include "PrestigiousPlayer.h"
#include "PrestigeHandler.h"

#include "Config.h"
#include <ScriptedGossip.h>

void PrestigiousPlayerScript::OnLevelChanged(Player* player, uint8 /*oldLevel*/)
{
    if (!sConfigMgr->GetOption<bool>("Prestigious.Enable", true))
    {
        return;
    }

    if (!player)
    {
        return;
    }

    if (player->GetLevel() < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
    {
        return;
    }

    sPrestigeHandler->UnflagItems(player);
}

bool PrestigiousPlayerScript::CanEquipItem(Player* player, uint8 /*slot*/, uint16& /*dest*/, Item* pItem, bool /*swap*/, bool /*not_loading*/)
{
    if (!sConfigMgr->GetOption<bool>("Prestigious.Enable", true))
    {
        return true;
    }

    if (!player || !pItem)
    {
        return true;
    }

    if (sPrestigeHandler->IsHeirloom(pItem))
    {
        return true;
    }

    if (pItem->HasFlag(ITEM_FIELD_FLAGS, sPrestigeHandler->ITEM_FIELD_FLAG_PRESTIGE_LOCK) &&
        player->GetLevel() < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
    {
        player->SendSystemMessage("You cannot equip this item since you acquired it from a previous playthrough. Reach max level to unlock the item.");
        return false;
    }

    return true;
}

void PrestigiousPlayerScript::OnEquip(Player* player, Item* /*it*/, uint8 /*bag*/, uint8 /*slot*/, bool /*update*/)
{
    if (!sConfigMgr->GetOption<bool>("Prestigious.Enable", true))
    {
        return;
    }

    if (!player)
    {
        return;
    }

    // Avoids exploits with switching gear while menu is open
    ClearGossipMenuFor(player);
    CloseGossipMenuFor(player);
}
