/*    
	DPMInotify.c	2.7
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

// nepx: It seems like the 32-bit CPU emulator isn't hooked up yet, which is fine. 
// This file tries to emulate all the the DPMI APIs using Linux system calls.

// Comment the line below if you don't want the 32-bit protected mode emulation
#define DPMI_EMULATION

#include <string.h>
#include "windows.h"
#include "DPMI.h"
#include "Log.h"

#ifdef DPMI_EMULATION
#include <sys/types.h> // modify_ldt
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <asm/ldt.h> // for struct user_desc

#if 0
#ifndef X386
#error "DPMI emulation only supported on x86(-64) platforms compiled with -m32"
#endif
#endif

static inline int modify_ldt(int func, void* ptr, unsigned long bytecount){
    return syscall(SYS_modify_ldt, func, ptr, bytecount);
}
static inline int set_thread_area(void* ptr){
    return syscall(SYS_set_thread_area, ptr);
}

#endif 

#define INITLDTSIZE 0x300

extern int TWIN_ThunkManager(int);

#ifdef X386
WORD native_cs,native_ss,native_ds;
#endif

BOOL
DPMI_Notify(UINT uAction,WORD wSel)
{
    switch(uAction) {
    case DN_INIT:
	if (!(LDT = (DSCR *)WINMALLOC(INITLDTSIZE*sizeof(DSCR),1,0))) {
 	    FatalAppExit(1,"Could not allocate LDT");
	}

	memset((LPSTR)LDT,'\0',INITLDTSIZE*sizeof(DSCR));
	if (!TWIN_ThunkManager(1))
	    FatalAppExit(0, "DPMI: Could not init thunks\n");
	nLDTSize = INITLDTSIZE;
	SetPhysicalAddress(0,(LPBYTE)(-1));		/* sel=0 is illegal */
	return TRUE;

#ifdef DPMI_EMULATION
    case DN_ASSIGN: {
        struct user_desc desc;
        desc.base_addr = LDT[wSel >> 3].lpSelBase;
        desc.entry_number = wSel >> 3;
        desc.limit = LDT[wSel >> 3].dwSelLimit;
        desc.seg_32bit = (LDT[wSel >> 3].w86Flags & DF_32) != 0;
        desc.contents = (LDT[wSel >> 3].w86Flags >> 1 & 3) ^ 1;
        desc.read_exec_only = (LDT[wSel >> 3].w86Flags & DF_CODE) != 0;
        printf("Base: %08x limit: %08x\n", desc.base_addr, desc.limit);
        desc.limit_in_pages = (LDT[wSel >> 3].w86Flags & DF_PAGES) != 0;
        desc.seg_not_present = 0;
        desc.useable = 1;
#if 1
        if(modify_ldt(1, &desc, sizeof(struct user_desc)) == -1){
#else
        if(set_thread_area(&desc)){
#endif
            perror("modify ldt");
            printf("modify ldt failed\n");
            abort();
        }

#if 0
        struct user_desc desc2;
        desc2.entry_number = wSel >> 3;
        if(modify_ldt(0, &desc, sizeof(struct user_desc))==-1){
            perror("modify ldt read");
            abort();
        }
        //printf("p/x *(struct user_desc*)%p\n", &desc2);
        //if(wSel == 0x6F)
        //__asm__("int3");
#endif
    return TRUE;
    }
#endif
    
    default:
#ifndef DPMI_EMULATION
	return FALSE;
#else
    printf("TODO: dpmi option %d\n", uAction);
    abort();
#endif
    }
}
