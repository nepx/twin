/*    
	Kernel.c	2.54
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
#include <stdlib.h>
#include <string.h>

#include "windows.h"
#include "WinDefs.h"
#include "kerndef.h"
#include "Resources.h"
#include "Module.h"
#include "Kernel.h"
#include "Driver.h"
#include "Log.h"
#include "WinMalloc.h"
#include "Strings32.h"
#include "peexe.h"

/******************************************************************************/

LPQUEUE AllocQueue(int);

/* external functions */
extern BOOL MenuMouseMove(LPMSG);
extern BOOL TWIN_ForwardButtonEvent(LPMSG);
extern BOOL HaveDisabledAncestor(HWND);

/*****************************************************************************/

LPQUEUE  SystemQueue;

/*****************/
/* API functions */
/*****************/

LONG WINAPI
SetSwapAreaSize(UINT uiSize)
{
    APISTR((LF_APISTUB,"SetSwapAreaSize(UINT=%x)\n", uiSize));
    return MAKELONG((uiSize)?uiSize:0x8000,0xc000);
}

BOOL WINAPI
DefineHandleTable(WORD wOffset)
{
    APISTR((LF_APISTUB,"DefineHandleTable(WORD=%x)\n", wOffset));
    return TRUE;
}

UINT WINAPI
RegisterWindowMessage(LPCSTR lpszMessage)
{
    UINT rc;
    APISTR((LF_APICALL,"RegisterWindowMessage(LPCSTR=%s)\n", lpszMessage));
    rc = GlobalAddAtom(lpszMessage);
    APISTR((LF_APIRET,"RegisterWindowMessage: returns UINT %x\n",rc));
    return rc;
}

DWORD WINAPI
GetShortPathName(LPCSTR inpath,LPSTR outpath,DWORD len)
{
	char pathname[256];
	const char *p;
	int  n;

	APISTR((LF_APICALL,"GetShortPathName(LPSTR=%p,LPSTR=%p,DWORD=%d)\n",
		inpath,outpath,len));

	if((p = strrchr(inpath,'\\'))) {
		p++;
	} else
		p = inpath;
	strncpy(pathname,p,256);

	n = strlen(pathname);
	if(len < n)
		n = len;
	strncpy(outpath,pathname,n);

        APISTR((LF_APIRET,"GetShortPathName returns int %d\n",n));
	return n;
}


#ifdef TWIN_BIN32

FARPROC WINAPI
SetUnhandledExceptionFilter(FARPROC fp)
{
	APISTR((LF_APISTUB,"SetUnhandleExceptionFilter(FARPROC=%p)\n",fp));
	return (FARPROC) 0;
}


UINT WINAPI
RegisterWindowMessageA(LPCSTR lpszMessage)
{
    UINT rc;
    APISTR((LF_APICALL,"RegisterWindowMessageA(LPCSTR=%s)\n", lpszMessage));
    rc = GlobalAddAtom(lpszMessage);
    APISTR((LF_APIRET,"RegisterWindowMessageA: returns UINT %x\n",rc));
    return rc;
}

UINT WINAPI
RegisterWindowMessageW(LPCWSTR lpszMessage)
{
    UINT rc;
    int    len;
    char  *lpstr;
	
    len = lstrlenW(lpszMessage);

    lpstr = WinMalloc(len);
    WideCharToMultiByte(0,0,(LPCWSTR)lpszMessage,0,lpstr,len,0,0);
    APISTR((LF_APICALL,"RegisterWindowMessageW(LPCSTR=%s)\n", lpstr));
    rc = GlobalAddAtom(lpstr);
    APISTR((LF_APIRET,"RegisterWindowMessageW returns UINT %x\n", rc));
    WinFree(lpstr);
    return rc;
}

#endif

/*
 * Take a win32 HMODULE32 and extract the modinfo 
 * saved in the Reserved1 field of the optional header
 * (alternatively, do a lookup for modules with the
 * baseaddress field set to HMODULE32
 */
