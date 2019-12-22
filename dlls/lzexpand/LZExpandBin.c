
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define NOAPIPROTO
#include "windows.h"

#include "WinDefs.h"
#include "kerndef.h"
#include "Endian.h"
#include "Log.h"
#include "Kernel.h"
#include "Resources.h"
#include "DPMI.h"
#include "BinTypes.h"
#include "ModTable.h"

 /* Entry Table LZEXPAND: */

extern long int Trap();
extern long int LZCopy();
extern long int LZOpenFile();
extern long int LZInit();
extern long int LZSeek();
extern long int LZRead();
extern long int LZClose();
extern long int LZStart();
extern long int CopyLZFile();
extern long int LZDone();
extern long int GetExpandedName();

#if defined (TWIN_BIN16)
/* Interface Segment Image LZEXPAND: */
extern long int IT_2H();
extern long int IT_OPENFILE();
extern long int IT_1H1L1I();
extern long int IT_LREADWR();
extern long int IT_1H();
extern long int IT_V();
extern long int IT_2LP();

static long int (*seg_image_LZEXPAND_0[])() =
{	/* nil */	0, 0,
	/* 001 */	IT_2H,		LZCopy,
	/* 002 */	IT_OPENFILE,	LZOpenFile,
	/* 003 */	IT_1H,		LZInit,
	/* 004 */	IT_1H1L1I,	LZSeek,
	/* 005 */	IT_LREADWR,	LZRead,
	/* 006 */	IT_1H,		LZClose,
	/* 007 */	IT_V,		LZStart,
	/* 008 */	IT_2H,		CopyLZFile,
	/* 009 */	IT_V,		LZDone,
	/* 00a */	IT_2LP,		GetExpandedName,
	0
};
#endif

 /* Segment Table LZEXPAND: */

SEGTAB SegmentTableLZEXPAND[] =
{	
#if defined(TWIN_BIN16)
	{ (char *) seg_image_LZEXPAND_0, 80, TRANSFER_CALLBACK, 80, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};
