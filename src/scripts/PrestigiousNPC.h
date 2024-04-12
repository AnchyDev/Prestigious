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

private:
    enum PrestigiousConstants {
        PRESTIGE_OPTIONS = 1,
        PRESTIGE_DO_PRESTIGE = 2,
        PRESTIGE_DO_PRESTIGE_SACRIFICE = 3,

        PRESTIGE_TEXT_CANNOT_PRESTIGE = 555123,
        PRESTIGE_TEXT_CAN_PRESTIGE = 555124,
        PRESTIGE_TEXT_OPTIONS = 555125,

        SPELL_FREEZE = 9454
    };
};

#endif // MODULE_PRESTIGIOUS_NPC_H