HINSTANCE
GetInstanceFromModule32(void *p)
{
	IMAGE_DOS_HEADER *lpDosHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	MODULEINFO *modinfo;
	HINSTANCE   hInstance;

	lpDosHeader = (IMAGE_DOS_HEADER *) p;
	pNTHeader = (PIMAGE_NT_HEADERS)(p + lpDosHeader->e_lfanew);

	modinfo = (MODULEINFO *) pNTHeader->OptionalHeader.Reserved1;

#if 0
/* Alternate lookup? */
	pNTHeader == modinfo->lpNTHeader;
#endif
	hInstance = modinfo->hInst;
	return hInstance;
}

FARPROC WINAPI
MakeProcInstance(FARPROC lpproc, HINSTANCE hInstance)
{
    APISTR((LF_APICALL,"MakeProcInstance(FARPROC=%x,HINSTANCE=%x)\n", 
	lpproc,hInstance));
    APISTR((LF_APIRET,"MakeProcInstance: returns FARPROC %x\n",lpproc));
    return lpproc;	
}

void WINAPI
FreeProcInstance(FARPROC lpproc)
{
    APISTR((LF_APICALL,"FreeProcInstance(FARPROC=%x)\n", lpproc));
    APISTR((LF_APIRET,"FreeProcInstance: returns void\n"));
    return;
}

BOOL  WINAPI
SetMessageQueue(int cMsg)
{
    LPTASKINFO lpTaskInfo;
    LPQUEUE    lpQueue;
    BOOL       bResult;

    if (!(lpTaskInfo = GETTASKINFO(GetCurrentTask()))) {
	return FALSE;
    }
    if(lpTaskInfo->lpQueue)
    	WinFree((char *)(lpTaskInfo->lpQueue));

    lpQueue = AllocQueue(cMsg);
    bResult = (BOOL) ((lpTaskInfo->lpQueue = lpQueue) != 0);
    RELEASETASKINFO(lpTaskInfo);
    return bResult;
}

VOID WINAPI
Sleep(DWORD sleeptime)
{
    APISTR((LF_APICALL, "Sleep(DWORD=%d)\n",sleeptime));

    DRVCALL_SYSTEM(PSSH_SLEEP,sleeptime,0,0);

    APISTR((LF_APIRET, "Sleep: returns void\n"));
    return;
}

BOOL
CreateDataInstance(HINSTANCE hInst, HMODULE hModule, HTASK hTask)
{
    MEMORYINFO *lpMemory;

#ifdef TWIN_BIN32
    if (HIWORD(hInst)) {
	hInst = GetInstanceFromModule32((void *) hInst);
    }
#endif
    if (!(lpMemory = GETHANDLEINFO(hInst)))
	return FALSE;

    lpMemory->hModule = hModule;
    lpMemory->hTask = hTask;

    RELEASEHANDLEINFO(lpMemory);
    return TRUE;
}


HMODULE
GetModuleFromInstance(HINSTANCE hInstance)
{
    MEMORYINFO *lpMemory;
    HANDLE hResult;

    if (hInstance == 0)
    {
        HTASK t;
	TASKINFO *taskinfo;
	HINSTANCE hInst;

        t = GetCurrentTask();
	taskinfo = GETTASKINFO(t);
	hInst = taskinfo->hInst;
	RELEASETASKINFO(taskinfo);
	if(hInst == 0) {
		return 0;
	}
        return GetModuleFromInstance(hInst); 
    }

#ifdef TWIN_BIN32
    if (HIWORD(hInstance)) {
	hInstance = GetInstanceFromModule32((void *) hInstance);
    }
#endif
    if (!(lpMemory = GETHANDLEINFO(hInstance))) {
        MODULEINFO *lpModule = GETMODULEINFO(hInstance);
        BOOL bGotModule =  lpModule ? TRUE : FALSE;
	if (!bGotModule)
	    return (HMODULE)0;
	else {
	    RELEASEMODULEINFO(lpModule);
	    return (HMODULE)hInstance;
	}
    }
    hResult = lpMemory->hModule;
    RELEASEHANDLEINFO(lpMemory);
    return hResult;
}

