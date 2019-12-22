/*    
	Critical.c	1.1
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
#include "stdio.h"
#include "Log.h"

/*
 * WARNING: These functions are NOT provided as functional and not thread-safe!
 */

VOID WINAPI
InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	printf("initialize critical section %p\n",lpCriticalSection);
}

VOID WINAPI
DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	printf("delete critical section %p\n",lpCriticalSection);
}

VOID WINAPI
EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	printf("enter critical section %p\n",lpCriticalSection);
}

VOID WINAPI
LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	printf("leave critical section %p\n",lpCriticalSection);
}

BOOL WINAPI
TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	printf("try enter critical section %p\n",lpCriticalSection);
    return TRUE;
}


typedef struct _tagMUTEX {
	struct _tagMUTEX *next;
	SECURITY_ATTRIBUTES security;
	DWORD             access;
	BOOL		  inherit;
	char		 *name;	
	HANDLE		  hMutex;
} MUTEX;

/* (WIN32) Mutex *********************************************************** */

#ifdef TWIN_BIN32
static MUTEX *lpMUTEX;

HANDLE
CreateMutexA(LPSECURITY_ATTRIBUTES *lpsec,BOOL bInherit,LPCSTR name)
{
	printf("CreateMutex()\n");
	return 0;
}

HANDLE
OpenMutexA(DWORD access,BOOL binherit,LPCSTR lpcstr)
{
	//HANDLE hMutex;
	MUTEX *lpMutex;

	//hMem = ResourceAlloc((LPSTR)p);
	for(lpMutex = lpMUTEX;lpMutex;lpMutex = lpMutex->next) {
		if(strcmp(lpMutex->name,lpcstr) == 0)
			return lpMutex->hMutex;
	}	
	return (HANDLE) 0;
}
#endif

HANDLE	WINAPI
CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes,
	BOOL bInitialOwner,
	LPCSTR lpName)
{
	APISTR((LF_API, "CreateMutex: (API) %s\n", lpName));
	return ((HANDLE) 0);
}

HANDLE	WINAPI
OpenMutex(DWORD dwDesiredAccess,
	BOOL bInheritHandle,
	LPCSTR lpName)
{
	APISTR((LF_API, "OpenMutex: (API) %s\n", lpName));
	return ((HANDLE) 0);
}

BOOL	WINAPI
ReleaseMutex(HANDLE hMutex)
{
	APISTR((LF_API, "ReleaseMutex(HANDLE=%x)\n", hMutex));
	return (TRUE);
}

BOOL	WINAPI
DisableThreadLibraryCalls(HMODULE hModule)
{
	APISTR((LF_API, "DisableThreadLibraryCalls(HMODULE=%x)\n", hModule));
	return TRUE;
}
