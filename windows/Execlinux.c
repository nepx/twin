/*    
	ExecLinux.c	1.5
    	Copyright 1997 Willows Software, Inc. 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.


For more information about the Willows Twin Libraries.

	http://www.willows.com	

To send email to the maintainer of the Willows Twin Libraries.

	mailto:twin@willows.com 

 */

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include <syscall.h>
#include "ldt.h"

#include "windows.h"

#include "DPMI.h"
#include "kerndef.h"		/* BINADDR */
#include "BinTypes.h"		/* ENV structure */
#include "Log.h"
#include "dos.h"
#include "Exec.h"
#include "Debug.h"

#define	STACK_DWORDS	0x4000

struct lsigaction
{
    void (*lsa_handler)();
    unsigned long lsa_mask;
    unsigned long lsa_flags;
    void (*lsa_restorer)();
};

static struct lsigaction sact;
static unsigned long TWIN_Stack[STACK_DWORDS];

#define sys_modify_ldt(a, b) syscall(123,1,(void *)a,(int)b)
#define sys_read_ldt(a, b) syscall(123,2,(void *)a,(int)b)

#define SIGACTION wine_sigaction

/* Similar to the sigaction function in libc, except it leaves alone the
   restorer field, which is used to specify the signal stack address */
static __inline__ int wine_sigaction( int sig, struct lsigaction *new,
                                      struct lsigaction *old )
{
    __asm__ __volatile__( "pushl %%ebx\n\t"
                          "movl %2,%%ebx\n\t"
                          "int $0x80\n\t"
                          "popl %%ebx"
                          : "=a" (sig)
                          : "0" (SYS_sigaction),
                            "g" (sig),
                            "c" (new),
                            "d" (old) );
    return 0;
}

int sysi86(int func, void *data)
{
    struct modify_ldt_ldt_s ldt_info;
    struct ssd *req;
    unsigned short wFlags;

    if (func == SI86DSCR)
    {
	req = (struct ssd *) data;
	wFlags = (unsigned short) req->acc1;
	wFlags |= ((unsigned short) req->acc2 << 8);

	ldt_info.entry_number = (unsigned int)(req->sel >> 3);
	ldt_info.base_addr = req->bo;

	if (req->ls == 0) 
	{
	    ldt_info.limit = 0;
	    ldt_info.limit_in_pages = 0;
	}
	else if ((req->ls - 1) > 0xFFFFF || (wFlags & DF_PAGES)) 
	{
	    ldt_info.limit = (req->ls - 1) >> 12;
	    ldt_info.limit_in_pages = 1;
	}
	else 
	{
	    ldt_info.limit = req->ls - 1;
	    ldt_info.limit_in_pages = 0;
	}

	ldt_info.seg_32bit = (wFlags & DF_32)?1:0;

	if (wFlags & DF_CODE) 
	    ldt_info.contents = MODIFY_LDT_CONTENTS_CODE;
	else if (wFlags & DF_EXPANDDOWN)
	    ldt_info.contents = MODIFY_LDT_CONTENTS_STACK;
	else
	    ldt_info.contents = MODIFY_LDT_CONTENTS_DATA;

	ldt_info.read_exec_only = (wFlags & DF_DWRITEABLE) ? 0 : 1;
	ldt_info.seg_not_present = (wFlags & DF_PRESENT) ? 0 : 1;
	return sys_modify_ldt(&ldt_info,sizeof(ldt_info));
    }
    else
	return -1;
}

static void
TWIN_GPFSigHandler(int sig)
{
    struct TWIN_i386_context_s  *sc;
    
    sc = (struct TWIN_i386_context_s *) ((long *) &sig + 1);
    TWIN_GPFHandler(sig, sc);
    TWIN_ValidateSegmentRegisters(sc);
}

static void
TWIN_DebugSigHandler (int sig)
{
    struct TWIN_i386_context_s  *sc;

    sc = (struct TWIN_i386_context_s *) ((long *) &sig + 1);

    if (sig == SIGTRAP)
	sc->eip--;
    
    DEBUG_TASK(sig, sc);
    TWIN_ValidateSegmentRegisters(sc);
}

int
TWIN_RegisterGPFSignal(int sig)
{
    int rc;
    sact.lsa_handler  = TWIN_GPFSigHandler;
    sact.lsa_flags    = SA_RESTART|SA_NOMASK;
    sact.lsa_restorer = (void (*)()) &TWIN_Stack[STACK_DWORDS-1];
    sact.lsa_mask     = 0;
    rc =  SIGACTION(sig, &sact, NULL);
    return rc;
}

int
TWIN_RegisterDebugSignal(int sig)
{
    int rc;
    sact.lsa_handler = TWIN_DebugSigHandler;
    sact.lsa_flags = 0;
    sact.lsa_restorer = (void (*)()) &TWIN_Stack[STACK_DWORDS-1];
    rc = SIGACTION(sig, &sact, NULL);
    return rc;
}

