#ifndef MODULE_PRESTIGIOUS_WORLD_H
#define MODULE_PRESTIGIOUS_WORLD_H

#include "ScriptMgr.h"

class PrestigiousWorldScript : public WorldScript
{
public:
    PrestigiousWorldScript() : WorldScript("PrestigiousWorldScript") { }

private:
    void OnUpdate(uint32 /*diff*/) override;
    void OnAfterConfigLoad(bool /*reload*/) override;
};

#endif // MODULE_PRESTIGIOUS_WORLD_H
