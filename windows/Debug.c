
#include <stdio.h>
#include <signal.h>
#include "windows.h"
#include "Debug.h"

#ifdef X386
void TWIN_RegisterDebugSignal(int);
#endif

void
TWIN_DebuggerProc(int opcode, int wparam, void *lparam)
{
	static void (*debugger)(int,int,void *);

	switch(opcode) {
	/* callback from debugger shared library */
	/* set debugger function */
	case TWIN_DBGINIT:
		debugger = lparam;
		return;

	/* routine to load the debugger shared library */
	/* loads it, and if it successfully called back */
	/* calls its initialization point */
	case TWIN_DBGLOAD:
#ifdef X386
		if(debugger == 0) 
        		LoadLibrary("debug");
		if(debugger)  {
			TWIN_RegisterDebugSignal(SIGINT);
			TWIN_RegisterDebugSignal(SIGTRAP);
		}
#endif

		break;

	case TWIN_DBGADDS:
	case TWIN_DBGADDR:
	case TWIN_DBGCALL:
		/* fall through, and call debugger */
		break;

	default:
		FatalAppExit(0,"Unknown DebuggerProc opcode\n");
	}

	if(debugger) 
		debugger(opcode,wparam,lparam);
}

