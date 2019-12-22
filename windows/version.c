/*    
	version.c	2.9
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
#include "version.h"
#include "Log.h"

char *GetTwinFilename();

#define DOSMINOR	0
#define DOSMAJOR	5
#define	DOSVERSION()	(DOSMAJOR<<8|DOSMINOR)
#define WINMINOR	10
#define WINMAJOR	3
#define	WINVERSION()	(WINMINOR<<8|WINMAJOR)

DWORD WINAPI
GetVersion(void)
{
    UINT res;
    DWORD rc;
    char ver[15], progname[40];
    LPSTR cmdline, p;
    WORD version;

    APISTR((LF_APICALL,"GetVersion()\n"));

    /* Look for the version through the program name, or default
     * to 'version'.  We remove the parameters and path from the
     * command line, if they exist.  But we leave the extension
     * in case there are two programs with names that differ only
     * in the extension. */
    cmdline = GetCommandLine();
    p = strrchr(cmdline, ' ');
    if (p)
	*p = '\0';
    p = strrchr(cmdline, '\\');
    if (p)
	cmdline = p + 1;
    p = strrchr(cmdline, '/');
    if (p)
	cmdline = p + 1;
    wsprintf(progname, "version\\%s", cmdline);

#ifdef TWIN32
    rc = MAKELONG(MAKEWORD(4,0),VER_BUILD);
#else
    rc = MAKELONG(WINVERSION(),DOSVERSION());
#endif
    /* Get the version value from the above result. */
    res = GetPrivateProfileString("version",progname,"",ver,sizeof(ver),
		GetTwinFilename());
    if (res == 0) {
	res = GetPrivateProfileString("version","version","",ver,sizeof(ver),
		GetTwinFilename());
	}

    if (res) {
	version = (WORD)GetPrivateProfileInt("version",ver,rc,
		GetTwinFilename());

	if (strcmp(ver, "win 3.1") == 0)
	    rc = MAKELONG(version,DOSVERSION());
	else
 	    rc = MAKELONG(version,VER_BUILD);
    }

    APISTR((LF_APIRET,"GetVersion: returns DWORD %x\n",rc));
    return rc; 
}

BOOL	WINAPI
GetVersionEx(LPOSVERSIONINFO lpOSVersionInfo)
{
	DWORD ver;

	APISTR((LF_API, "GetVersionEx:"
		" (API) lpOSVersionInfo %p\n",
		lpOSVersionInfo));

	if (!lpOSVersionInfo)
		return (FALSE);
	ver = GetVersion();
	lpOSVersionInfo->dwMajorVersion = LOBYTE(LOWORD(ver));
	lpOSVersionInfo->dwMinorVersion = HIBYTE(LOWORD(ver));
	lpOSVersionInfo->dwBuildNumber =  HIWORD(ver);
	lpOSVersionInfo->dwPlatformId = VER_PLATFORM_WIN32_NT;
	strcpy(lpOSVersionInfo->szCSDVersion, VER_CSDVERSION);

	return (TRUE);
}
