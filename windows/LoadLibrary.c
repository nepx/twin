/*    
	LoadLibrary.c	2.51
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
#include <string.h>
#include <ctype.h>

#include "windows.h"
#include "windowsx.h"
#include "Endian.h"
#include "dos.h"
#include "Log.h"
#include "kerndef.h"
#include "Resources.h"
#include "Module.h"
#include "Kernel.h"
#include "BinTypes.h"
#include "WinConfig.h"
#include "WinDefs.h"
#include "compat.h"
#include "Driver.h"

extern TWINRT DefaultTwinRT;
extern HMODULE CreateModuleInstance(LPSTR);
extern void FreeModuleInstance(HMODULE);
extern LPSTR strpbrkr(LPCSTR,LPCSTR);
extern BOOL InitBinary(LPSTR,UINT);

/* From LoadDLL.c */
extern int LoadNewInstance(MODULEINFO *, LPSTR, WORD);
extern void LoadModuleFromDscr(MODULEINFO *,LPMODULETAB,WORD);
extern int  LoadModuleFromFile(MODULEINFO *,LPSTR,LPSTR,WORD);

extern BOOL TWIN_InitDriver();

HANDLE TWIN_LoadFile(WORD , LPSTR , LPARAM );
HANDLE TWIN_LoadMDT(WORD , LPSTR , LPARAM );

#ifdef STRICT
typedef FARPROC (CALLBACK* GETPADDRPROC)(LPMODULEINFO, UINT);
#else
typedef FARPROC GETPADDRPROC;
#endif

GETPADDRPROC lpfnOEMGetPAddr = NULL;

WORD return_thunk_selector;

#ifdef TWIN32
DWORD          GetModuleFileName(HMODULE,LPTSTR,DWORD);
#else
int          GetModuleFileName(HINSTANCE,LPSTR,int);
#endif
FARPROC      GetProcAddress(HINSTANCE,LPCSTR);
HINSTANCE    InternalLoadDLL(MODULETAB *);

extern MODULETAB TWIN_ModuleTable[];

#ifdef TWIN_BIN32
HINSTANCE WINAPI
LoadLibraryA(LPCSTR lpszLibFileName)
{
	return LoadLibrary(lpszLibFileName);
}
#endif

char *
GetModuleAlias(LPCSTR lpszLibFileName,char *FileName,int cblen)
{
    char *libname = (char *)lpszLibFileName;
    
    /* special case */
    if(lpszLibFileName == 0)
	return 0;

    if(stricmp(libname,"KERNEL32.dll") == 0)
	libname = "kernel";
    else if(stricmp(libname,"USER32.dll") == 0)
	libname = "user";
    else if(stricmp(libname,"GDI32.dll") == 0)
	libname = "gdi";
    else if(stricmp(libname,"SHELL32.dll") == 0)
	libname = "shell";
    else if(stricmp(libname,"comdlg32.dll") == 0)
	libname = "commdlg";

    /* these are new names, use alias */
    else if(stricmp(libname,"msvcrt.dll") == 0)
	libname = "msvcrt";
    else if(stricmp(libname,"winmm.dll") == 0)
	libname = "mmsystem";
    else if(stricmp(libname,"version.dll") == 0)
	libname = "version";
    else if(stricmp(libname,"wsock32.dll") == 0)
	libname = "winsock";
    else 

        if ((GetPrivateProfileString("alias32",libname,0,FileName,
		cblen, GetTwinFilename()))) {
    		libname = FileName;
	}

    if(strcmp(libname,lpszLibFileName)) {
    	logstr(LF_LOG,"LoadLibrary: aliased %s to %s\n",
		lpszLibFileName,libname);
    }
    return libname;
}

HINSTANCE WINAPI
LoadLibraryEx(LPCTSTR lpszLibFileName, HANDLE zHandle, DWORD flags)
{
    HANDLE hModule = 0;
    HINSTANCE hInst;
    char *libname = (char *)lpszLibFileName;
    char FileName[256];

    APISTR((LF_APICALL,"LoadLibraryEx(%s,%d,%d)\n",
	lpszLibFileName?lpszLibFileName:"NULL",zHandle,flags));

    if (!strlen(lpszLibFileName)) {
	APISTR((LF_APIFAIL,"LoadLibraryEx: returns HINSTANCE %x\n",0)); 
	return (HINSTANCE)0;
    }

    libname = GetModuleAlias(lpszLibFileName,FileName,256);

    /* try to find the .dll that corresponds with the library */
    hModule = TWIN_LoadFile(ILL_NOERROR|ILL_NEWINST,"",
			      (LPARAM)libname);

    /* what is the data instance we will use for it? */
    hInst = GetInstanceFromModule(hModule);

    APISTR((LF_APIRET,"LoadLibraryEx: returns HINSTANCE %x\n",hInst)); 
    return hInst;
}

