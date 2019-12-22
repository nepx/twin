

#define NOAPIPROTO
#include "windows.h"
#include "Resources.h"
#include "ModTable.h"
#include "Segment.h"
#include "Log.h"

extern long int Trap();

extern long int OleRegisterClientDoc();
extern long int OleRenameClientDoc();
extern long int OleRevokeClientDoc();
extern long int OleRevertClientDoc();
extern long int OleSavedClientDoc();
extern long int OleQueryClientVersion();
extern long int OleQueryLinkFromClip();
extern long int OleQueryCreateFromClip();
extern long int OleLoadFromStream();
extern long int OleEnumObjects();

#if defined(TWIN_BIN16)
extern long int IT_V();
extern long int IT_REGCLIENTDOC();
extern long int IT_1LP1I1W();
extern long int IT_1D1LP();
extern long int IT_1D();
extern long int IT_OLELOADFROMSTREAM();
extern long int IT_OLEENUMOBJ();


 /* Interface Segment Image OLECLI: */

static long int (*seg_image_OLECLI_0[])() =
{	/* nil */	0, 0,
	/* 001 */	Trap, 0,
	/* 002 */	Trap, 0,
	/* 003 */	Trap, 0,
	/* 004 */	IT_OLELOADFROMSTREAM, OleLoadFromStream,
	/* 005 */	Trap, 0,
	/* 006 */	Trap, 0,
	/* 007 */	Trap, 0,
	/* 008 */	Trap, 0,
	/* 009 */	IT_1LP1I1W, OleQueryLinkFromClip,
	/* 00a */	IT_1LP1I1W, OleQueryCreateFromClip,
	/* 00b */	Trap, 0,
	/* 00c */	Trap, 0,
	/* 00d */	Trap, 0,
	/* 00e */	Trap, 0,
	/* 00f */	Trap, 0,
	/* 010 */	Trap, 0,
	/* 011 */	Trap, 0,
	/* 012 */	Trap, 0,
	/* 013 */	Trap, 0,
	/* 014 */	Trap, 0,
	/* 015 */	Trap, 0,
	/* 016 */	Trap, 0,
	/* 017 */	Trap, 0,
	/* 018 */	Trap, 0,
	/* 019 */	Trap, 0,
	/* 01a */	Trap, 0,
	/* 01b */	Trap, 0,
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
	/* 026 */	Trap, 0,
	/* 027 */	Trap, 0,
	/* 028 */	Trap, 0,
	/* 029 */	IT_REGCLIENTDOC,  OleRegisterClientDoc,
	/* 02a */	IT_1D, OleRevokeClientDoc,
	/* 02b */	IT_1D1LP, OleRenameClientDoc,
	/* 02c */	IT_1D, OleRevertClientDoc,
	/* 02d */	IT_1D, OleSavedClientDoc,
	/* 02e */	Trap, 0,
	/* 02f */	IT_OLEENUMOBJ, OleEnumObjects,
	/* 030 */	Trap, 0,
	/* 031 */	Trap, 0,
	/* 032 */	Trap, 0,
	/* 033 */	Trap, 0,
	/* 034 */	Trap, 0,
	/* 035 */	Trap, 0,
	/* 036 */	Trap, 0,
	/* 037 */	Trap, 0,
	/* 038 */	Trap, 0,
	/* 039 */	Trap, 0,
	/* 03a */	Trap, 0,
	/* 03b */	IT_V, OleQueryClientVersion,
	/* 03c */	Trap, 0,
	0
};
#endif

 /* Segment Table OLECLI: */

SEGTAB SegmentTableOLECLI[] =
{	
#if defined(TWIN_BIN16)
	{ (char *) seg_image_OLECLI_0, 480, TRANSFER_CALLBACK, 480, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};

