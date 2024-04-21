#include "PrestigiousWorld.h"
#include "PrestigeHandler.h"

#include "Config.h"

void PrestigiousWorldScript::OnUpdate(uint32 diff)
{
    if (!sConfigMgr->GetOption<bool>("Prestigious.Enable", true))
    {
        return;
    }

    sPrestigeHandler->GetScheduler()->Update(diff);
    sPrestigeHandler->HandleQueue();
}

void PrestigiousWorldScript::OnAfterConfigLoad(bool reload)
{
    if (!sConfigMgr->GetOption<bool>("Prestigious.Enable", true))
    {
        return;
    }

    if (reload)
    {
        sPrestigeHandler->SavePrestigeLevels();
    }

    sPrestigeHandler->LoadRewards();
    sPrestigeHandler->LoadPrestigeLevels();
    sPrestigeHandler->LoadWhitelistedQuests();
    sPrestigeHandler->LoadItemLevelBrackets();
}

void PrestigiousWorldScript::OnShutdownInitiate(ShutdownExitCode /*code*/, ShutdownMask /*mask*/)
{
    if (!sConfigMgr->GetOption<bool>("Prestigious.Enable", true))
    {
        return;
    }

    sPrestigeHandler->SavePrestigeLevels();
}
