/*    
	MenuAPI.c	2.40
    	Copyright 1997 Willows Software, Inc. 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.


For more information about the Willows Twin Libraries.

	http://www.willows.com	

To send email to the maintainer of the Willows Twin Libraries.

	mailto:twin@willows.com 

 */



#ifndef OEMRESOURCE
#define OEMRESOURCE	/* for OBM_CLOSE */
#endif
#include "windows.h"
#include "windowsx.h"
#undef OEMRESOURCE

#include "Log.h"
#include "kerndef.h"
#include "WinDefs.h"
#include "Menus.h"
#include "Listbox.h"
#include "Resources.h"	/* MENUDATA */
#include "Frame.h"
#include "Messages.h"
#include "Module.h"

typedef MENUDATA *LPMENUDATA;

/* internal */
static LPMENUDATA ProcessMenuTemplate(LPBYTE *);
static BOOL ModifyMenuEx(HMENU32, UINT, UINT, UINT, LPCSTR, UINT);
static BOOL ChangeMIFlags(HMENU32, UINT, UINT, UINT);
static HBITMAP LoadSysMenuBitmap();

HMENU32 TWIN_FindMenuItem(HMENU32,UINT);
HMENU TWIN_LoadMenuIndirect(LPMENUDATA);

static HBITMAP SystemBitmaps[8];

#define SB_OBM_CLOSE_L	0
#define SB_OBM_SYSMENU	1
#define SB_OBM_RESTORE  2
#define SB_OBM_REDUCE	3
#define SB_OBM_ZOOM	4
#define SB_OBM_RESTORED 5
#define SB_OBM_REDUCED	6
#define SB_OBM_ZOOMD	7

/* exported */
LPSTR GetMenuItemData(HMENU,UINT,UINT);
WORD MeasureWindowMenu(HWND,HWND); /* to MenuProc, WindowCreate */
DWORD CalcPopupMenuDimensions(HMENU, HWND); /* to MenuProc */
void MenuDrawItem(HDC,HMENU32,HWND,WORD,WORD); /* to MenuProc */
void DrawRestoreGlyph(HDC, LPRECT, BOOL);
void ModifyMenuBar(HWND,HWND,UINT);
HMENU ModifySystemMenu(HWND,UINT);

/* imported */
extern BOOL CheckMenuHandle32(HMENU);
extern LONG LBoxAPI(HMENU32, UINT, LPARAM);
extern HPEN 	GetSysColorPen(int);
extern BOOL TWIN_RedrawWindow(HWND, const RECT *, HRGN, UINT);
extern HFONT TWIN_GetMenuFont();

BOOL WINAPI		/* API */
IsMenu(HMENU hMenu)
{
    return CheckMenuHandle32(hMenu);
}

HMENU WINAPI		/* API */
CreateMenu()
{
    MENUCREATESTRUCT mc;
    DWORD dwRet;

    mc.hFont = TWIN_GetMenuFont();
    mc.dwStyle = LBS_PRELOADED|LBS_OWNERDRAWVARIABLE|
		 LBS_HASSTRINGS|LBS_NOTIFY;
    dwRet = GetMenuCheckMarkDimensions();
    mc.dwIndents = MAKELONG(LOWORD(dwRet)+2,LOWORD(dwRet));
    return (HMENU)LBoxAPI((HMENU32)NULL,LBA_CREATE,(LPARAM)&mc);
}

HMENU WINAPI		/* API */
CreatePopupMenu()
{
    return CreateMenu();
}

BOOL WINAPI		/* API */
DestroyMenu(HMENU hMenu)
{
    HMENU32 hMenu32;
    BOOL bResult;

    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return FALSE;

    bResult = LBoxAPI(hMenu32,LBA_DESTROY,0);
    RELEASELBOXINFO(hMenu32);
    return bResult;
}

static BOOL
ModifyMenuEx(HMENU32 hMenu32, UINT uiPosition, UINT uiFlags,
			UINT uiIDNewItem, LPCSTR lpNewItem, UINT uiAction)
{
    MENUITEMSTRUCT mis;
    DWORD dwIndents;
    LONG lFlags = 0;
    WORD wIndex;
    BOOL rc;
    LPSTR lpItem = (LPSTR)lpNewItem;
    HMENU32 hMenu32orig = hMenu32;

    if (!hMenu32)
	return FALSE;
    memset((char *)&mis, '\0', sizeof(MENUITEMSTRUCT));
    mis.wItemFlags = uiFlags;
    if (uiAction == LBA_MODIFYITEM) {
	mis.wPosition = (WORD)uiPosition;
	mis.wIDNewItem = (WORD)uiPosition;
    }
    else {
	if (uiFlags & MF_BYPOSITION)
	    mis.wPosition = (WORD)uiPosition;
	else 
	    mis.wPosition = (WORD)uiIDNewItem;
	mis.wIDNewItem = (WORD)uiIDNewItem;
    }
    if ((uiAction != LBA_APPENDITEM) && (!((uiAction == LBA_INSERTITEM) &&
		(uiFlags & MF_BYPOSITION)))) {
	mis.wAction = LCA_GET|LCA_FLAGS;
	lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);
	if (lFlags == (LONG)-1) {
	    if (((uiAction == LBA_INSERTITEM) || (uiAction == LBA_MODIFYITEM))
			 && (!(uiFlags & MF_BYPOSITION))) 
		hMenu32 = TWIN_FindMenuItem(hMenu32,uiPosition);
	    else
		hMenu32 = TWIN_FindMenuItem(hMenu32,uiIDNewItem);
	    if (!hMenu32) {
		return FALSE;
	    }
	    mis.wItemFlags &= ~MF_BYPOSITION;
	    mis.wAction = LCA_GET|LCA_FLAGS;
	    lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);
	    if (lFlags == (LONG)-1) {
	        if (hMenu32 != hMenu32orig)
		  RELEASELBOXINFO(hMenu32); 
		return FALSE;
	    }
	}
    }
    uiFlags |= MF_BYPOSITION;
    if (uiAction == LBA_MODIFYITEM) {
	wIndex = mis.wPosition;
	mis.wPosition = (WORD)-1;
	mis.wAction = LCA_INDENTS;
	dwIndents = (DWORD)LBoxAPI(hMenu32,LBA_GETDATA,(LPARAM)&mis); 
	mis.wLeftIndent = LOWORD(dwIndents);
	mis.wRightIndent = HIWORD(dwIndents);
	mis.wIDNewItem = (WORD)uiIDNewItem;
	mis.wPosition = wIndex;
	mis.wAction = LCA_SET|LCA_ALL;
    }
    else if (uiAction == LBA_DELETEITEM) {
	if (lFlags & MF_POPUP) {
	    mis.wItemFlags = MF_BYPOSITION;
	    mis.wAction = LCA_GET | LCA_ITEMID;
	    DestroyMenu((HMENU)(UINT)LOWORD(LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis)));
	}
    }
    if (uiFlags & MF_BITMAP) {
	if (!IsGDIObject((HANDLE)lpNewItem)) {
	/* special case -- pre-defined system bitmap (undocumented) */
	    switch (LOWORD((DWORD)lpNewItem)) {
		case SB_OBM_CLOSE_L:
		case SB_OBM_REDUCE:
		case SB_OBM_ZOOM:
		    break;
		case SB_OBM_SYSMENU:
		/* Here we can substitute the bitmap right away */
		    if (!SystemBitmaps[SB_OBM_SYSMENU])
			LoadSysMenuBitmap();
		    lpItem = (LPSTR)(DWORD)SystemBitmaps[SB_OBM_SYSMENU];
		    break;
		case SB_OBM_RESTORE:
		/* This one we leave as special because it uses */
		/* different bitmaps when pressed/unpressed */
		    if (!SystemBitmaps[SB_OBM_RESTORE]) {
			SystemBitmaps[SB_OBM_RESTORE] =
				LoadBitmap(0,(LPSTR)OBM_RESTORE);
			SystemBitmaps[SB_OBM_RESTORED] =
				LoadBitmap(0,(LPSTR)OBM_RESTORED);
		    }
		    break;
		default:
		    break; /* error */
	    }
	}
    }
    mis.wItemFlags = (WORD)uiFlags;
    mis.lpItemData = lpItem;
    rc = (BOOL)LBoxAPI(hMenu32,uiAction,(LPARAM)&mis);
    if (hMenu32 != hMenu32orig)
      RELEASELBOXINFO(hMenu32);
    return rc;
}


