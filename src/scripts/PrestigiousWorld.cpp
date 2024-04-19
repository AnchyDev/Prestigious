#include "PrestigiousWorld.h"
#include "PrestigeHandler.h"

void PrestigiousWorldScript::OnUpdate(uint32 diff)
{
    sPrestigeHandler->GetScheduler()->Update(diff);
}

void PrestigiousWorldScript::OnAfterConfigLoad(bool /*reload*/)
{
    sPrestigeHandler->LoadRewards();
}
