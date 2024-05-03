#include "PrestigeHandler.h"

#include "Config.h"
#include "Pet.h"

PrestigeHandler::PrestigeHandler()
{
    // Alliance Racials
    {
        racialMap.emplace(RACE_HUMAN, std::unordered_set<uint32>{
            20599, // Diplomacy
            59752, // Every man for Himself
            20864, // Mace Specialization
            58985, // Perception
            20597, // Sword Specialization
            20598, // The Human Spirit

            668, // Common (Language)
        });

        racialMap.emplace(RACE_DWARF, std::unordered_set<uint32>{
            2481, // Find Treasure
            20596, // Frost Resistance
            20595, // Gun Specialization
            59224, // Mace Specialization
            20594, // Stoneform

            668, // Common (Language)
            672, // Dwarvish (Language)
        });

        racialMap.emplace(RACE_NIGHTELF, std::unordered_set<uint32>{
            21009, // Elusiveness
            20583, // Nature Resistance
            20582, // Quickness
            58984, // Shadowmeld
            20585, // Wisp Spirit

            668, // Common (Language)
            671, // Darnassian (Language)
        });

        racialMap.emplace(RACE_GNOME, std::unordered_set<uint32>{
            20592, // Arcane Resistance
            20593, // Engineering Specialization
            20589, // Escape Artist
            20591, // Expansive Mind

            668, // Common (Language)
            7340, // Gnomish (Language)
        });

        racialMap.emplace(RACE_DRAENEI, std::unordered_set<uint32>{
            28875, // Gemcutting
            59542, // Gift of the Naaru
            6562, // Heroic Presence
            59535, // Shadow Resistance

            668, // Common (Language)
            29932, // Draenei (Language)
        });
    }

    // Horde Racials
    {
        racialMap.emplace(RACE_ORC, std::unordered_set<uint32>{
            20574, // Axe Specialization
            20572, // Blood Fury
            21563, // Command
            20573, // Hardiness

            669, // Orcish (Language)
        });

        racialMap.emplace(RACE_UNDEAD_PLAYER, std::unordered_set<uint32>{
            20577, // Cannibalize
            20579, // Shadow Resistance
            5227, // Underwater Breathing
            7744, // Will of the Forsaken

            669, // Orcish (Language)
            17737, // Gutterspeak (Language)
        });

        racialMap.emplace(RACE_TAUREN, std::unordered_set<uint32>{
            20552, // Cultivation
            20550, // Endurance
            20551, // Nature Resistance
            20549, // War Stomp

            669, // Orcish (Language)
            670, // Taurahe (Language)
        });

        racialMap.emplace(RACE_TROLL, std::unordered_set<uint32>{
            20557, // Beast Slaying
            26297, // Berserking
            26290, // Bow Specialization
            20555, // Regeneration
            20558, // Throwing Specialization

            669, // Orcish (Language)
            7341, // Troll (Language)
        });

        racialMap.emplace(RACE_BLOODELF, std::unordered_set<uint32>{
            28877, // Arcane Affinity
            28730, // Arcane Torrent
            833, // Magic Resistance

            669, // Orcish (Language)
            813, // Thalassian (Language)
        });
    }

    // Class Spells
    {
        spellMap.emplace(CLASS_WARRIOR, std::unordered_set<uint32>{
            6603, // Auto Attack
            107, // Block
            81, // Dodge
            2764, // Throw

            2457, // Battle Stance
            78, // Heroic Strike
        });

        spellMap.emplace(CLASS_PALADIN, std::unordered_set<uint32>{
            6603, // Auto Attack
            107, // Block
                81, // Dodge

                635, // Holy Light
                21084, // Seal of Righteousness
        });

        spellMap.emplace(CLASS_HUNTER, std::unordered_set<uint32>{
            6603, // Auto Attack
                81, // Dodge

                75, // Auto Shot
                2973, // Raptor Strike
        });

        spellMap.emplace(CLASS_ROGUE, std::unordered_set<uint32>{
            6603, // Auto Attack
                81, // Dodge
                674, // Dual Wield
                2764, // Throw

                2098, // Eviscerate
                1752, // Sinister Strike
        });

        spellMap.emplace(CLASS_PRIEST, std::unordered_set<uint32>{
            6603, // Auto Attack
                81, // Dodge
                5019, // Shoot

                2050, // Lesser Heal
                585, // Smite
        });

        spellMap.emplace(CLASS_SHAMAN, std::unordered_set<uint32>{
            6603, // Auto Attack
                107, // Block
                81, // Dodge

                403, // Lightning Bolt
                331, // Healing Wave
        });

        spellMap.emplace(CLASS_MAGE, std::unordered_set<uint32>{
            6603, // Auto Attack
                81, // Dodge
                5019, // Shoot

                133, // Fireball
                168, // Frost Armor
        });

        spellMap.emplace(CLASS_WARLOCK, std::unordered_set<uint32>{
            6603, // Auto Attack
                81, // Dodge
                5019, // Shoot

                687, // Demon Skin
                686, // Shadow Bolt
        });

        spellMap.emplace(CLASS_DRUID, std::unordered_set<uint32>{
            6603, // Auto Attack
                81, // Dodge

                5176, // Wrath
                5185, // Healing Touch
        });

        spellMap.emplace(CLASS_DEATH_KNIGHT, std::unordered_set<uint32>{
            6603, // Auto Attack
                81, // Dodge
                674, // Dual Wield

                10846, // First Aid (Rank 4)
                18632, // Heavy Runecloth Bandage
                18629, // Runecloth Bandage
                10841, // Heavy Mageweave Bandage
                10840, // Mageweave Bandage
                7929, // Heavy Silk Bandage
                7928, // Silk Bandage
                3278, // Heavy Wool Bandage
                3277, // Wool Bandage
                3276, // Heavy Linen Bandage
                3275, // Linen Bandage
                7934, // Anti-Venom

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

    // Professions
    {
        // Alchemy
        {
            professionMap.emplace(2259); // Alchemy - Rank 1
            professionMap.emplace(3101); // Alchemy - Rank 2
            professionMap.emplace(3464); // Alchemy - Rank 3
            professionMap.emplace(11611); // Alchemy - Rank 4
            professionMap.emplace(28596); // Alchemy - Rank 5
            professionMap.emplace(51304); // Alchemy - Rank 6
        }

        // Blacksmithing
        {
            professionMap.emplace(2018); // Blacksmithing - Rank 1
            professionMap.emplace(3100); // Blacksmithing - Rank 2
            professionMap.emplace(3538); // Blacksmithing - Rank 3
            professionMap.emplace(9785); // Blacksmithing - Rank 4
            professionMap.emplace(29844); // Blacksmithing - Rank 5
            professionMap.emplace(51300); // Blacksmithing - Rank 6
        }

        // Enchanting
        {
            professionMap.emplace(7411); // Enchanting - Rank 1
            professionMap.emplace(7412); // Enchanting - Rank 2
            professionMap.emplace(7413); // Enchanting - Rank 3
            professionMap.emplace(13920); // Enchanting - Rank 4
            professionMap.emplace(28029); // Enchanting - Rank 5
            professionMap.emplace(51313); // Enchanting - Rank 6
            professionMap.emplace(13262); // Enchanting - Disenchant
        }

        // Engineering
        {
            professionMap.emplace(4036); // Engineering - Rank 1
            professionMap.emplace(49383); // Engineering - Rank 1
            professionMap.emplace(4037); // Engineering - Rank 2
            professionMap.emplace(4038); // Engineering - Rank 3
            professionMap.emplace(12656); // Engineering - Rank 4
            professionMap.emplace(30350); // Engineering - Rank 5
            professionMap.emplace(51306); // Engineering - Rank 6
        }

        // Herbalism
        {
            professionMap.emplace(2383); // Herbalism - Find Herbs
            professionMap.emplace(32605); // Herbalism - Rank 1
            professionMap.emplace(2366); // Herbalism - Rank 1
            professionMap.emplace(2368); // Herbalism - Rank 2
            professionMap.emplace(3570); // Herbalism - Rank 3
            professionMap.emplace(11993); // Herbalism - Rank 4
            professionMap.emplace(28695); // Herbalism - Rank 5
            professionMap.emplace(50300); // Herbalism - Rank 6
            professionMap.emplace(55428); // Herbalism - Lifeblood - Rank 1
            professionMap.emplace(55480); // Herbalism - Lifeblood - Rank 2
            professionMap.emplace(55500); // Herbalism - Lifeblood - Rank 3
            professionMap.emplace(55501); // Herbalism - Lifeblood - Rank 4
            professionMap.emplace(55502); // Herbalism - Lifeblood - Rank 5
            professionMap.emplace(55503); // Herbalism - Lifeblood - Rank 6
        }

        // Inscription
        {
            professionMap.emplace(45357); // Inscription - Rank 1
            professionMap.emplace(45358); // Inscription - Rank 2
            professionMap.emplace(45359); // Inscription - Rank 3
            professionMap.emplace(45360); // Inscription - Rank 4
            professionMap.emplace(45361); // Inscription - Rank 5
            professionMap.emplace(45363); // Inscription - Rank 6
            professionMap.emplace(51005); // Inscription - Milling
        }

        // Jewelcrafting
        {
            professionMap.emplace(25229); // Jewelcrafting - Rank 1
            professionMap.emplace(25230); // Jewelcrafting - Rank 2
            professionMap.emplace(28894); // Jewelcrafting - Rank 3
            professionMap.emplace(28895); // Jewelcrafting - Rank 4
            professionMap.emplace(28897); // Jewelcrafting - Rank 5
            professionMap.emplace(51311); // Jewelcrafting - Rank 6
            professionMap.emplace(31252); // Jewelcrafting - Prospecting
        }

        // Leatherworking
        {
            professionMap.emplace(2108); // Leatherworking - Rank 1
            professionMap.emplace(3104); // Leatherworking - Rank 2
            professionMap.emplace(3811); // Leatherworking - Rank 3
            professionMap.emplace(10662); // Leatherworking - Rank 4
            professionMap.emplace(32549); // Leatherworking - Rank 5
            professionMap.emplace(51301); // Leatherworking - Rank 6
            professionMap.emplace(10660); // Leatherworking - Tribal
            professionMap.emplace(10658); // Leatherworking - Elemental
            professionMap.emplace(10656); // Leatherworking - Dragonscale
        }

        // Mining
        {
            professionMap.emplace(32606); // Mining - Rank 1
            professionMap.emplace(2575); // Mining - Rank 1
            professionMap.emplace(2576); // Mining - Rank 2
            professionMap.emplace(3564); // Mining - Rank 3
            professionMap.emplace(10248); // Mining - Rank 4
            professionMap.emplace(29354); // Mining - Rank 5
            professionMap.emplace(50310); // Mining - Rank 6
            professionMap.emplace(53120); // Mining - Toughness - Rank 1
            professionMap.emplace(53121); // Mining - Toughness - Rank 2
            professionMap.emplace(53122); // Mining - Toughness - Rank 3
            professionMap.emplace(53123); // Mining - Toughness - Rank 4
            professionMap.emplace(53124); // Mining - Toughness - Rank 5
            professionMap.emplace(53040); // Mining - Toughness - Rank 6
            professionMap.emplace(2656); // Mining - Smelting
            professionMap.emplace(2580); // Mining - Find Minerals
        }

        // Skinning
        {
            professionMap.emplace(8613); // Skinning - Rank 1
            professionMap.emplace(8617); // Skinning - Rank 2
            professionMap.emplace(8618); // Skinning - Rank 3
            professionMap.emplace(10768); // Skinning - Rank 4
            professionMap.emplace(32678); // Skinning - Rank 5
            professionMap.emplace(50305); // Skinning - Rank 6
            professionMap.emplace(53125); // Skinning - Master of Anatomy - Rank 1
            professionMap.emplace(53662); // Skinning - Master of Anatomy - Rank 2
            professionMap.emplace(53663); // Skinning - Master of Anatomy - Rank 3
            professionMap.emplace(53664); // Skinning - Master of Anatomy - Rank 4
            professionMap.emplace(53665); // Skinning - Master of Anatomy - Rank 5
            professionMap.emplace(53666); // Skinning - Master of Anatomy - Rank 6
        }

        // Tailoring
        {
            professionMap.emplace(3908); // Tailoring - Rank 1
            professionMap.emplace(3909); // Tailoring - Rank 2
            professionMap.emplace(3910); // Tailoring - Rank 3
            professionMap.emplace(12180); // Tailoring - Rank 4
            professionMap.emplace(26790); // Tailoring - Rank 5
            professionMap.emplace(51309); // Tailoring - Rank 6
            professionMap.emplace(26797); // Tailoring - Spellfire
            professionMap.emplace(26801); // Tailoring - Shadoweave
            professionMap.emplace(26798); // Tailoring - Mooncloth
        }

        // Cooking
        {
            professionMap.emplace(2550); // Cooking - Rank 1
            professionMap.emplace(3102); // Cooking - Rank 2
            professionMap.emplace(3413); // Cooking - Rank 3
            professionMap.emplace(18260); // Cooking - Rank 4
            professionMap.emplace(33359); // Cooking - Rank 5
            professionMap.emplace(51296); // Cooking - Rank 6
        }

        // First Aid
        {
            professionMap.emplace(3273); // First Aid - Rank 1
            professionMap.emplace(3274); // First Aid - Rank 2
            professionMap.emplace(7924); // First Aid - Rank 3
            professionMap.emplace(10846); // First Aid - Rank 4
            professionMap.emplace(27028); // First Aid - Rank 5
            professionMap.emplace(45542); // First Aid - Rank 6
        }

        // Fishing
        {
            professionMap.emplace(7620); // Fishing - Rank 1
            professionMap.emplace(7731); // Fishing - Rank 2
            professionMap.emplace(7732); // Fishing - Rank 3
            professionMap.emplace(18248); // Fishing - Rank 4
            professionMap.emplace(33095); // Fishing - Rank 5
            professionMap.emplace(51294); // Fishing - Rank 6
            professionMap.emplace(43308); // Fishing - Find Fish
        }
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

void PrestigeHandler::DoPrestige(Player* player, bool sacrificeArmor)
{
    if (!player ||
        !player->GetGUID() ||
        !player->GetSession())
    {
        return;
    }

    PrestigeState state;
    state.IsSacrifice = sacrificeArmor;
    state.AvgItemLevel = 0;
    state.Message = "";
    state.QueueState = QueueState::QUEUE_RESET_INIT;

    AddPrestigeState(player, state);
}

void PrestigeHandler::ResetLevel(Player* player)
{
    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Resetting player level..");
    }

    uint32 isHeroClass = player->getClass() == CLASS_DEATH_KNIGHT;
    uint32 level = isHeroClass ? 55 : 1;

    player->SetLevel(level, true);
    player->SetUInt32Value(PLAYER_XP, 0);
    player->InitStatsForLevel(true);

    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Player level reset to '{}'.", level);
    }
}

void PrestigeHandler::ResetTalents(Player* player)
{
    player->resetTalents(true);
}

void PrestigeHandler::ResetSpells(Player* player)
{
    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Unlearning player spells..");
    }

    auto spellMap = player->GetSpellMap();
    for (auto& spellEntry : spellMap)
    {
        auto spellId = spellEntry.first;

        if (!spellId)
        {
            continue;
        }

        // Don't unlearn racials or starter spells.
        if (IsRacialSpell(player->getRace(), spellId) ||
            IsClassStarterSpell(player->getClass(), spellId))
        {
            // Send the unlearn packet and re-learn them after.
            player->SendLearnPacket(spellId, false);
            continue;
        }

        // Don't unlearn professions
        if (!sConfigMgr->GetOption<bool>("Prestigious.Unlearn.Professions", true))
        {
            if (IsProfession(spellId) ||
                IsRecipe(spellId))
            {
                continue;
            }
        }

        // Don't unlearn mounts
        if (!sConfigMgr->GetOption<bool>("Prestigious.Unlearn.Mounts", false))
        {
            if (IsMount(spellId))
            {
                continue;
            }
        }

        // Don't unlearn pets
        if (!sConfigMgr->GetOption<bool>("Prestigious.Unlearn.NonCombatPet", false))
        {
            if (IsNonCombatPet(spellId))
            {
                continue;
            }
        }

        player->removeSpell(spellId, SPEC_MASK_ALL, false);
        player->SendLearnPacket(spellId, false);
    }

    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Player spells unlearned.");
    }
}

void PrestigeHandler::ResendRankedSpells(Player* player)
{
    // When removing ranked spells, we need to re-send the learn packet to the client
    // so they know they have the lower ranked spells.
    for (auto& spell : player->GetSpellMap())
    {
        uint32 spellId = spell.first;

        if (!IsRacialSpell(player->getRace(), spellId) &&
            !IsClassStarterSpell(player->getClass(), spellId))
        {
            continue;
        }

        player->SendLearnPacket(spellId, true);
    }
}

void PrestigeHandler::ResetSkills(Player* player)
{
    if (!sConfigMgr->GetOption<bool>("Prestigious.ResetSkills", true))
    {
        return;
    }

    std::vector<uint32> skills
    {
        SKILL_UNARMED,

        SKILL_2H_MACES,
        SKILL_2H_AXES,
        SKILL_2H_SWORDS,

        SKILL_MACES,
        SKILL_AXES,
        SKILL_SWORDS,
        SKILL_DAGGERS,
        SKILL_FIST_WEAPONS,

        SKILL_STAVES,
        SKILL_POLEARMS,

        SKILL_GUNS,
        SKILL_BOWS,
        SKILL_CROSSBOWS,
        SKILL_WANDS,
        SKILL_THROWN,

        SKILL_DEFENSE
    };

    for (auto& skill : skills)
    {
        if (!player->HasSkill(skill))
        {
            continue;
        }

        player->SetSkill(skill, 1, player->getClass() == CLASS_DEATH_KNIGHT ? 270 : 1, player->GetMaxSkillValueForLevel());
    }
}

void PrestigeHandler::DesummonMinion(Player* player)
{
    if (!player->GetMinionGUID())
    {
        return;
    }

    auto minion = player->GetFirstMinion();
    if (!minion)
    {
        return;
    }

    minion->DespawnOrUnsummon();
}

void PrestigeHandler::DesummonPet(Player* player)
{
    auto pet = player->GetPet();
    if (!pet)
    {
        return;
    }

    pet->DespawnOrUnsummon();
}

void PrestigeHandler::ResetQuests(Player* player)
{
    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Resetting player quest status..");
    }

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

        if (IsWhitelistedQuest(quest))
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

        if (IsWhitelistedQuest(quest))
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

    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Player quest status reset.");
    }
}

