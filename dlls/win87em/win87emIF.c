#define NOAPIPROTO
#include "windows.h"
#include "Resources.h"
#include "ModTable.h"
#include "Segment.h"
#include "Log.h"

#if defined(TWIN_BIN16)
extern long int _86_Win87Em();

 /* Interface Segment Image WIN87EM: */

static long int (*seg_image_WIN87EM_0[])() =
{	/* nil */	0, 0,
	/* 001 */	_86_Win87Em, 0,
	0
};
#endif

 /* Segment Table WIN87EM: */

SEGTAB SegmentTableWIN87EM[] =
{
#if defined(TWIN_BIN16)
	{ (char *) seg_image_WIN87EM_0, 8, TRANSFER_CALLBACK, 8, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};

