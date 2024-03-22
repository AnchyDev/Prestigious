#ifndef MODULE_PRESTIGIOUS_PLAYER_H
#define MODULE_PRESTIGIOUS_PLAYER_H

#include "ScriptMgr.h"
#include "Player.h"

class PrestigiousPlayerScript : public PlayerScript
{
public:
    PrestigiousPlayerScript() : PlayerScript("PrestigiousPlayerScript") { }

private:
    void OnLevelChanged(Player* /*player*/, uint8 /*oldLevel*/) override;
};

#endif // MODULE_PRESTIGIOUS_PLAYER_H