void PrestigeHandler::ResetHomebindAndPosition(Player* player)
{
    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Resetting player homebind and position..");
    }

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

    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Player homebind and position reset.");
    }
}

void PrestigeHandler::ResetActionbar(Player* player)
{
    PlayerInfo const* pInfo = sObjectMgr->GetPlayerInfo(player->getRace(), player->getClass());

    if (!pInfo)
    {
        LOG_WARN("module.prestigious", "Prestige> Failed to load player information for player {}.", player->GetName());
        return;
    }

    // Clear actionbar
    for (uint8 i = 0; i <= MAX_ACTION_BUTTONS; i++)
    {
        player->removeActionButton(i);
    }

    // Clear actions on client-side
    player->SendActionButtons(2);

    if (!player || !pInfo)
    {
        return;
    }

    // Repopulate actionbar
    for (auto it = pInfo->action.begin(); it != pInfo->action.end(); ++it)
    {
        auto actionButton = player->addActionButton(it->button, it->action, it->type);
        if (!actionButton)
        {
            continue;
        }

        actionButton->uState = ACTIONBUTTON_NEW;
    }

    // Send new actions to client
    player->SendActionButtons(1);
}

void PrestigeHandler::IterateItems(Player* player, bool deleteEquipped)
{
    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Deleting/flagging player items..");
    }

    uint32 flagged = 0;
    uint32 deleted = 0;

    // Equipped items
    if (deleteEquipped ||
        sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
    {
        for (uint32 i = 0; i < INVENTORY_SLOT_BAG_START; ++i)
        {
            auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (!item)
            {
                continue;
            }

            if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
            {
                if (auto itemProto = item->GetTemplate())
                {
                    if (itemProto->RequiredLevel < 1)
                    {
                        SetItemFlagged(item, true);
                        flagged++;
                    }
                }
            }

            if (deleteEquipped)
            {
                player->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
                deleted++;
            }
        }
    }

    // Default bag items
    if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
    {
        for (uint32 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
        {
            auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (!item)
            {
                continue;
            }

            if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
            {
                if (auto itemProto = item->GetTemplate())
                {
                    if (itemProto->RequiredLevel < 1)
                    {
                        SetItemFlagged(item, true);
                        flagged++;
                    }
                }
            }
        }
    }

    // Additional bags
    if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
    {
        for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
        {
            auto bag = player->GetBagByPos(i);

            if (!bag)
            {
                continue;
            }

            for (uint32 j = 0; j < bag->GetBagSize(); ++j)
            {
                auto item = player->GetItemByPos(i, j);
                if (!item)
                {
                    continue;
                }

                if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
                {
                    if (auto itemProto = item->GetTemplate())
                    {
                        if (itemProto->RequiredLevel < 1)
                        {
                            SetItemFlagged(item, true);
                            flagged++;
                        }
                    }
                }
            }
        }
    }

    // Items from the buyback tab
    if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
    {
        for (uint32 i = BUYBACK_SLOT_START; i < BUYBACK_SLOT_END; ++i)
        {
            if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
            {
                auto item = player->GetItemFromBuyBackSlot(i);
                if (!item)
                {
                    continue;
                }

                if (auto itemProto = item->GetTemplate())
                {
                    if (itemProto->RequiredLevel < 1)
                    {
                        SetItemFlagged(item, true);
                        flagged++;
                    }
                }
            }
        }
    }

    // Items from the keyring
    if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
    {
        for (uint32 i = KEYRING_SLOT_START; i < KEYRING_SLOT_END; ++i)
        {
            auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (!item)
            {
                continue;
            }

            if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
            {
                if (auto itemProto = item->GetTemplate())
                {
                    if (itemProto->RequiredLevel < 1)
                    {
                        SetItemFlagged(item, true);
                        flagged++;
                    }
                }
            }
        }
    }

    // Items from the main bank slots
    if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
    {
        for (uint32 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
        {
            auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (!item)
            {
                continue;
            }

            if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
            {
                if (auto itemProto = item->GetTemplate())
                {
                    if (itemProto->RequiredLevel < 1)
                    {
                        SetItemFlagged(item, true);
                        flagged++;
                    }
                }
            }
        }
    }

    // Items from the additional bank bag slots
    if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
    {
        for (uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
        {
            auto bag = player->GetBagByPos(i);

            if (!bag)
            {
                continue;
            }

            for (uint32 j = 0; j < bag->GetBagSize(); ++j)
            {
                auto item = player->GetItemByPos(i, j);
                if (!item)
                {
                    continue;
                }

                if (sConfigMgr->GetOption<bool>("Prestigious.FlagItems", true))
                {
                    if (auto itemProto = item->GetTemplate())
                    {
                        if (itemProto->RequiredLevel < 1)
                        {
                            SetItemFlagged(item, true);
                            flagged++;
                        }
                    }
                }
            }
        }
    }

    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> {} player items were deleted.", deleted);
        LOG_INFO("module.prestigious", "Prestige> {} player items were flagged.", flagged);
    }
}

void PrestigeHandler::EquipDefaultItems(Player* player)
{
    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Equipping default items..");
    }

    if(player->GetDisplayId() != player->GetNativeDisplayId())
    {
        player->DeMorph();
    }

    auto startOutfit = GetCharStartOutfitEntry(player->getRace(), player->getClass(), player->getGender());
    if (!startOutfit)
    {
        LOG_ERROR("module.prestigious", "Failed to find start outfit for race {}, class {}, gender {} for player {}", player->getRace(), player->getClass(), player->getGender(), player->GetName());
        return;
    }

    for (uint8 i = 0; i < MAX_OUTFIT_ITEMS; ++i)
    {
        auto itemEntry = startOutfit->ItemId[i];

        if (!itemEntry ||
            itemEntry == -1)
        {
            continue;
        }

        // Check if the item can be stored, to prevent worldserver error
        ItemPosCountVec sDest;
        auto result = player->CanStoreNewItem(INVENTORY_SLOT_BAG_0, NULL_SLOT, sDest, itemEntry, 1);

        if (result != EQUIP_ERR_OK)
        {
            continue;
        }

        player->StoreNewItemInBestSlots(itemEntry, 1);
    }

    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Pretige> Default items were equipped.");
    }
}