HINSTANCE WINAPI
LoadLibrary(LPCSTR lpszLibFileName)
{
	return LoadLibraryEx(lpszLibFileName,0,0);
}

HINSTANCE
InternalLoadDLL(MODULETAB *lpModTable)
{
	int n;
	static int bLoadImmediate;

	TWIN_InitDriver();

	if(lpModTable == 0) {
		bLoadImmediate++;
		return 0;
	}
	if(bLoadImmediate) {
        	HMODULE hModule;
		HINSTANCE hInstance;

    		hModule = TWIN_LoadMDT(ILL_NEWINST|ILL_DSCR,"",(LPARAM)lpModTable);
		hInstance = GetInstanceFromModule(hModule);

		return hInstance;
	}

	for(n=1;n<256;n++) {

		if(TWIN_ModuleTable[n].flags == 0) {

			TWIN_ModuleTable[n].flags = lpModTable->flags;
			TWIN_ModuleTable[n].dscr = lpModTable->dscr;
			break;
		}
	}

	return 0;
}

void WINAPI
FreeLibrary(HINSTANCE hInst)
{
    HMODULE hModule = hInst;
    MODULEINFO *modinfo;

    APISTR((LF_APICALL,"FreeLibrary(HINSTANCE=%x)\n",hInst));

    if (!(modinfo = GETMODULEINFO(hInst))) {
	hModule = GetModuleFromInstance(hInst);
	if (!(modinfo = GETMODULEINFO(hModule))) {
	    APISTR((LF_APIFAIL,"FreeLibrary: (not loaded) returns void\n"));
	    return;
	}
    }

    if(modinfo->nRefCnt) {
	--modinfo->nRefCnt;

	if(modinfo->nRefCnt == 0) {
		if(modinfo->hSharedObject) {
		   DRVCALL_SYSTEM(PSSH_FREELIBRARY,0,0,modinfo->hSharedObject);
		}
		RELEASEMODULEINFO(modinfo);

		hModule = GetModuleFromInstance(hInst);
		FreeModuleInstance(hModule);
		APISTR((LF_APIRET,"FreeLibrary: (unloaded) returns void\n"));
		return;
	}
    }

    RELEASEMODULEINFO(modinfo);

    APISTR((LF_APIRET,"FreeLibrary: (still loaded) returns void\n"));
}

