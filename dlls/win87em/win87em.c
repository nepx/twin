#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "windows.h"
#include "Log.h"
#include "BinTypes.h"

#define CARRY_FLAG 1

void
WIN87EM()
{
	return;
}

void
_86_Win87Em (ENV *envp,LONGPROC f)
{
	switch(LOWORD(envp->reg.bx)) {
	    case 0:	/* Initialize the FP emulator */
	    case 1:	/* Reset the FP emulator */
	    case 2:	/* Stop the FP emulator */
	    case 3:	/* Set coprocessor error exception handler */
		break;
	    case 10:	/* Retrieve FP stack count */
		envp->reg.ax = 0;
		break;
	    case 11:	/* Query coprocessor presence */
		envp->reg.ax = 1;
		break;
	}
	envp->reg.flags &= ~CARRY_FLAG;
	envp->reg.sp += RET_86;
}