bool PrestigeHandler::IsRacialSpell(uint32 race, uint32 spellId)
{
    auto it = racialMap.find(static_cast<Races>(race));
    if (it == racialMap.end())
    {
        return false;
    }

    auto racials = it->second;
    if (racials.find(spellId) == racials.end())
    {
        return false;
    }

    return true;
}

bool PrestigeHandler::IsClassStarterSpell(uint32 pClass, uint32 spellId)
{
    auto it = spellMap.find(static_cast<Classes>(pClass));
    if (it == spellMap.end())
    {
        return false;
    }

    auto spells = it->second;
    if (spells.find(spellId) == spells.end())
    {
        return false;
    }

    return true;
}

bool PrestigeHandler::IsProfession(uint32 spellId)
{
    auto spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        return false;
    }

    if (professionMap.find(spellId) == professionMap.end())
    {
        return false;
    }

    return true;
}

bool PrestigeHandler::IsRecipe(uint32 spellId)
{
    auto spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        return false;
    }

    if (spellInfo->Effects[0].Effect == SPELL_EFFECT_TRADE_SKILL)
    {
        return false;
    }

    if ((spellInfo->Attributes & SPELL_ATTR0_IS_TRADESKILL) != SPELL_ATTR0_IS_TRADESKILL)
    {
        return false;
    }

    return true;
}

