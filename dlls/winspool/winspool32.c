
#include "if32.h"
#include "winspool32.h"
#include "platform.h"

#ifdef TWIN_BIN32

void
IT32_Version32End()
{
    if32_stdcall_0(MapTableWINSPOOL[256].maddr);
}

#endif