HTASK
GetTaskFromInstance(HINSTANCE hInstance)
{
    LPMEMORYINFO lpMemory;
    HANDLE hTask;

    if (hInstance == 0)
	return GetCurrentTask();

#ifdef TWIN_BIN32
    if (HIWORD(hInstance)) {
	hInstance = GetInstanceFromModule32((void *) hInstance);
    }
#endif
    if (!(lpMemory = GETHANDLEINFO(hInstance)))
	return (HTASK)0;

    hTask = lpMemory->hTask;
    RELEASEHANDLEINFO(lpMemory);
    return hTask;
}

HINSTANCE
GetInstanceFromModule(HMODULE hModule)
{
    LPMODULEINFO lpModuleInfo;
    HANDLE hInst;
    if (hModule == 0)
	return (HINSTANCE)0;

#ifdef TWIN_BIN32
    if (HIWORD(hModule)) {
	return GetInstanceFromModule32((void *)hModule);
    }
#endif

    if (!(lpModuleInfo = GETMODULEINFO(hModule))) {
        LPHANDLEINFO lpHandleInfo = GETHANDLEINFO(hModule);
	BOOL bHandle = lpHandleInfo ? TRUE : FALSE;
	RELEASEHANDLEINFO(lpHandleInfo);
	if (!bHandle) {
	    return (HINSTANCE)NULL;
	}
	else
	    return (HINSTANCE)hModule;
    }
    
    hInst = lpModuleInfo->hInst;
    RELEASEMODULEINFO(lpModuleInfo);
    return hInst;
}

LPQUEUE
AllocQueue(int cMsg)
{
     LPQUEUE    lpQueue;
     LPQUEUEMSG lpQueueMsg;
     int nCnt;

    nCnt = sizeof(QUEUE);
    nCnt += sizeof(QUEUEMSG) * cMsg;

    if ((lpQueue = (LPQUEUE) WinMalloc(nCnt))) {

	    /* initialize queue flags and pointers */
	    lpQueue->wQueueFlags = 0;
	    lpQueue->lpQueueFree = (LPQUEUEMSG)(lpQueue+1);
	    lpQueue->lpQueueNext = 0;

	    /* initialize next pointers in the free list*/
	    for (nCnt = 0,lpQueueMsg = lpQueue->lpQueueFree;
			nCnt < (cMsg-1); 
			nCnt++, lpQueueMsg++)

		lpQueueMsg->lpNextMsg = lpQueueMsg + 1;

	    /* set the last one to NULL */
	    lpQueueMsg->lpNextMsg = 0;
    }
    
    return lpQueue;
}

BOOL WINAPI
GetInputState(void)
{
	LPQUEUE    lpQueue;
	lpQueue = SystemQueue;	

	return (lpQueue->lpQueueNext)?TRUE:FALSE;
}

DWORD WINAPI
GetQueueStatus(UINT fuFlags)
{
    APISTR((LF_APISTUB,"GetQueueStatus(UINT=%x)\n",fuFlags));
    return 0L;
}

/* we should use the HardwareEnabled flag when adding mouse or
 * key messages to the system queue. Currently it is ignored.
 */

static BOOL SystemQueueEnabled = TRUE;

BOOL WINAPI
EnableHardwareInput(BOOL fEnableInput)
{
	BOOL	last;
	last    	   = SystemQueueEnabled;
	SystemQueueEnabled = fEnableInput;
	return last;
}

/*
 * QueueAddMsg:
 *	message == 0	insert message at front 
 *	message == -1	append to message queue
 *	message == msg  append after message
 */

