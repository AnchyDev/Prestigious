#include "PrestigiousWorld.h"
#include "PrestigeHandler.h"

void PrestigiousWorldScript::OnUpdate(uint32 diff)
{
    sPrestigeHandler->GetScheduler()->Update(diff);
}

void PrestigiousWorldScript::OnAfterConfigLoad(bool reload)
{
    if (reload)
    {
        sPrestigeHandler->SavePrestigeLevels();
    }

    sPrestigeHandler->LoadRewards();
    sPrestigeHandler->LoadPrestigeLevels();
    sPrestigeHandler->LoadWhitelistedQuests();
}

void PrestigiousWorldScript::OnShutdownInitiate(ShutdownExitCode /*code*/, ShutdownMask /*mask*/)
{
    sPrestigeHandler->SavePrestigeLevels();
}
