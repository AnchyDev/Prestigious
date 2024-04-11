#ifndef MODULE_PRESTIGIOUS_HANDLER_H
#define MODULE_PRESTIGIOUS_HANDLER_H

#include "Player.h"
#include "TaskScheduler.h"

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
        ITEM_FIELD_FLAG_PRESTIGE_LOCK = 0x80000000 // This is equivalent to ITEM_FIELD_FLAG_UNK26, which seems to be unused.
    };

    bool CanPrestige(Player* /*player*/);
    void DoPrestige(Player* /*player*/);

    void ResetLevel(Player* /*player*/);
    void ResetQuests(Player* /*player*/);
    void ResetHomebindAndPosition(Player* /*player*/);
    void ResetActionbar(Player* /*player*/);

    void UnlearnAllSpells(Player* /*player*/);
    void DesummonMinion(Player* /*players*/);
    void LearnRacials(Player* /*player*/);
    void LearnClassSpells(Player* /*player*/);

    /// <summary>
    /// Deletes/flags items from inventory, bank, buyback, keychain, bags, etc..
    /// </summary>
    /// <param name="player">The player to delete/flag the items from.</param>
    /// <returns>The average item level of deleted/flagged equipment.</returns>
    uint32 IterateItems(Player* /*player*/);
    void EquipDefaultItems(Player* /*player*/);
    bool UnequipItems(Player* /*player*/);

    bool IsRacialSpell(uint32 /*race*/, uint32 /*spellId*/);
    bool IsClassStarterSpell(uint32 /*class*/, uint32 /*spellId*/);

    bool IsProfession(uint32 /*spellId*/);
    bool IsRecipe(uint32 /*spellId*/);

    void RewardPlayer(Player* /*player*/, uint32 /*avgLevel*/);

    void SetItemFlagged(Item* /*item*/, bool /*flag*/);
    void UnflagItems(Player* /*player*/);

private:
    std::unordered_map<Races, std::unordered_set<uint32>> racialMap;
    std::unordered_map<Classes, std::unordered_set<uint32>> spellMap;
};

#define sPrestigeHandler PrestigeHandler::GetInstance()

#endif // MODULE_PRESTIGIOUS_HANDLER_H
