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
    ResetSpells(player);
    ResetQuests(player);
    ResetHomebindAndPosition(player);

    DeleteItems(player);
    EquipDefaultItems(player);

    player->SaveToDB(false, false);
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

    // Learn default starter spells
    {
        auto classEntry = sChrClassesStore.LookupEntry(player->getClass());

        if (!classEntry)
        {
            LOG_WARN("module", "Prestige> Failed to load class entry for player {}.", player->GetName());
            return;
        }

        auto spellFamily = classEntry->spellfamily;

        for (auto it = sSkillLineAbilityStore.begin(); it != sSkillLineAbilityStore.end(); ++it)
        {
            if (it->AcquireMethod != 2)
            {
                continue;
            }

            if (it->RaceMask != 0 &&
                (it->RaceMask & player->getRaceMask()) != player->getRaceMask())
            {
                continue;
            }

            if (it->ClassMask != 0 &&
                (it->ClassMask & player->getClassMask()) != player->getClassMask())
            {
                continue;
            }

            if (!player->IsSpellFitByClassAndRace(it->Spell))
            {
                continue;
            }

            auto spellInfo = sSpellMgr->GetSpellInfo(it->Spell);

            if (spellInfo->SpellLevel == 0)
            {
                continue;
            }

            if (spellInfo->SpellFamilyName != spellFamily)
            {
                continue;
            }

            if (!SpellMgr::IsSpellValid(spellInfo))
            {
                continue;
            }

            LOG_INFO("module", "Prestige> Learning spell {}:{}", it->Spell, spellInfo->SpellName[0]);

            player->learnSpell(it->Spell);
        }
    }

    auto playerInfo = sObjectMgr->GetPlayerInfo(player->getRace(), player->getClass());
    if (!playerInfo)
    {
        LOG_WARN("module", "Prestige> Failed to load player information for player {}.", player->GetName());
        return;
    }

    // Re-cast spells like blood presence / battle stance
    for (auto& castSpell : playerInfo->castSpells)
    {
        player->CastSpell(player, castSpell, true);
    }

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
    for (uint32 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
    {
        player->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
    }

    // Delete items in additional bags
    for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    {
        auto bag = player->GetBagByPos(i);

        if (!bag)
        {
            continue;
        }

        for (uint32 j = 0; j < bag->GetBagSize(); ++j)
        {
            player->RemoveItem(i, j, true);
        }
    }

    // Delete items from the buyback tab
    for (uint32 i = BUYBACK_SLOT_START; i < BUYBACK_SLOT_END; ++i)
    {
        player->RemoveItemFromBuyBackSlot(i, true);
    }

    // Delete items from the keyring
    for (uint32 i = KEYRING_SLOT_START; i < KEYRING_SLOT_END; ++i)
    {
        player->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
    }

    // Delete items from the main bank slots
    for (uint32 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
    {
        player->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
    }

    // Delete items from the additional bank bag slots
    for (uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
    {
        auto bag = player->GetBagByPos(i);

        if (!bag)
        {
            continue;
        }

        for (uint32 j = 0; j < bag->GetBagSize(); ++j)
        {
            player->RemoveItem(i, j, true);
        }
    }

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
