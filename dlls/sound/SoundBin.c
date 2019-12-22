
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

extern long int OpenSound();
extern long int CloseSound();
extern long int SetVoiceQueueSize();
extern long int SetVoiceNote();
extern long int SetVoiceAccent();
extern long int StartSound();
extern long int StopSound();

#if defined(TWIN_BIN16)
extern long int Trap();
extern long int IT_V();
extern long int IT_V();
extern long int IT_2I();
extern long int IT_4I();
extern long int IT_5I();
extern long int IT_V();
extern long int IT_V();


 /* Interface Segment Image SOUND: */

static long int (*seg_image_SOUND_0[])() =
{	/* nil */	0, 0,
	/* 001 */	IT_V,  OpenSound,
	/* 002 */	IT_V,  CloseSound,
	/* 003 */	IT_2I,  SetVoiceQueueSize,
	/* 004 */	IT_4I,  SetVoiceNote,
	/* 005 */	IT_5I, SetVoiceAccent,
	/* 006 */	Trap, 0,
	/* 007 */	Trap, 0,
	/* 008 */	Trap, 0,
	/* 009 */	IT_V,  StartSound,
	/* 00a */	IT_V,  StopSound,
	/* 00b */	Trap, 0,
	/* 00c */	Trap, 0,
	/* 00d */	Trap, 0,
	/* 00e */	Trap, 0,
	/* 00f */	Trap, 0,
	/* 010 */	Trap, 0,
	0
};
#endif

 /* Segment Table SOUND: */

SEGTAB SegmentTableSOUND[] =
{	
#if defined(TWIN_BIN16)
	{ (char *) seg_image_SOUND_0, 128, TRANSFER_CALLBACK, 128, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};

