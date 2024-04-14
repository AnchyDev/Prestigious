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
    bool CanEquipItem(Player* /*player*/, uint8 /*slot*/, uint16& /*dest*/, Item* /*pItem*/, bool /*swap*/, bool /*not_loading*/) override;
    void OnEquip(Player* /*player*/, Item* /*it*/, uint8 /*bag*/, uint8 /*slot*/, bool /*update*/) override;
    void OnSave(Player* /*player*/) override;
};

#endif // MODULE_PRESTIGIOUS_PLAYER_H
