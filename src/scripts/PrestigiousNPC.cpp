#include "PrestigiousNPC.h"
#include "PrestigeHandler.h"

#include "Chat.h"
#include "Config.h"
#include "GameTime.h"
#include "Player.h"
#include "ScriptedGossip.h"

bool PrestigiousNPCScript::OnGossipHello(Player* player, Creature* creature)
{
    ClearGossipMenuFor(player);

    if (!sConfigMgr->GetOption<bool>("Prestigious.Enable", false))
    {
        CloseGossipMenuFor(player);
        return false;
    }

    if (sPrestigeHandler->CanPrestige(player))
    {
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "I would like to prestige.", GOSSIP_SENDER_MAIN, PRESTIGE_OPTIONS);

        SendGossipMenuFor(player, PRESTIGE_TEXT_CAN_PRESTIGE, creature);
    }
    else
    {
        SendGossipMenuFor(player, PRESTIGE_TEXT_CANNOT_PRESTIGE, creature);
    }

    return true;
}

bool PrestigiousNPCScript::OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
{
    if (!player)
    {
        return false;
    }

    if (!sConfigMgr->GetOption<bool>("Prestigious.Enable", false))
    {
        CloseGossipMenuFor(player);
        return false;
    }

    switch (action)
    {
    case PRESTIGE_OPTIONS:
    {
        ClearGossipMenuFor(player);

        auto itemLevel = player->GetAverageItemLevel();
        bool isDeathKnight = player->getClass() == CLASS_DEATH_KNIGHT;

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, Acore::StringFormatFmt("I would like to keep my equipment|n|cffFF0000({}x Multiplier)|r", sPrestigeHandler->GetBaseMultiplier(isDeathKnight)), GOSSIP_SENDER_MAIN, PRESTIGE_DO_PRESTIGE, "Are you sure you would like to prestige?|n|nThis will reset your level and quest progress.", 0, false);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, Acore::StringFormatFmt("I would like to sacrifice my equipment|n|cffFF0000({} Item Level, {}x Multiplier)|r", itemLevel, sPrestigeHandler->GetMultiplierForItemLevel(itemLevel, isDeathKnight)), GOSSIP_SENDER_MAIN, PRESTIGE_DO_PRESTIGE_SACRIFICE, "Are you sure you would like to prestige?|n|nThis will reset your level and progress.|n|n|cffFF0000THIS OPTION WILL DELETE YOUR EQUIPMENT, YOU CANNOT UNDO THIS", 0, false);

        SendGossipMenuFor(player, PRESTIGE_TEXT_OPTIONS, creature);
    }
        break;
    case PRESTIGE_DO_PRESTIGE:
        if (sPrestigeHandler->CanPrestige(player))
        {
            CloseGossipMenuFor(player);
            sPrestigeHandler->DoPrestige(player, false);
        }
        break;
    case PRESTIGE_DO_PRESTIGE_SACRIFICE:
        if (sPrestigeHandler->CanPrestige(player))
        {
            CloseGossipMenuFor(player);
            sPrestigeHandler->DoPrestige(player, true);
        }
        break;
    }

    return true;
}