bool PrestigeHandler::IsHeirloom(Item* item)
{
    if (!item)
    {
        return false;
    }

    auto itemProto = item->GetTemplate();
    if (!itemProto)
    {
        return false;
    }

    return itemProto->Quality == ITEM_QUALITY_HEIRLOOM;
}

bool PrestigeHandler::IsMount(uint32 spellId)
{
    auto spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        return false;
    }

    auto effect = spellInfo->Effects[0];
    if (effect.ApplyAuraName != SPELL_AURA_MOUNTED)
    {
        return false;
    }

    return true;
}

bool PrestigeHandler::IsNonCombatPet(uint32 spellId)
{
    auto spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        return false;
    }

    auto effect = spellInfo->Effects[0];
    if (effect.Effect != SPELL_EFFECT_SUMMON)
    {
        return false;
    }

    int32 creature = effect.MiscValue;
    if (creature <= 0)
    {
        return false;
    }

    auto cProto = sObjectMgr->GetCreatureTemplate(uint32(creature));
    if (!cProto)
    {
        return false;
    }

    if (cProto->type != CREATURE_TYPE_NON_COMBAT_PET)
    {
        return false;
    }

    return true;
}

bool PrestigeHandler::HasItemsEquipped(Player* player)
{
    if (!player)
    {
        return false;
    }

    bool hasEquipped = false;

    for (uint32 i = 0; i < INVENTORY_SLOT_BAG_START; ++i)
    {
        auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (!item)
        {
            continue;
        }

        hasEquipped = true;
    }

    return hasEquipped;
}