/*  WARNING:  This has a reference-counting problem:  If the item is in a
    POPUP menu, that popup's LBOXINFO will be ref-counted when gotten
    and must be "del-ref'd" when the caller is done with it. */
HMENU32
TWIN_FindMenuItem(HMENU32 hMenu32, UINT uiIDItem)
{
    UINT uiItem;
    LONG lFlags;
    MENUITEMSTRUCT mnis;
    int nCount,i;
    HMENU32 hMenu32Ret;

    if (!hMenu32)
	return 0L;
    memset((char *)&mnis, '\0', sizeof(MENUITEMSTRUCT));
    mnis.wPosition = (WORD)-1;
    mnis.wAction = LCA_ITEMCOUNT;
    nCount = (int)LBoxAPI(hMenu32,LBA_GETDATA,(LPARAM)&mnis);
    mnis.wItemFlags = MF_BYPOSITION;
    for (i=0; i<nCount; i++) {
	mnis.wPosition = (WORD)i;
	mnis.wAction = LCA_GET|LCA_ITEMID;
	uiItem = (UINT)LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	if (uiItem == uiIDItem)
	    return hMenu32;
	mnis.wAction = LCA_GET|LCA_FLAGS;
	lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	if (LOWORD(lFlags) & MF_POPUP) {
	    if ((hMenu32Ret = TWIN_FindMenuItem
			(GETMENUINFO((HMENU)uiItem),uiIDItem)))
		return hMenu32Ret;
	}
    }
    return (HMENU32)0;
}


BOOL WINAPI		/* API */
InsertMenu(HMENU hMenu, UINT uiPosition, UINT uiFlags,
			UINT uiIDNewItem, LPCSTR lpNewItem)
{
    BOOL bResult = FALSE;
    HMENU32 hMenu32;
    APISTR((LF_APICALL,"InsertMenu(%x,%x,%x,%x,%p)\n",
	hMenu,uiPosition,uiFlags,uiIDNewItem,lpNewItem));
    if((hMenu32 = GETMENUINFO(hMenu))) {
	bResult = ModifyMenuEx(hMenu32,
			uiPosition,uiFlags,uiIDNewItem,lpNewItem,
			LBA_INSERTITEM);
	RELEASELBOXINFO(hMenu32);
    }
    APISTR((LF_APIRET,"InsertMenu: returns BOOL %d\n",bResult));
    return bResult;
}

BOOL WINAPI
InsertMenuItem(HMENU hMenu,UINT ItemID,BOOL bPosition,LPMENUITEMINFO lpmenuinfo)
{
	BOOL rc;
    	HMENU32 hMenu32;
 	UINT opcode;
	
    	APISTR((LF_APICALL,"InsertMenuItem(%x,%x,%d,%p)\n",
		hMenu,ItemID,bPosition,lpmenuinfo));

	if(bPosition) {
		if(ItemID == (UINT) -1)
			opcode = LBA_APPENDITEM;
		else
			opcode = LBA_INSERTITEM;
	}
	else	opcode = LBA_INSERTITEM;

	if(lpmenuinfo->hSubMenu) {
		
		rc = AppendMenu(hMenu,MF_POPUP,(UINT)lpmenuinfo->hSubMenu,
			lpmenuinfo->dwTypeData);

    		APISTR((LF_APIRET,"InsertMenuItem: returns BOOL %d\n",rc));
		return rc;
	}
    	rc = ModifyMenuEx(hMenu32 = GETMENUINFO(hMenu),
			ItemID,
			lpmenuinfo->fType|lpmenuinfo->fState, 	       
							/* same as uiflags */
			lpmenuinfo->wID,   /* id or position*/
			lpmenuinfo->dwTypeData, opcode);

    	RELEASELBOXINFO(hMenu32);

    	APISTR((LF_APIRET,"InsertMenuItem: returns BOOL %d\n",rc));
	return rc;
}

BOOL WINAPI		/* API */
AppendMenu(HMENU hMenu, UINT uiFlags, UINT uiIDNewItem, LPCSTR lpNewItem)
{
    BOOL bResult = FALSE;
    HMENU32 hMenu32;
    APISTR((LF_APICALL,"AppendMenu(%x,%x,%d,%p)\n",
	hMenu,uiFlags,uiIDNewItem,lpNewItem));
    if((hMenu32 = GETMENUINFO(hMenu))) {
    	bResult = ModifyMenuEx(hMenu32,(UINT)-1,uiFlags,uiIDNewItem,lpNewItem,
			LBA_APPENDITEM);
    	RELEASELBOXINFO(hMenu32);
    }
    APISTR((LF_APIRET,"AppendMenu: returns BOOL %d\n",bResult));
    return bResult;
}

BOOL WINAPI 		/* API */
ModifyMenu(HMENU hMenu, UINT uiPosition, UINT uiFlags,
			UINT uiIDNewItem, LPCSTR lpNewItem)
{
    BOOL bResult = FALSE;
    HMENU32 hMenu32;
    APISTR((LF_APICALL,"ModifyMenu(%x,%x,%x,%x,%p)\n",
	hMenu,uiPosition,uiFlags,uiIDNewItem,lpNewItem));
    if((hMenu32 = GETMENUINFO(hMenu))) {
    	bResult =  ModifyMenuEx(hMenu32,
			uiPosition,uiFlags,uiIDNewItem,lpNewItem,
			LBA_MODIFYITEM);
    	RELEASELBOXINFO(hMenu32);
    }
    APISTR((LF_APIRET,"ModifyMenu: returns BOOL %d\n",bResult));
    return bResult;
}

BOOL WINAPI		/* API */
RemoveMenu(HMENU hMenu, UINT idItem, UINT uiFlags)
{
    BOOL bResult = FALSE;
    HMENU32 hMenu32;
    if ((hMenu32 = GETMENUINFO(hMenu))) {
	if (uiFlags & MF_BYPOSITION) {
		bResult = ModifyMenuEx(hMenu32,idItem,uiFlags,
				0,NULL,LBA_REMOVEITEM);
	}
	else {
		bResult = ModifyMenuEx(hMenu32,0,uiFlags,
				idItem,NULL,LBA_REMOVEITEM);
	}
	RELEASELBOXINFO(hMenu32);
    }
    return bResult;
}

BOOL WINAPI		/* API */
DeleteMenu(HMENU hMenu, UINT idItem, UINT uiFlags)
{
    BOOL bResult = FALSE;
    HMENU32 hMenu32;
    if ((hMenu32 = GETMENUINFO(hMenu))) {
	if (uiFlags & MF_BYPOSITION) {
		bResult =  ModifyMenuEx(hMenu32,idItem,
				uiFlags,0,NULL,LBA_DELETEITEM);
	}
	else {
        	bResult =  ModifyMenuEx(hMenu32,0,
				uiFlags,idItem,NULL, LBA_DELETEITEM);
	}
	RELEASELBOXINFO(hMenu32);
    }
    return bResult;
}

BOOL WINAPI 		/* API */
ChangeMenu(HMENU hMenu, UINT uiPosition, LPCSTR lpNewItem,
			UINT uiIDNewItem, UINT uiFlags)
{
    BOOL rc;

    if (lpNewItem == NULL)
	uiFlags |= MF_SEPARATOR;
    if (uiFlags & MF_CHANGE) {
	uiFlags &= ~MF_CHANGE;
	rc = ModifyMenu(hMenu,uiPosition,uiFlags,uiIDNewItem,lpNewItem);
    }
    else if (uiFlags & MF_APPEND) {
	uiFlags &= ~MF_APPEND;
	rc = AppendMenu(hMenu,uiFlags,uiIDNewItem,lpNewItem);
    }
    else if (uiFlags & MF_DELETE) {
	uiFlags &= ~MF_DELETE;
	rc = DeleteMenu(hMenu,uiPosition,uiFlags);
    }
    else if (uiFlags & MF_REMOVE) {
	uiFlags &= ~MF_REMOVE;
	rc = RemoveMenu(hMenu,uiPosition,uiFlags);
    }
    else 
	rc = InsertMenu(hMenu,uiPosition,uiFlags,uiIDNewItem,lpNewItem);
    
    return rc;
}

