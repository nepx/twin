
#include "if32.h"
#include "commctrl32.h"
#include "platform.h"

#ifdef TWIN_BIN32

void
IT32_MenuHelp()
{
    if32_stdcall_7(MapTableCOMCTL32[2].maddr);
}

void
IT32_DrawStatusTextA()
{
    if32_stdcall_4(MapTableCOMCTL32[5].maddr);
}

void
IT32_CreateStatusWindowA()
{
    if32_stdcall_4(MapTableCOMCTL32[6].maddr);
}

void
IT32_CreateMappedBitmap()
{
    if32_stdcall_5(MapTableCOMCTL32[8].maddr);
}

void
IT32_CreatePropertySheetPage()
{
    if32_stdcall_1(MapTableCOMCTL32[12].maddr);
}

void
IT32_MakeDragList()
{
    if32_stdcall_1(MapTableCOMCTL32[13].maddr);
}

void
IT32_DrawInsert()
{
    if32_stdcall_3(MapTableCOMCTL32[15].maddr);
}

void
IT32_InitCommonControls()
{
    if32_stdcall_0(MapTableCOMCTL32[17].maddr);
}

void
IT32_CreatePropertySheetPageA()
{
    if32_stdcall_1(MapTableCOMCTL32[18].maddr);
}

void
IT32_CreatePropertySheetPageW()
{
    if32_stdcall_1(MapTableCOMCTL32[19].maddr);
}

void
IT32_CreateToolbarEx()
{
    if32_stdcall_13(MapTableCOMCTL32[22].maddr);
}

void
IT32_DestroyPropertySheetPage()
{
    if32_stdcall_1(MapTableCOMCTL32[23].maddr);
}

void
IT32_ImageList_Add()
{
    if32_stdcall_3(MapTableCOMCTL32[40].maddr);
}

void
IT32_ImageList_AddMasked()
{
    if32_stdcall_3(MapTableCOMCTL32[42].maddr);
}

void
IT32_ImageList_BeginDrag()
{
    if32_stdcall_4(MapTableCOMCTL32[43].maddr);
}

void
IT32_ImageList_Create()
{
    if32_stdcall_5(MapTableCOMCTL32[45].maddr);
}

void
IT32_ImageList_Destroy()
{
    if32_stdcall_1(MapTableCOMCTL32[46].maddr);
}

void
IT32_ImageList_DragEnter()
{
    if32_stdcall_3(MapTableCOMCTL32[47].maddr);
}

void
IT32_ImageList_DragLeave()
{
    if32_stdcall_1(MapTableCOMCTL32[48].maddr);
}

void
IT32_ImageList_DragMove()
{
    if32_stdcall_2(MapTableCOMCTL32[49].maddr);
}

void
IT32_ImageList_DragShowNolock()
{
    if32_stdcall_1(MapTableCOMCTL32[50].maddr);
}

void
IT32_ImageList_Draw()
{
    if32_stdcall_6(MapTableCOMCTL32[51].maddr);
}

void
IT32_ImageList_DrawEx()
{
    if32_stdcall_10(MapTableCOMCTL32[52].maddr);
}

void
IT32_ImageList_EndDrag()
{
    if32_stdcall_0(MapTableCOMCTL32[55].maddr);
}

void
IT32_ImageList_GetBkColor()
{
    if32_stdcall_1(MapTableCOMCTL32[56].maddr);
}

void
IT32_ImageList_GetDragImage()
{
    if32_stdcall_2(MapTableCOMCTL32[57].maddr);
}

void
IT32_ImageList_GetIcon()
{
    if32_stdcall_3(MapTableCOMCTL32[58].maddr);
}

void
IT32_ImageList_GetIconSize()
{
    if32_stdcall_3(MapTableCOMCTL32[59].maddr);
}

void
IT32_ImageList_GetImageCount()
{
    if32_stdcall_1(MapTableCOMCTL32[60].maddr);
}

void
IT32_ImageList_GetImageInfo()
{
    if32_stdcall_3(MapTableCOMCTL32[61].maddr);
}

void
IT32_ImageList_LoadImage()
{
    if32_stdcall_7(MapTableCOMCTL32[63].maddr);
}

void
IT32_ImageList_LoadImageA()
{
    if32_stdcall_7(MapTableCOMCTL32[64].maddr);
}

void
IT32_ImageList_LoadImageW()
{
    if32_stdcall_7(MapTableCOMCTL32[65].maddr);
}

void
IT32_ImageList_Merge()
{
    if32_stdcall_6(MapTableCOMCTL32[66].maddr);
}

void
IT32_ImageList_Remove()
{
    if32_stdcall_2(MapTableCOMCTL32[68].maddr);
}

void
IT32_ImageList_Replace()
{
    if32_stdcall_4(MapTableCOMCTL32[69].maddr);
}

void
IT32_ImageList_ReplaceIcon()
{
    if32_stdcall_3(MapTableCOMCTL32[70].maddr);
}

void
IT32_ImageList_SetBkColor()
{
    if32_stdcall_2(MapTableCOMCTL32[75].maddr);
}

void
IT32_ImageList_SetDragCursorImage()
{
    if32_stdcall_4(MapTableCOMCTL32[76].maddr);
}

void
IT32_ImageList_SetIconSize()
{
    if32_stdcall_3(MapTableCOMCTL32[79].maddr);
}

void
IT32_ImageList_SetOverlayImage()
{
    if32_stdcall_3(MapTableCOMCTL32[81].maddr);
}

void
IT32_PropertySheet()
{
    if32_stdcall_1(MapTableCOMCTL32[86].maddr);
}

void
IT32_PropertySheetA()
{
    if32_stdcall_1(MapTableCOMCTL32[87].maddr);
}

void
IT32_PropertySheetW()
{
    if32_stdcall_1(MapTableCOMCTL32[88].maddr);
}

void
IT32_Commctrl32End()
{
    if32_stdcall_0(MapTableCOMCTL32[400].maddr);
}

#endif