bool PrestigeHandler::HasNonStarterSpells(Player* player)
{
    auto spells = player->GetSpellMap();

    for (auto& spell : spells)
    {
        uint32 spellId = spell.first;
        auto spellState = spell.second;

        if (!spellState)
        {
            continue;
        }

        if (IsRacialSpell(player->getRace(), spellId) ||
            IsClassStarterSpell(player->getClass(), spellId))
        {
            continue;
        }

        if (spellState->State == PLAYERSPELL_REMOVED ||
            spellState->State == PLAYERSPELL_TEMPORARY)
        {
            continue;
        }

        bool isInSpec = spellState->IsInSpec(player->GetActiveSpec());

        if (!spellState->Active ||
            !isInSpec)
        {
            continue;
        }

        if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
        {
            LOG_INFO("module.prestigious", "non-started spell found {} in spec {} player specmask {} spell specmask {} for player {}", spellId, isInSpec, player->GetActiveSpec(), uint32(spellState->specMask), player->GetName());
        }

        return true;
    }

    return false;
}

void PrestigeHandler::RewardPlayer(Player* player, float multiplier)
{
    auto prestigeLevel = GetPrestigeLevel(player);
    std::vector<std::pair<uint32, uint32>> mailItems;

    for (auto reward : rewards)
    {
        // Invalid prestige level for reward
        if (prestigeLevel < 0)
        {
            continue;
        }

        // Skip rewards for specific prestige when not eligible.
        if (reward.PrestigeLevel != 0 &&
            unsigned(prestigeLevel) != reward.PrestigeLevel)
        {
            continue;
        }

        uint32 count = reward.Scalable ? std::ceil(float(reward.Count) * multiplier) : reward.Count;

        mailItems.push_back(std::pair(reward.Entry, count));

        if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
        {
            LOG_INFO("module.prestigious", "Adding reward '{}' with count '{}' to player '{}'.", reward.Entry, reward.Count * multiplier, player->GetName());
        }
    }

    auto title = sConfigMgr->GetOption<std::string>("Prestigious.Reward.Mail.Title", "Prestige Reward");
    auto body = sConfigMgr->GetOption<std::string>("Prestigious.Reward.Mail.Body", "You have been rewarded for prestiging.");

    bool result = SendMailItems(player, mailItems, title, body);

    if (!result)
    {
        player->SendSystemMessage("|cffFF0000Failed to mail rewards for prestige. Contact an Administrator!|r");
        return;
    }

    player->SendSystemMessage("|cff00FF00You have been rewarded for prestiging, check your mailbox for your items.|r");
}

void PrestigeHandler::SacrificeRewardPlayer(Player* player, uint32 avgLevel)
{
    float multiplier = GetMultiplierForItemLevel(avgLevel, player->getClass() == CLASS_DEATH_KNIGHT);

    RewardPlayer(player, multiplier);
}

