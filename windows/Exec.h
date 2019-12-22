/********************************************************************

	@(#)Exec.h	1.6
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

********************************************************************/
 
#ifndef Exec__h
#define Exec__h

struct TWIN_i386_context_s
{
    unsigned short gs, __gsh;
    unsigned short fs, __fsh;
    unsigned short es, __esh;
    unsigned short ds, __dsh;
    unsigned long edi;
    unsigned long esi;
    unsigned long ebp;
    unsigned long esp;
    unsigned long ebx;
    unsigned long edx;
    unsigned long ecx;
    unsigned long eax;
    unsigned long trapno;
    unsigned long err;
    unsigned long eip;
    unsigned short cs, __csh;
    unsigned long eflags;
    unsigned long esp_at_signal;
    unsigned short ss, __ssh;
};

#ifndef SI86DSCR
#define SI86DSCR	75
struct ssd {
        unsigned int sel;   /* descriptor selector */
        unsigned int bo;    /* segment base or gate offset */
        unsigned int ls;    /* segment limit or gate selector */
        unsigned int acc1;  /* access byte 5 */
        unsigned int acc2;  /* access bits in byte 6 or gate count */
};
#endif /* SI86DSCR */

int sysi86(int , void *);

int  TWIN_RegisterGPFSignal(int);
void  TWIN_GPFHandler(int, struct TWIN_i386_context_s *);

void TWIN_ValidateSegmentRegisters(struct TWIN_i386_context_s *);

int TWIN_RegisterDebugSignal(int);

void TWIN_DebuggerProc(int,int,void *);

#define DEBUG_TASK(x,y)	TWIN_DebuggerProc(TWIN_DBGCALL,x,y);

#endif /* Exec__h */