static BOOL
ChangeMIFlags(HMENU32 hMenu32, UINT uiItem, UINT uiFlags, UINT uiMask)
{
    MENUITEMSTRUCT mis;
    LONG lFlags;
    HMENU32 hMenu32orig = hMenu32;

    if (!hMenu32)
	return -1;
    memset((char *)&mis, '\0', sizeof(MENUITEMSTRUCT));
    mis.wPosition = (WORD)uiItem;
    mis.wAction = LCA_GET | LCA_FLAGS;
    mis.wItemFlags = uiFlags;
    lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);
    if (lFlags < 0) { 
	if (uiFlags & MF_BYPOSITION) 
	    return -1;
	hMenu32 = TWIN_FindMenuItem(hMenu32,uiItem);
	if (!hMenu32)
	    return -1;
	mis.wAction = LCA_GET|LCA_FLAGS;
	lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);
	if (lFlags < 0) {
	    if (hMenu32 != hMenu32orig)
	      RELEASELBOXINFO(hMenu32);
	    return -1;
	}
    }
    mis.wItemFlags = LOWORD(lFlags) | MF_BYPOSITION;
    if (uiFlags & uiMask)
	mis.wItemFlags |= uiMask;
    else
	mis.wItemFlags &= ~uiMask;
    if (uiMask == MF_DISABLED) {
	if (uiFlags & MF_GRAYED)
	    mis.wItemFlags |= MF_GRAYED;
	else
	    mis.wItemFlags &= ~MF_GRAYED;
    }
    mis.wAction = LCA_SET | LCA_FLAGS;
    if (LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis) < 0) {
        if (hMenu32 != hMenu32orig)
	    RELEASELBOXINFO(hMenu32);
	return -1;
    }
    if (hMenu32 != hMenu32orig)
      RELEASELBOXINFO(hMenu32);
    return (LOWORD(lFlags) & uiMask);
}
	

BOOL WINAPI		/* API */
EnableMenuItem(HMENU hMenu, UINT uiIDEnableItem, UINT uiEnable)
{
    LONG retcode = 0;
    HMENU32 hMenu32;

    hMenu32 = GETMENUINFO(hMenu);
    if(hMenu32) {
	retcode = (BOOL)ChangeMIFlags(hMenu32,
		uiIDEnableItem, uiEnable,MF_DISABLED);
	RELEASELBOXINFO(hMenu32);
    }
    return retcode;
}

BOOL WINAPI		/* API */
CheckMenuItem(HMENU hMenu, UINT uiIDCheckItem, UINT uiCheck)
{
    BOOL rc = 0;
    HMENU32 hMenu32;

    hMenu32 = GETMENUINFO(hMenu);
    if(hMenu32) {
    	rc = (BOOL)ChangeMIFlags(hMenu32, 
		uiIDCheckItem,uiCheck,MF_CHECKED);
    	RELEASELBOXINFO(hMenu32);
    }
    return rc;
}

BOOL WINAPI		/* API */
HiliteMenuItem(HWND hWnd, HMENU hMenu, UINT uiIDHiliteItem, UINT uiHilite)
{
    BOOL bResult = 0;
    HMENU32 hMenu32;

    hMenu32 = GETMENUINFO(hMenu);
    if(hMenu32) {
    	bResult = (BOOL)ChangeMIFlags(hMenu32, 
	       uiIDHiliteItem,uiHilite,MF_HILITE);
    	RELEASELBOXINFO(hMenu32);
    }
    return bResult;
}

HMENU
TWIN_LoadMenuIndirect(LPMENUDATA lpMenu)
{
    HMENU hMenu;

    if (lpMenu == 0)
	return 0;
    hMenu = CreateMenu();
    if (hMenu == 0)
	return 0;
    while(1) {
	if (lpMenu->menuflag & MF_POPUP)
	    lpMenu->menuid = (UINT)TWIN_LoadMenuIndirect(lpMenu->menupopup);
	if (!(lpMenu->menuflag &
		(MF_BITMAP|MF_MENUBARBREAK|MF_MENUBREAK|MF_OWNERDRAW)) &&
	    (!lpMenu->menuname || !strlen(lpMenu->menuname)))
	    lpMenu->menuflag |= MF_SEPARATOR;
	AppendMenu(hMenu, lpMenu->menuflag & ~MF_END,
		      lpMenu->menuid,
		      lpMenu->menuname);
	if (lpMenu->menuflag & MF_END)
	    break;
	lpMenu++;
    }
    return hMenu;
}

HMENU WINAPI
TWIN_LoadMenu(HINSTANCE hInstance, LPCSTR lpMenuName,int nStyle)
{
    	HMENU  hMenu;
	HANDLE hResInfo;
	HANDLE hResData;
	MENUDATA *lpMenu;

    	APISTR((LF_APICALL,"LoadMenu(HINSTANCE=%x,LPCSTR=%p\n",
		hInstance,lpMenuName));

#if 0
	/* if we have a menu hook, call it */
	if(lpfnMenuHook) {
		/* if hook function handles it, return HMENU */
		if((*lpfnMenuHook)(hInstance,lpMenuName,&hMenu)) {
    			APISTR((LF_APIRET,"LoadMenu: returns HMENU %d\n",
				hMenu));
			return hMenu;
		}
		/* otherwise drop down into default handler */
	}
#endif

	hResInfo = FindResource(hInstance,lpMenuName,RT_MENU);
	if(hResInfo == 0) {
    		APISTR((LF_APIFAIL,"LoadMenu: returns HMENU 0\n"));
		return 0;
	}

	hResData = LoadResource(hInstance,hResInfo);

	if(hResData == 0) {
    		APISTR((LF_APIFAIL,"LoadMenu: returns HMENU 0\n"));
		return 0;
	}

	lpMenu = (MENUDATA *) LockResource(hResData);

	hMenu = TWIN_LoadMenuIndirect(lpMenu);

	UnlockResource(hResData);
	FreeResource(hResData);

    	APISTR((LF_APIRET,"LoadMenu: returns HMENU %x\n",hMenu));
    	return hMenu;
}

static MENUDATA *
ProcessMenuTemplate(LPBYTE *lplpTemplate)
{   
    int nCount = 0;
    LPBYTE lpTemplate = *lplpTemplate;
    MENUDATA *lpMenu = (MENUDATA *)NULL;
    UINT uiFlag;
    int len;

    while (TRUE) {
        if (!nCount)
            lpMenu = (MENUDATA *)WinMalloc(sizeof(MENUDATA));
        else
            lpMenu = (MENUDATA *)WinRealloc
                        ((LPSTR)lpMenu,sizeof(MENUDATA)*(nCount+1));
        memset((LPSTR)&lpMenu[nCount],'\0',sizeof(MENUDATA));
        uiFlag = *(UINT *)lpTemplate;
        lpMenu[nCount].menuflag = uiFlag;
        lpTemplate += sizeof(UINT);
        if (!(uiFlag & MF_POPUP)) {
            lpMenu[nCount].menuid = *(UINT *)lpTemplate;
            lpTemplate += sizeof(UINT);
        }
        len = strlen((LPSTR)lpTemplate);
        if (len++) {
            lpMenu[nCount].menuname = WinMalloc(len);
            strcpy(lpMenu[nCount].menuname,(LPSTR)lpTemplate);
        }
        lpTemplate += len;
        if (uiFlag & MF_POPUP)
            lpMenu[nCount].menupopup = ProcessMenuTemplate(&lpTemplate);
        if (uiFlag & MF_END) {
            *lplpTemplate = lpTemplate;
            return lpMenu;
        }
        nCount++;
    }
}   