bool PrestigeHandler::SendMailItems(Player* player, std::vector<std::pair<uint32, uint32>>& mailItems, std::string header, std::string body)
{
    using SendMailTempateVector = std::vector<std::pair<uint32, uint32>>;

    std::vector<SendMailTempateVector> allItems;

    auto AddMailItem = [&allItems](uint32 itemEntry, uint32 itemCount)
        {
            SendMailTempateVector toSendItems;

            ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemEntry);
            if (!itemTemplate)
            {
                LOG_ERROR("module.prestigious", "> PrestigeHandler::SendMailItems: Item id {} is invalid", itemEntry);
                return;
            }

            if (itemCount < 1 || (itemTemplate->MaxCount > 0 && itemCount > static_cast<uint32>(itemTemplate->MaxCount)))
            {
                LOG_ERROR("module.prestigious", "> PrestigeHandler::SendMailItems: Incorrect item count ({}) for item id {}", itemCount, itemEntry);
                return;
            }

            while (itemCount > itemTemplate->GetMaxStackSize())
            {
                if (toSendItems.size() <= MAX_MAIL_ITEMS)
                {
                    toSendItems.emplace_back(itemEntry, itemTemplate->GetMaxStackSize());
                    itemCount -= itemTemplate->GetMaxStackSize();
                }
                else
                {
                    allItems.emplace_back(toSendItems);
                    toSendItems.clear();
                }
            }

            toSendItems.emplace_back(itemEntry, itemCount);
            allItems.emplace_back(toSendItems);
        };

    for (auto& [itemEntry, itemCount] : mailItems)
    {
        AddMailItem(itemEntry, itemCount);
    }

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    MailSender sender(MAIL_CREATURE, MAIL_SENDER_CHROMIE);
    MailDraft draft(header, body);

    for (auto const& items : allItems)
    {
        for (auto const& [itemEntry, itemCount] : items)
        {
            if (Item* mailItem = Item::CreateItem(itemEntry, itemCount))
            {
                mailItem->SaveToDB(trans);
                draft.AddItem(mailItem);
            }
        }
    }

    draft.SendMailTo(trans, MailReceiver(player, player->GetGUID().GetCounter()), sender);

    CharacterDatabase.CommitTransaction(trans);

    return true;
}

void PrestigeHandler::SetItemFlagged(Item* item, bool flag)
{
    if (!item)
    {
        return;
    }

    if (flag && !item->HasFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_PRESTIGE_LOCK))
    {
        item->SetFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_PRESTIGE_LOCK);
    }
    else if (!flag && item->HasFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_PRESTIGE_LOCK))
    {
        item->RemoveFlag(ITEM_FIELD_FLAGS, ITEM_FIELD_FLAG_PRESTIGE_LOCK);
    }
}

void PrestigeHandler::UnflagItems(Player* player)
{
    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> Unflagging player items..");
    }

    uint32 unflagged = 0;

    // Equipped items
    for (uint32 i = 0; i < INVENTORY_SLOT_BAG_START; ++i)
    {
        auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (!item)
        {
            continue;
        }

        SetItemFlagged(item, false);
        unflagged++;
    }

    // Default bag items
    for (uint32 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
    {
        auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (!item)
        {
            continue;
        }

        SetItemFlagged(item, false);
        unflagged++;
    }

    // Additional bags
    for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    {
        auto bag = player->GetBagByPos(i);

        if (!bag)
        {
            continue;
        }

        for (uint32 j = 0; j < bag->GetBagSize(); ++j)
        {
            auto item = player->GetItemByPos(i, j);
            if (!item)
            {
                continue;
            }

            SetItemFlagged(item, false);
            unflagged++;
        }
    }

    // Items from the buyback tab
    for (uint32 i = BUYBACK_SLOT_START; i < BUYBACK_SLOT_END; ++i)
    {
        auto item = player->GetItemFromBuyBackSlot(i);
        if (!item)
        {
            continue;
        }

        SetItemFlagged(item, false);
        unflagged++;
    }

    // Items from the keyring
    for (uint32 i = KEYRING_SLOT_START; i < KEYRING_SLOT_END; ++i)
    {
        auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (!item)
        {
            continue;
        }

        SetItemFlagged(item, false);
        unflagged++;
    }

    // Items from the main bank slots
    for (uint32 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
    {
        auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (!item)
        {
            continue;
        }

        SetItemFlagged(item, false);
        unflagged++;
    }

    // Items from the additional bank bag slots
    for (uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
    {
        auto bag = player->GetBagByPos(i);

        if (!bag)
        {
            continue;
        }

        for (uint32 j = 0; j < bag->GetBagSize(); ++j)
        {
            auto item = player->GetItemByPos(i, j);
            if (!item)
            {
                continue;
            }

            SetItemFlagged(item, false);
            unflagged++;
        }
    }

    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Prestige> {} player items were unflagged.", unflagged);
    }
}

float PrestigeHandler::GetMultiplierForItemLevel(uint32 itemLevel, bool isDeathKnight)
{
    float baseMultiplier = GetBaseMultiplier(isDeathKnight);

    float multiplier = 1.0f;

    for (const auto& [bracket, bracketMultiplier] : itemLevelBrackets)
    {
        if (itemLevel >= bracket)
        {
            multiplier = bracketMultiplier;
        }
    }

    return baseMultiplier * multiplier;
}

void PrestigeHandler::LoadPrestigeLevels()
{
    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", "Loading prestige levels from 'prestige_tracker' table..");
    }

    auto qResult = CharacterDatabase.Query("SELECT * FROM `prestige_tracker`");

    if (!qResult)
    {
        return;
    }

    prestigeLevels.clear();

    do
    {
        auto fields = qResult->Fetch();

        auto guid = fields[0].Get<uint64>();
        auto level = fields[1].Get<uint32>();

        prestigeLevels.emplace(guid, level);
    } while (qResult->NextRow());

    if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
    {
        LOG_INFO("module.prestigious", ">> Loaded '{}' prestige levels.", prestigeLevels.size());
    }
}

void PrestigeHandler::SavePrestigeLevels()
{
    LOG_INFO("module.prestigious", "Saving prestige levels into 'prestige_tracker' table..");

    uint32 count = 0;

    for (auto& [guid, level] : prestigeLevels)
    {
        CharacterDatabase.Execute("INSERT INTO prestige_tracker (guid, prestigeLevel) VALUES ({}, {}) ON DUPLICATE KEY UPDATE guid={}, prestigeLevel={}", guid, level, guid, level);
        count++;
    }

    LOG_INFO("module.prestigious", ">> Saved '{}' prestige levels.", count);
}

