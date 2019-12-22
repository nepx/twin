
#include "if32.h"
#include "msvcrt32.h"
#include "platform.h"

#ifdef TWIN_BIN32

void
IT32_msvcrt32End()
{
    if32_stdcall_0(MapTableMSVCRT[1200].maddr);
}

#endif
