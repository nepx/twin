
#include "windows.h"
#include "toolhelp.h"

#include "kerndef.h"
#include "Kernel.h"
#include "Resources.h"
#include "Module.h"
#include "Log.h"

#include <stdio.h>
#include <string.h>

/****** Class Information  ****************************************************/
BOOL    WINAPI 
ClassFirst(CLASSENTRY FAR* lpClass)
{
    LOGSTR((LF_APISTUB,"ClassFirst(CLASSENTRY*=%p)\n",
	lpClass));
    return FALSE;
}
BOOL    WINAPI 
ClassNext(CLASSENTRY FAR* lpClass)
{
    LOGSTR((LF_APISTUB,"ClassNext(lpClass=%p)\n",
	lpClass));
    return FALSE;
}

/****** Interrupt Handling ****************************************************/
BOOL    WINAPI 
InterruptRegister(HTASK hTask, FARPROC lpfnIntCallback)
{
    LOGSTR((LF_APISTUB,"InterruptRegister(HTASK=%x,FARPROC=%p)\n",
	hTask,lpfnIntCallback));
    return FALSE;
}
BOOL    WINAPI 
InterruptUnRegister(HTASK hTask)
{
    LOGSTR((LF_APISTUB,"InterruptUnRegister(HTASK=%x)\n",
	hTask));
    return FALSE;
}

/****** Local Memory Information  *********************************************/
BOOL    WINAPI 
LocalFirst(LOCALENTRY FAR* lpLocal, HGLOBAL hHeap)
{
    LOGSTR((LF_APISTUB,"LocalFirst(LOCALENTRY*=%p,HGLOBAL=%x)\n",
	lpLocal,hHeap));
    return FALSE;
}
BOOL    WINAPI 
LocalInfo(LOCALINFO FAR* lpLocal, HGLOBAL hHeap)
{
    LOGSTR((LF_APISTUB,"LocalInfo(LOCALINFO *=%p,HGLOBAL=%x)\n",
	lpLocal,hHeap));
    return FALSE;
}
BOOL    WINAPI 
LocalNext(LOCALENTRY FAR* lpLocal)
{
    LOGSTR((LF_APISTUB,"LocalNext(LOCALENTRY*=%p)\n",
	lpLocal));
    return FALSE;
}

/****** Memory Information  ***************************************************/
DWORD   WINAPI 
MemoryRead(WORD wSel, DWORD dwOffset, void FAR* lpBuffer, DWORD dwcb)
{
    LOGSTR((LF_APISTUB,"MemoryRead(%x,%x,%p,%x)\n",
	wSel,dwOffset,lpBuffer,dwcb));
    return FALSE;
}
DWORD   WINAPI 
MemoryWrite(WORD wSel, DWORD dwOffset, void FAR* lpBuffer, DWORD dwcb)
{
    LOGSTR((LF_APISTUB,"MemoryWrite(%x,%x,%p,%x)\n",
	wSel,dwOffset,lpBuffer,dwcb));
    return FALSE;
}

/****** Notification Information  *********************************************/
BOOL    WINAPI 
NotifyRegister(HTASK hTask, LPFNNOTIFYCALLBACK lpfn, WORD wFlags)
{
    LOGSTR((LF_APISTUB,"NotifyRegister(HTASK=%x,FARPROC=%p,%x)\n",
	hTask,lpfn,wFlags));
    return FALSE;
}
BOOL    WINAPI 
NotifyUnRegister(HTASK hTask)
{
    LOGSTR((LF_APISTUB,"NotifyUnRegister(HTASK=%x)\n",
	hTask));
    return FALSE;
}


/****** Stack Information  ****************************************************/
BOOL    WINAPI 
StackTraceFirst(STACKTRACEENTRY FAR* lpStackTrace, HTASK hTask)
{
    LOGSTR((LF_APISTUB,"StackTraceFirst(STACKTRACE*=%p,HTASK=%x)\n",
	lpStackTrace,hTask));
    return FALSE;
}
BOOL    WINAPI 
StackTraceNext(STACKTRACEENTRY FAR* lpStackTrace)
{
    LOGSTR((LF_APISTUB,"StackTraceNext(STACKTRACEENTRY*=%p)\n",
	lpStackTrace));
    return FALSE;
}

BOOL    WINAPI 
SystemHeapInfo(SYSHEAPINFO FAR* lpSysHeap)
{
    LOGSTR((LF_APISTUB,"SystemHeapInfo(SYSHEAPINFO*=%p)\n",
	lpSysHeap));
    return FALSE;
}
void    WINAPI 
TerminateApp(HTASK hTask, WORD wFlags)
{
    LOGSTR((LF_CONSOLE,"TerminateApp(HTASK=%x,WORD=%x)\n",
	hTask,wFlags));
    /* terminate all threads */
    exit(0);
}