HMENU WINAPI
LoadMenuIndirect(const void *lpVoid)
{
    HMENU hMenu;
    UINT wVersion;
    LPBYTE lpMenuTemplate;
    LPMENUDATA lpMenu;
    MENUITEMTEMPLATEHEADER *lpmith = (MENUITEMTEMPLATEHEADER *)lpVoid;

    if ((wVersion =  lpmith->versionNumber)) {
        logstr(-1,"ERROR ***, version number %x in menu item template\n",wVersion);
        return 0;
    } 

    lpMenuTemplate  = (LPBYTE) lpmith + sizeof(MENUITEMTEMPLATEHEADER);
    lpMenuTemplate += lpmith->offset; 

    lpMenu = ProcessMenuTemplate(&lpMenuTemplate);
    hMenu = TWIN_LoadMenuIndirect(lpMenu);

    return hMenu;
}

typedef struct tagCOLUMNDATA {
    BOOL fBreak;
    int nWidth;
    int nCount;
} COLUMNDATA;

DWORD
CalcPopupMenuDimensions(HMENU hMenu, HWND hWndOwner)
{
    HMENU32 hMenu32;
    MENUITEMSTRUCT mnis;
    MEASUREITEMSTRUCT mis;
    BITMAP bm;
    int nCount, i, j, k;
    LONG lFlags;
    HDC hDC;
    HFONT hFont;
    LPSTR lpItemData;
    int nNumColumns = 1;
    int nItemWidth=0, nItemHeight=0;
    int nMenuWidth,nMenuHeight;
    int nMaxWidth = 0, nMaxHeight = 0;
    int nCurrHeight = 0;
    int *lpItemHeights;
    COLUMNDATA *lpColumnData;
    DWORD dwDimension, dwIndents;
    RECT rc;
    int X,Y;

    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return 0L;

    mnis.wAction = LCA_ITEMCOUNT;
    mnis.wPosition = (WORD)-1;
    nCount = LBoxAPI(hMenu32,LBA_GETDATA,(LPARAM)&mnis);
    mnis.wAction = LCA_FONT;
    hFont = (HFONT)LBoxAPI(hMenu32,LBA_GETDATA,(LPARAM)&mnis);
    hDC = GetDC((HWND)0);
    if (hFont > 0) 
	SelectObject(hDC,hFont);
    lpColumnData = (COLUMNDATA *)WinMalloc(sizeof(COLUMNDATA));
    lpColumnData[0].nCount = lpColumnData[0].nWidth = 0;
    lpColumnData[0].fBreak = FALSE;
    lpItemHeights = (int *)WinMalloc(sizeof(int)*nCount);
    memset((LPSTR)lpItemHeights,'\0',sizeof(int)*nCount);
    for (i=0; i<nCount; i++) {
	mnis.wPosition = (WORD)i;
	mnis.wAction = LCA_GET | LCA_FLAGS;
	mnis.wItemFlags = MF_BYPOSITION;
	lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	if (lFlags < 0)
	    continue;
	if (lFlags & MF_SEPARATOR) {
	    nItemHeight = 5;
	    nItemWidth = 0;
	}
	else {
	    mnis.wAction = LCA_GET | LCA_CONTENTS;
	    lpItemData = (LPSTR)LBoxAPI
				(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	    mnis.wAction = LCA_GET | LCA_INDENTS;
	    dwIndents = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	    if (lFlags & MF_OWNERDRAW) {
		mis.CtlType = ODT_MENU;
		mis.CtlID = 0;
		mnis.wAction = LCA_GET | LCA_ITEMID;
		mis.itemID = (UINT)((int)((short)LOWORD(LBoxAPI
				(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis))));
		mis.itemData = (DWORD)lpItemData;
		mis.itemHeight = mis.itemWidth = 0;
		SendMessage(hWndOwner,WM_MEASUREITEM,0,(LONG)&mis);
		nItemHeight = (int)((short)mis.itemHeight);
		nItemWidth = (int)((short)mis.itemWidth);
	    }
	    else {
		if (lFlags & MF_BITMAP) {
		    GetObject((HGDIOBJ)(DWORD)lpItemData,sizeof(BITMAP),&bm);
		    nItemHeight = bm.bmHeight+2;
		    nItemWidth = bm.bmWidth;
		}
		else {		/* MF_STRING */
		    SetRectEmpty(&rc);
		    dwDimension = GetTextExtent(hDC,lpItemData,strlen(lpItemData));
		    if (dwDimension) {
			nItemHeight = (int)HIWORD(dwDimension)*7/4;
			nItemWidth = (int)((short)LOWORD(dwDimension));
		    }
		}
	    }
	    nItemWidth += LOWORD(dwIndents) + HIWORD(dwIndents);
	}
	if (lFlags & (MF_MENUBREAK|MF_MENUBARBREAK)) {
	    lpColumnData[nNumColumns-1].nWidth = nMaxWidth;
	    lpColumnData = (COLUMNDATA *)WinRealloc((LPSTR)lpColumnData,
			sizeof(COLUMNDATA)*(++nNumColumns));
	    lpColumnData[nNumColumns-1].nCount = 0;
	    lpColumnData[nNumColumns-1].fBreak = 
				(lFlags & MF_MENUBARBREAK) ? TRUE : FALSE;
	    nMaxHeight = max(nCurrHeight,nMaxHeight);
	    nCurrHeight = nMaxWidth = 0;
	}
	lpItemHeights[i] = nItemHeight;
	nCurrHeight += nItemHeight;
	lpColumnData[nNumColumns-1].nCount++;
	nMaxWidth = max(nItemWidth,nMaxWidth);
    }
    ReleaseDC((HWND)0,hDC);
    lpColumnData[nNumColumns-1].nWidth = nMaxWidth;
    nMaxWidth = 0;
    nMenuHeight = max(nCurrHeight,nMaxHeight);
    mnis.wPosition = (WORD)-1;
    mnis.wAction = LCA_SET | LCA_RECT;
    mnis.lpItemData = (LPSTR)&rc;
    mnis.wItemFlags = MF_BYPOSITION;
    X = Y = 0;
    for (i=0,j=0; i<nNumColumns && j<nCount; i++) {
	if (lpColumnData[i].fBreak)
	    X++;
	for (k=0; k<lpColumnData[i].nCount; k++) {
	    mnis.wPosition = (WORD)j;
	    rc.left = X;
	    rc.top = Y;
	    rc.right =  X + lpColumnData[i].nWidth;
	    rc.bottom = Y + lpItemHeights[j];
	    LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	    Y += lpItemHeights[j];
	    j++;
	}
	X += lpColumnData[i].nWidth;
    }

    nMenuWidth = X + 2*GetSystemMetrics(SM_CXBORDER);
    nMenuHeight += 2*GetSystemMetrics(SM_CYBORDER);

    WinFree((LPSTR)lpItemHeights);
    WinFree((LPSTR)lpColumnData);

    RELEASELBOXINFO(hMenu32);

    return MAKELONG((WORD)nMenuWidth,(WORD)nMenuHeight);
}

BOOL WINAPI		/* API */
TrackPopupMenu(HMENU hMenu, UINT uiFlags, int x, int y,
		int nSystemMenu, HWND hWnd, const RECT *lpReserved)
{
    HWND hWndTrackPopup,hWndOldFocus;
    TRACKPOPUPSTRUCT tps;
    MSG msg,msgHook;
    POINT pt;
    int ret, nState;

    memset((LPVOID)&tps,'\0',sizeof(TRACKPOPUPSTRUCT));
    tps.hMenu = hMenu;
    tps.uiFlags = uiFlags;
    tps.x = x;
    tps.y = y;
    tps.bSystemMenu = (BOOL)nSystemMenu;
    tps.hWndOwner = hWnd;
    tps.lprc = (RECT *)lpReserved;

    if (!(hWndTrackPopup = CreateWindowEx(WS_EX_TRANSPARENT,
	"TRACKPOPUP",NULL,
	WS_POPUP,
	x,y,10,10,
	0, 0, GetWindowInstance(hWnd),
	(LPVOID)&tps))) {
	ClearWF(hWnd,WFTRACKPOPUPACTIVE);
	return FALSE;
    }

    hWndOldFocus = SetFocus ( hWndTrackPopup );
    nState = (uiFlags & TPM_RIGHTBUTTON) ? GetKeyState(VK_RBUTTON) :
			GetKeyState(VK_LBUTTON);
    if (nState & 0x8000)
	SendMessage(hWndTrackPopup,
		(uiFlags & TPM_RIGHTBUTTON) ? WM_RBUTTONDOWN : WM_LBUTTONDOWN,
		0, 0L);
    
    SetWindowWord(hWndTrackPopup,TP_STATUS,0);
    
    while (GetWindowWord(hWndTrackPopup,TP_STATUS) == 0) {
	if (GetMessage(&msg, 0, 0, 0)) {
	    TranslateMessage (&msg) ;

	    if ((ret = IsMouseOrKeyboardMsg(msg.message))) {
		msgHook = msg;
		if (ret == WM_MOUSEFIRST) {
		    msgHook.hwnd = hWnd;
		    pt.x = LOWORD(msg.lParam);
		    pt.y = HIWORD(msg.lParam);
		    MapWindowPoints(msg.hwnd,msgHook.hwnd,&pt,1);
		    msgHook.lParam = MAKELPARAM(pt.x,pt.y);
		}
		else 
		    msgHook.hwnd = hWndOldFocus;
		if (CallMsgFilter(&msgHook,MSGF_MENU))
		    continue;
	    }

	    DispatchMessage (&msg) ;
	}
	if (!PeekMessage(&msg,hWnd,0,0,PM_NOYIELD|PM_NOREMOVE))
	    SendMessage(tps.hWndOwner,WM_ENTERIDLE,MSGF_MENU,
			(LPARAM)tps.hWndOwner);
    }
    SendMessage(hWnd,WM_MENUSELECT,GET_WM_MENUSELECT_MPS(0,-1,0));
    ReleaseCapture();
    if (hWndOldFocus) 
	SetFocus(hWndOldFocus);
    DestroyWindow(hWndTrackPopup);
    return TRUE;
}

void  WINAPI
DrawMenuBar(HWND hWnd)
{
    HMENU hMenu;
    HMENU32 hMenu32;
    HWND hWndMenu;
    MENUITEMSTRUCT mnis;
    RECT rcWnd,rc;
    HDC hDC;
    int nCount,i;
    WORD wMenuHeight;

    if (!(hMenu = GetWindowMenu(hWnd)))
	return;
    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return;

    hWndMenu = GetWindowFrame(hWnd);
    GetClientRect(hWndMenu, &rcWnd);
    wMenuHeight = MeasureWindowMenu(hWnd,hWndMenu);
    if (wMenuHeight < (WORD)rcWnd.bottom) {
	SetWindowMenuHeight(hWnd,wMenuHeight);
	GetClientRect(hWndMenu, &rcWnd);
    }
    hDC = GetDC(hWndMenu);
    SetRect(&rc,rcWnd.left,rcWnd.top,rcWnd.right,rcWnd.bottom);
    FillRect(hDC, &rc, GetSysColorBrush(COLOR_MENU));
    mnis.wAction = LCA_ITEMCOUNT;
    mnis.wPosition = (WORD)-1;
    nCount = LBoxAPI(hMenu32,LBA_GETDATA,(LPARAM)&mnis);
    for (i=0; i<nCount; i++) 
	MenuDrawItem(hDC,hMenu32,hWnd,(WORD)i,ODA_DRAWENTIRE);

    SelectObject(hDC, GetStockObject(BLACK_PEN));
    SelectObject(hDC, GetStockObject(NULL_BRUSH));
#ifndef TWIN32
    MoveTo(hDC,rcWnd.left,rcWnd.bottom-1);
    LineTo(hDC,rcWnd.right,rcWnd.bottom-1);
#endif

    ReleaseDC(hWndMenu,hDC);
    RELEASELBOXINFO(hMenu32);
}

WORD
MeasureWindowMenu(HWND hWnd, HWND hWndMenu)
{
    HMENU hMenu;
    HMENU32 hMenu32;
    RECT rcWnd, rcNC, rcItemRect;
    int nMenuWidth,nLineWidth;
    MEASUREITEMSTRUCT mis;
    MENUITEMSTRUCT mnis;
    HBITMAP hBitmap;
    BITMAP bm;
    int nCount,i;
    HDC hDC;
    HFONT hFont;
    TEXTMETRIC tm;
    LONG lFlags;
    WORD wSpacing, wItemHeight, wItemWidth = 0;
    DWORD dwExtent;
    WORD X=0, wLine=0;

    if (!(hMenu = GetWindowMenu(hWnd)))
	return 0;
    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return 0;

    GetWindowRect(hWnd, &rcWnd);
    CalcExpectedNC(&rcNC,GetWindowLong(hWnd,GWL_STYLE),
			 GetWindowLong(hWnd,GWL_EXSTYLE));
    nMenuWidth = rcWnd.right - rcWnd.left - rcNC.left - rcNC.right;
    nLineWidth = nMenuWidth;
    hDC = GetDC(hWndMenu);
    mnis.wAction = LCA_FONT;
    mnis.wPosition = (WORD)-1;
    if ((hFont = (HFONT)LBoxAPI(hMenu32,LBA_GETDATA,(LPARAM)&mnis)))
	hFont = SelectObject(hDC, hFont);
    else
	hFont = 0;
    GetTextMetrics(hDC, &tm);
    wSpacing = LOWORD(GetTextExtent(hDC, " ", 1));
    wItemHeight = GetSystemMetrics(SM_CYMENU) - 1;

    mnis.wAction = LCA_ITEMCOUNT;
    nCount = LBoxAPI(hMenu32,LBA_GETDATA,(LPARAM)&mnis);
    for (i=0; i<nCount; i++) {
	mnis.wPosition = (WORD)i;
	mnis.wAction = LCA_GET | LCA_FLAGS;
	mnis.wItemFlags = MF_BYPOSITION;
	lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	if (lFlags < 0)
	    continue;
	if (lFlags & MF_OWNERDRAW)
	{
		mis.CtlType = ODT_MENU; /* add some stuff here */
		SendMessage(hWnd, WM_MEASUREITEM, 0, (DWORD)&mis);
	}
	else if (lFlags & MF_BITMAP)
	{
		mnis.wAction = LCA_GET | LCA_CONTENTS;
		mnis.lpItemData = (LPSTR)LBoxAPI(hMenu32,LBA_MODIFYITEM,
			(LPARAM)&mnis);
		hBitmap = (HBITMAP)(DWORD)mnis.lpItemData;
		if (hBitmap == (HBITMAP)SB_OBM_RESTORE) 
			hBitmap = SystemBitmaps[SB_OBM_RESTORE];
		if (!GetObject((HGDIOBJ)hBitmap,sizeof(BITMAP),(LPVOID)&bm))
			continue;
		wItemWidth = bm.bmWidth;
	}
	else
	{
		mnis.wAction = LCA_GET | LCA_CONTENTS;
		mnis.lpItemData = (LPSTR)LBoxAPI(hMenu32,LBA_MODIFYITEM,
			(LPARAM)&mnis);
		/* compute item string extent */
		if(mnis.lpItemData)
		{
			char itemstr[256], *pitem;
			int flag;
			/* replace leading '\b' with MF_HELP flag */
			if (*mnis.lpItemData == '\b')
			{
				mnis.lpItemData++;
				lFlags |= MF_HELP;
			}
			/* copy item string (but discard first '&') */
			for (flag = 0, pitem = itemstr;
			     (*pitem++ = *mnis.lpItemData++);
			    )
			{
				if (flag || (pitem[-1] != '&')) continue;
				pitem--;
				flag = 1;
			}
			mnis.lpItemData = itemstr;
			/* get text extent (without '\b' and '&') */
			dwExtent = GetTextExtent(hDC, itemstr,
				strlen(itemstr));
		}
		else
			dwExtent = 0;
		wItemWidth = 2*wSpacing+LOWORD(dwExtent);
	}
	if ((int)wItemWidth > nLineWidth)
	{
		wLine++;
		nLineWidth = nMenuWidth;
		X = 0;
	}
	nLineWidth -= wItemWidth;
	if (lFlags & MF_HELP)
	{
		SetRect(&rcItemRect,nMenuWidth-wItemWidth,wLine*wItemHeight,
			nMenuWidth,(wLine+1)*wItemHeight);
	}
	else
	{ 
		SetRect(&rcItemRect,X,wLine*wItemHeight,
			X+wItemWidth,(wLine+1)*wItemHeight);
		X += wItemWidth;
	}
	mnis.wAction = LCA_SET | LCA_RECT;
	mnis.lpItemData = (LPSTR)&rcItemRect;
	LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
    }

    ReleaseDC(hWndMenu,hDC);
    RELEASELBOXINFO(hMenu32);
    return ((wLine+1)*wItemHeight + 1);
}

void
MenuDrawItem(HDC hDC, HMENU32 hMenu32, HWND hWndOwner, WORD wPos, WORD wAction)
{
    HFONT hLast,hFont;
    MENUITEMSTRUCT mnis;
    LONG lFlags;
    RECT rcItemRect;
    LPSTR lpItemData;
    DRAWITEMSTRUCT dis;
    HDC hdcMemory;
    HBITMAP hBitmap,hBmpOld;
    BITMAP bm;
    BOOL bInvert;

    mnis.wPosition = (WORD)-1;
    mnis.wAction = LCA_FONT;
    if ((hFont = (HFONT)LBoxAPI(hMenu32,LBA_GETDATA,(LPARAM)&mnis)))
	hLast = SelectObject(hDC, hFont);
    else 
	hLast = 0;

    mnis.wPosition = wPos;
    mnis.wItemFlags = MF_BYPOSITION;
    mnis.wAction = LCA_GET | LCA_FLAGS;
    lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
    if (lFlags & MF_OWNERDRAW) {
	dis.CtlType = ODT_MENU;
	dis.CtlID = 0;
	mnis.wAction = LCA_GET | LCA_ITEMID;
	dis.itemID = (UINT)((int)((short)LOWORD(LBoxAPI
			(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis))));
	dis.itemAction = wAction;
	dis.itemState = (lFlags & MF_CHECKED)?ODS_CHECKED:0;
	dis.itemState |= (lFlags & MF_DISABLED)?ODS_DISABLED:0;
	dis.itemState |= (lFlags & MF_GRAYED)?ODS_GRAYED:0;
	dis.itemState |= (lFlags & MF_HILITE)?ODS_SELECTED:0;
	dis.hwndItem = ((LPOBJHEAD)hMenu32)->hObj;
	dis.hDC = hDC;
	mnis.wAction = LCA_GET | LCA_RECT;
	mnis.lpItemData = (LPSTR)&dis.rcItem;
	LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	mnis.wAction = LCA_GET | LCA_CONTENTS;
	dis.itemData = (DWORD)LBoxAPI
			(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	SendMessage(hWndOwner,WM_DRAWITEM,0,(LPARAM)&dis);
    }
    else {
	mnis.wAction = LCA_GET | LCA_RECT;
	mnis.lpItemData = (LPSTR)&rcItemRect;
	LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	mnis.wAction = LCA_GET | LCA_CONTENTS;
	lpItemData = (LPSTR)LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mnis);
	SetBkMode(hDC, TRANSPARENT);
	if (lFlags & MF_BITMAP) {
	    if ((DWORD)lpItemData == SB_OBM_RESTORE) {
		hBitmap = (lFlags & MF_HILITE)?SystemBitmaps[SB_OBM_RESTORED]:
						SystemBitmaps[SB_OBM_RESTORE];
		bInvert = FALSE;
	    }
	    else {
		hBitmap = (HBITMAP)(DWORD)lpItemData;
		bInvert = (lFlags & MF_HILITE) ? TRUE : FALSE;
	    }

	    hdcMemory = CreateCompatibleDC(hDC);
	    GetObject(hBitmap,sizeof(BITMAP),&bm);
	    hBmpOld = SelectObject(hdcMemory,hBitmap);
	    BitBlt(hDC,
			rcItemRect.left,
			rcItemRect.top,
			rcItemRect.right-rcItemRect.left,
			rcItemRect.bottom-rcItemRect.top,
			hdcMemory,0,0,
			SRCCOPY);
	    SelectObject(hdcMemory,hBmpOld);
	    DeleteDC(hdcMemory);
	    if (bInvert) 
		InvertRect(hDC,&rcItemRect);
	}
	else {
	    if (!(lFlags & MF_HILITE)) {
		FillRect(hDC,&rcItemRect,GetSysColorBrush(COLOR_MENU));
		SetTextColor(hDC,
		    (lFlags & MF_GRAYED)?
			GetSysColor(COLOR_GRAYTEXT):
			GetSysColor(COLOR_MENUTEXT));
	    }
	    else {
		FillRect(hDC,&rcItemRect,GetSysColorBrush(COLOR_HIGHLIGHT));
		SetTextColor(hDC,
		    (lFlags & MF_GRAYED)?
			GetSysColor(COLOR_GRAYTEXT):
			GetSysColor(COLOR_HIGHLIGHTTEXT));
	    }
	    DrawText(hDC,lpItemData,lpItemData?-1:0,&rcItemRect,
			DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_EXPANDTABS);
	}
    }
    if(hLast)
	SelectObject(hDC, hLast);
}

HMENU WINAPI
GetMenu(HWND hWnd)
{
    HMENU hMenu;

    hMenu = GetWindowMenu(hWnd);
    return hMenu;
}

BOOL WINAPI
SetMenu(HWND hWnd, HMENU hMenu)
{
    HMENU32 hMenu32 = 0;
    LPTRACKPOPUPSTRUCT lptps;
    HWND hWndMenu;

    if (!IsTopLevel(hWnd))
	return FALSE;

    if (hMenu && !(hMenu32 = GETMENUINFO(hMenu)))
	return FALSE;

    if (hMenu == SetWindowMenu(hWnd, hMenu)) {
	if(hMenu32)
        	RELEASELBOXINFO(hMenu32);
	return TRUE;
    }
    if (!(hWndMenu = GetWindowFrame(hWnd))) {
	if(hMenu32)
        	RELEASELBOXINFO(hMenu32);
	return FALSE;
    }

    lptps = (LPTRACKPOPUPSTRUCT)GetWindowLong(hWndMenu,LWD_LPMENUDATA);
    if (hMenu == 0) {
	if (lptps) {
	    SetWindowLong(hWndMenu, LWD_LPMENUDATA, 0L);
	    WinFree((LPSTR)lptps);
	    lptps = 0;
	}
	//no free required, hMenu == 0
	//RELEASELBOXINFO(hMenu32);
	//return TRUE;
    }
    else if (lptps == NULL) {
	lptps = (LPTRACKPOPUPSTRUCT)WinMalloc(sizeof(TRACKPOPUPSTRUCT));
	if (!lptps) {
	    RELEASELBOXINFO(hMenu32);
	    return FALSE;
	}
	memset((LPSTR)lptps,'\0',sizeof(TRACKPOPUPSTRUCT));
	SetWindowLong(hWndMenu, LWD_LPMENUDATA, (LONG)lptps);
    }
    if (lptps) {
	    lptps->hMenu = hMenu;
	    lptps->hWndOwner = hWnd;
	    lptps->uiFlags |= TP_MENUBAR;
    }

    SetWindowMenuHeight(hWnd,MeasureWindowMenu(hWnd,hWndMenu));

    TWIN_RedrawWindow(hWnd,NULL,0,RDW_FRAME|RDW_INVALIDATE);

    if(hMenu32)
    	RELEASELBOXINFO(hMenu32);

    return TRUE;
}

HMENU WINAPI
GetSystemMenu(HWND hWnd, BOOL bRevert)
{
    HMENU hSysMenu;
    DWORD dwWinStyle;

    hSysMenu = GetWindowSysMenu(hWnd);
    if (!bRevert)
	return hSysMenu;
    else {
	if (hSysMenu)
	    DestroyMenu(hSysMenu);
	dwWinStyle = GetWindowStyle(hWnd);
	hSysMenu = LoadMenu(0, (dwWinStyle & WS_CHILD)?
		"CHILDSYSMENU":"SYSMENU");
	SetWindowSysMenu(hWnd, hSysMenu);
	return (HMENU)0;
    }
}

HMENU WINAPI
GetSubMenu(HMENU hMenu, int nPos)
{
    UINT uiItemID;
    HMENU32 hMenu32;
    MENUITEMSTRUCT mis;

    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return 0;
    memset((char *)&mis, '\0', sizeof(MENUITEMSTRUCT));
    mis.wItemFlags = MF_BYPOSITION;
    mis.wPosition = (WORD)nPos;
    mis.wAction = LCA_GET | LCA_ITEMID;
    uiItemID = (UINT)LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);

    RELEASELBOXINFO(hMenu32);

    if (IsMenu((HMENU)uiItemID))
	return (HMENU)uiItemID;
    else
	return 0;
}

