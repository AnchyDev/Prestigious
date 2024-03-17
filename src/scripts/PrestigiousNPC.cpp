#include "PrestigiousNPC.h"

#include "Player.h"
#include "ScriptedGossip.h"

bool PrestigiousNPCScript::OnGossipHello(Player* player, Creature* creature)
{
    ClearGossipMenuFor(player);

    if (CanPrestige(player))
    {
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "I would like to prestige.", GOSSIP_SENDER_MAIN, PRESTIGE_DO_PRESTIGE, "Are you sure you want to prestige? |n|n This will kick you while resetting your level and all quests.", 0, false);

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
    switch (action)
    {
    case PRESTIGE_DO_PRESTIGE:
        if (CanPrestige(player))
        {
            CloseGossipMenuFor(player);
            DoPrestige(player);
        }
        break;
    }

    return true;
}

bool PrestigiousNPCScript::CanPrestige(Player* player)
{
    if (!player)
    {
        return false;
    }

    if (player->GetLevel() < 80)
    {
        return false;
    }

    return true;
}

void PrestigiousNPCScript::DoPrestige(Player* player)
{
    if (!player ||
        !player->GetGUID() ||
        !player->GetSession())
    {
        return;
    }

    auto accountId = player->GetSession()->GetAccountId();
    auto guid = player->GetGUID();

    uint32 pRace = player->getRace();
    uint32 pClass = player->getClass();
    uint32 isHeroClass = pClass == CLASS_DEATH_KNIGHT;

    LockCharacter(guid);

    // Kick the player to start the prestige process.
    player->GetSession()->LogoutPlayer(true);

    ResetLevel(guid, isHeroClass);
    ResetSpells(guid);
    ResetQuests(guid);
    ResetHomebindAndPosition(guid, pRace, pClass);

    UnlockCharacter(guid);
}

void PrestigiousNPCScript::LockCharacter(ObjectGuid guid)
{
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_BAN);
    stmt->SetData(0, guid.GetCounter());
    stmt->SetData(1, 24 * HOUR);
    stmt->SetData(2, "Prestige");
    stmt->SetData(3, "Locked character during prestige process.");
    CharacterDatabase.Execute(stmt);
}

void PrestigiousNPCScript::UnlockCharacter(ObjectGuid guid)
{
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_BAN);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);
}

void PrestigiousNPCScript::ResetLevel(ObjectGuid guid, bool isHeroClass)
{
    uint32 level = isHeroClass ? 55 : 1;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_LEVEL);
    stmt->SetData(0, uint8(level));
    stmt->SetData(1, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    sCharacterCache->UpdateCharacterLevel(guid, level);
}

void PrestigiousNPCScript::ResetSpells(ObjectGuid guid)
{
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SPELL);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);
}

void PrestigiousNPCScript::ResetQuests(ObjectGuid guid)
{
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_QUESTSTATUS);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_QUESTSTATUS_REWARDED);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_SEASONAL_CHAR);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_DAILY_CHAR);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_WEEKLY_CHAR);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_QUEST_STATUS_MONTHLY_CHAR);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);
}

void PrestigiousNPCScript::ResetHomebindAndPosition(ObjectGuid guid, uint32 pRace, uint32 pClass)
{
    QueryResult result = WorldDatabase.Query("SELECT map, zone, position_x, position_y, position_z, orientation FROM playercreateinfo WHERE race = {} AND class = {}", pRace, pClass);

    if (!result)
    {
        return;
    }

    auto fields = result->Fetch();

    auto mapId = fields[0].Get<uint32>();
    auto zoneId = fields[1].Get<uint32>();
    auto posX = fields[2].Get<float>();
    auto posY = fields[3].Get<float>();
    auto posZ = fields[4].Get<float>();
    auto posO = fields[5].Get<float>();

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_HOMEBIND);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_HOMEBIND);
    stmt->SetData(0, guid.GetCounter());
    stmt->SetData(1, mapId);
    stmt->SetData(2, zoneId);
    stmt->SetData(3, posX);
    stmt->SetData(4, posY);
    stmt->SetData(5, posZ);
    stmt->SetData(6, posO);
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_POSITION);
    stmt->SetData(0, posX);
    stmt->SetData(1, posY);
    stmt->SetData(2, posZ);
    stmt->SetData(3, posO);
    stmt->SetData(4, mapId);
    stmt->SetData(5, zoneId);
    stmt->SetData(6, guid.GetCounter());
    CharacterDatabase.Execute(stmt);
}
