/*    
	w32sys.c	1.0
    	Copyright 2000,2001 Willows Software, Inc. 

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

#include "windows.h"
#include "Log.h"
#include "peexe.h"

typedef struct
{
	BYTE bMajor;
	BYTE bMinor;
	WORD wBuildNumber;
	BOOL fDebug;
} WIN32SINFO, *LPWIN32SINFO;

WORD WINAPI
GetWin32sInfo(LPWIN32SINFO lpInfo)
{
	lpInfo->bMajor = 1;
	lpInfo->bMinor = 3;
	lpInfo->wBuildNumber = 0;
	lpInfo->fDebug = FALSE;
	return 0;
}

BOOL WINAPI
IsPeFormat(LPSTR fn, HFILE hf)
{
	BOOL ret = FALSE;
	DWORD xmagic;
	OFSTRUCT ofs;
	IMAGE_DOS_HEADER mzh;

	if (fn)
		hf = OpenFile(fn, &ofs, OF_READ);

	if (hf != HFILE_ERROR)
		{
		_lread(hf, &mzh, sizeof(mzh));
		if (mzh.e_magic == IMAGE_DOS_SIGNATURE)
			{
			_llseek(hf, mzh.e_lfanew, SEEK_SET);
			_lread(hf, &xmagic, sizeof(xmagic));
			if (xmagic == IMAGE_NT_SIGNATURE)
				ret = TRUE;
			}
		}

	return ret;
 
}

WORD WINAPI
GetW32SysVersion(void)
{
	return 0x100;
}

WORD WINAPI
GetPEResourceTable(HFILE hf)
{
	return 0;
}
