#include "Prestigious.h"

#include "scripts/PrestigiousNPC.h"
#include "scripts/PrestigiousWorld.h"
#include "scripts/PrestigiousPlayer.h"

void SC_AddPrestigiousScripts()
{
    new PrestigiousNPCScript();
    new PrestigiousWorldScript();
    new PrestigiousPlayerScript();
}
