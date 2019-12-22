
#include "if32.h"
#include "version32.h"
#include "platform.h"

#ifdef TWIN_BIN32

void
IT32_GetFileVersionInfoA()
{
    if32_stdcall_4(MapTableVERSION[0].maddr);
}

void
IT32_GetFileVersionInfoSizeA()
{
    if32_stdcall_2(MapTableVERSION[1].maddr);
}

void
IT32_VerQueryValueA()
{
    if32_stdcall_4(MapTableVERSION[11].maddr);
}

void
IT32_Version32()
{
    if32_stdcall_0(MapTableVERSION[15].maddr);
}

#endif
