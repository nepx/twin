
#define NOAPIPROTO
#include "windows.h"
#include "Resources.h"
#include "ModTable.h"
#include "Segment.h"
#include "Log.h"

#if defined(TWIN_BIN16)
extern long int Trap();

/* Interface Segment Image COMM: */

extern long int IT_SETQUE();
extern long int IT_SETCOM();
extern long int IT_1I1LPV1I();
extern long int IT_1LPDCB();
extern long int IT_1I();
extern long int IT_V();
extern long int IT_2I();
extern long int IT_1I1LPCOMSTAT();
extern long int IT_1ILPV1I();
extern long int IT_1I1H2I();

extern 	long	inicom();
extern 	long	setcom();
extern 	long	setque();
extern 	long    reccom();
extern 	long	sndcom();
extern 	long	ctx();
extern 	long	trmcom();
extern 	long	stacom();
extern 	long	cextfcn();
extern 	long	cflush();
extern 	long	cevt();
extern 	long	cevtGet();
extern 	long	csetbrk();
extern 	long	cclrbrk();
extern 	long	getdcb();
extern 	long	SuspendOpenCommPorts();
extern 	long	ReactivateOpenCommPorts();
extern 	long	CommWriteString();
extern 	long	ReadCommString();
extern 	long	EnableNotification();

static long int (*seg_image_COMM_0[])() = 
{
	/* 000 */	Trap, 0,
	/* 001 */	IT_1LPDCB, inicom,
	/* 002 */	IT_1LPDCB, setcom,
	/* 003 */	IT_SETQUE, setque,
	/* 004 */	IT_1I, reccom,
	/* 005 */	IT_2I, sndcom,
	/* 006 */	IT_2I, ctx,
	/* 007 */	IT_1I, trmcom,
	/* 008 */	IT_1I1LPCOMSTAT, stacom,
	/* 009 */	IT_2I, cextfcn,
	/* 00a */	IT_2I, cflush,
	/* 00b */	IT_2I, cevt,
	/* 00c */	IT_2I, cevtGet,
	/* 00d */	IT_1I1LPV1I, CommWriteString,
	/* 00e */	IT_1I, cclrbrk,
	/* 00f */	IT_1I, getdcb,
	/* 010 */	Trap, 0,
	/* 011 */	IT_V, SuspendOpenCommPorts,
	/* 012 */	IT_V, ReactivateOpenCommPorts,
	/* 013 */	IT_1I1LPV1I, CommWriteString,
	/* 014 */	Trap, ReadCommString,
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
	/* 029 */	Trap, 0,
	/* 02a */	Trap, 0,
	/* 02b */	Trap, 0,
	/* 02c */	Trap, 0,
	/* 02d */	Trap, 0,
	/* 02e */	Trap, 0,
	/* 02f */	Trap, 0,
	/* 030 */	Trap, 0,
	/* 031 */	Trap, 0,
	/* 032 */	Trap, 0,
	/* 033 */	Trap, 0,
	/* 034 */	Trap, 0,
	/* 034 */	Trap, 0,
	/* 036 */	Trap, 0,
	/* 037 */	Trap, 0,
	/* 038 */	Trap, 0,
	/* 039 */	Trap, 0,
	/* 03a */	Trap, 0,
	/* 03b */	Trap, 0,
	/* 03c */	Trap, 0,
	/* 03d */	Trap, 0,
	/* 03e */	Trap, 0,
	/* 03f */	Trap, 0,
	/* 040 */	Trap, 0,
	/* 041 */	Trap, 0,
	/* 042 */	Trap, 0,
	/* 043 */	Trap, 0,
	/* 044 */	Trap, 0,
	/* 045 */	Trap, 0,
	/* 046 */	Trap, 0,
	/* 047 */	Trap, 0,
	/* 048 */	Trap, 0,
	/* 049 */	Trap, 0,
	/* 04a */	Trap, 0,
	/* 04b */	Trap, 0,
	/* 04c */	Trap, 0,
	/* 04d */	Trap, 0,
	/* 04e */	Trap, 0,
	/* 04f */	Trap, 0,
	/* 050 */	Trap, 0,
	/* 051 */	Trap, 0,
	/* 052 */	Trap, 0,
	/* 053 */	Trap, 0,
	/* 054 */	Trap, 0,
	/* 055 */	Trap, 0,
	/* 056 */	Trap, 0,
	/* 057 */	Trap, 0,
	/* 058 */	Trap, 0,
	/* 059 */	Trap, 0,
	/* 05a */	Trap, 0,
	/* 05b */	Trap, 0,
	/* 05c */	Trap, 0,
	/* 05d */	Trap, 0,
	/* 05e */	Trap, 0,
	/* 05f */	Trap, 0,
	/* 060 */	Trap, 0,
	/* 061 */	Trap, 0,
	/* 062 */	Trap, 0,
	/* 063 */	Trap, 0,
	/* 064 */	IT_1I1H2I, EnableNotification,
	0
};
#endif

 /* Segment table COMM: */

SEGTAB SegmentTableCOMM[] = {
#if defined(TWIN_BIN16)
	{ (char*)seg_image_COMM_0, 800, TRANSFER_CALLBACK, 800, 0, 0 },
#endif
	/* end */       { 0, 0, 0, 0, 0, 0 }
};

