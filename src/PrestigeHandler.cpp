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

    DeleteItems(player);
    EquipDefaultItems(player);
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

void PrestigeHandler::DeleteItems(Player* player)
{
    LOG_INFO("module", "Prestige> Deleting player items..");

    // Delete equipped items.
    for (uint32 i = 0; i < INVENTORY_SLOT_BAG_START; ++i)
    {
        player->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
    }

    // Delete default bag items
    for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    {
        player->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
    }

    // TODO: Clean bank, keychain, additional bags, etc..

    LOG_INFO("module", "Prestige> Player items were deleted.");
}

void PrestigeHandler::EquipDefaultItems(Player* player)
{
    auto startOutfit = GetCharStartOutfitEntry(player->getRace(), player->getClass(), player->getGender());

    for (uint8 i = 0; i < MAX_OUTFIT_ITEMS; ++i)
    {
        auto itemEntry = startOutfit->ItemId[i];

        if (!itemEntry ||
            itemEntry == -1)
        {
            continue;
        }

        player->StoreNewItemInBestSlots(itemEntry, 1);
    }
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