int32 PrestigeHandler::GetPrestigeLevel(Player* player)
{
    if (!player)
    {
        return -1;
    }

    auto guid = player->GetGUID();
    if (!guid)
    {
        return -1;
    }

    auto it = prestigeLevels.find(guid.GetRawValue());
    if (it == prestigeLevels.end())
    {
        auto result = prestigeLevels.emplace(guid.GetRawValue(), 0);

        if (!result.second)
        {
            return -1;
        }

        it = result.first;
    }

    return it->second;
}

void PrestigeHandler::UpdatePrestigeLevel(Player* player, uint32 level)
{
    if (!player)
    {
        return;
    }

    auto guid = player->GetGUID();
    if (!guid)
    {
        return;
    }

    auto it = prestigeLevels.find(guid.GetRawValue());
    if (it == prestigeLevels.end())
    {
        return;
    }

    it->second = level;
}

void PrestigeHandler::LoadWhitelistedQuests()
{
    LOG_INFO("module.prestigious", "Loading quests from 'prestige_whitelist_quest' table..");

    auto qResult = WorldDatabase.Query("SELECT * FROM `prestige_whitelist_quest`");

    if (!qResult)
    {
        return;
    }

    whitelistQuests.clear();

    do
    {
        auto fields = qResult->Fetch();

        whitelistQuests.emplace(fields[0].Get<uint32>());
    } while (qResult->NextRow());

    LOG_INFO("module.prestigious", ">> Loaded '{}' quests.", whitelistQuests.size());
}

bool PrestigeHandler::IsWhitelistedQuest(uint32 questId)
{
    return whitelistQuests.find(questId) != whitelistQuests.end();
}

void PrestigeHandler::LoadRewards()
{
    LOG_INFO("module.prestigious", "Loading rewards from 'prestige_rewards' table..");

    auto qResult = WorldDatabase.Query("SELECT * FROM `prestige_rewards`");

    if (!qResult)
    {
        return;
    }

    rewards.clear();

    do
    {
        auto fields = qResult->Fetch();

        PrestigeReward reward;

        reward.Entry = fields[0].Get<uint32>();
        reward.Count = fields[1].Get<uint32>();
        reward.Scalable = fields[2].Get<bool>();
        reward.PrestigeLevel = fields[3].Get<uint32>();

        rewards.push_back(reward);
    } while (qResult->NextRow());

    LOG_INFO("module.prestigious", ">> Loaded '{}' item levels.", rewards.size());
}

void PrestigeHandler::LoadItemLevelBrackets()
{
    LOG_INFO("module.prestigious", "Loading itemlevel brackets from 'prestige_sacrifice_brackets' table..");

    auto qResult = WorldDatabase.Query("SELECT * FROM `prestige_sacrifice_brackets` ORDER BY itemlevel ASC");

if (!qResult)
{
    return;
}

itemLevelBrackets.clear();

do
{
    auto fields = qResult->Fetch();

    uint32 itemLevel = fields[0].Get<uint32>();
    uint32 multiplier = fields[1].Get<float>();

    itemLevelBrackets.emplace(itemLevel, multiplier);
} while (qResult->NextRow());

LOG_INFO("module.prestigious", ">> Loaded '{}' item level brackets.", itemLevelBrackets.size());
}

float PrestigeHandler::GetBaseMultiplier(bool isDeathKnight)
{
    return isDeathKnight ?
        sConfigMgr->GetOption<float>("Prestigious.Reward.Multiplier.Base.DeathKnight", 0.5f) :
        sConfigMgr->GetOption<float>("Prestigious.Reward.Multiplier.Base", 1.0f);
}

void PrestigeHandler::AddPrestigeState(Player* player, PrestigeState state)
{
    if (!player)
    {
        return;
    }

    auto it = prestigeQueue.find(player);
    if (it != prestigeQueue.end())
    {
        return;
    }

    prestigeQueue.emplace(player, state);
}

void PrestigeHandler::UpdateQueueState(Player* player, QueueState state)
{
    if (!player)
    {
        return;
    }

    auto it = prestigeQueue.find(player);
    if (it == prestigeQueue.end())
    {
        return;
    }

    it->second.QueueState = state;
}

void PrestigeHandler::DequeuePrestige(Player* player)
{
    if (!player)
    {
        return;
    }

    auto it = prestigeQueue.find(player);
    if (it == prestigeQueue.end())
    {
        return;
    }

    prestigeQueue.erase(it);
}

void PrestigeHandler::HandleQueue()
{
    if (prestigeQueue.empty())
    {
        return;
    }

    std::vector<Player*> playersToDequeue;

    for (auto& item : prestigeQueue)
    {
        Player* player = item.first;
        PrestigeState* state = &item.second;

        if (!player ||
            !player->IsInWorld() ||
            player->IsBeingTeleported() ||
            !player->GetSession() ||
            player->GetSession()->IsKicked())
        {
            continue;
        }

        switch (state->QueueState)
        {
        case QueueState::QUEUE_RESET_INIT:
            QueueResetInit(player);
            break;

        case QueueState::QUEUE_RESET_EQUIPMENT:
            QueueResetEquipment(player, state);
            break;

        case QueueState::QUEUE_RESET_LEVEL:
            QueueResetLevel(player);
            break;

        case QueueState::QUEUE_RESET_EQUIP_NEW_ITEMS:
            QueueResetNewEquipment(player);
            break;

        case QueueState::QUEUE_RESET_TELEPORT:
            QueueResetHomebindAndPosition(player);
            break;

        case QueueState::QUEUE_RESET_SPELLS:
            QueueResetSpells(player);
            break;

        case QueueState::QUEUE_RESET_RESEND_SPELLS:
            QueueResetResendSpells(player);
            break;

        case QueueState::QUEUE_RESET_SKILLS:
            QueueResetSkills(player);
            break;

        case QueueState::QUEUE_RESET_ACTIONS:
            QueueResetActions(player);
            break;

        case QueueState::QUEUE_RESET_ACTIONBAR:
            QueueResetActionBar(player);
            break;

        case QueueState::QUEUE_RESET_QUESTS:
            QueueResetQuests(player);
            break;

        case QueueState::QUEUE_RESET_DESUMMON_PETS:
            QueueResetDesummonPets(player);
            break;

        case QueueState::QUEUE_RESET_COMPLETE:
            QueueResetComplete(player, state);
            playersToDequeue.push_back(player);
            break;

        case QueueState::QUEUE_RESET_FAILED:
            player->SendSystemMessage(state->Message);
            playersToDequeue.push_back(player);
            break;
        }
    }

    // Clean completed/failed players
    for(auto player : playersToDequeue)
    {
        DequeuePrestige(player);
    }
}

