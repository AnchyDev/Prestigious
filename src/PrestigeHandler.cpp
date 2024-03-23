#include "PrestigeHandler.h"

bool PrestigeHandler::CanPrestige(Player* player)
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

void PrestigeHandler::DoPrestige(Player* player)
{
    if (!player ||
        !player->GetGUID() ||
        !player->GetSession())
    {
        LOG_ERROR("module", "Failed to get player session during prestige process.");
        return;
    }

    auto accountId = player->GetSession()->GetAccountId();
    auto guid = player->GetGUID();

    uint32 pRace = player->getRace();
    uint32 pClass = player->getClass();
    uint32 isHeroClass = pClass == CLASS_DEATH_KNIGHT;

    // Lock the character incase the player tries relogging
    LockCharacter(guid);

    // Save the current player state to DB and logout.
    player->SaveToDB(false, false);
    player->GetSession()->LogoutPlayer(false);

    // Schedule for later in the future in the case of race condition.
    Scheduler.Schedule(3s, [this, guid, isHeroClass, pRace, pClass](TaskContext context)
    {
        // Start prestige process
        ResetLevel(guid, isHeroClass);
        ResetSpells(guid);
        ResetQuests(guid);
        ResetHomebindAndPosition(guid, pRace, pClass);

        StoreAllItems(guid);

        // Unlock the character
        UnlockCharacter(guid);
    });
}

void PrestigeHandler::LockCharacter(ObjectGuid guid)
{
    LOG_INFO("module", "Prestige> Locking character..");

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_BAN);
    stmt->SetData(0, guid.GetCounter());
    stmt->SetData(1, 24 * HOUR);
    stmt->SetData(2, "Prestige");
    stmt->SetData(3, "Locked character during prestige process.");
    CharacterDatabase.Execute(stmt);

    LOG_INFO("module", "Prestige> Character locked.");
}

void PrestigeHandler::UnlockCharacter(ObjectGuid guid)
{
    LOG_INFO("module", "Prestige> Unlocking character..");

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_BAN);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    LOG_INFO("module", "Prestige> Character unlocked.");
}

void PrestigeHandler::ResetLevel(ObjectGuid guid, bool isHeroClass)
{
    LOG_INFO("module", "Prestige> Resetting player level..");

    uint32 level = isHeroClass ? 55 : 1;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_LEVEL);
    stmt->SetData(0, uint8(level));
    stmt->SetData(1, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    sCharacterCache->UpdateCharacterLevel(guid, level);

    LOG_INFO("module", "Prestige> Player level reset to '{}'.", level);
}

void PrestigeHandler::ResetSpells(ObjectGuid guid)
{
    LOG_INFO("module", "Prestige> Resetting player spells..");

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SPELL);
    stmt->SetData(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    LOG_INFO("module", "Prestige> Player spells reset.");
}

void PrestigeHandler::ResetQuests(ObjectGuid guid)
{
    LOG_INFO("module", "Prestige> Resetting player quest status..");

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

    LOG_INFO("module", "Prestige> Player quest status reset.");
}

void PrestigeHandler::ResetHomebindAndPosition(ObjectGuid guid, uint32 pRace, uint32 pClass)
{
    LOG_INFO("module", "Prestige> Resetting player homebind and position..");

    QueryResult result = WorldDatabase.Query("SELECT map, zone, position_x, position_y, position_z, orientation FROM playercreateinfo WHERE race = {} AND class = {}", pRace, pClass);

    if (!result)
    {
        LOG_INFO("module", "Prestige> No default homebind position found, skipping..");
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

    LOG_INFO("module", "Prestige> Player homebind and position reset.");
}

void PrestigeHandler::StoreAllItems(ObjectGuid guid)
{
    LOG_INFO("module", "Prestige> Storing player items..");

    // Save these item guids to a separate table to be able to retrieve them at max level via mail.
    {
        QueryResult result = CharacterDatabase.Query("SELECT item FROM character_inventory WHERE guid = {}", guid.GetRawValue());

        if (!result)
        {
            LOG_INFO("module", "Prestige> No items to store, skipping..");
            return;
        }

        do
        {
            auto fields = result->Fetch();

            auto itemGuid = fields[0].Get<uint32>();

            CharacterDatabase.Execute("INSERT INTO prestige_backup_inventory (player_guid, item_guid) VALUES ({}, {})", guid.GetRawValue(), itemGuid);
        } while (result->NextRow());
    }

    LOG_INFO("module", "Prestige> Removing references to items..");
    // Clean all references to items in inventory (bags, bank, etc..)
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_INVENTORY);
        stmt->SetData(0, guid.GetCounter());
        CharacterDatabase.Execute(stmt);
    }

    LOG_INFO("module", "Prestige> Player items reset.");
}
