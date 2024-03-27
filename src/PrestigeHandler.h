#ifndef MODULE_PRESTIGIOUS_HANDLER_H
#define MODULE_PRESTIGIOUS_HANDLER_H

#include "Player.h"
#include "TaskScheduler.h"

class PrestigeHandler
{
private:
    PrestigeHandler() { }

public:
    static PrestigeHandler* GetInstance()
    {
        static PrestigeHandler instance;

        return &instance;
    }

    bool CanPrestige(Player* /*player*/);
    void DoPrestige(Player* /*player*/);

    void ResetLevel(Player* /*player*/);
    void ResetSpells(Player* /*player*/);
    void ResetQuests(Player* /*player*/);
    void ResetHomebindAndPosition(Player* /*player*/);

    void DeleteItems(Player* /*player*/);
    void EquipDefaultItems(Player* /*player*/);
};

#define sPrestigeHandler PrestigeHandler::GetInstance()

#endif // MODULE_PRESTIGIOUS_HANDLER_H
