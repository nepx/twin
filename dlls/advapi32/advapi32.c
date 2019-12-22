
#include "if32.h"
#include "advapi32.h"
#include "platform.h"

#ifdef TWIN_BIN32

void
IT32_RegCloseKey()
{
    if32_stdcall_1(MapTableADVAPI32[280].maddr);
}

void
IT32_advapi32End()
{
    if32_stdcall_0(MapTableADVAPI32[500].maddr);
}

#endif
