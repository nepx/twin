/*    
	CreateProcess.c	1.2
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
#include "Resources.h"
#include "Module.h"
#include "KrnTask.h"
#include "Kernel.h"
#include "Log.h"

VOID WINAPI
ExitProcess(UINT uExitCode)
{
	HINSTANCE    hInst;
	HMODULE      hModule;
	MODULEINFO   *modinfo;

	APISTR((LF_API,"ExitProcess(%d)\n",uExitCode));

	hInst = TWIN_GetInstanceFromTask( GetCurrentTask());
	hModule = GetModuleFromInstance(hInst);
	modinfo = GETMODULEINFO(hModule);

	modinfo->exit_code = uExitCode;
	/* this is not protected, but I'm assuming */
	/* that we are just exiting, so it should  */
	/* be alright */
	longjmp(modinfo->env,1);
}

BOOL WINAPI
CreateProcess
(
  LPCTSTR  lpApplicationName,	/* pointer to name of executable module */
  LPTSTR  lpCommandLine,	/* pointer to command line string */
  LPSECURITY_ATTRIBUTES  lpProcessAttributes,	/* pointer to process security attributes */
  LPSECURITY_ATTRIBUTES  lpThreadAttributes,	/* pointer to thread security attributes */
  BOOL  bInheritHandles,	/* handle inheritance flag */
  DWORD  dwCreationFlags,	/* creation flags */ 
  LPVOID  lpEnvironment,	/* pointer to new environment block */
  LPCTSTR  lpCurrentDirectory,	/* pointer to current directory name */ 
  LPSTARTUPINFO  lpStartupInfo,	/* pointer to STARTUPINFO */ 
  LPPROCESS_INFORMATION  lpProcessInformation 	/* pointer to PROCESS_INFORMATION */
)
{
  APISTR((LF_APISTUB,"CreateProcess()\n"));
  return( FALSE );
}

LPTSTR WINAPI
GetCommandLine(VOID)
{
	LPMODULEINFO modinfo;
	HMODULE      hModule;
	HINSTANCE    hInst;
	char *lpstr;

	APISTR((LF_APICALL,"GetCommandLine()\n"));

	hInst = TWIN_GetInstanceFromTask(GetCurrentTask());
	hModule = GetModuleFromInstance(hInst);	
	modinfo = GETMODULEINFO(hModule);
	
	lpstr = strchr(modinfo->lpFileName,' ');
	if(lpstr) lpstr++;
	else      lpstr = modinfo->lpFileName;

	RELEASEMODULEINFO(modinfo);
	APISTR((LF_APIRET,"GetCommandLine: returns %p\n",lpstr));
	return lpstr;
}

LPVOID WINAPI
GetEnvironmentStrings(VOID)
{
	LPTSTR penv;
	APISTR((LF_APICALL,"GetEnvironmentsStrings()\n"));
	penv = (LPTSTR)  WinMalloc(64*1024);
	/*BuildEnvBuffer(GetCommandLine(),penv);*/
	penv[0] = 0;
	penv[1] = 0;
	APISTR((LF_APIRET,"GetEnvironmentsStrings: return %p\n",penv));
	return penv;
}

LPTSTR
GetEnvironmentStringsW(VOID)
{
	return GetEnvironmentStrings();
}

BOOL WINAPI
FreeEnvironmentStrings(LPTSTR lpszEnvBlock)
{
	APISTR((LF_APICALL,"FreeEnvironmentStrings(%p)\n",lpszEnvBlock));
	WinFree(lpszEnvBlock);
	APISTR((LF_APIRET,"FreeEnvironmentStrings: returns TRUE\n"));
	return TRUE;
}

BOOL WINAPI
FreeEnvironmentStringsW(LPTSTR lpszEnvBlock)
{
	return FreeEnvironmentStrings(lpszEnvBlock);
}

