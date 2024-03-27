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

    void LockCharacter(ObjectGuid /*guid*/);
    void UnlockCharacter(ObjectGuid /*guid*/);

    void ResetLevel(Player* /*player*/);
    void ResetSpells(Player* /*player*/);
    void ResetQuests(Player* /*player*/);
    void ResetHomebindAndPosition(ObjectGuid /*guid*/, uint8 /*race*/, uint8 /*class*/);

    /// <summary>
    /// Stored all the items in player inventory, bank, keychain, etc.. into void to be retrieved later.
    /// </summary>
    /// <param name="guid">The player globally unique identifier (guid).</param>
    void StoreAllItems(ObjectGuid /*guid*/);

    /// <summary>
    /// Adds the default starting items to the player.
    /// </summary>
    /// <param name="guid">The player globally unique identifier (guid).</param>
    /// <param name="pRace">The player characters race.</param>
    /// <param name="pClass">The player characters class.</param>
    /// <param name="pGender">The player characters gender.</param>
    void AddDefaultItems(ObjectGuid /*guid*/, uint8 /*pRace*/, uint8 /*pClass*/, uint8 /*pGender*/);

    int32 InventoryTypeToEquipSlot(uint32 /*invType*/);

public:
    TaskScheduler Scheduler;
};

#define sPrestigeHandler PrestigeHandler::GetInstance()

#endif // MODULE_PRESTIGIOUS_HANDLER_H
