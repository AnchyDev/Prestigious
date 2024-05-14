#ifndef MODULE_PRESTIGIOUS_HANDLER_H
#define MODULE_PRESTIGIOUS_HANDLER_H

#include "Player.h"
#include "TaskScheduler.h"

struct PrestigeReward
{
    uint32 Entry;
    uint32 Count;
    bool Scalable;
    uint32 PrestigeLevel;
};

class PrestigeHandler
{
private:
    PrestigeHandler();

public:
    static PrestigeHandler* GetInstance()
    {
        static PrestigeHandler instance;

        return &instance;
    }

    enum PrestigeHandlerConstants : uint32
    {
        ITEM_FIELD_FLAG_PRESTIGE_LOCK = 0x80000000, // This is equivalent to ITEM_FIELD_FLAG_UNK26, which seems to be unused.
        PLAYER_FLAGS_SACRIFICED = 0x80000000, // This is equivalent to PLAYER_FLAGS_UNK31, which seems to be unused.
        MAIL_SENDER_CHROMIE = 555111, // Used as the mail sender for rewards
        SPELL_AURA_BANISH = 44836, // Banish used to lock player
    };

    enum QueueState
    {
        QUEUE_RESET_INIT,
        QUEUE_RESET_EQUIPMENT,
        QUEUE_RESET_LEVEL,
        QUEUE_RESET_EQUIP_NEW_ITEMS,
        QUEUE_RESET_TELEPORT,
        QUEUE_RESET_SPELLS,
        QUEUE_RESET_RESEND_SPELLS,
        QUEUE_RESET_SKILLS,
        QUEUE_RESET_ACTIONS,
        QUEUE_RESET_ACTIONBAR,
        QUEUE_RESET_QUESTS,
        QUEUE_RESET_DESUMMON_PETS,
        QUEUE_RESET_COMPLETE,
        QUEUE_RESET_FAILED,
    };

    struct PrestigeState
    {
        QueueState QueueState;
        bool IsSacrifice;
        uint32 AvgItemLevel;
        std::string Message;
    };

    bool CanPrestige(Player* /*player*/);
    void DoPrestige(Player* /*player*/, bool /*sacrificeArmor*/);

    void ResetLevel(Player* /*player*/);
    void ResetTalents(Player* /*player*/);
    void ResetQuests(Player* /*player*/);
    void ResetHomebindAndPosition(Player* /*player*/);
    void ResetActionbar(Player* /*player*/);

    void ResetSpells(Player* /*player*/);
    void ResendRankedSpells(Player* /*player*/);
    void ResetSkills(Player* /*player*/);
    void DesummonMinion(Player* /*player*/);
    void DesummonPet(Player* /*player*/);

    void IterateItems(Player* /*player*/, bool /*deleteEquipped*/);
    void EquipDefaultItems(Player* /*player*/);
    bool UnequipItems(Player* /*player*/);

    bool IsRacialSpell(uint32 /*race*/, uint32 /*spellId*/);
    bool IsClassStarterSpell(uint32 /*class*/, uint32 /*spellId*/);
    bool IsGenericSpell(uint32 /*spellId*/);

    bool IsProfession(uint32 /*spellId*/);
    bool IsRecipe(uint32 /*spellId*/);
    bool IsHeirloom(Item* /*item*/);
    bool IsMount(uint32 /*spellId*/);
    bool IsNonCombatPet(uint32 /*spellId*/);
    bool IsStarterAmmo(uint32 /*itemId*/);
    bool IsRidingSpell(uint32 /*spellId*/);

    bool HasItemsEquipped(Player* /*player*/);
    bool HasNonStarterSpells(Player* /*player*/);

    void RewardPlayer(Player* /*player*/, float /*multiplier*/);
    void SacrificeRewardPlayer(Player* /*player*/, uint32 /*avgLevel*/);
    bool SendMailItems(Player* /*player*/, std::vector<std::pair<uint32, uint32>>& /*mailItems*/, std::string /*header*/, std::string /*body*/);

    void SetItemFlagged(Item* /*item*/, bool /*flag*/);
    void UnflagItems(Player* /*player*/);

    float GetMultiplierForItemLevel(uint32 /*itemLevel*/, bool /*isDeathKnight*/);

    void LoadPrestigeLevels();
    void SavePrestigeLevels();
    int32 GetPrestigeLevel(Player* /*player*/);
    void UpdatePrestigeLevel(Player* /*player*/, uint32 /*level*/);

    void LoadWhitelistedQuests();
    bool IsWhitelistedQuest(uint32 /*questId*/);

    void LoadRewards();
    void LoadItemLevelBrackets();

    float GetBaseMultiplier(bool /*isDeathKnight*/);

    void UpdateQueueState(Player* /*player*/, QueueState /*state*/);
    void AddPrestigeState(Player* /*player*/, PrestigeState /*state*/);
    void DequeuePrestige(Player* /*player*/);
    void HandleQueue();

    void QueueResetInit(Player* /*player*/);
    void QueueResetEquipment(Player* /*player*/, PrestigeState* /*state*/);
    void QueueResetLevel(Player* /*player*/);
    void QueueResetNewEquipment(Player* /*player*/);
    void QueueResetHomebindAndPosition(Player* /*player*/);
    void QueueResetSpells(Player* /*player*/);
    void QueueResetResendSpells(Player* /*player*/);
    void QueueResetSkills(Player* /*player*/);
    void QueueResetActions(Player* /*player*/);
    void QueueResetActionBar(Player* /*player*/);
    void QueueResetQuests(Player* /*player*/);
    void QueueResetDesummonPets(Player* /*player*/);
    void QueueResetComplete(Player* /*player*/, PrestigeState* /*state*/);

    TaskScheduler* GetScheduler();

private:
    std::unordered_set<uint32> genericSpellMap;
    std::unordered_map<Races, std::unordered_set<uint32>> racialMap;
    std::unordered_map<Classes, std::unordered_set<uint32>> spellMap;
    std::unordered_set<uint32> professionMap;
    std::vector<PrestigeReward> rewards;
    std::unordered_map<uint64, int32> prestigeLevels;
    std::unordered_set<uint32> whitelistQuests;
    std::map<uint32, float> itemLevelBrackets;
    std::unordered_map<Player*, PrestigeState> prestigeQueue;

    TaskScheduler scheduler;
};

#define sPrestigeHandler PrestigeHandler::GetInstance()

#endif // MODULE_PRESTIGIOUS_HANDLER_H
