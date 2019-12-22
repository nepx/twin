
#define NOAPIPROTO
#include "windows.h"
#include "Resources.h"
#include "ModTable.h"
#include "Segment.h"
#include "Log.h"

extern long int Trap();

extern long int OleRegisterServer();
extern long int OleRevokeServer();
extern long int OleBlockServer();
extern long int OleUnblockServer();
extern long int OleRegisterServerDoc();
extern long int OleRevokeServerDoc();
extern long int OleRenameServerDoc();

#if defined(TWIN_BIN16)
extern long int IT_REGSERVER();
extern long int IT_REGSERVERDOC();
extern long int IT_1D();
extern long int IT_1D1LP();
extern long int IT_UNBLOCKSERVER();


 /* Interface Segment Image OLESVR: */

static long int (*seg_image_OLESVR_0[])() =
{	/* nil */	0, 0,
	/* 001 */	Trap, 0,
	/* 002 */	IT_REGSERVER,  OleRegisterServer,
	/* 003 */	IT_1D, OleRevokeServer,
	/* 004 */	IT_1D,  OleBlockServer,
	/* 005 */	IT_UNBLOCKSERVER, OleUnblockServer,
	/* 006 */	IT_REGSERVERDOC, OleRegisterServerDoc,
	/* 007 */	IT_1D, OleRevokeServerDoc,
	/* 008 */	IT_1D1LP, OleRenameServerDoc,
	/* 009 */	Trap, 0,
	/* 00a */	Trap, 0,
	/* 00b */	Trap, 0,
	/* 00c */	Trap, 0,
	0
};
#endif

 /* Segment Table OLESVR: */

SEGTAB SegmentTableOLESVR[] =
{	
#if defined(TWIN_BIN16)
	{ (char *) seg_image_OLESVR_0, 96, TRANSFER_CALLBACK, 96, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};
