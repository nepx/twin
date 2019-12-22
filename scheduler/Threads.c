/*    
	Threads.c	1.3
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
#include "windowsx.h"
#include "Log.h"

#include "kerndef.h"
#include "Resources.h"
#include "Module.h"
#include "Kernel.h"
#include "Log.h"


/* (WIN32) Threads ********************************************************* */

BOOL WINAPI EnumThreadWindows(DWORD dwThreadId, WNDENUMPROC lpfn, 
			      LPARAM lParam)
{
    return EnumTaskWindows((HTASK) dwThreadId, lpfn, lParam);
}

DWORD WINAPI GetCurrentProcessId(void)
{
    return (DWORD) GetCurrentTask();
}

DWORD WINAPI GetWindowThreadProcessId(HWND hWnd, LPDWORD lpdwProcessId)
{
    HTASK hTask;
    
    hTask = GetWindowTask(hWnd);
    if (lpdwProcessId)
	*lpdwProcessId = (DWORD) hTask;
    
    return (DWORD) hTask;
}

BOOL WINAPI PostThreadMessage(DWORD idThread, UINT Msg, 
			      WPARAM wParam, LPARAM lParam)
{
    return FALSE;
}

/*************************************************************/

int WINAPI
GetThreadPriority(HANDLE hThread)
{
	APISTR((LF_APISTUB, "GetThreadPriority()\n"));
	return 0;
}

BOOL WINAPI
SetThreadPriority(HANDLE hThread,int priority)
{
	APISTR((LF_APISTUB, "SetThreadPriority()\n"));
	return TRUE;
}

BOOL WINAPI
SendNotifyMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	APISTR((LF_APISTUB, "SendNotifyMessage()\n"));
	return TRUE;
}

DWORD WINAPI
ResumeThread(HANDLE hThread)
{
	APISTR((LF_APISTUB, "ResumeThread()\n"));
	return 0;
}

DWORD WINAPI
GetCurrentThreadId()
{
    LPTASKINFO lpTaskInfo;
    HTASK hTask = GetCurrentTask();

    APISTR((LF_APICALL, "GetCurrentThreadId()\n"));
    if ((lpTaskInfo = GETTASKINFO(hTask))) {
        APISTR((LF_APIRET, "GetCurrentThreadId returns DWORD %x\n",
		lpTaskInfo->ThreadData));
    	return lpTaskInfo->ThreadData;
    }

    APISTR((LF_APIRET, "GetCurrentThreadId returns DWORD 0\n"));
    return 0;
}

HANDLE 
GetCurrentThread()
{
	APISTR((LF_APISTUB, "GetCurrentThread()\n"));
	return ((HANDLE)1);
}

DWORD GetProcessVersion(UINT x)
{
	APISTR((LF_APISTUB, "GetProcessVersion()\n"));
	return 0;
}

LCID WINAPI
GetThreadLocale(void)
{
	APISTR((LF_APISTUB, "GetThreadLocale()\n"));
	return 0;
}

DWORD
SuspendThread(HANDLE hThread)
{
	APISTR((LF_APISTUB, "SuspendThread()\n"));
	return 0;
}

/**************************************************************************/
