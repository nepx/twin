/*    
	w32sysBin.c	1.0
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

#define NOAPIPROTO
#include "windows.h"
#include "Endian.h"
#include "Resources.h"
#include "DPMI.h"
#include "ModTable.h"
#include "Segment.h"
#include "Log.h"

extern long int IsPeFormat();
extern long int GetW32SysVersion();
extern long int GetPEResourceTable();
extern long int GetWin32sInfo();

 /* Interface Segment Image W32SYS: */
#if defined(TWIN_BIN16)
extern long int Trap();
extern long int IT_V();
extern long int IT_1H();

typedef struct
{
	BYTE bMajor;
	BYTE bMinor;
	WORD wBuildNumber;
	BOOL fDebug;
} WIN32SINFO, *LPWIN32SINFO;

long int
IT_GETW32SINFO(ENV *envp,LONGPROC f)
	{
	DWORD retcode;
	WIN32SINFO w32si;
	LPBYTE lpStruct;

	lpStruct = (LPBYTE)GetAddress(GETWORD(SP+6),GETWORD(SP+4));
	retcode = GetWin32sInfo(&w32si);
	*(lpStruct+0) = w32si.bMajor;
	*(lpStruct+1) = w32si.bMinor;
	PUTWORD(lpStruct+2,w32si.wBuildNumber);
	PUTDWORD(lpStruct+4,w32si.fDebug);
	envp->reg.sp += LP_86 + RET_86;
	envp->reg.ax = LOWORD(retcode);
	envp->reg.dx = HIWORD(retcode);
	return retcode;
	}

long int
IT_ISPEFMT(ENV *envp,LONGPROC f)
	{
	DWORD retcode;
	HFILE hf;
	LPSTR fn;

	hf = (HFILE)(UINT)GETWORD(SP+4);
	fn = (LPSTR)GetAddress(GETWORD(SP+8),GETWORD(SP+6));
	retcode = IsPeFormat(fn, hf);
	envp->reg.sp += HANDLE_86 + LP_86 + RET_86;
	envp->reg.ax = LOWORD(retcode);
	envp->reg.dx = HIWORD(retcode);
	return retcode;
	}

static long int (*seg_image_W32SYS_0[])() =
{	/* nil */	0, 0,
	/* 001 */	Trap, 0,
	/* 002 */	IT_ISPEFMT, IsPeFormat,
	/* 003 */	Trap, 0,
	/* 004 */	Trap, 0,
	/* 005 */	IT_V, GetW32SysVersion,
	/* 006 */	Trap, 0,
	/* 007 */	IT_1H, GetPEResourceTable,
	/* 008 */	Trap, 0,
	/* 009 */	Trap, 0,
	/* 00a */	Trap, 0,
	/* 00b */	Trap, 0,
	/* 00c */	IT_GETW32SINFO, GetWin32sInfo,
	0
};
#endif

 /* Segment Table W32SYS: */

SEGTAB SegmentTableW32SYS[] =
{
#if defined(TWIN_BIN16)
	{ (char *) seg_image_W32SYS_0, 88, TRANSFER_CALLBACK, 88, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};
