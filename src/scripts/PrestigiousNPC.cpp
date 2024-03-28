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
        SendGossipMenuFor(player, PRESTIGE_TEXT_CANNOT_PRESTIGE, creature);
        return true;
    }

    if (sPrestigeHandler->CanPrestige(player))
    {
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "I would like to prestige.", GOSSIP_SENDER_MAIN, PRESTIGE_DO_PRESTIGE, "Are you sure you want to prestige? |n|n This will kick you and lock your character while resetting your progress.", 0, false);

        SendGossipMenuFor(player, PRESTIGE_TEXT_CAN_PRESTIGE, creature);
    }
    else
    {
        SendGossipMenuFor(player, PRESTIGE_TEXT_CANNOT_PRESTIGE, creature);
    }

    return true;
}

bool PrestigiousNPCScript::OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action)
{
    if (!sConfigMgr->GetOption<bool>("Prestigious.Enable", false))
    {
        CloseGossipMenuFor(player);
        return true;
    }

    switch (action)
    {
    case PRESTIGE_DO_PRESTIGE:
        if (sPrestigeHandler->CanPrestige(player))
        {
            CloseGossipMenuFor(player);
            sPrestigeHandler->DoPrestige(player);
        }
        break;
    }

    return true;
}
