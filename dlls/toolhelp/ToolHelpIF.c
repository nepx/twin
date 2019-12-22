#include <stdio.h>
#include <string.h>

#include "windows.h"
#include "print.h"
#include "commdlg.h"

#include "kerndef.h"
#include "Endian.h"
#include "Log.h"
#include "BinTypes.h"
#include "Kernel.h"		/* MEMORYINFO */
#include "PrinterBin.h"
#include "DPMI.h"
#include "dos.h"
#include "toolhelp.h"
#include "Resources.h"
#include "Module.h"

static void GetTASKENTRY(LPBYTE, TASKENTRY *);

void
IT_1LPTE(ENV *envp,LONGPROC f)
{
    DWORD retcode;
    LPBYTE lpStruct;
    TASKENTRY te;

    lpStruct = (LPBYTE)GetAddress(GETWORD(SP+6),GETWORD(SP+4));
    te.hNext = (HANDLE)(UINT)GETWORD(lpStruct + DWORD_86 + HANDLE_86 + HANDLE_86 +
	HANDLE_86 + HANDLE_86 + WORD_86 + WORD_86 +
	WORD_86 + WORD_86 + WORD_86 + WORD_86 +
	HANDLE_86 + MAX_MODULE_NAME + 1 + WORD_86);
    te.dwSize = GETDWORD(lpStruct);
    retcode = (f)(&te);
    if (retcode)
	GetTASKENTRY(lpStruct,&te);
    envp->reg.sp += LP_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

void
IT_1LP1HTE(ENV *envp,LONGPROC f)
{
    DWORD retcode;
    LPBYTE lpStruct;
    TASKENTRY te;

    lpStruct = (LPBYTE)GetAddress(GETWORD(SP+8),GETWORD(SP+6));
    te.dwSize = GETDWORD(lpStruct);
    retcode = (f)(&te, (HANDLE)(UINT)GETWORD(SP+4));
    if (retcode)
	GetTASKENTRY(lpStruct,&te);
    envp->reg.sp += LP_86 + HANDLE_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

static void
GetTASKENTRY(LPBYTE lpData, TASKENTRY *lpte)
{
    LPBYTE ptr = lpData;

    PUTDWORD(ptr,lpte->dwSize);
    ptr += DWORD_86;
    PUTWORD(ptr,(UINT)lpte->hTask);
    ptr += HANDLE_86;
    PUTWORD(ptr,(UINT)lpte->hTaskParent);
    ptr += HANDLE_86;
    PUTWORD(ptr,(UINT)lpte->hInst);
    ptr += HANDLE_86;
    PUTWORD(ptr,(UINT)lpte->hModule);
    ptr += HANDLE_86;
    PUTWORD(ptr,lpte->wSS);
    ptr += WORD_86;
    PUTWORD(ptr,lpte->wSP);
    ptr += WORD_86;
    PUTWORD(ptr,lpte->wStackTop);
    ptr += WORD_86;
    PUTWORD(ptr,lpte->wStackMinimum);
    ptr += WORD_86;
    PUTWORD(ptr,lpte->wStackBottom);
    ptr += WORD_86;
    PUTWORD(ptr,lpte->wcEvents);
    ptr += WORD_86;
    PUTWORD(ptr,(UINT)lpte->hQueue);
    ptr += HANDLE_86;
    memcpy(ptr,lpte->szModule,MAX_MODULE_NAME+1);
    ptr += MAX_MODULE_NAME+1;
    PUTWORD(ptr,lpte->wPSPOffset);
    ptr += WORD_86;
    PUTWORD(ptr,(UINT)lpte->hNext);
}

void
IT_NOTIFYREG (ENV *envp,LONGPROC f)	/* NotifyRegister */
{
    DWORD retcode, dwProc;
    HTASK hTask;

    dwProc = GETDWORD(SP+6);
#ifdef	LATER
    dwProc = (dwProc)?make_native_thunk(dwProc,(DWORD)hsw_notifyreg):0L;
#endif
    hTask = GetSelectorHandle(GETWORD(SP+10));
    retcode = NotifyRegister(hTask,
			(LPFNNOTIFYCALLBACK)dwProc,
			GETWORD(SP+4));
    envp->reg.sp += HANDLE_86 + LP_86 + WORD_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

void
IT_INTERRUPTREG (ENV *envp,LONGPROC f)	/* InterruptRegister */
{
    DWORD retcode, dwProc;
    HTASK hTask;

    dwProc = GETDWORD(SP+4);
#ifdef	LATER
    dwProc = (dwProc)?make_native_thunk(dwProc,(DWORD)hsw_interruptreg):0L;
#endif
    hTask = GetSelectorHandle(GETWORD(SP+8));
    retcode = InterruptRegister(hTask,(FARPROC)dwProc);
    envp->reg.sp += HANDLE_86 + LP_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

void
IT_MEMMANINFO (ENV *envp,LONGPROC f)	/* MemManInfo */
{
    DWORD retcode,dwCount,dwBuffer;
    LPBYTE lpStruct;
    MEMMANINFO mmi;
    int i;

    if (!(lpStruct = (LPBYTE)GetAddress(GETWORD(SP+6),GETWORD(SP+4))))
	retcode = 0;
    else {
	mmi.dwSize = sizeof(MEMMANINFO);
	retcode = MemManInfo(&mmi);
	if (retcode) {
	    dwBuffer = GETDWORD(lpStruct);
	    lpStruct += DWORD_86;
	    for (i = 0, dwCount = 2*DWORD_86;
		 i < 9;
		 dwCount += DWORD_86, i++, lpStruct += 4) {
		if (dwCount >= dwBuffer)
		    break;
		PUTDWORD(lpStruct,*((LPDWORD)&mmi+1+i));
	    }
	    if ((dwCount - DWORD_86 + WORD_86) < dwBuffer)
		PUTWORD(lpStruct,mmi.wPageSize);
	}
    }
    envp->reg.sp += LP_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

MODULEINFO *GetModuleFromIndex(int );

void
IT_GLOBALENTRYHANDLE (ENV *envp,LONGPROC f)
{
    DWORD retcode,dwSize;
    UINT uSel,uIndex;
    LPBYTE lpData;
    GLOBALENTRY ge;
    LPMODULEINFO lpModInfo;

    lpData = (LPBYTE)GetAddress(GETWORD(SP+8), GETWORD(SP+6));
    dwSize = GETDWORD(lpData);
    if (dwSize != (3*DWORD_86 + HANDLE_86 + 3*WORD_86 + INT_86 +
		HANDLE_86 + 2*WORD_86 + 2*DWORD_86))
	retcode = 0;
    else {
	ge.dwSize = sizeof(GLOBALENTRY);
	uSel = (UINT)GETWORD(SP+4);
	if ((retcode = (DWORD)GlobalEntryHandle(&ge,GetSelectorHandle(uSel)))) {
	    PUTDWORD(lpData+4,ge.dwAddress);
	    PUTDWORD(lpData+8,ge.dwBlockSize);
	    PUTWORD(lpData+12,(UINT)ge.hBlock);
	    PUTWORD(lpData+14,ge.wcLock);
	    PUTWORD(lpData+16,ge.wcPageLock);
	    PUTWORD(lpData+18,ge.wFlags);
	    PUTWORD(lpData+20,ge.wHeapPresent);
#ifdef	LATER
	    PUTWORD(lpData+22,ge.hOwner);
#else
	    /* this is a kludge for libraries that depend on load order */
	    if (!(uIndex = (UINT)GetModuleIndex(uSel)))
		/* this is an index for KERNEL (maybe!) */
		uIndex = 1;
	    lpModInfo = GetModuleFromIndex(uIndex);
	    PUTWORD(lpData+22,(UINT)lpModInfo->ObjHead.hObj);
#endif
	    PUTWORD(lpData+24,ge.wType);
	    PUTWORD(lpData+26,ge.wData);
	    PUTDWORD(lpData+28,ge.dwNext);
	    PUTDWORD(lpData+32,ge.dwNextAlt);
	}
    }

    envp->reg.sp += LP_86 + HANDLE_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

void
IT_TIMERCOUNT (ENV *envp,LONGPROC f)
{
    DWORD   retcode;
    LPBYTE lpStruct;
    TIMERINFO ti;

    lpStruct = (LPBYTE)GetAddress(GETWORD(SP+6),GETWORD(SP+4));
    if (lpStruct) {
	ti.dwSize = GETDWORD(lpStruct);
	retcode = (f)(&ti);
	if (retcode) {
	    PUTDWORD(lpStruct+4,ti.dwmsSinceStart);
	    PUTDWORD(lpStruct+8,ti.dwmsThisVM);
	}
    }
    else
	retcode = (f)(NULL);

    envp->reg.sp += LP_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

