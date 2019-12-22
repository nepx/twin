#include "windows.h"
#include "toolhelp.h"

#include "kerndef.h"
#include "Kernel.h"
#include "Resources.h"
#include "Module.h"
#include "toolhelp.h"
#include "Log.h"

#include "DPMI.h"
#include "Endian.h"

#include <stdio.h>
#include <string.h>

extern ENV *envp_global;

extern LPOBJHEAD TWIN_GetTaskHead();

static BOOL
FillTaskEntry(TASKENTRY *lpte, LPBYTE lpData)
{
    int len;
    TASKINFO *lpTask;
    MODULEINFO *lpModule;

    if (!lpData || !lpte)
	return FALSE;

    /* Values that can be found in TDB. */
    lpte->hModule = (HMODULE)(UINT)GETWORD(lpData+0x1e);
    lpModule = GETMODULEINFO(lpte->hModule);
    if (lpModule == 0)
	return FALSE;

    /* Determine the rest of the data. */
    lpte->hInst = GetInstanceFromModule(lpte->hModule);
    lpte->hTask = GetTaskFromInstance(lpte->hInst);
    lpte->hTaskParent = 0;
    lpTask = GETTASKINFO(lpte->hTask);
    if (lpTask == 0)
	return FALSE;

    lpte->wStackTop = lpTask->uiAutodataSize;
    lpte->wStackMinimum = lpTask->uiAutodataSize + lpTask->uiStackSize;
    lpte->wStackBottom = lpTask->uiAutodataSize + lpTask->uiStackSize;
    if (lpTask->ObjHead.lpObjNext)
	lpte->hNext = (HANDLE)(UINT)((TASKINFO *)lpTask->ObjHead.lpObjNext)->wTDBSelector;
    else
	lpte->hNext = 0;
    lpte->wSS = envp_global->reg.ss;
    lpte->wSP = envp_global->reg.sp;
    lpte->wcEvents = 0;
    lpte->hQueue = 0;
    len = strlen(lpModule->lpModuleName);
    if (len > 8) len = 8;
    memcpy(lpte->szModule, lpModule->lpModuleName, len);
    lpte->szModule[len] = '\0';
    lpte->wPSPOffset = 0x100; /* into TDB? */
    RELEASETASKINFO(lpTask);
    RELEASEMODULEINFO(lpModule);
    return TRUE;
}

BOOL    WINAPI
TaskFirst(TASKENTRY FAR* lpte)
{
    LPTASKINFO lpTaskInfo = (TASKINFO *)TWIN_GetTaskHead();
    HTASK hTask = lpTaskInfo->ObjHead.hObj;
    BOOL retcode;

    LOGSTR((LF_APICALL,"TaskFirst(TASKENTRY=%x) handle %x\n", 
	lpte, hTask));
    retcode = FillTaskEntry(lpte, GetPhysicalAddress((UINT)hTask));
    LOGSTR((LF_APIRET,"TaskFirst: returns BOOL %d\n", retcode));
    return retcode;
}

BOOL    WINAPI
TaskNext(TASKENTRY FAR* lpte)
{
    BOOL retcode;

    LOGSTR((LF_APICALL,"TaskNext(TASKENTRY=%x) handle %x\n", 
	lpte, lpte->hNext));
    retcode = FillTaskEntry(lpte, GetPhysicalAddress((UINT)lpte->hNext));
    LOGSTR((LF_APIRET,"TaskNext: returns BOOL %d\n", retcode));
    return retcode;
}

BOOL    WINAPI
TaskFindHandle(TASKENTRY FAR* lpte, HTASK hTask)
{
    BOOL retcode;

    LOGSTR((LF_APICALL,"TaskFindHandle(TASKENTRY=%x) handle %x\n", 
	lpte, hTask));
    retcode = FillTaskEntry(lpte, GetPhysicalAddress((UINT)hTask));
    LOGSTR((LF_APIRET,"TaskFindHandle: returns BOOL %d\n", retcode));
    return retcode;
}

DWORD   WINAPI
TaskSetCSIP(HTASK hTask, WORD wCS, WORD wIP)
{
    LOGSTR((LF_APISTUB,"TaskSetCSIP(HTASK=%x,WORD=%x,WORD=%x)\n",
	hTask,wCS,wIP));
    return 0;
}
DWORD   WINAPI 
TaskGetCSIP(HTASK hTask)
{
    LOGSTR((LF_APISTUB,"TaskGetCSIP(HTASK=%x)\n",
	hTask));
    return 0;
}
BOOL    WINAPI 
TaskSwitch(HTASK hTask, DWORD dwNewCSIP)
{
    LOGSTR((LF_APISTUB,"TaskSwitch(HTASK=%x,DWORD=%x)\n",
	hTask,dwNewCSIP));
    return FALSE;
}
