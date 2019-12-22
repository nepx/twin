
#define NOAPIPROTO
#include "windows.h"
#include "Resources.h"
#include "ModTable.h"
#include "Segment.h"
#include "Log.h"

extern long int InquireSystem();
extern long int Get80x87SaveSize();
extern long int CreateSystemTimer();
extern long int KillSystemTimer();
extern long int EnableSystemTimers();
extern long int DisableSystemTimers();

#if defined(TWIN_BIN16)
extern long int IT_INQSYSTEM();
extern long int IT_V();
extern long int IT_1H();
extern long int IT_1I1D();
extern long int Trap();

 /* Interface Segment Image SYSTEM: */

static long int (*seg_image_SYSTEM_0[])() =
{	/* nil */	0, 0,
	/* 001 */	IT_INQSYSTEM,  InquireSystem,
	/* 002 */	IT_1I1D, CreateSystemTimer,
	/* 003 */	IT_1H, KillSystemTimer,
	/* 004 */	IT_V, EnableSystemTimers,
	/* 005 */	IT_V, DisableSystemTimers,
	/* 006 */	Trap, 0,
	/* 007 */	IT_V,  Get80x87SaveSize,
	0
};
#endif

 /* Segment Table SYSTEM: */

SEGTAB SegmentTableSYSTEM[] =
{	
#if defined(TWIN_BIN16)
	{ (char *) seg_image_SYSTEM_0, 56, TRANSFER_CALLBACK, 56, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};
