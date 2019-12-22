
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


extern long int Trap();
extern long int DdeInitialize();
extern long int DdeUninitialize();
extern long int DdePostAdvise();
extern long int DdeCreateStringHandle();
extern long int DdeNameService();
extern long int DdeKeepStringHandle();
extern long int DdeFreeStringHandle();
extern long int DdeEnableCallback();

#if defined(TWIN_BIN16)
extern long int IT_1D();
extern long int IT_1D1LP1I();
extern long int IT_3L1UI();
extern long int IT_DDEINIT();
extern long int IT_2D();
extern long int IT_3D();
extern long int IT_2D1UI();

 /* Interface Segment Image DDEML: */

static long int (*seg_image_DDEML_0[])() =
{	/* nil */	0, 0,
	/* 001 */	Trap, 0,
	/* 002 */	IT_DDEINIT, DdeInitialize,
	/* 003 */	IT_1D, DdeUninitialize,
	/* 004 */	Trap, 0,
	/* 005 */	Trap, 0,
	/* 006 */	Trap, 0,
	/* 007 */	Trap, 0,
	/* 008 */	Trap, 0,
	/* 009 */	Trap, 0,
	/* 00a */	Trap, 0,
	/* 00b */	Trap, 0,
	/* 00c */	Trap, 0,
	/* 00d */	IT_3D, DdePostAdvise,
	/* 00e */	Trap, 0,
	/* 00f */	Trap, 0,
	/* 010 */	Trap, 0,
	/* 011 */	Trap, 0,
	/* 012 */	Trap, 0,
	/* 013 */	Trap, 0,
	/* 014 */	Trap, 0,
	/* 015 */	IT_1D1LP1I, DdeCreateStringHandle,
	/* 016 */	IT_2D, DdeFreeStringHandle,
	/* 017 */	Trap, 0,
	/* 018 */	IT_2D, DdeKeepStringHandle,
	/* 019 */	Trap, 0,
	/* 01a */	IT_2D1UI, DdeEnableCallback,
	/* 01b */	IT_3L1UI, DdeNameService,
	/* 01c */	Trap, 0,
	/* 01d */	Trap, 0,
	/* 01e */	Trap, 0,
	/* 01f */	Trap, 0,
	/* 020 */	Trap, 0,
	/* 021 */	Trap, 0,
	/* 022 */	Trap, 0,
	/* 023 */	Trap, 0,
	/* 024 */	Trap, 0,
	/* 025 */	Trap, 0,
	0
};
#endif

 /* Segment Table DDEML: */

SEGTAB SegmentTableDDEML[] =
{	
#if defined(TWIN_BIN16)
	{ (char *) seg_image_DDEML_0, 296, TRANSFER_CALLBACK, 296, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};

