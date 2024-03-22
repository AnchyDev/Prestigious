#ifndef MODULE_PRESTIGIOUS_HANDLER_H
#define MODULE_PRESTIGIOUS_HANDLER_H

#include "Player.h"

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

    void ResetLevel(ObjectGuid /*guid*/, bool /*isHeroClass*/);
    void ResetSpells(ObjectGuid /*guid*/);
    void ResetQuests(ObjectGuid /*guid*/);
    void ResetHomebindAndPosition(ObjectGuid /*guid*/, uint32 /*race*/, uint32 /*class*/);

    void StoreAllItems(ObjectGuid /*guid*/);
};

#define sPrestigeHandler PrestigeHandler::GetInstance()

#endif // MODULE_PRESTIGIOUS_HANDLER_H
