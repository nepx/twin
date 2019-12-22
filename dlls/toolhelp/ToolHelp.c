/*    
	ToolHelp.c	2.10
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


#include "windows.h"

#include "toolhelp.h"

#include "kerndef.h"
#include "Kernel.h"
#include "Resources.h"
#include "Module.h"
#include "Log.h"

#include <stdio.h>
#include <string.h>

extern MODULEINFO *GetModuleFromIndex(int);

static BOOL
FillModuleEntry(MODULEENTRY *lpme,MODULEINFO *modinfo)
{
    OBJHEAD *lpObjNext;

    if (!modinfo || !lpme)
	return FALSE;
    lpme->dwSize = sizeof(MODULEENTRY);
    if (modinfo->lpModuleName)
	strcpy(&lpme->szModule[0],modinfo->lpModuleName);
    lpme->hModule = modinfo->ObjHead.hObj;
    lpme->wcUsage = 1;	/* TODO LATER -- support reference count */
    if (modinfo->lpFileName)
	strcpy(&lpme->szExePath[0],modinfo->lpFileName);
    if ((lpObjNext = (LPOBJHEAD)modinfo->ObjHead.lpObjNext))
	lpme->wNext = (WORD)(UINT)lpObjNext->hObj;
    else
	lpme->wNext = 0;
    return TRUE;
}

BOOL WINAPI
ModuleFirst(MODULEENTRY *lpme)
{
    BOOL retcode;

    retcode = FillModuleEntry(lpme,GetModuleFromIndex(0));
    LOGSTR((LF_LOG,"ModuleFirst: struct size %x, we need %x\n", 
	lpme->dwSize, (unsigned int) sizeof(MODULEENTRY)));

    return retcode;
}

BOOL WINAPI
ModuleNext(MODULEENTRY *lpme)
{
    BOOL retcode = FALSE;

    if (lpme && lpme->wNext) {
        LPMODULEINFO modinfo = GETMODULEINFO((lpme->wNext));
	retcode = FillModuleEntry(lpme,modinfo);
	RELEASEMODULEINFO(modinfo);
    }
    return retcode;
}

BOOL WINAPI
ModuleFindHandle(MODULEENTRY *lpme,HMODULE hModule)
{
  return(FALSE);
}

BOOL WINAPI
ModuleFindName(MODULEENTRY *lpme,LPCSTR lpszName)
{
  BOOL retcode;
  lpme->wNext = (WORD)(UINT)GetModuleHandle(lpszName);
  retcode = ModuleNext(lpme);
  return retcode;
}

BOOL WINAPI
MemManInfo(MEMMANINFO *lpmmi)
{
    DWORD dwSize;

    LOGSTR((LF_APISTUB,"MemManInfo: \n"));

    if (lpmmi) {
	dwSize = 2*sizeof(DWORD);

	if (dwSize >= lpmmi->dwSize)
	    return FALSE;
	dwSize += sizeof(DWORD);
	lpmmi->dwLargestFreeBlock = 0;

	if (dwSize >= lpmmi->dwSize)
	    return TRUE;
	dwSize += sizeof(DWORD);
	lpmmi->dwMaxPagesAvailable = 0x1000;

	if (dwSize >= lpmmi->dwSize)
	    return TRUE;
	dwSize += sizeof(DWORD);
	lpmmi->dwMaxPagesLockable = 0;

	if (dwSize >= lpmmi->dwSize)
	    return TRUE;
	dwSize += sizeof(DWORD);
	lpmmi->dwTotalLinearSpace = 0;

	if (dwSize >= lpmmi->dwSize)
	    return TRUE;
	dwSize += sizeof(DWORD);
	lpmmi->dwTotalUnlockedPages = 0;

	if (dwSize >= lpmmi->dwSize)
	    return TRUE;
	dwSize += sizeof(DWORD);
	lpmmi->dwFreePages = 0;

	if (dwSize >= lpmmi->dwSize)
	    return TRUE;
	dwSize += sizeof(DWORD);
	lpmmi->dwTotalPages = 0;

	if (dwSize >= lpmmi->dwSize)
	    return TRUE;
	dwSize += sizeof(DWORD);
	lpmmi->dwFreeLinearSpace = 0;

	if (dwSize >= lpmmi->dwSize)
	    return TRUE;
	dwSize += sizeof(WORD);
	lpmmi->dwSwapFilePages = 0;

	if (dwSize >= lpmmi->dwSize)
	    return TRUE;
	lpmmi->wPageSize = 0x1000;
	return TRUE;
    }
    return FALSE;
}

BOOL WINAPI
GlobalEntryHandle(GLOBALENTRY *lpge, HGLOBAL hGlobal)
{
    if (!lpge)
	return FALSE;

    LOGSTR((LF_APISTUB,"GlobalEntryHandle: hGlobal %x\n",hGlobal));

    if (lpge->dwSize != sizeof(GLOBALENTRY))
	return FALSE;

    memset((LPSTR)lpge,'\0', sizeof(GLOBALENTRY));
    lpge->dwSize = sizeof(GLOBALENTRY);

#ifdef	LATER
    /* return all correct values */
#endif
    return TRUE;
}

BOOL    WINAPI 
GlobalEntryModule(GLOBALENTRY * lpGlobal, HMODULE hModule, WORD wSeg)
{
    LOGSTR((LF_APISTUB,"GlobalEntryModule(%x,%x,%x)\n",
	lpGlobal,hModule,wSeg));
    return FALSE;
}

BOOL    WINAPI 
GlobalFirst(GLOBALENTRY * lpGlobal, WORD wFlags)
{
    LOGSTR((LF_APISTUB,"GlobalFirst(%x,%x)\n",
	lpGlobal,wFlags));
    return FALSE;
}

WORD    WINAPI 
GlobalHandleToSel(HGLOBAL hMem)
{
    LOGSTR((LF_APISTUB,"GlobalHandleToSel(%x)\n",hMem));
    return FALSE;
}

BOOL    WINAPI 
GlobalInfo(GLOBALINFO * lpGlobalInfo)
{
    LOGSTR((LF_APISTUB,"GlobalInfo(%x)\n",lpGlobalInfo));
    return FALSE;
}

BOOL    WINAPI 
GlobalNext(GLOBALENTRY * lpGlobal, WORD wFlags)
{
    LOGSTR((LF_APISTUB,"GlobalNext(%x,%x)\n",
	lpGlobal,wFlags));
    return FALSE;
}

BOOL WINAPI
TimerCount(TIMERINFO *lpti)
{
    if (!lpti)
	return FALSE;

    LOGSTR((LF_APISTUB,"TimerCount: \n"));

    return FALSE;
}
