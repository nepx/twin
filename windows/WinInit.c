/*    
	WinNat.c	2.51
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "windows.h"
#include "windowsx.h"
#include "Log.h"
#include "kerndef.h"
#include "ModTable.h"
#include "Driver.h"
#include "compat.h"
#include "mfs_config.h"
#include "WinConfig.h"
#include "Kernel.h"
#include "WinMalloc.h"
#include "Resources.h"
#include "Module.h"
#include "Debug.h"
#include "dos.h"
#include "DPMI.h"

/****************************************************************/
/*								*/
/* imported stuff 						*/
/*								*/
/****************************************************************/

extern MODULETAB TWIN_ModuleTable[256];

extern DWORD InitTwinDriver(DWORD *);
extern void TWIN_InitLSD();
extern BOOL GdiInitDC(void);
extern void TWIN_InitSysColors(void);
extern void InitFileSystem(void);
extern DWORD TWIN_DrvCallback(DWORD,LPARAM,LPARAM,LPVOID);
extern short TWIN_TerminateCommdlg( void );
extern BOOL InitBinary(LPSTR,UINT);
extern BOOL TWIN_InitializeBinaryCode();
extern HANDLE TWIN_LoadMDT(WORD , LPSTR , LPARAM );
extern BOOL bIsBinaryApp;
extern BOOL SetSystemMetrics(int, int);
extern HINSTANCE    InternalLoadDLL(MODULETAB *);
extern int TWIN_ThunkManager(int);
extern void VirtualInit(int);
extern void debuggerbreak(void);
extern void LoadPreLoads(void);
extern void InitDebugSystem(void);
extern BOOL InitClasses(void);
extern BOOL InitWindows();

static LPMODULETAB ReadAppDscrTable(LPSTR,MODULEDSCR * );

TWINDRVSUBPROC **DrvEntryTab;
char *DEBUG_argv0;

/****************************************************************/
/*								*/
/* exported stuff						*/
/*								*/
/****************************************************************/

/* these are the major entry/exit points of library */
BOOL   		TWIN_LibExit(int,DWORD);	
LPMODULETAB    	TWIN_LibEntry(char *,MODULEDSCR *);

int __argc;		/* directly (modified) from argc */
char **__argv;		/* same */
char *_acmdln;		/* concatenated version of __argv[*] */
char **__environ;	/* we will build this from ~/.twinrc file */

/* this should be moved down to the driver when we understand the MAC... */
static LPSTR SetCommandLine(int, char **);

DWORD TWIN_DriverCaps;

/****************************************************************/
/*								*/
/* implementation 						*/
/*								*/
/****************************************************************/

static BOOL
TWIN_InitLibrary(void)
{
    static BOOL bInit = FALSE;
    TWINDRVCONFIG DrvConfig;

    if (!bInit) {
	/*
	 *  Protect against multiple calls and recursive calls.  Various
	 *  API locations can make a call to us, in addition to the
	 *  normal startup path through main().  This is so that the
	 *  API's that require this initialization can be called from
	 *  constructors in the OWL and MFC libraries.  The API's must
	 *  in turn call us, so make sure we only initialize once.
	 */
	bInit = TRUE;

	/* Initialize the filesystem code */
	InitFileSystem();

	/* this initializes loadable software devices */
	TWIN_InitLSD();

	DrvConfig.lpDrvCallback = &TWIN_DrvCallback;

	DRVCALL_INITSYSTEM(DSUBSYSTEM_INIT,0,1,&DrvConfig);

	/* set system metrics */
	/* we should set menu height, rather than hardcode at 25 */
	/* The rule of thumb is MULDIV(fontsize,7,4) 	     */
	/* Where fontsize is the actual font size, not points    */
	/* SetSystemMetrics(SM_CYCAPTION,7*12/4); */

        SetSystemMetrics(SM_CXSCREEN,DrvConfig.nScreenWidth);
	SetSystemMetrics(SM_CYSCREEN,DrvConfig.nScreenHeight);
	SetSystemMetrics(SM_CXFULLSCREEN,DrvConfig.nScreenHeight);
	SetSystemMetrics(SM_CYFULLSCREEN,DrvConfig.nScreenHeight-
				GetSystemMetrics(SM_CYCAPTION));

	/* this initializes DC cache */
	GdiInitDC();

	/* this initializes the system color table */
	TWIN_InitSysColors();

	/* this initializes MFS layer */
	MFS_INIT();

    	xdoscall(XDOS_INIT,0,0,0);

	/* this initializes the binary machinery */
	TWIN_InitializeBinaryCode();

	if (GetConfigOption(WD_DEBUGGER))
		TWIN_DebuggerProc(TWIN_DBGLOAD,0,0);

	InitClasses();
	if (!InitWindows()) {
		FatalAppExit(0,"Cannot initialize window manager\n");
	}
    }
    return bInit;
}

