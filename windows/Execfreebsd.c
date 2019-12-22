/*    
	Execfreebsd.c	1.5
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
#include <sys/syscall.h>
#include <machine/segments.h>
#include <machine/sysarch.h>
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

static struct sigaction sact;

int sys_modify_ldt(struct modify_ldt_ldt_s *ldt_info, int size)
  {
    int ret;
#ifdef __i386__
        long d[2];
	{
	  /*struct modify_ldt_ldt_s {
	    unsigned int  entry_number;
	    unsigned long base_addr;
	    unsigned int  limit;
	    unsigned int  seg_32bit:1;
	    unsigned int  contents:2;
	    unsigned int  read_exec_only:1;
	    unsigned int  limit_in_pages:1;
	    unsigned int  seg_not_present:1;
	    unsigned int  useable:1;
	  };*/

	  *d = ((ldt_info->base_addr & 0x0000ffff) << 16) |
	    (ldt_info->limit & 0x0ffff);
	  *(d+1) = (ldt_info->base_addr & 0xff000000) |
	    ((ldt_info->base_addr & 0x00ff0000)>>16) |
	    (ldt_info->limit & 0xf0000) |
	    (ldt_info->contents << 10) |/*type*/
	    ((ldt_info->read_exec_only == 0) << 9) |
	    ((ldt_info->seg_32bit != 0) << 22) |
	    ((ldt_info->limit_in_pages != 0) << 23) |
	    0xf000;
	}
        ret = i386_set_ldt(ldt_info->entry_number, (union descriptor*)d, 1);
        if (ret < 0)
        {
            perror("i386_set_ldt");
            MSG("Did you reconfigure the kernel with \"options USER_LDT\"?\n");
    	    exit(1);
        }
#else
#error "not implimented"
#endif  /* __i386__ */
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
    sact.sa_handler  = TWIN_GPFSigHandler;
    sact.sa_flags    = SA_RESTART;
    sigemptyset(&sact.sa_mask);
    rc =  sigaction(sig, &sact, NULL);
    return rc;
}

int
TWIN_RegisterDebugSignal(int sig)
{
    int rc;
    sact.sa_handler = TWIN_DebugSigHandler;
    sact.sa_flags = 0;
    rc =  sigaction(sig, &sact, NULL);
    return rc;
}