BOOL 
QueueAddMsg(HTASK hTask, WORD Insert, LPMSG lpMsg)
{
    LPTASKINFO lpTaskInfo;
    LPQUEUE    lpQueue;
    LPQUEUEMSG lpQueueMsg,lpQueuePtr;

    if(hTask) {
	if (!(lpTaskInfo = GETTASKINFO(hTask))) {
	    return FALSE;
	}
    	lpQueue = lpTaskInfo->lpQueue;
	RELEASETASKINFO(lpTaskInfo);
    } else  {
#ifdef LATER
	    /* check if system queue enabled   */
	    /* if(SystemQueueEnabled == FALSE) */
	    /*   return FALSE;	 	       */
#endif
	lpQueue = SystemQueue;	
    }
    if(lpQueue == 0) {
	return FALSE;
    }

#ifdef	LATER
    /* accelerate mouse moves... */
    if (lpMsg->message == WM_MOUSEMOVE) {
	if (MenuMouseMove(lpMsg))
	    return TRUE;
    }
#endif

    /* see if any free messages */
    if ((lpQueueMsg = lpQueue->lpQueueFree) == 0) {
	return FALSE;
    }

    /* copy the new message in */
    memcpy((LPSTR)&(lpQueueMsg->Message), (LPSTR)lpMsg, sizeof(MSG));
    if (lpQueueMsg->Message.message == WM_CONVERT)
	lpQueueMsg->wmc = *((WMCONVERT *)lpQueueMsg->Message.lParam);

    /* take message off the free list */
    lpQueue->lpQueueFree = lpQueueMsg->lpNextMsg;

    /* if no messages yet, then insert... */
    if(lpQueue->lpQueueNext == 0)
	Insert = 0;

    /* insert msg */
    if(Insert == 0) {
	/* set the next pointer of this message to the first message */
	lpQueueMsg->lpNextMsg = lpQueue->lpQueueNext;

	/* set the message queue to point to the new message */
	lpQueue->lpQueueNext = lpQueueMsg;
    } else {
	/* append msg to queue */
	/* find the message before this message */
	for(lpQueuePtr = lpQueue->lpQueueNext;
            lpQueuePtr->lpNextMsg;
	    lpQueuePtr = lpQueuePtr->lpNextMsg);
		
	/* set the last pointer to this one, and terminate this one */
	lpQueuePtr->lpNextMsg = lpQueueMsg;
	lpQueueMsg->lpNextMsg = 0;
    }
    return(TRUE);
}

BOOL 
QueueGetMsg(LPQUEUE lpQueue, LPMSG lpMsg, HWND hWnd,
		UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wFlags)
{

#define	POOLSIZE	16
#define	POOLMASK	0x000f

    LPQUEUEMSG lpQueueMsg, lpQueuePtr, lpQueueNext;
    MSG msg;
    int	   nmsgs;
    int	   rc;
    static WMCONVERT ConvPool[POOLSIZE];
    static int nPoolIndex = 0;
    extern BOOL IsMouseOrKeyboardMsg(UINT);
    UINT cFlags;

    if(lpQueue == 0) {
	    lpQueue = SystemQueue;
    }

    /* search for a message meeting our filters... */
    for(lpQueueMsg = lpQueue->lpQueueNext,nmsgs=0,rc=0;
	lpQueueMsg;
	lpQueueMsg = lpQueueNext) {

	cFlags = wFlags;

	lpQueueNext = (LPQUEUEMSG)(lpQueueMsg->lpNextMsg);

	msg = lpQueueMsg->Message;

	if(msg.hwnd && !IsWindow(msg.hwnd)) {
		/* we have an event for a non-existent window */
		/* don't even try to pass it on, instead flush */
		/* it from the queue, and get another... */
		cFlags |= PM_REMOVE;
	} else {

	if ((lpQueue == SystemQueue) &&
	    (IsMouseOrKeyboardMsg(msg.message) == WM_MOUSEFIRST))
	    if (!TWIN_ForwardButtonEvent(&msg))
		continue;

	if (wMsgFilterMin) {
	    if (msg.message < wMsgFilterMin)
		continue;
	}

	if (wMsgFilterMax) {
	     if (msg.message > wMsgFilterMax)
		continue;
	}

	if (hWnd == HWND_BROADCAST) {
		if (msg.hwnd)
			continue;

	} else if (hWnd) {
		if (msg.hwnd &&
		    msg.hwnd != hWnd) {
	            if (!IsChild(hWnd, msg.hwnd))
			continue;
		}
	}
	}

	if(msg.hwnd && !IsWindow(msg.hwnd))
	    rc++;

	else {
	    if (msg.hwnd &&
		GetWindowTask(msg.hwnd) != GetCurrentTask())
	    {
		ReadyTask(GetWindowTask(msg.hwnd));
		continue;
	    }
	    
	    if (msg.hwnd &&
			HaveDisabledAncestor(msg.hwnd) && 
			lpQueue == SystemQueue) {
		if ((rc == 0) &&
			(msg.message != WM_MOUSEMOVE) &&
			(msg.message != WM_NCMOUSEMOVE))
		    MessageBeep(0);
		rc++;
	    }
	    else {
		rc = 0;
		memcpy((LPSTR)lpMsg,(LPSTR)&(msg), sizeof(MSG));
		if (lpMsg->message == WM_CONVERT) {
		    memcpy((LPSTR)&ConvPool[nPoolIndex],
			(LPSTR)&(lpQueueMsg->wmc),
			sizeof(WMCONVERT));
		    lpMsg->lParam = (LPARAM)&ConvPool[nPoolIndex];
		    lpMsg->wParam |= HSWMC_ALLOC;
		    nPoolIndex = (nPoolIndex+1) & POOLMASK;
		}
	    }
	}

	/* are we removing it? */
	if (cFlags & PM_REMOVE) {
		/* is it the first one in the queue */
		if (lpQueueMsg == lpQueue->lpQueueNext)
		    lpQueue->lpQueueNext = lpQueueMsg->lpNextMsg;
		else {
		    /* find the message before this one */
		    for(lpQueuePtr = lpQueue->lpQueueNext;
			    lpQueuePtr->lpNextMsg != lpQueueMsg;
			    lpQueuePtr = lpQueuePtr->lpNextMsg);

		/* set the next message of our previous to our next msg */
		    lpQueuePtr->lpNextMsg = lpQueueMsg->lpNextMsg;
		}

		/* add this message to the free list */
		lpQueueMsg->lpNextMsg = lpQueue->lpQueueFree;
		lpQueue->lpQueueFree = lpQueueMsg;
	}
	if(rc == 0) return(TRUE);
    }
    return FALSE;
}