int WINAPI 
GetMenuItemCount(HMENU hMenu)
{
    HMENU32 hMenu32;
    MENUITEMSTRUCT mis;
    int nCount;

    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return (int)-1;

    memset((char *)&mis, '\0', sizeof(MENUITEMSTRUCT));
    mis.wAction = LCA_ITEMCOUNT;
    mis.wPosition = (WORD)-1;
    nCount = (int)LBoxAPI(hMenu32,LBA_GETDATA,(LPARAM)&mis);

    RELEASELBOXINFO(hMenu32);
    return nCount;
}

UINT WINAPI
GetMenuItemID(HMENU hMenu, int nPos)
{
    HMENU32 hMenu32;
    MENUITEMSTRUCT mis;
    LONG lFlags;
    UINT rc;

    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return (UINT)-1;

    memset((char *)&mis, '\0', sizeof(MENUITEMSTRUCT));
    mis.wItemFlags = MF_BYPOSITION;
    mis.wPosition = (WORD)nPos;
    mis.wAction = LCA_GET | LCA_FLAGS;
    lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);
    if (lFlags & MF_POPUP)
	return (UINT)-1;
    mis.wAction = LCA_GET | LCA_ITEMID;
    rc = (UINT)LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);

    RELEASELBOXINFO(hMenu32);
    return rc;
}