HANDLE
TWIN_LoadFile(WORD flags, LPSTR lpCmdLine, LPARAM lParam)
{
        LPMODULETAB mod_tab = 0,mdt;
        LPMODULEDSCR mod_dscr = 0;
        char szModuleName[80];
        char FileName[_MAX_PATH];
        LPSTR lpszFileName = 0, lpszModuleName;
        HMODULE hModule;
        LPMODULEINFO modinfo = 0;
        DWORD hSO;
	DWORD ret;

	/* we have a filename... */
	lpszFileName = (LPSTR)lParam;

	/* get just the component part... */
	_splitpath(lpszFileName,NULL,NULL,szModuleName,NULL);

        /* special case, this is a builtin module... */
        /* internally, it is called KERNEL, but this is the same... */
    
        if (!strcasecmp(szModuleName,"KRNL386")) {	/* KERNEL on 386 CPU */
	   return GetModuleHandle("KERNEL");
        }
        if (!strcasecmp(szModuleName,"KERNEL32")) {	/* KERNEL on 386 CPU */
	   return GetModuleHandle("KERNEL");
        }

	/********************************************************/
	/*  Step 1. Check if already loaded.			*/
	/********************************************************/

	hModule = GetModuleHandle(szModuleName);

	if(hModule) {
		modinfo = GETMODULEINFO(hModule);
		modinfo->nRefCnt++;
		
	        RELEASEMODULEINFO(modinfo);
		return hModule;
	}

	/********************************************************/
	/*  Step 2. Alias substitution				*/
	/********************************************************/

	if ((GetPrivateProfileString("boot",lpszFileName,"",FileName,
		sizeof(FileName), GetTwinFilename())))
	    lpszFileName = FileName;

	/********************************************************/
	/*  Step 3. Try to load a native version of the file.  	*/ 
	/********************************************************/

	if ((hSO = DRVCALL_SYSTEM(PSSH_LOADLIBRARY,0,0,lpszFileName))) {
                
		/* we loaded a native library, so return its handle */
		hModule = GetModuleHandle(szModuleName);
		if(hModule == 0) {

			hModule = CreateModuleInstance(szModuleName);
		}

		modinfo = GETMODULEINFO(hModule);
		modinfo->lpFileName = lpCmdLine;
		modinfo->nRefCnt = 1;
		modinfo->hSharedObject = (void *) hSO;

	        RELEASEMODULEINFO(modinfo);
		return hModule;
	} 

	/********************************************************/
    	/* Step 4. Module alias check.				*/
    	/*     e.g COMMDLG.DRV = COMMGLG.DRV			*/
	/*							*/
	/* The purpose of this code is to allow real binary     */
	/* dll's to replace our remaining internal dll's. 	*/
	/* 							*/
	/* Use the above alias to use binary vs. native dll's   */
	/* Use this to use binary vs. built-in dll's		*/
	/********************************************************/

	if (GetPrivateProfileString("boot",szModuleName,"",FileName,
			sizeof(FileName), GetTwinFilename())) 
	    lpszFileName = FileName;

	else { /* Try to find a descriptor in mdt */
	    int j = 0;

	    mdt = &TWIN_ModuleTable[0];
	    while(mdt[++j].dscr)
		if (!strcasecmp(mdt[j].dscr->name,szModuleName)) {
		    mod_dscr = mdt[j].dscr;
		    mod_tab = &mdt[j];
		    break;
		}
	}

        if (!mod_dscr) {

            /* we don't have a module descriptor, so we must have a file? */
	    InitBinary(lpCmdLine,1); 
        }


	/********************************************************/
        /* Create a hModule for this file 			*/
	/********************************************************/

        if (!hModule) {
	    hModule = CreateModuleInstance(szModuleName);
	    modinfo = GETMODULEINFO(hModule);

	    /* set the filename */
	    modinfo->lpFileName = lpCmdLine;
        }

	/********************************************************/
        /* we have a module descriptor 				*/
	/********************************************************/

	lpszModuleName = szModuleName;

        if (modinfo && modinfo->fMakeInstance) {
	    LoadNewInstance(modinfo, lpCmdLine, flags);
        }
        else if (mod_dscr) {
    
	    LoadModuleFromDscr(modinfo,mod_tab,flags);

        } else {	/* no dscr, just name */

	    if (!(ret = LoadModuleFromFile(modinfo,lpszFileName,lpCmdLine,flags)))
	       	FreeModuleInstance(hModule);

	    modinfo->nRefCnt++;
	    RELEASEMODULEINFO(modinfo);

	    return (HMODULE)ret;
        }
    
	RELEASEMODULEINFO(modinfo);
    	return hModule;
}

HANDLE
TWIN_LoadMDT(WORD flags, LPSTR lpCmdLine, LPARAM lParam)
{
        LPMODULETAB mod_tab;
        LPMODULEDSCR mod_dscr;
        LPSTR lpszModuleName;
        HMODULE hModule;
        LPMODULEINFO modinfo;

	/* we have a module descriptor table */
	mod_tab = (LPMODULETAB)lParam;
	mod_dscr = (LPMODULEDSCR)mod_tab->dscr;

	/* what is the module name ? */
	lpszModuleName = mod_dscr->name;

	/* if its loaded already, return its hModule */
	if ((hModule = GetModuleHandle(lpszModuleName))) {
	    return hModule;
	}

	/* not loaded so create the hModule for it */
	hModule = CreateModuleInstance(lpszModuleName);
	modinfo = GETMODULEINFO(hModule);

	/* set the filename */
	modinfo->lpFileName = lpCmdLine;

    	if (modinfo->fMakeInstance) {
		LoadNewInstance(modinfo, lpCmdLine, flags);
    	} else {
		LoadModuleFromDscr(modinfo,mod_tab,flags);
        }
    
	RELEASEMODULEINFO(modinfo);
	return hModule;
}

