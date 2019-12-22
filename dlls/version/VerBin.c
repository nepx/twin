/*    
	VerBin.c	1.0
    	Copyright 1999 Willows Software, Inc. 

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
#include "Resources.h"
#include "ModTable.h"
#include "Segment.h"
#include "Log.h"

extern long int Trap();

extern long int GetFileResourceSize();
extern long int GetFileResource();
extern long int GetFileVersionInfoSize();
extern long int GetFileVersionInfo();
extern long int VerFindFile();
extern long int VerInstallFile();
extern long int VerLanguageName();
extern long int VerQueryValue();

 /* Interface Segment Image VER: */
#if defined(TWIN_BIN16)
extern long int IT_GETFILERESOURCESIZE();
extern long int IT_GETFILERESOURCE();
extern long int IT_GETFILEVERSIONINFOSIZE();
extern long int IT_GETFILEVERSIONINFO();
extern long int IT_1UI7LP();
extern long int IT_VERLANGUAGENAME();
extern long int IT_VERQUERYVALUE();

static long int (*seg_image_VER_0[])() =
{	/* nil */	0, 0,
	/* 001 */	Trap, 0,
	/* 002 */	IT_GETFILERESOURCESIZE,		GetFileResourceSize,
	/* 003 */	IT_GETFILERESOURCE,		GetFileResource,
	/* 004 */	Trap, 0,
	/* 005 */	Trap, 0,
	/* 006 */	IT_GETFILEVERSIONINFOSIZE,	GetFileVersionInfoSize,
	/* 007 */	IT_GETFILEVERSIONINFO,		GetFileVersionInfo,
	/* 008 */	IT_1UI7LP,			VerFindFile,
	/* 009 */	IT_1UI7LP,			VerInstallFile,
	/* 00a */	IT_VERLANGUAGENAME,		VerLanguageName,
	/* 00b */	IT_VERQUERYVALUE,		VerQueryValue,
	0
};
#endif

 /* Segment Table VER: */

SEGTAB SegmentTableVERSION[] =
{	
#if defined(TWIN_BIN16)
	{ (char *) seg_image_VER_0, 88, TRANSFER_CALLBACK, 88, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};