int WINAPI
GetMenuString(HMENU hMenu, UINT uiIDItem, LPSTR lpString,
			int nMaxCount, UINT uiFlags)
{
    HMENU32 hMenu32;
    HMENU32 hMenu32a;
    MENUITEMSTRUCT mis;
    LPSTR lpItemString;

    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return (UINT)-1;

    memset((char *)&mis, '\0', sizeof(MENUITEMSTRUCT));
    mis.wAction = LCA_GET | LCA_CONTENTS;
    mis.wItemFlags = uiFlags;
    mis.wPosition = (WORD)uiIDItem;
    lpItemString = (LPSTR)LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);
    if (((LONG)lpItemString != (LONG)-1) && HIWORD(lpItemString)) {
	strncpy(lpString,lpItemString,nMaxCount);
	RELEASELBOXINFO(hMenu32);
	return strlen(lpString);
    }

    if (lpItemString == NULL || !HIWORD(lpItemString)) {
        RELEASELBOXINFO(hMenu32);
	return 0;
    }

    if (uiFlags & MF_BYPOSITION) {
        RELEASELBOXINFO(hMenu32);
	return 0;
    }

    hMenu32a = TWIN_FindMenuItem(hMenu32,uiIDItem);
    if (!hMenu32a) {
        RELEASELBOXINFO(hMenu32);
        return 0;
    }
    hMenu = ((LPOBJHEAD)hMenu32a)->hObj;
    if (hMenu32a != hMenu32)
      RELEASELBOXINFO(hMenu32a);

    RELEASELBOXINFO(hMenu32);
    return GetMenuString(hMenu,uiIDItem,lpString,nMaxCount,uiFlags);
}