BOOL TWIN_InitDriver()
{
    static int entry;

    if ( DrvEntryTab == NULL ) {
	if(entry == 0) {
	  entry++;
	  DrvEntryTab = (TWINDRVSUBPROC **) DriverBootstrap();

	  /* Initialize Debug System */
	  InitDebugSystem();

	  /* LDT setup, required by InitializeNativeTask  */
    	  DPMI_Notify(DN_INIT,0);

	  /* now we have threads setup */
          InitializeNativeTask("");
	}

	(void)TWIN_InitLibrary();
    }

    return TRUE;
}

void 
LoadTwinLibInit( ENTRYTAB *lpLibInitTable)
{
    /*
     *  Systems which do not support automatically running
     *  init code before calling main will provide initialization
     *  entry points for linked native DLLs.  We run them from
     *  here;  they will setup the module description tables, and
     *  call each library's LibMain, if needed.
     */
    if(lpLibInitTable == 0)
	return;

    while (lpLibInitTable->name != NULL) {
	(*(lpLibInitTable->fun))();
	lpLibInitTable++;
    }
}

int
LoadTwinModDscr(int argc,char **argv, MODULEDSCR *ModDscrApp)
{
	int  rc;
	LPMODULETAB mdt;

	DEBUG_argv0 = argv[0];

        _acmdln = SetCommandLine(argc,argv);

	mdt = TWIN_LibEntry(_acmdln,ModDscrApp);

        rc = (BOOL)TWIN_LoadMDT(ILL_APPL|ILL_DSCR,_acmdln,(LPARAM)&mdt[0]);

	return rc;
}

LPMODULETAB
TWIN_LibEntry( char *lpCmd,MODULEDSCR *ModDscrApp)
{
    LPMODULETAB mdt;

    if ( !TWIN_InitDriver() )
	return 0;

    TWIN_DriverCaps = DRVCALL_INITDRIVER(DSUBSYSTEM_GETCAPS,0,0,0);

    mdt = ReadAppDscrTable(lpCmd,ModDscrApp);

    return mdt;
}

BOOL
TWIN_LibExit(int bExit,DWORD dwreturn)
{
	static int libexit;

	if(libexit) {
		if(bExit)
			_exit(dwreturn);	
		return FALSE;
	}
	libexit++;

#ifdef WINMALLOC_CHECK
        logstr(LF_LOG,"Delete Stock Objects\n");
	TWIN_DeleteStockObjects ();

        logstr(LF_LOG,"Dumping GDI\n");
	TWIN_DumpGdiObjects ();
#endif

	/* cleanup virtual memory */
	VirtualInit(0);

	DRVCALL_INITSYSTEM(DSUBSYSTEM_INIT,0,0,0);

	/* de-initialize DPMI layer */
        DPMI_Notify(DN_EXIT,0);

	/* deinitialize xdos */
	MFS_EXIT();

#ifdef WINMALLOC_CHECK
        LOGSTR((LF_LOG,"Cleanup GlobalAlloc\n"));
	TWIN_HandleCleanup();

	/* remove thunk tables... */
	TWIN_ThunkManager(0);

	/* how is the fixed pool looking */
	WinSaveString((LPSTR) -1);
	WinStrdup((LPSTR) -1);

	/* lets dump our memory chain */
        logstr(LF_LOG,"Check Memory Allocation\n");
	(void) WinMallocInfo(0, 0, 0, 0);

	/* lets free our memory chain */
	TWIN_FreeAllMemory();

#endif
    	if(bExit)
		exit(dwreturn);

	return(TRUE);   
}

void WINAPI
FatalExit(int Code)
{
	ExitWindows(Code,0);
}

void
OldExitWindows(void)
{
	ExitWindows(0,0);
}

