
/*    
	Module.c	2.15
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

#include <stdio.h>

#include "windows.h"
#include "Resources.h"
#include "Module.h"
#include "DPMI.h"
#include "Log.h"
#include "thread.h"

static LPOBJHEAD lpModuleHead = (LPOBJHEAD)NULL;
static MODULEINFO *lpModuleTable[256];

static BYTE bModIndex;

MODULEINFO *GetModuleFromIndex(int module)
{
	MODULEINFO *modinfo;
	
	if(module == 0)
		modinfo = (MODULEINFO *)lpModuleHead;
	else
		modinfo = lpModuleTable[module];

	return modinfo;
}

int
NewModuleEntry(MODULEINFO *modinfo)
{
    lpModuleTable[++bModIndex] = modinfo;
    return bModIndex;
}

void
FreeModuleEntry(MODULEINFO *modinfo)
{
    MODULEINFO *lpModuleTemp;

    if (bModIndex == modinfo->bModIndex) 
	bModIndex--;

    lpModuleTable[modinfo->bModIndex] = (LPMODULEINFO)0;

    if (modinfo == (LPMODULEINFO)lpModuleHead)
	lpModuleHead = (LPOBJHEAD)modinfo->ObjHead.lpObjNext;
    else {
	for (lpModuleTemp = (LPMODULEINFO)lpModuleHead;
	     lpModuleTemp &&
		(LPMODULEINFO)lpModuleTemp->ObjHead.lpObjNext != modinfo;
	     lpModuleTemp = (LPMODULEINFO)lpModuleTemp->ObjHead.lpObjNext);

	if (lpModuleTemp)
	    lpModuleTemp->ObjHead.lpObjNext = modinfo->ObjHead.lpObjNext;
    }
}


/* Supplemental functions */

HMODULE
CreateModuleInstance(LPSTR lpszModuleName)
{
    MODULEINFO *modinfo;
    HMODULE hModule;
    int bModIndex;

    if (!(modinfo = CREATEHMODULE(hModule))) {
	return (HMODULE)0;
    }

    modinfo->lpModuleName = WinStrdup(lpszModuleName);

    modinfo->atmModuleName = AddAtom(lpszModuleName);

    bModIndex = NewModuleEntry(modinfo);
	
    modinfo->bModIndex = bModIndex;

    modinfo->ObjHead.lpObjNext = (LPOBJHEAD)lpModuleHead;
    lpModuleHead = (LPOBJHEAD)modinfo;

    RELEASEMODULEINFO(modinfo);
    return hModule;
}

void
FreeModuleInstance(HMODULE hModule)
{
    MODULEINFO *modinfo;

    if (!(modinfo = GETMODULEINFO(hModule))) {
	return;
    }

    DeleteAtom(modinfo->atmModuleName);

    modinfo->atmModuleName = 0;
    WinFree(modinfo->lpModuleName);

    FreeModuleEntry(modinfo);

    RELEASEMODULEINFO(modinfo);
    FREEHMODULE(hModule);
}

int WINAPI
GetModuleUsage(HINSTANCE hInstance)
{
    MODULEINFO *modinfo;
    HMODULE hModule;
    int refcnt;

    
    hModule = GetModuleFromInstance(hInstance);
    if(hModule == 0)
	return 0;

    if (!(modinfo = GETMODULEINFO(hModule))) {
	return 0;
    }

    refcnt = modinfo->nRefCnt;

    RELEASEMODULEINFO(modinfo);

    if(refcnt == 0) {
	refcnt = 1;
	
    }	
    return refcnt;
}

MODULEINFO *
GetModuleFromSelector(WORD uSel)
{
    MODULEINFO *modinfo;

    if (!(modinfo = lpModuleTable[GetModuleIndex(uSel)]))
	return 0;

    return modinfo;
}

#ifdef TWIN_BIN32
char * GetModuleAlias(LPCTSTR,char *,int);
HMODULE WINAPI
GetModuleHandleA(LPCTSTR lpModuleName)
{
	HINSTANCE	hInst;
	HMODULE		hModule;
	void	       *PEB;
	char 	       *modname;
	char		FileName[256];

	APISTR((LF_APICALL,"GetModuleHandleA(%s)\n",
		lpModuleName?lpModuleName:"NULL"));

	modname = GetModuleAlias(lpModuleName,FileName,256);

	hModule = GetModuleHandle(modname);
	hInst = GetInstanceFromModule(hModule);
	
	if(lpModuleName == 0) {
		MODULEINFO     *modinfo;
		modinfo = GETMODULEINFO(hModule);
		PEB = 	(void *) modinfo->lpNTBaseAddress;
    		RELEASEMODULEINFO(modinfo);

		APISTR((LF_APIRET,"GetModuleHandleA returns BASEADDRESS %p\n",
			PEB));
		
		return PEB;
	}

	APISTR((LF_APIRET,"GetModuleHandleA returns HMODULE %x\n",hInst));

	return hInst;
}
#endif
HMODULE WINAPI
GetModuleHandle(LPCSTR lpszModule)
{
    MODULEINFO *mi;
    ATOM atom;
    LPOBJHEAD lpModuleTemp;
    LPSTR lp;
    LPSTR lpModName;
    LPSTR lpStr;

    if (!HIWORD(lpszModule)) {
	return GetModuleFromInstance((HINSTANCE)(UINT)lpszModule);
    }

    /* get a copy of the string, we are going to modify it */
    lpStr = WinMalloc(strlen(lpszModule)+1);
    strcpy(lpStr,lpszModule);

    /* extract the filename portion */
    if ((lpModName = strrchr(lpStr,'/'))) {
	lpModName++;
    } else
	lpModName = lpStr;

    /* drop off any extensions */
    if ((lp = strchr(lpModName,'.')))	/* strip off possible extension */
	*lp = '\0';

    if (!(atom = FindAtom(lpModName))) {
	WinFree(lpStr);
	return 0;
    }

    /* lpModName points to our internal copy */

    for (lpModuleTemp = lpModuleHead; lpModuleTemp;
	lpModuleTemp = (LPOBJHEAD)lpModuleTemp->lpObjNext) {
	mi = (MODULEINFO *)lpModuleTemp;

	if (mi->atmModuleName == atom) {
	    WinFree(lpStr);
	    return lpModuleTemp->hObj;
        }
    }

    /* can nolonger use the string */
    WinFree(lpStr);
    return 0;
}

void DEBUG_DumpModule(int module)
{
	logstr(LF_CONSOLE,"DEBUG_DumpModules %x\n",module);
}

void DEBUG_WalkModules()
{
    	MODULEINFO *lpModuleTemp;
	logstr(LF_CONSOLE,"DEBUG_WalkModules\n");
	logstr(LF_CONSOLE,        "address instance name  filename\n");

	for (lpModuleTemp = (LPMODULEINFO)lpModuleHead;
	     lpModuleTemp ;
	     lpModuleTemp = (LPMODULEINFO)lpModuleTemp->ObjHead.lpObjNext) {

		logstr(LF_CONSOLE,"%8.8p %4.4x   %8.8s  %s\n",
			lpModuleTemp, lpModuleTemp->hInst, 
			lpModuleTemp->lpModuleName ,lpModuleTemp->lpFileName);
	}
}

