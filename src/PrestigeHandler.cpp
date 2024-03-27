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

    ResetLevel(player);
    //ResetSpells(player);
    ResetQuests(player);
    ResetHomebindAndPosition(player);
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

void PrestigeHandler::ResetLevel(Player* player)
{
    LOG_INFO("module", "Prestige> Resetting player level..");

    uint32 isHeroClass = player->getClass() == CLASS_DEATH_KNIGHT;
    uint32 level = isHeroClass ? 55 : 1;

    player->SetLevel(level, true);
    player->InitStatsForLevel(true);

    LOG_INFO("module", "Prestige> Player level reset to '{}'.", level);
}

void PrestigeHandler::ResetSpells(Player* player)
{
    LOG_INFO("module", "Prestige> Resetting player spells..");

    auto spellMap = player->GetSpellMap();
    for (auto& spellEntry : spellMap)
    {
        auto spellId = spellEntry.first;

        player->removeSpell(spellId, SPEC_MASK_ALL, false);
    }

    player->LearnDefaultSkills();
    player->LearnCustomSpells();

    LOG_INFO("module", "Prestige> Player spells reset.");
}

void PrestigeHandler::ResetQuests(Player* player)
{
    LOG_INFO("module", "Prestige> Resetting player quest status..");

    player->ResetDailyQuestStatus();
    player->ResetWeeklyQuestStatus();
    player->ResetMonthlyQuestStatus();

    // Reset all previously completed quests
    auto quests = player->getRewardedQuests();
    for (auto& quest : quests)
    {
        if (!quest)
        {
            continue;
        }

        player->SetQuestStatus(quest, QUEST_STATUS_NONE, true);
        player->RemoveRewardedQuest(quest, true);
    }

    // Purge quest log
    for (auto questSlot = 0; questSlot < MAX_QUEST_LOG_SIZE; questSlot++)
    {
        auto quest = player->GetQuestSlotQuestId(questSlot);
        if (!quest)
        {
            continue;
        }

        player->TakeQuestSourceItem(quest, false);
        player->AbandonQuest(quest);
        player->RemoveActiveQuest(quest, true);
        player->RemoveTimedAchievement(ACHIEVEMENT_TIMED_TYPE_QUEST, quest);

        player->SetQuestSlot(questSlot, 0);
        player->SetQuestSlotState(questSlot, QUEST_STATE_NONE);
    }

    LOG_INFO("module", "Prestige> Player quest status reset.");
}

void PrestigeHandler::ResetHomebindAndPosition(Player* player)
{
    LOG_INFO("module", "Prestige> Resetting player homebind and position..");

    auto playerInfo = sObjectMgr->GetPlayerInfo(player->getRace(), player->getClass());

    WorldLocation worldLoc(playerInfo->mapId, playerInfo->positionX, playerInfo->positionY, playerInfo->positionZ, playerInfo->orientation);

    player->SetHomebind(worldLoc, playerInfo->areaId);

    // Update Hearthstone binding point for UI
    {
        WorldPacket data(SMSG_BINDPOINTUPDATE, 4 + 4 + 4 + 4 + 4);

        data << float(worldLoc.GetPositionX());
        data << float(worldLoc.GetPositionY());
        data << float(worldLoc.GetPositionZ());
        data << uint32(worldLoc.GetMapId());
        data << uint32(playerInfo->areaId);

        player->SendDirectMessage(&data);
    }

    player->TeleportTo(worldLoc);

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

void PrestigeHandler::AddDefaultItems(ObjectGuid guid, uint8 pRace, uint8 pClass, uint8 pGender)
{
    auto startOutfit = GetCharStartOutfitEntry(pRace, pClass, pGender);

    for (uint8 i = 0; i < MAX_OUTFIT_ITEMS; ++i)
    {
        auto itemEntry = startOutfit->ItemId[i];

        if (!itemEntry ||
            itemEntry == -1)
        {
            continue;
        }

        auto item = Item::CreateItem(itemEntry, 1);
        auto itemGuid = item->GetGUID();

        auto equipSlot = InventoryTypeToEquipSlot(item->GetTemplate()->InventoryType);

        if (equipSlot == -1)
        {
            LOG_WARN("module", "Failed to find a valid equip slot for item {} for guid {}.", item->GetEntry(), guid.GetRawValue());
            continue;
        }
        
        CharacterDatabase.Execute("INSERT INTO character_inventory (guid, bag, slot, item) VALUES ({}, {}, {}, {})", guid.GetRawValue(), 0, equipSlot, itemGuid.GetCounter());

        LOG_INFO("module", "Created default item {} with guid {}.", item->GetTemplate()->Name1, itemGuid.GetCounter());
    }

    // TODO: Add the items to play.
}

int32 PrestigeHandler::InventoryTypeToEquipSlot(uint32 invType)
{
    switch (invType)
    {
        case INVTYPE_HEAD:
            return EQUIPMENT_SLOT_HEAD;

        case INVTYPE_NECK:
            return EQUIPMENT_SLOT_NECK;

        case INVTYPE_SHOULDERS:
            return EQUIPMENT_SLOT_SHOULDERS;

        case INVTYPE_CLOAK:
            return EQUIPMENT_SLOT_BACK;

        case INVTYPE_CHEST:
            return EQUIPMENT_SLOT_CHEST;

        case INVTYPE_BODY:
            return EQUIPMENT_SLOT_BODY;

        case INVTYPE_WRISTS:
            return EQUIPMENT_SLOT_WRISTS;

        case INVTYPE_HANDS:
            return EQUIPMENT_SLOT_HANDS;

        case INVTYPE_WAIST:
            return EQUIPMENT_SLOT_WAIST;

        case INVTYPE_LEGS:
            return EQUIPMENT_SLOT_LEGS;

        case INVTYPE_FEET:
            return EQUIPMENT_SLOT_FEET;
    }

    return -1;
}