BOOL WINAPI
ExitWindows(DWORD dwreturn,UINT reserved)
{
	static int exitcode;
	if(exitcode) {
		LOGSTR((LF_CONSOLE,"ExitWindows...\n"));
		debuggerbreak();
		exit(dwreturn);
	}
	exitcode++;
	if(reserved == 0) {
#ifdef	LATER
	/* according to definition we have to send WM_QUERYENDSESSION
	   to notify all apps; if all apps agree, send them WM_ENDSESSION
	   before finally shutting down */
#endif
	}
	APISTR((LF_API,"ExitWindows(%d,%d)\n",dwreturn,reserved));
	return TWIN_LibExit(1,dwreturn);	
}

void WINAPI
FatalAppExit(UINT wAction,LPCSTR lpText)
{
    static int flag;

    APISTR((LF_CONSOLE,"FatalAppExit(UINT=%x,LPCSTR=%s)\n",
	wAction,lpText));

    flag++;
    if(flag != 1)
    	ExitWindows(0,0);
	
    if(GetConfigOption(WD_FATALERR)) {
  		debuggerbreak();
    }
	
    DeleteTask(0);

    ExitWindows(0,0);
}

void
DebugBreak()
{
#if defined(TWIN_BIN16)
	LOGSTR((LF_CONSOLE,"DebugBreak\n"));
	debuggerbreak();
#else
	FatalAppExit(0,"DebugBreak not supported!\n");
#endif
}

WORD
InitApp(HINSTANCE hInstance)
{
    APISTR((LF_APICALL,"InitApp(HINSTANCE=%x)\n",hInstance));
    APISTR((LF_APIRET,"InitApp: returns WORD %d\n",1));
    return 1;
}

WORD
WaitEvent(HANDLE taskID)
{
    APISTR((LF_APICALL,"WaitEvent(HANDLE=%x)\n",taskID));
    APISTR((LF_APIRET,"WaitEvent: returns WORD %d\n",0));
    return 0;
}

static LPSTR
SetCommandLine(int argc, char **argv)
{
    int nCount,nLen;
    LPSTR lpCmd,lpCmdLine;
    
    __argc = argc;
    __argv = argv;

    if( argc == 0 )
    {
      return( NULL );
    }

    for (nCount = 0, nLen = 0; nCount < __argc; nCount ++)
	nLen += strlen(__argv[nCount]) + 1;

    lpCmd = WINMALLOC(nLen + 1,1,0);
    lpCmd[0] = '\0';

    if (__argc > 1) {
	for (nCount = 1; nCount < __argc; nCount++) {
	    strcat(lpCmd, " ");
	    strcat(lpCmd, __argv[nCount]);
	}
    }

    lpCmdLine = WinStrdup(lpCmd);
    WINFREE(lpCmd);

    return lpCmdLine;
}

LPSTR
GetDOSEnvironment(void)
{
    APISTR((LF_APISTUB,"GetDOSEnvironment()\n"));
    return (LPSTR) __environ;
}

static LPMODULETAB
ReadAppDscrTable(LPSTR lpCmd,MODULEDSCR *ModDscrApp )
{
    HANDLE hModule;
    LPSTR lpCmdLine;
    LPMODULETAB mdt;
    register int i;

    if( lpCmd == NULL )
    {
      lpCmdLine = "";
    }
    else
    {
      lpCmdLine = (LPSTR)strchr(lpCmd,' ');
      if (lpCmdLine) lpCmdLine++;
      else lpCmdLine = "";
    }

    mdt = &TWIN_ModuleTable[0];
    TWIN_ModuleTable[0].flags = 0;      	/* flags for module */
    TWIN_ModuleTable[0].dscr = ModDscrApp;	/* ptr to mod descriptor */

    if (bIsBinaryApp) {
        lpCmdLine = (LPSTR)strchr(lpCmdLine,' ');
        if (lpCmdLine)
            lpCmdLine++;
        else
            lpCmdLine = "";
        InitBinary(lpCmdLine,0);        /* bInitBinary gets set here    */
    }

    for (i=1; mdt[i].dscr; i++) {
        /* check if module should be pre-loaded */
        if (!(mdt[i].flags & MODULE_NO_LOAD))
            hModule = TWIN_LoadMDT(ILL_DSCR,lpCmdLine,(LPARAM)&mdt[i]);
    }

    /* set flag to internalloadlibrary to not defer loading...*/
    InternalLoadDLL(0);

    LoadPreLoads();

    return mdt;
}

