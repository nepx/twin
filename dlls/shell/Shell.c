/*    
	Shell.c	2.13
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

#include <string.h>

#include "windows.h"
#include "windowsx.h"
#define __SHELLAPI_H__

#include "Log.h"
#include "kerndef.h"
#include "KrnAtoms.h"

DWORD WINAPI 
SHGetFileInfo(LPCTSTR pszPath, DWORD dwFileAttributes,SHFILEINFO *psfi, UINT cbFileInfo,
    UINT uFlags)
{
	LOGSTR((LF_APISTUB,"SHGetFileInfo(%p,%x,%p,%d,%x)\n",
		pszPath,dwFileAttributes,psfi,cbFileInfo,uFlags));
	return 0;
}

typedef struct {
	int foo;
} SHELLFOLDER,*LPSHELLFOLDER;

DWORD WINAPI 
SHGetDesktopFolder(LPSHELLFOLDER FAR *psfi)
{
	LOGSTR((LF_APISTUB,"SHGetDesktopFolder(%p)\n",psfi));
	return 0;
}

void WINAPI 
Control_RunDLL( HWND hwnd, LPCVOID code, LPCSTR cmd, DWORD arg4 )
{
	LOGSTR((LF_APISTUB,"Control_RunDLL(%x,%p,%p,%x)\n",
		hwnd,code,cmd,arg4));
}

long int
ExtractAssociatedIcon(/*HWND hWnd, WORD wIndex*/)
{
	return 0;
}

HICON WINAPI
ExtractIcon(HINSTANCE hInst, LPCSTR lpszExeFileName, UINT nIconIndex)
{
    APISTR((LF_APISTUB,"ExtractIcon(HINSTANCE=%x,LPCSTR=%s,UINT=%x0\n",
	hInst,lpszExeFileName?lpszExeFileName:"NULL",nIconIndex));
    return (HICON)0;
}
#ifdef TWIN_BIN32
HINSTANCE WINAPI
ShellExecuteA(HWND hWnd, LPCSTR lpOperation, LPCSTR lpFile,
		LPCSTR lpParameters, LPCSTR lpDirectory, int nShowCmd)
{
	APISTR((LF_API,"ShellExecuteA(%x,%p,%s,%p,%p,%d)\n",
		hWnd, lpOperation, lpFile,
		lpParameters, lpDirectory, nShowCmd));
		
	return ShellExecute(hWnd, lpOperation, lpFile,
		lpParameters, lpDirectory, nShowCmd);
}

#endif

HINSTANCE WINAPI
ShellExecute(HWND hWnd, LPCSTR lpOperation, LPCSTR lpFile,
		LPCSTR lpParameters, LPCSTR lpDirectory, int nShowCmd)
{
    HINSTANCE hInst;
    APISTR((LF_APICALL,
      "ShellExecute(HWND=%x,LPCSTR=%p,LPCSTR=%s,LPCSTR=%p,LPCSTR=%p,int=%d)\n",
	hWnd,lpOperation,lpFile,lpParameters,lpDirectory,nShowCmd));
    hInst = (HINSTANCE)WinExec(lpFile,nShowCmd);
    APISTR((LF_APIRET,"ShellExecute: returns HINSTANCE %x\n",hInst));
    return hInst;
}

HINSTANCE WINAPI
FindExecutable(LPCSTR lpszFile, LPCSTR lpszDir, LPSTR lpResult)
{
    APISTR((LF_APISTUB,"FindExecutable(LPCSTR=%s,LPCSTR=%s,LPSTR=%x)\n",
	lpszFile,lpszDir,lpResult));
   /* later, add a search for the application to match file if not .exe */
    strcpy(lpResult,lpszFile);
    return (HINSTANCE) 33;
}

void WINAPI
RegisterShellHook(HWND hWnd, int foo)
{
    APISTR((LF_APISTUB,"RegisterShellHook(HWND=%x,int=%d)\n",hWnd,foo));
}

/* (WIN32) File Security *************************************************** */
   
BOOL    WINAPI
GetFileSecurity(LPCSTR lpszFile,
        SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd,
        DWORD dw, LPDWORD lp)
{
        APISTR((LF_APISTUB, "GetFileSecurity(LPCSTR=%s,...)\n", lpszFile));
        return (TRUE);
}       
   
BOOL    WINAPI
SetFileSecurity(LPCSTR lpszFile,
        SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd)
{   
        APISTR((LF_APISTUB, "SetFileSecurity(LPCSTR=%s,...)\n", lpszFile));
        return (TRUE);
}
