
#include "if32.h"
#include "shell32.h"
#include "platform.h"

#ifdef TWIN_BIN32

void
IT32_Control_RunDLL()
{
    if32_stdcall_4(MapTableSHELL[135].maddr);
}

void
IT32_Control_RunDLLA()
{
    if32_stdcall_4(MapTableSHELL[138].maddr);
}

void
IT32_DragAcceptFiles()
{
    if32_stdcall_2(MapTableSHELL[201].maddr);
}

void
IT32_DragFinish()
{
    if32_stdcall_1(MapTableSHELL[202].maddr);
}

void
IT32_DragQueryFile()
{
    if32_stdcall_4(MapTableSHELL[226].maddr);
}

void
IT32_DragQueryFileA()
{
    if32_stdcall_4(MapTableSHELL[228].maddr);
}

void
IT32_SHGetDesktopFolder()
{
    if32_stdcall_1(MapTableSHELL[287].maddr);
}

void
IT32_SHGetFileInfo()
{
    if32_stdcall_5(MapTableSHELL[288].maddr);
}

void
IT32_SHGetFileInfoA()
{
    if32_stdcall_5(MapTableSHELL[289].maddr);
}

void
IT32_ShellAboutA()
{
    if32_stdcall_4(MapTableSHELL[326].maddr);
}

void
IT32_ShellAboutW()
{
    if32_stdcall_4(MapTableSHELL[327].maddr);
}

void
IT32_ShellExecuteA()
{
    if32_stdcall_6(MapTableSHELL[328].maddr);
}

void
IT32_Shell32End()
{
    if32_stdcall_0(MapTableSHELL[520].maddr);
}

#endif
