
#include "if32.h"
#include "commdlg32.h"
#include "platform.h"

#ifdef TWIN_BIN32

void
IT32_ChooseColorA()
{
    if32_stdcall_1(MapTableCOMMDLG[101].maddr);
}

void
IT32_ChooseFontA()
{
    if32_stdcall_1(MapTableCOMMDLG[103].maddr);
}

void
IT32_CommDlgExtendedError()
{
    if32_stdcall_0(MapTableCOMMDLG[105].maddr);
}

void
IT32_FindTextA()
{
    if32_stdcall_1(MapTableCOMMDLG[106].maddr);
}

void
IT32_GetFileTitleA()
{
    if32_stdcall_3(MapTableCOMMDLG[108].maddr);
}

void
IT32_GetOpenFileNameA()
{
    if32_stdcall_1(MapTableCOMMDLG[110].maddr);
}

void
IT32_GetSaveFileNameA()
{
    if32_stdcall_1(MapTableCOMMDLG[112].maddr);
}

void
IT32_PrintDlgA()
{
    if32_stdcall_1(MapTableCOMMDLG[117].maddr);
}

void
IT32_ReplaceTextA()
{
    if32_stdcall_1(MapTableCOMMDLG[119].maddr);
}

#endif