void PrestigeHandler::QueueResetInit(Player* player)
{
    int32 prestigeLevel = GetPrestigeLevel(player);
    if (prestigeLevel == -1)
    {
        player->SendSystemMessage("An error occured while trying to prestige: Failed to get prestige level. Contact an Administrator!");
        LOG_ERROR("module.prestigious", "Failed to get prestige level for player '{}'.", player->GetName());
        return;
    }

    UpdatePrestigeLevel(player, prestigeLevel + 1);

    UpdateQueueState(player, QueueState::QUEUE_RESET_EQUIPMENT);
}

void PrestigeHandler::QueueResetEquipment(Player* player, PrestigeState* state)
{
    state->AvgItemLevel = player->GetAverageItemLevel();

    if (!state->IsSacrifice)
    {
        bool result = UnequipItems(player);

        if (!result)
        {
            state->Message = "Failed to prestige, there was no space in your inventory to store your equipment.";
            state->QueueState = QueueState::QUEUE_RESET_FAILED;
            return;
        }
    }

    // Flag / Delete Items.
    IterateItems(player, state->IsSacrifice);

    UpdateQueueState(player, QueueState::QUEUE_RESET_LEVEL);
}

void PrestigeHandler::QueueResetLevel(Player* player)
{
    // Ensure items were deleted / unequipped before continue
    if (HasItemsEquipped(player))
    {
        return;
    }

    ResetLevel(player);
    ResetTalents(player);

    UpdateQueueState(player, QueueState::QUEUE_RESET_EQUIP_NEW_ITEMS);
}

void PrestigeHandler::QueueResetNewEquipment(Player* player)
{
    auto level = player->GetLevel();

    // Ensure the level reset
    if (level != 1 &&
        level != 55)
    {
        return;
    }

    EquipDefaultItems(player);

    UpdateQueueState(player, QueueState::QUEUE_RESET_TELEPORT);
}

void PrestigeHandler::QueueResetHomebindAndPosition(Player* player)
{
    // Wait for default items to be equipped first
    if (!HasItemsEquipped(player))
    {
        return;
    }

    ResetHomebindAndPosition(player);

    UpdateQueueState(player, QueueState::QUEUE_RESET_SPELLS);
}

void PrestigeHandler::QueueResetSpells(Player* player)
{
    // Wait for relocation
    if (player->IsBeingTeleported() ||
        !player->IsInWorld())
    {
        return;
    }

    ResetSpells(player);

    UpdateQueueState(player, QueueState::QUEUE_RESET_RESEND_SPELLS);
}

void PrestigeHandler::QueueResetResendSpells(Player* player)
{
    ResendRankedSpells(player);

    UpdateQueueState(player, QueueState::QUEUE_RESET_SKILLS);
}

void PrestigeHandler::QueueResetSkills(Player* player)
{
    ResetSkills(player);

    UpdateQueueState(player, QueueState::QUEUE_RESET_ACTIONS);
}

void PrestigeHandler::QueueResetActions(Player* player)
{
    CharacterDatabase.Execute("DELETE FROM character_action WHERE guid = {}", player->GetGUID().GetCounter());

    UpdateQueueState(player, QueueState::QUEUE_RESET_ACTIONBAR);
}

void PrestigeHandler::QueueResetActionBar(Player* player)
{
    ResetActionbar(player);

    UpdateQueueState(player, QueueState::QUEUE_RESET_QUESTS);
}

void PrestigeHandler::QueueResetQuests(Player* player)
{
    ResetQuests(player);

    UpdateQueueState(player, QueueState::QUEUE_RESET_DESUMMON_PETS);
}

void PrestigeHandler::QueueResetDesummonPets(Player* player)
{
    DesummonMinion(player);
    DesummonPet(player);

    UpdateQueueState(player, QueueState::QUEUE_RESET_COMPLETE);
}

void PrestigeHandler::QueueResetComplete(Player* player, PrestigeState* state)
{
    if (state->IsSacrifice)
    {
        SacrificeRewardPlayer(player, state->AvgItemLevel);

        if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
        {
            LOG_INFO("module.prestigious", "Rewarded player '{}' for sacrificing gear during prestige.", player->GetName());
        }
    }
    else
    {
        auto multiplier = GetBaseMultiplier(player->getClass() == CLASS_DEATH_KNIGHT);

        RewardPlayer(player, multiplier);

        if (sConfigMgr->GetOption<bool>("Prestigious.Debug", false))
        {
            LOG_INFO("module.prestigious", "Rewarded player '{}' for prestiging.", player->GetName());
        }
    }

    player->SaveToDB(false, false);

    if (sConfigMgr->GetOption<bool>("Prestigious.Announcement", true))
    {
        std::string message = state->IsSacrifice ?
            Acore::StringFormatFmt("|cffFFFFFFPlayer |cff00FF00{} |cffFFFFFFhas |cffFF0000sacrificed|cffFFFFFF their item level |cff00FF00{}|cffFFFFFF equipment, |cffFFFFFFprestiging to prestige level |cff00FF00{}!|r", player->GetName(), state->AvgItemLevel, sPrestigeHandler->GetPrestigeLevel(player)) :
            Acore::StringFormatFmt("|cffFFFFFFPlayer |cff00FF00{} |cffFFFFFFhas prestiged to prestige level |cff00FF00{}!|r", player->GetName(), sPrestigeHandler->GetPrestigeLevel(player));

        sWorld->SendServerMessage(SERVER_MSG_STRING, message);
    }
}

bool PrestigeHandler::UnequipItems(Player* player)
{
    for (uint32 i = 0; i < INVENTORY_SLOT_BAG_START; ++i)
    {
        auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (!item)
        {
            continue;
        }

        ItemPosCountVec newDest;
        uint8 off_msg = player->CanStoreItem(NULL_BAG, NULL_SLOT, newDest, item, false);
        if (off_msg != EQUIP_ERR_OK)
        {
            return false;
        }

        player->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
        player->StoreItem(newDest, item, true);
    }

    return true;
}

TaskScheduler* PrestigeHandler::GetScheduler()
{
    return &scheduler;
}