static DWORD
TWIN_GetModuleFileName(HMODULE hInstance,LPTSTR lpszFileName, DWORD cbFileName,int bflag)
{
    HMODULE hModule;
    LPMODULEINFO lpModuleInfo;
    int len;

    if (!(hModule = GetModuleFromInstance(hInstance)))
	hModule = hInstance;
    if (!(lpModuleInfo = GETMODULEINFO(hModule)))
	return 0;

    if (lpModuleInfo->lpFileName && strlen(lpModuleInfo->lpFileName) > 0) {
	len = strlen(strncpy(lpszFileName,lpModuleInfo->lpFileName,cbFileName));
    } else {
	GetCurrentDirectory(cbFileName,lpszFileName);
	strcat(lpszFileName,"/");
	strcat(lpszFileName,lpModuleInfo->lpModuleName);
	len = strlen(lpszFileName);
    }
    RELEASEMODULEINFO(lpModuleInfo);
    return len;
}
#ifdef TWIN_BIN32
DWORD WINAPI
GetModuleFileNameA(HINSTANCE hInstance,LPSTR lpstr,int len)
{
	DWORD rc;
	APISTR((LF_APICALL,"GetModuleFileNameA(%x,%p,%d)\n",
		hInstance,lpstr,len));

	rc = TWIN_GetModuleFileName(hInstance,lpstr, len, 0);
	APISTR((LF_APIRET,"GetModuleFileName returns DWORD %x\n",rc));
	return rc;	
}

DWORD WINAPI
GetModuleFileNameW(HINSTANCE hInstance,LPSTR lpstr,int len)
{
	DWORD rc;
	APISTR((LF_APICALL,"GetModuleFileNameA(%x,%p,%d)\n",
		hInstance,lpstr,len));

	rc = TWIN_GetModuleFileName(hInstance,lpstr, len, 1);
	APISTR((LF_APIRET,"GetModuleFileName returns DWORD %x\n",rc));
	return rc;	
}
#endif

DWORD WINAPI
GetModuleFileName(HMODULE hInstance,LPTSTR lpszFileName, DWORD cbFileName)
{
	return TWIN_GetModuleFileName(hInstance,lpszFileName, cbFileName,0);
}

DWORD 
PEGetProcAddressByName(MODULEINFO   *modinfo, LPCSTR lpName)
{
	DWORD i;
	MAPTABLE *mp;

	logstr(LF_API,"PEGetProcAddressByName(%p,%p) map=%p\n",
		modinfo,lpName,modinfo->map_table);
	if((mp = modinfo->map_table)) {
		char *name;

		for(i=0;mp[i].lpstr;i++) {
			name = 	mp[i].lpstr;
			if (strlen(name) == 0)
			   continue;

			if (strcasecmp(lpName,name) == 0)
			    return (DWORD) mp[i].faddr;
		}
		logstr(LF_LOG,"function %s not found\n",
			lpName);
	}

	/* the name is not in the module... */
	return 0;
}

DWORD
PEGetProcAddressByOrdinal(MODULEINFO   *modinfo, int ordinal,char *lpName)
{
	MAPTABLE *mp;

	logstr(LF_API,"PEGetProcAddressByOrdinal(%p,%d,%p) map=%p\n",
		modinfo,ordinal,lpName,modinfo->map_table);

	// search for native entry first
	if((mp = modinfo->map_table)) {
		int max;
		char *name;

		for(max=0;modinfo->map_table[max].lpstr;max++);

		if(ordinal > max) {
			
	           logstr(LF_ERROR,"ordinal exceeds table size %d %d\n",
			ordinal,max);
		   return (DWORD) 0;
		}

		name = mp[ordinal].lpstr;

		if((lpName == 0) || (name && lpName && (strcasecmp(lpName,name) == 0))) {
			DWORD faddress;
			faddress = (DWORD) mp[ordinal].faddr;
			return (DWORD) faddress;
		}

		return PEGetProcAddressByName(modinfo,lpName);

	}
	return 0;
}
/*
 * called from win32 binaries, return interface function 
 */
