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

    bool CanPrestige(Player* /*player*/);
    void DoPrestige(Player* /*player*/);

    void ResetLevel(Player* /*player*/);
    void ResetQuests(Player* /*player*/);
    void ResetHomebindAndPosition(Player* /*player*/);

    void UnlearnAllSpells(Player* /*player*/);
    void DesummonMinion(Player* /*players*/);
    void LearnRacials(Player* /*player*/);
    void LearnClassSpells(Player* /*player*/);

    void DeleteItems(Player* /*player*/);
    void EquipDefaultItems(Player* /*player*/);

    bool IsRacialSpell(uint32 /*race*/, uint32 /*spellId*/);
    bool IsClassStarterSpell(uint32 /*class*/, uint32 /*spellId*/);

private:
    std::unordered_map<Races, std::unordered_set<uint32>> racialMap;
    std::unordered_map<Classes, std::unordered_set<uint32>> spellMap;
};

#define sPrestigeHandler PrestigeHandler::GetInstance()

#endif // MODULE_PRESTIGIOUS_HANDLER_H
