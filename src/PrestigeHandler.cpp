#include "PrestigeHandler.h"

PrestigeHandler::PrestigeHandler()
{
    // Alliance Racials
    {
        racialMap.emplace(RACE_HUMAN, std::vector<uint32>{
            20599, // Diplomacy
            59752, // Every man for Himself
            20864, // Mace Specialization
            58985, // Perception
            20597, // Sword Specialization
            20598, // The Human Spirit
        });

        racialMap.emplace(RACE_DWARF, std::vector<uint32>{
            2481, // Find Treasure
            20596, // Frost Resistance
            20595, // Gun Specialization
            59224, // Mace Specialization
            20594, // Stoneform
        });

        racialMap.emplace(RACE_NIGHTELF, std::vector<uint32>{
            21009, // Elusiveness
            20583, // Nature Resistance
            20583, // Quickness
            58984, // Shadowmeld
            20585, // Wisp Spirit
        });

        racialMap.emplace(RACE_GNOME, std::vector<uint32>{
            20592, // Arcane Resistance
            20593, // Engineering Specialization
            20589, // Escape Artist
            20591, // Expansive Mind
        });

        racialMap.emplace(RACE_DRAENEI, std::vector<uint32>{
            28875, // Gemcutting
            59542, // Gift of the Naaru
            6562, // Heroic Presence
            59535, // Shadow Resistance
        });
    }

    // Horde Racials
    {
        racialMap.emplace(RACE_ORC, std::vector<uint32>{
            20574, // Axe Specialization
            20572, // Blood Fury
            21563, // Command
            20573, // Hardiness
        });

        racialMap.emplace(RACE_UNDEAD_PLAYER, std::vector<uint32>{
            20577, // Cannibalize
            20579, // Shadow Resistance
            5227, // Underwater Breathing
            7744, // Will of the Forsaken
        });

        racialMap.emplace(RACE_TAUREN, std::vector<uint32>{
            20552, // Cultivation
            20550, // Endurance
            20551, // Nature Resistance
            20549, // War Stomp
        });

        racialMap.emplace(RACE_TROLL, std::vector<uint32>{
            20557, // Beast Slaying
            26297, // Berserking
            26290, // Bow Specialization
            20555, // Regeneration
            20558, // Throwing Specialization
        });

        racialMap.emplace(RACE_BLOODELF, std::vector<uint32>{
            28877, // Arcane Affinity
            28730, // Arcane Torrent
            833, // Magic Resistance
        });
    }

    // Class Spells
    {
        spellMap.emplace(CLASS_WARRIOR, std::vector<uint32>{
            6603, // Auto Attack
            107, // Block
            81, // Dodge
            2764, // Throw

            2457, // Battle Stance
            78, // Heroic Strike
        });

        spellMap.emplace(CLASS_PALADIN, std::vector<uint32>{
            6603, // Auto Attack
            107, // Block
            81, // Dodge

            635, // Holy Light
            21084, // Seal of Righteousness
        });

        spellMap.emplace(CLASS_HUNTER, std::vector<uint32>{
            6603, // Auto Attack
            81, // Dodge

            75, // Auto Shot
            2973, // Raptor Strike
        });

        spellMap.emplace(CLASS_ROGUE, std::vector<uint32>{
            6603, // Auto Attack
            81, // Dodge
            674, // Dual Wield
            2764, // Throw

            2098, // Eviscerate
            1752, // Sinister Strike
        });

        spellMap.emplace(CLASS_PRIEST, std::vector<uint32>{
            6603, // Auto Attack
            81, // Dodge
            5019, // Shoot

            2050, // Lesser Heal
            585, // Smite
        });

        spellMap.emplace(CLASS_SHAMAN, std::vector<uint32>{
            6603, // Auto Attack
            107, // Block
            81, // Dodge

            403, // Lightning Bolt
            331, // Healing Wave
        });

        spellMap.emplace(CLASS_MAGE, std::vector<uint32>{
            6603, // Auto Attack
            81, // Dodge
            5019, // Shoot

            133, // Fireball
            168, // Frost Armor
        });

        spellMap.emplace(CLASS_WARLOCK, std::vector<uint32>{
            6603, // Auto Attack
            81, // Dodge
            5019, // Shoot

            687, // Demon Skin
            686, // Shadow Bolt
        });

        spellMap.emplace(CLASS_DRUID, std::vector<uint32>{
            6603, // Auto Attack
            81, // Dodge

            5176, // Wrath
            5185, // Healing Touch
        });

        spellMap.emplace(CLASS_DEATH_KNIGHT, std::vector<uint32>{
            6603, // Auto Attack
            81, // Dodge
            674, // Dual Wield
            10846, // First Aid (Rank 4)

            48266, // Blood Presence
            45902, // Blood Strike
            49410, // Forceful Deflection
            59921, // Frost Fever
            45477, // Icy Touch
            61455, // Runic Focus
            59879, // Blood Plague
            47541, // Death Coil
            49576, // Death Grip
            45462, // Plague Strike
        });
    }
}

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
    ResetQuests(player);
    ResetHomebindAndPosition(player);

    DeleteItems(player);
    EquipDefaultItems(player);

    UnlearnAllSpells(player);

    // Update DB for removed spells.
    player->SaveToDB(false, false);

    LearnRacials(player);
    LearnClassSpells(player);

    // Update DB with new spells.
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

void PrestigeHandler::UnlearnAllSpells(Player* player)
{
    LOG_INFO("module", "Prestige> Unlearning player spells..");

    auto spellMap = player->GetSpellMap();
    for (auto& spellEntry : spellMap)
    {
        auto spellId = spellEntry.first;

        player->removeSpell(spellId, SPEC_MASK_ALL, false);
    }

    LOG_INFO("module", "Prestige> Player spells unlearned.");
}

void PrestigeHandler::LearnRacials(Player* player)
{
    LOG_INFO("module", "Prestige> Learning player racials..");

    auto it = racialMap.find(static_cast<Races>(player->getRace()));
    if (it == racialMap.end())
    {
        LOG_WARN("module", "Failed to find racials for player {} race {}.", player->GetName(), player->getRace());
        return;
    }

    auto racials = it->second;

    for (auto racial : racials)
    {
        if (!racial ||
            player->HasSpell(racial))
        {
            continue;
        }

        player->learnSpell(racial);
    }

    LOG_INFO("module", "Prestige> Player racial spells learned.");
}

void PrestigeHandler::LearnClassSpells(Player* player)
{
    LOG_INFO("module", "Prestige> Learning player class spells..");

    auto it = spellMap.find(static_cast<Classes>(player->getClass()));
    if (it == spellMap.end())
    {
        LOG_WARN("module", "Failed to find spells for player {} class {}.", player->GetName(), player->getClass());
        return;
    }

    auto spells = it->second;

    for (auto spell : spells)
    {
        if (!spell ||
            player->HasSpell(spell))
        {
            continue;
        }

        player->learnSpell(spell);
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
        // Dont try to cast a spell the player does not have
        if (!player->HasSpell(castSpell))
        {
            continue;
        }

        player->CastSpell(player, castSpell, true);
    }

    LOG_INFO("module", "Prestige> Player class spells learned.");
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