LPSTR
GetMenuItemData(HMENU hMenu, UINT uiItem, UINT uiFlags)
{
    HMENU32 hMenu32;
    HMENU32 hMenu32a;
    MENUITEMSTRUCT mis;
    LPSTR lpItemData;

    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return (LPSTR)-1;

    memset((char *)&mis, '\0', sizeof(MENUITEMSTRUCT));
    mis.wAction = LCA_GET | LCA_CONTENTS;
    mis.wItemFlags = uiFlags;
    mis.wPosition = (WORD)uiItem;
    lpItemData = (LPSTR)LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);
    if (lpItemData == (LPSTR)-1) {
	if (uiFlags & MF_BYPOSITION) {
	    RELEASELBOXINFO(hMenu32);
	    return lpItemData;
	}
	hMenu32a = TWIN_FindMenuItem(hMenu32,uiItem);
	if (!hMenu32a) {
	    RELEASELBOXINFO(hMenu32);
	    return lpItemData;
	}
	hMenu = ((LPOBJHEAD)hMenu32a)->hObj;
	if (hMenu32a != hMenu32) {
	    RELEASELBOXINFO(hMenu32a);
	    RELEASELBOXINFO(hMenu32);
	    return GetMenuItemData(hMenu,uiItem,uiFlags);
	}
    }
    RELEASELBOXINFO(hMenu32);
    return lpItemData;
}

UINT WINAPI
GetMenuState(HMENU hMenu, UINT uiID, UINT uiFlags)
{
    HMENU32 hMenu32;
    MENUITEMSTRUCT mis;
    LONG lFlags;

    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return (UINT)-1;

    memset((char *)&mis, '\0', sizeof(MENUITEMSTRUCT));
    mis.wAction = LCA_GET | LCA_FLAGS;
    mis.wPosition = (WORD)uiID;
    mis.wItemFlags = (WORD)uiFlags;

    if ((lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis)) < 0) {

        HMENU32 hMenu32orig = hMenu32;
	if (uiFlags & MF_BYPOSITION) {
	    RELEASELBOXINFO(hMenu32);
	    return (UINT)-1;
	}

	hMenu32 = TWIN_FindMenuItem(hMenu32,uiID);

	if (!hMenu32) {
	    RELEASELBOXINFO(hMenu32orig);
	    return (UINT)-1;
	}
	hMenu = ((LPOBJHEAD)hMenu32)->hObj;
	if (hMenu32 != hMenu32orig)
	  RELEASELBOXINFO(hMenu32);

	RELEASELBOXINFO(hMenu32orig);
	return GetMenuState(hMenu,uiID,uiFlags);
    }
    RELEASELBOXINFO(hMenu32);
    return (UINT)(lFlags & ~MF_BYPOSITION);
}

BOOL WINAPI
SetMenuItemBitmaps(HMENU hMenu, UINT uiItem, UINT uiFlags,
			HBITMAP hBitmapUnchecked, HBITMAP hBitmapChecked)
{
    HMENU32 hMenu32;
    HMENU32 hMenu32orig;
    MENUITEMSTRUCT mis;
    LONG lFlags;

    if (!(hMenu32 = GETMENUINFO(hMenu)))
	return FALSE;

    hMenu32orig = hMenu32;
    memset((char *)&mis, '\0', sizeof(MENUITEMSTRUCT));
    mis.wPosition = (WORD)uiItem;
    mis.wAction = LCA_GET | LCA_FLAGS;
    mis.wItemFlags = uiFlags;
    lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);
    if (lFlags < 0) { 
	if (uiFlags & MF_BYPOSITION)  {
	    RELEASELBOXINFO(hMenu32);
	    return FALSE;
	}
	hMenu32 = TWIN_FindMenuItem(hMenu32,uiItem);
	if (!hMenu32) {
	    RELEASELBOXINFO(hMenu32orig);
	    return FALSE;
	}
	mis.wAction = LCA_GET|LCA_FLAGS;
	lFlags = LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis);
	if (lFlags < 0) {
	    if (hMenu32 != hMenu32orig)
	      RELEASELBOXINFO(hMenu32);
	    RELEASELBOXINFO(hMenu32orig);
	    return FALSE;
	}
	mis.wItemFlags = MF_BYPOSITION;
    }
    mis.hCheckedBmp = hBitmapChecked;
    mis.hUncheckedBmp = hBitmapUnchecked;
    mis.wAction = LCA_SET | LCA_BITMAPS;
    if (LBoxAPI(hMenu32,LBA_MODIFYITEM,(LPARAM)&mis) < 0) {
        if (hMenu32 != hMenu32orig)
	  RELEASELBOXINFO(hMenu32);
        RELEASELBOXINFO(hMenu32orig);
	return FALSE;
    }
    if (hMenu32 != hMenu32orig)
      RELEASELBOXINFO(hMenu32);
    RELEASELBOXINFO(hMenu32orig);
    return TRUE;
}

