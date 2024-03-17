#include "PrestigiousNPC.h"

#include "Chat.h"
#include "GameTime.h"
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

    auto maxLevel = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);

    if (player->GetLevel() < maxLevel)
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

    // Lock the character incase the player tries relogging
    LockCharacter(guid);

    ChatHandler(player->GetSession()).SendSysMessage("|cffFFFFFF[Prestige]: |cff00FF00Prestiging character, please wait..|r");

    // Freeze the character in its current position
    player->AddAura(SPELL_FREEZE, player);

    // Save the current player state to DB
    player->SaveToDB(false, false);

    // Start prestige process
    ResetLevel(guid, isHeroClass);
    ResetSpells(guid);
    ResetQuests(guid);
    ResetHomebindAndPosition(guid, pRace, pClass);

    StoreAllItems(guid);

    // Unlock the character
    UnlockCharacter(guid);

    ChatHandler(player->GetSession()).SendSysMessage("|cffFFFFFF[Prestige]: |cff00FF00Prestige complete, logging out..|r");


    // TODO: THIS CRASHES WORLDSERVER
    scheduler.Schedule(3s, [player](TaskContext /*context*/)
    {
        if (!player ||
            !player->GetSession() ||
            !player->IsInWorld())
        {
            return;
        }

        // Unfreeze the character
        player->RemoveAura(SPELL_FREEZE);

        player->GetSession()->SetLogoutStartTime(GameTime::GetGameTime().count());

        // Log the player out without saving
        player->GetSession()->LogoutPlayer(false);
    });
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

void PrestigiousNPCScript::StoreAllItems(ObjectGuid guid)
{
    // Save these item guids to a separate table to be able to retrieve them at max level via mail.
    {
        QueryResult result = CharacterDatabase.Query("SELECT item FROM character_inventory WHERE guid = {}", guid.GetRawValue());

        if (!result)
        {
            return;
        }

        do
        {
            auto fields = result->Fetch();

            auto itemGuid = fields[0].Get<uint32>();

            CharacterDatabase.Execute("INSERT INTO prestige_backup_inventory (player_guid, item_guid) VALUES ({}, {})", guid.GetRawValue(), itemGuid);
        } while (result->NextRow());
    }

    // Clean all references to items in inventory (bags, bank, etc..)
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_INVENTORY);
        stmt->SetData(0, guid.GetCounter());
        CharacterDatabase.Execute(stmt);
    }
}

void PrestigiousNPCScript::OnUpdate(Creature* /*obj*/, uint32 diff)
{
    scheduler.Update(diff);
}
