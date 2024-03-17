#ifndef MODULE_PRESTIGIOUS_NPC_H
#define MODULE_PRESTIGIOUS_NPC_H

#include "ScriptMgr.h"

class PrestigiousNPCScript : public CreatureScript
{
public:
    PrestigiousNPCScript() : CreatureScript("PrestigiousNPCScript") { }

private:
    bool OnGossipHello(Player* /*player*/, Creature* /*creature*/) override;
    bool OnGossipSelect(Player* /*player*/, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/) override;

    bool CanPrestige(Player* /*player*/);
    void DoPrestige(Player* /*player*/);

    void LockCharacter(ObjectGuid /*guid*/);
    void UnlockCharacter(ObjectGuid /*guid*/);

    void ResetLevel(ObjectGuid /*guid*/, bool /*isHeroClass*/);
    void ResetSpells(ObjectGuid /*guid*/);
    void ResetQuests(ObjectGuid /*guid*/);
    void ResetHomebindAndPosition(ObjectGuid /*guid*/, uint32 /*race*/, uint32 /*class*/);

private:
    enum PrestigiousConstants {
        PRESTIGE_DO_PRESTIGE = 1,

        PRESTIGE_TEXT_CANNOT_PRESTIGE = 555123,
        PRESTIGE_TEXT_CAN_PRESTIGE = 555124
    };
};

#endif // MODULE_PRESTIGIOUS_NPC_H