FARPROC WINAPI
GetProcAddress32(HINSTANCE hInstance, LPCSTR lpszProc)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	FARPROC	     fp;

       if(hInstance == 0)
       		hInstance = GetInstanceFromModule(GetModuleHandle("USER"));

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}
        APISTR((LF_APICALL,"GetProcAddress32(HINSTANCE=%x,LPCSTR=%s) %s\n",
		hInstance,
		HIWORD(lpszProc)?lpszProc:"ordinal",
		modinfo?modinfo->lpModuleName:"unknown"));

	/* is this a 32bit binary? */
	if(modinfo && modinfo->lptwinrt) {
		/* we need to return the interface function */
    		fp = modinfo->lptwinrt->lpfgetprocaddr(hInstance,lpszProc,0);
        	APISTR((LF_APIRET,"GetProcAddress32 returns FARPROC %p\n",fp));
		RELEASEMODULEINFO(modinfo);
		return fp;
	}

	/* we need to return the interface function, we don't have one */
	/* this is a call to a native or bin16 function */
	
        if (HIWORD((DWORD)lpszProc)) {
		fp = (FARPROC) PEGetProcAddressByName(modinfo, lpszProc);
	} else
		fp = (FARPROC) PEGetProcAddressByOrdinal(modinfo, (int)lpszProc,0);

        APISTR((LF_APIRET,"GetProcAddress32 returns FARPROC %p\n",fp));
	if(modinfo)
		RELEASEMODULEINFO(modinfo);
	return fp;
}

FARPROC WINAPI
TWIN_GetProcAddress(HINSTANCE hInstance, LPCSTR lpszProc,int bUnicode)
{
    HANDLE hModule;
    LPMODULEINFO lpModuleInfo;
    ENTRYTAB *lpEntry;
    int i;
    FARPROC retFunc = 0;

    if (!(lpModuleInfo = GETMODULEINFO(hInstance))) {
	if (!(hModule = GetModuleFromInstance(hInstance)))
	    return (FARPROC)NULL;
	if (!(lpModuleInfo = GETMODULEINFO(hModule)))
	    return (FARPROC)NULL;
    }

    if (!HIWORD((DWORD)lpszProc)) {
	i = LOWORD((DWORD)lpszProc);
	lpEntry = &lpModuleInfo->lpEntryTable[i];
	retFunc =  (FARPROC)lpEntry->fun;

	if (!retFunc && lpfnOEMGetPAddr)
	    retFunc = (FARPROC)lpfnOEMGetPAddr(lpModuleInfo,i);

	RELEASEMODULEINFO(lpModuleInfo);
	return retFunc;
    }
    else {	/* search by name */
	for(i=0,lpEntry = lpModuleInfo->lpEntryTable;
	    lpEntry->name;
	    i++,lpEntry++) 
	    if (!strcasecmp(lpszProc,lpEntry->name)) { 
		    retFunc =  (FARPROC)lpEntry->fun;
		    if (!retFunc && lpfnOEMGetPAddr)
			retFunc = (FARPROC)lpfnOEMGetPAddr(lpModuleInfo,i);

		    RELEASEMODULEINFO(lpModuleInfo);
		    return retFunc;
	    }
    }

    RELEASEMODULEINFO(lpModuleInfo);
    return NULL;
}


FARPROC WINAPI
GetProcAddress(HINSTANCE hInstance, LPCSTR lpszProc)
{
	HMODULE      hModule = 0;
	MODULEINFO   *modinfo = 0;
        FARPROC 	retFunc = 0;

	APISTR((LF_APICALL,"GetProcAddress(HINSTANCE=%x,LPCSTR=%p)\n",
		hInstance,lpszProc));

    	if(hInstance == 0)
       		hInstance = GetInstanceFromModule(GetModuleHandle("USER"));

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
		retFunc = modinfo->lptwinrt->lpfgetprocaddr(hInstance,lpszProc,0);
	} else
		retFunc = DefaultTwinRT.lpfgetprocaddr(hInstance,lpszProc,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	APISTR((LF_APIRET,"GetProcAddress: returns PROC %p\n",retFunc));
	return retFunc;
}