DWORD WINAPI
GetMenuCheckMarkDimensions()
{
    static DWORD dwCheckmarkDim = 0;
    HDC hDC;
    int nWidth, nHeight;

    APISTR((LF_APICALL, "GetMenuCheckMarkDimensions()\n"));

    if (dwCheckmarkDim == 0) {
	hDC = GetDC(0);
	dwCheckmarkDim = GetTextExtent(hDC,"12345",5);
	ReleaseDC(0,hDC);

	nWidth = HIWORD(dwCheckmarkDim);
	nHeight = nWidth*7/4 - 2;
	dwCheckmarkDim = MAKELONG(nWidth,nHeight);
    }

    APISTR((LF_APIRET, "GetMenuCheckMarkDimensions: returns DWORD %x\n",
			dwCheckmarkDim));
    return dwCheckmarkDim;
}

void
ModifyMenuBar(HWND hWnd,HWND hWndChild,UINT uiAction)
{
	HMENU hMenu,hSystemMenu;
	int nMenuCount;

	hMenu = GetMenu(hWnd);
	if (uiAction == SW_MAXIMIZE) {

/* Insert SYSMENU bitmap as first menu item */

	    if (GetMenuItemData(hMenu,0,MF_BYPOSITION) != 
			(LPSTR)MAKELONG((UINT)SystemBitmaps[SB_OBM_SYSMENU],0)) {
		hSystemMenu = ModifySystemMenu(hWndChild,SW_MAXIMIZE);
		InsertMenu(hMenu,
			0,
			MF_BITMAP|MF_POPUP|MF_BYPOSITION,
			(UINT)hSystemMenu,
			(LPSTR)SB_OBM_SYSMENU);
	    }

/* Insert RESTORE bitmap as last right-aligned menu item */

	    nMenuCount = GetMenuItemCount(hMenu);
	    if (GetMenuItemID(hMenu,nMenuCount-1) != SC_RESTORE) 
		AppendMenu(hMenu,
			MF_BITMAP|MF_HELP,
			SC_RESTORE,
			(LPSTR)SB_OBM_RESTORE);
	}
	else if (uiAction == SW_RESTORE) {
	    hSystemMenu = ModifySystemMenu(hWndChild,SW_RESTORE);
	    RemoveMenu(hMenu,(UINT)hSystemMenu,MF_BYCOMMAND);
	    RemoveMenu(hMenu,SC_RESTORE,MF_BYCOMMAND);
	}
}

HMENU
ModifySystemMenu(HWND hWnd, UINT uiAction)
{
    HMENU hSystemMenu;

    hSystemMenu = GetSystemMenu(hWnd,FALSE);
    if (hSystemMenu == 0)
	return (HMENU)0;


    if ((uiAction == SW_RESTORE) && (GetWindowStyle(hWnd) & WS_MAXIMIZE))
      uiAction = SW_MAXIMIZE;

    switch (uiAction) {

	case SW_MINIMIZE:
	    EnableMenuItem(hSystemMenu,SC_SIZE,MF_DISABLED);
	    EnableMenuItem(hSystemMenu,SC_MINIMIZE,MF_DISABLED);
	    break;

	case SW_MAXIMIZE:
	    EnableMenuItem(hSystemMenu,SC_MOVE,MF_DISABLED);
	    EnableMenuItem(hSystemMenu,SC_SIZE,MF_DISABLED);
	    EnableMenuItem(hSystemMenu,SC_MINIMIZE,MF_ENABLED);
	    EnableMenuItem(hSystemMenu,SC_MAXIMIZE,MF_DISABLED);
	    break;

	case SW_RESTORE:
	    EnableMenuItem(hSystemMenu,SC_MOVE,MF_ENABLED);
	    EnableMenuItem(hSystemMenu,SC_SIZE,MF_ENABLED);
	    EnableMenuItem(hSystemMenu,SC_MINIMIZE,MF_ENABLED);
	    EnableMenuItem(hSystemMenu,SC_MAXIMIZE,MF_ENABLED);
	    break;
    }
    return hSystemMenu;
}

static HBITMAP
LoadSysMenuBitmap(void)
{
	HBITMAP hSysMenuBitmap,hBitmap,hOldBitmap;
	HDC hDCDest,hDCSrc;
	BITMAP bm;

	if (SystemBitmaps[SB_OBM_SYSMENU])
	    return SystemBitmaps[SB_OBM_SYSMENU];

	hDCDest = CreateCompatibleDC(0);
	hDCSrc = CreateCompatibleDC(0);

	hBitmap = LoadBitmap(0,(LPSTR)OBM_CLOSE);
	GetObject(hBitmap,sizeof(BITMAP),(LPVOID)&bm);
	hOldBitmap = SelectObject(hDCSrc,hBitmap);
	hSysMenuBitmap = CreateCompatibleBitmap(hDCSrc,
				bm.bmWidth/2,bm.bmHeight);
	SelectObject(hDCDest,hSysMenuBitmap);
	BitBlt(hDCDest,0,0,bm.bmWidth/2,bm.bmHeight,
			hDCSrc,bm.bmWidth/2,0,
			SRCCOPY);

	/* The bitmaps will be unselected by GDI */
	DeleteDC(hDCDest);
	DeleteDC(hDCSrc);
	DeleteObject(hBitmap);

	SystemBitmaps[SB_OBM_SYSMENU] = hSysMenuBitmap;
	return hSysMenuBitmap;
}

extern TWINRT DefaultTwinRT;

HMODULE GetModuleFromInstance(HINSTANCE);

HMENU WINAPI		/* API */
LoadMenu(HINSTANCE hInstance, LPCSTR lpMenuName)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HMENU		hMenu;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hMenu =  modinfo->lptwinrt->lpfmenu(hInstance,lpMenuName,0);
	} else
		hMenu =  DefaultTwinRT.lpfmenu(hInstance,lpMenuName,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);
	
	return hMenu;
}

HMENU
LoadMenuA(HINSTANCE hInstance, LPCTSTR lpMenuName)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HMENU		hMenu;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hMenu = modinfo->lptwinrt->lpfmenu(hInstance,lpMenuName,0);
	} else
		hMenu = DefaultTwinRT.lpfmenu(hInstance,lpMenuName,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);
	
	return hMenu;
}
HMENU
LoadMenuW(HINSTANCE hInstance, LPCTSTR lpMenuName)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HMENU		hMenu;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hMenu = modinfo->lptwinrt->lpfmenu(hInstance,lpMenuName,1);
	} else
		hMenu = DefaultTwinRT.lpfmenu(hInstance,lpMenuName,1);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return hMenu;
}

BOOL SetMenuContextHelpId(
    HMENU hmenu, 	
    DWORD dwContextHelpId 	
   )
{
	APISTR((LF_APISTUB,"SetMenuContextHelpId(%x,%x)\n",
		hmenu,dwContextHelpId));
	return FALSE;
}

DWORD GetMenuContextHelpId(
    HMENU hmenu )
{
	APISTR((LF_APISTUB,"GetMenuContextHelpId(%x)\n", hmenu));
	return FALSE;
}

BOOL CheckMenuRadioItem(
    HMENU hmenu, 	
    UINT idFirst, 	
    UINT idLast, 	
    UINT idCheck, 	
    UINT uFlags	
   )
{
	APISTR((LF_APISTUB,"CheckMenuRadioItem(%x,%x,%x,%x,%x)\n", 
		hmenu,idFirst,idLast,idCheck,uFlags));
	return FALSE;
}
   
