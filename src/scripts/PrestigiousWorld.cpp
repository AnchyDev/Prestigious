#include "PrestigiousWorld.h"
#include "PrestigeHandler.h"

void PrestigiousWorldScript::OnUpdate(uint32 diff)
{
    sPrestigeHandler->Scheduler.Update(diff);
}