HINSTANCE WINAPI
LoadModule(LPCSTR lpszModuleName, LPVOID lpvParamBlock)
{
    HMODULE hModule;
    HINSTANCE hInst;
    PARAMBLOCK *lpb;

    APISTR((LF_APICALL,"LoadModule(LPCSTR=%s,LPVOID=%p)\n",
	    lpszModuleName,lpvParamBlock));

    lpb = (PARAMBLOCK *)lpvParamBlock;

    hModule = TWIN_LoadFile(ILL_NOERROR|ILL_EXEC|ILL_NEWINST,
				  lpb->lpszCmdLine,
				  (LPARAM)lpszModuleName);

    hInst = GetInstanceFromModule(hModule);

    APISTR((LF_APIRET,"LoadModule: returns HINSTANCE %x\n",hInst));
    return hInst;
}

BOOL WINAPI
FreeModule(HINSTANCE hInst)
{
    APISTR((LF_APICALL,"FreeModule(HINSTANCE=%x)\n",hInst));

    FreeLibrary(hInst);

    APISTR((LF_APIRET,"FreeModule: returns TRUE\n"));
    return TRUE;
}

HDRVR WINAPI
GetNextDriver(HDRVR hDrvr, DWORD fdwFlags)
{
    APISTR((LF_APISTUB,"GetNextDriver(HDRVR=%x,DWORD=%x)\n",hDrvr,fdwFlags));
    return 0;
}

HINSTANCE
LoadDriver(LPCSTR lpszDriver)
{
	char buf[_MAX_PATH];
	int i;
 
	for (i=0;lpszDriver[i];i++)
		buf[i] = tolower(lpszDriver[i]);

	buf[i] = '\0';
	strcat(buf,".drv");

	return LoadLibrary(buf);
}

void
TWIN_AppendExtension(char *lpstr,char *lpext)
{
	char *basename = lpstr;
	char *lpbase;

	if ( (lpbase = strrchr(basename,'/')) )
		basename = lpbase + 1;
	if ( (lpbase = strrchr(basename,'\\')) )
		basename = lpbase + 1;
	
	if ( (lpbase = strrchr(basename,'.')) == 0 ) {
	   strcat(lpstr,".");
	} else
	   return;
	
	strcat(lpstr,lpext);
}


UINT WINAPI
WinExec(LPCSTR lpCmdLine, UINT fuCmdShow)
{
	LPSTR lpszProgramName,lpTemp;
	PARAMBLOCK pb;
	char buf[BUFFER];
	UINT uiShow;
	HMODULE hModule;

	APISTR((LF_APICALL,"WinExec(cmdline=%s,cmdshow=%x\n",
		lpCmdLine,fuCmdShow));

	strcpy(buf,lpCmdLine);
	if ((lpTemp = strchr(buf,' '))) {
	    *lpTemp = '\0';
	    pb.lpszCmdLine = (char *) WinStrdup(++lpTemp);
	}
	else pb.lpszCmdLine = (char *) WinStrdup("");

	uiShow = fuCmdShow;
	pb.lpShow = &uiShow;
	pb.wSegEnv = 0;

	TWIN_AppendExtension(buf,"exe");

	lpszProgramName = WinMalloc(strlen(buf)+1);
	strcpy(lpszProgramName,buf);
	hModule = LoadModule(lpszProgramName,(LPVOID)&pb);
	WinFree(lpszProgramName);
	APISTR((LF_APIRET,"WinExec: returns UINT %x\n",hModule));
	return (UINT)hModule;
}

/* GetModuleDescription -- convenience function returning module description */
/* string read from the non-resident name table of the binary header */

LPSTR
GetModuleDescription(HINSTANCE hInstance)
{
    HMODULE hModule;
    LPMODULEINFO lpModuleInfo;
    LPSTR lpszResult;

    if (!(hModule = GetModuleFromInstance(hInstance)))
	hModule = hInstance;
    if (!(lpModuleInfo = GETMODULEINFO(hModule)))
	return 0;

    lpszResult = lpModuleInfo->lpModuleDescr;
    RELEASEMODULEINFO(lpModuleInfo);

    return lpszResult;
}


int
TWIN_GetWin32Flag(HWND hWnd)
{
	HINSTANCE 	hInst;
    	HMODULE 	hModule;
    	LPMODULEINFO 	lpModuleInfo;
	
	hInst = GetWindowInstance(hWnd);	
	hModule = GetModuleFromInstance(hInst);
	lpModuleInfo = GETMODULEINFO(hModule);

	if(lpModuleInfo && lpModuleInfo->dwWin32flag)
		return 1;
	return 0;
}

WORD
TWIN_GetReturnThunkSel(void)
{
	return return_thunk_selector;
}