LPQUEUE
QueueGetPtr(HTASK hTask)
{
	LPTASKINFO lpTaskInfo;
	LPQUEUE lpQueue;

    	if (!(lpTaskInfo = GETTASKINFO(hTask))) {
	    return (LPQUEUE)NULL;
	}
	else {
	    lpQueue = lpTaskInfo->lpQueue;
	    RELEASETASKINFO(lpTaskInfo);
	    return lpQueue;
	}
}

void 
QueueSetFlags(HTASK hTask, WORD wFlags)
{
    LPTASKINFO lpTaskInfo;

    if (!(lpTaskInfo = GETTASKINFO(hTask)))
	return;

    lpTaskInfo->lpQueue->wQueueFlags |= wFlags;
    RELEASETASKINFO(lpTaskInfo);
}

void
QueueClearFlags(HTASK hTask, WORD wFlags)
{
    LPTASKINFO lpTaskInfo;

    if (!(lpTaskInfo = GETTASKINFO(hTask)))
	return;

    lpTaskInfo->lpQueue->wQueueFlags &= ~wFlags;
    RELEASETASKINFO(lpTaskInfo);
}

DWORD WINAPI
GetAppCompatFlags(HANDLE hTask)
{
	APISTR((LF_APISTUB,"GetAppCompatFlags(HANDLE=%x)\n",hTask));
	return 0;	/* Task module was built to target Windows 3.1 */
}

/* (WIN32) Kernel Error **************************************************** */

static DWORD dwLastError = 0;

DWORD	WINAPI
GetLastError(VOID)
{
	DWORD rc;
	APISTR((LF_APICALL, "GetLastError()\n"));

	rc = dwLastError;

	APISTR((LF_APIRET, "GetLastError: returns DWORD %x\n",rc));

	return rc;
}

VOID	WINAPI
SetLastError(DWORD dwError)
{
	APISTR((LF_APICALL, "SetLastError(DWORD=%x)\n",dwError));
	dwLastError = dwError;
	APISTR((LF_APIRET, "SetLastError: returns void\n"));
}

VOID	WINAPI
SetLastErrorEx(DWORD dwError, DWORD dwType)
{
	APISTR((LF_APICALL, "SetLastErrorEx(DWORD=%x,DWORD=%x)\n",
		dwError,dwType));

	dwLastError = dwError;

	APISTR((LF_APIRET, "SetLastErrorEx: returns void\n"));
}
