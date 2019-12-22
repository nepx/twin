/*    
	System.c	2.41
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

********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include "windows.h"
#include "windowsx.h"
#include "kerndef.h"
#include "Log.h"

#define PUBLIC
#define PRIVATE	static

#define DOSMINOR	0
#define DOSMAJOR	5
#define	DOSVERSION()	(DOSMAJOR<<8|DOSMINOR)
#define WINMINOR	10
#define WINMAJOR	3
#define	WINVERSION()	(WINMINOR<<8|WINMAJOR)

DWORD
InquireSystem(WORD wFlag, WORD wDriveNumber, BOOL bOptionalEnable)
{
    APISTR((LF_APISTUB,"InquireSystem(WORD=%x,WORD=%x,BOOL=%d)\n",
	wFlag,wDriveNumber,bOptionalEnable));
    return 0L;
}

WORD
Get80x87SaveSize(void)
{
    APISTR((LF_APISTUB,"Get80x87SaveSize()\n"));
#ifndef	NOFPU87
    return 0x5e;
#else
    return 0;
#endif
}

/* these are undocumented APIs */

WORD CreateSystemTimer(int, FARPROC);
WORD KillSystemTimer(WORD);
void EnableSystemTimers(void);
void DisableSystemTimers(void);

WORD
CreateSystemTimer(int nRate, FARPROC lpCallBack)
{
    APISTR((LF_APISTUB,
	"CreateSystemTimer(int=%x,FARPROC=%x)\n",
	nRate,lpCallBack));
    return 1;
}

WORD
KillSystemTimer(WORD wTimer)
{
    APISTR((LF_APISTUB,"KillSystemTimer(WORD=%x)\n",wTimer));
    return wTimer;
}

void
EnableSystemTimers(void)
{
    APISTR((LF_APISTUB,"EnableSystemTimers()\n"));
}

void
DisableSystemTimers(void)
{
    APISTR((LF_APISTUB,"DisableSystemTimers()\n"));
}
