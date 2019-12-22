
#define NOAPIPROTO
#include "windows.h"
#include "Resources.h"
#include "ModTable.h"
#include "Segment.h"
#include "Log.h"

extern long int Trap();

extern long int GlobalHandleToSel(); 
extern long int GlobalFirst(); 
extern long int GlobalNext(); 
extern long int GlobalInfo(); 
extern long int GlobalEntryHandle(); 
extern long int GlobalEntryModule(); 
extern long int LocalInfo(); 
extern long int LocalFirst(); 
extern long int LocalNext(); 
extern long int ModuleFirst(); 
extern long int ModuleNext(); 
extern long int ModuleFindName(); 
extern long int ModuleFindHandle(); 
extern long int TaskFirst(); 
extern long int TaskNext(); 
extern long int TaskFindHandle(); 
extern long int StackTraceFirst(); 
extern long int StackTraceNext(); 
extern long int ClassFirst(); 
extern long int ClassNext(); 
extern long int SystemHeapInfo(); 
extern long int MemManInfo(); 
extern long int NotifyRegister(); 
extern long int NotifyUnRegister(); 
extern long int InterruptRegister(); 
extern long int InterruptUnRegister(); 
extern long int TerminateApp(); 
extern long int MemoryRead(); 
extern long int MemoryWrite(); 
extern long int TimerCount(); 
extern long int TaskSetCSIP(); 
extern long int TaskGetCSIP(); 
extern long int TaskSwitch(); 
extern long int _86_SystemHeapInfo();

#if defined(TWIN_BIN16)
extern long int IT_1H();
extern long int IT_NOTIFYREG();
extern long int IT_INTERRUPTREG();
extern long int IT_1LPME();
extern long int IT_1LPTE();
extern long int IT_1LP1HTE();
extern long int IT_MEMMANINFO();
extern long int IT_GLOBALENTRYHANDLE();
extern long int IT_TIMERCOUNT();
extern long int IT_1HTASK();

 /* Interface Segment Image TOOLHELP: */

static long int (*seg_image_TOOLHELP_0[])() =
{	/* nil */	0, 0,
	/* 001 */	Trap, 0,
	/* 002 */	Trap, 0,
	/* 003 */	Trap, 0,
	/* 004 */	Trap, 0,
	/* 005 */	Trap, 0,
	/* 006 */	Trap, 0,
	/* 007 */	Trap, 0,
	/* 008 */	Trap, 0,
	/* 009 */	Trap, 0,
	/* 00a */	Trap, 0,
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
	/* 035 */	Trap, 0,
	/* 036 */	IT_GLOBALENTRYHANDLE, GlobalEntryHandle,
	/* 037 */	Trap, 0,
	/* 038 */	Trap, 0,
	/* 039 */	Trap, 0,
	/* 03a */	Trap, 0,
	/* 03b */	IT_1LPME, ModuleFirst,
	/* 03c */	IT_1LPME, ModuleNext,
	/* 03d */	Trap, 0,
	/* 03e */	Trap, 0,
	/* 03f */	IT_1LPTE, TaskFirst,
	/* 040 */	IT_1LPTE, TaskNext,
	/* 041 */	IT_1LP1HTE, TaskFindHandle,
	/* 042 */	Trap, 0,
	/* 043 */	Trap, 0,
	/* 044 */	Trap, 0,
	/* 045 */	Trap, 0,
	/* 046 */	Trap, 0,
	/* 047 */	_86_SystemHeapInfo, 0,
	/* 048 */	IT_MEMMANINFO, MemManInfo,
	/* 049 */	IT_NOTIFYREG, NotifyRegister,
	/* 04a */	IT_1HTASK, NotifyUnRegister,
	/* 04b */	IT_INTERRUPTREG, InterruptRegister,
	/* 04c */	IT_1HTASK, InterruptUnRegister,
	/* 04d */	Trap, 0,
	/* 04e */	Trap, 0,
	/* 04f */	Trap, 0,
	/* 050 */	IT_TIMERCOUNT, TimerCount,
	/* 051 */	Trap, 0,
	/* 052 */	Trap, 0,
	/* 053 */	Trap, 0,
	0
};
#endif

 /* Segment Table TOOLHELP: */

SEGTAB SegmentTableTOOLHELP[] =
{	
#if defined(TWIN_BIN16)
	{ (char *) seg_image_TOOLHELP_0, 664, TRANSFER_CALLBACK, 664, 0, 0 },
#endif
	/* end */	{ 0, 0, 0, 0, 0, 0 }
};

