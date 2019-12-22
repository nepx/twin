/*
	@(#)DrvEvents.c	1.20
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

 
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/times.h>

#include "windows.h"
#include "windowsx.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "Driver.h"

#include "Log.h"
#include "DrvDP.h"		/* PRIVATEDISPLAY */
#include "mtlock.h"

static DWORD DrvSwapMouseButton(LPARAM, LPARAM, LPVOID);
static DWORD DrvDblClkTime(LPARAM, LPARAM, LPVOID);
static DWORD DrvBeep(LPARAM, LPARAM, LPVOID);
static DWORD DrvGetSystemTime(LPARAM, LPARAM, LPVOID);
static DWORD DrvEventsUnblockSelectWait(LPARAM, LPARAM, LPVOID);
static DWORD DrvEventsDoNothing(LPARAM,LPARAM,LPVOID);

DWORD DrvEventsTab(void);

extern BOOL DriverInSelectWait(void);

static TWINDRVSUBPROC DrvEventsEntryTab[] = {
	DrvEventsDoNothing,
	DrvEventsDoNothing,
	DrvEventsDoNothing,
	DrvSwapMouseButton,
	DrvDblClkTime,
	DrvBeep,
	DrvGetSystemTime,
	DrvEventsUnblockSelectWait};

DWORD
DrvEventsTab(void)
{
	return (DWORD)DrvEventsEntryTab;
}

static DWORD
DrvEventsDoNothing(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{	
	return 0L;
}

static LPBYTE map = 0;
static int nCount = 0;

static DWORD
DrvSwapMouseButton(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
    PRIVATEDISPLAY *dp = GETDP();
    BYTE bMap;
    BOOL fState;

    if (map == 0) {
        LOCK_SECTION( _MTLOCK_VIDEO);
	nCount = XGetPointerMapping(dp->display,&bMap,1);
        UNLOCK_SECTION( _MTLOCK_VIDEO);

	if (nCount == 0)
	    return 0L;
        LOCK_SECTION( _MTLOCK_VIDEO);
	map = (LPBYTE)WinMalloc(nCount);
        UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    LOCK_SECTION( _MTLOCK_VIDEO);
    nCount = XGetPointerMapping(dp->display,map,nCount);
    UNLOCK_SECTION( _MTLOCK_VIDEO);

    if (nCount == 0)
	return 0L;

    if (nCount == 1) { /* one-button job */
	fState = FALSE;
    }
    else {
        LOCK_SECTION( _MTLOCK_VIDEO);
	if (nCount == 2) {
	    fState = (map[0] == 1);
	    bMap = map[0];
	    map[0] = map[1];
	    map[1] = bMap;
	}
	else { /* more than 2 buttons */
	    fState = (map[0] == 1 && map[2] == 3);
	    bMap = map[0];
	    map[0] = map[2];
	    map[2] = bMap;
	}
        UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    LOCK_SECTION( _MTLOCK_VIDEO);
    XSetPointerMapping(dp->display,map,nCount);
    UNLOCK_SECTION( _MTLOCK_VIDEO);

    return (DWORD)fState;
}

static DWORD
DrvDblClkTime(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
    BOOL fSet = (BOOL)dwParam1;
    UINT uInterval = (UINT)dwParam2;
    PRIVATEDISPLAY *dp = GETDP();
    DWORD dwOldTime = dp->DoubleClickTime;

    if (fSet)
	dp->DoubleClickTime = (DWORD)uInterval;

    return dwOldTime;
}

static DWORD
DrvBeep(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
    DWORD dwBeep = (DWORD)dwParam1;
    PRIVATEDISPLAY *dp = GETDP();

    LOCK_SECTION( _MTLOCK_VIDEO);
    XBell(dp->display,dwBeep==0?100:LOWORD(dwBeep));
    UNLOCK_SECTION( _MTLOCK_VIDEO);
    return 0L;
}

#ifdef HAVE_SYSCONF

static DWORD
DrvGetSystemTime(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
    DWORD dwTime;
    struct tms t_buffer;
    long clk_tick;

    /* if times fails, this will return 0   */
    /* we do not need to return -1 as no one */
    /* examines it anyway                   */
    t_buffer.tms_utime = 0;
    t_buffer.tms_stime = 0;
    times(&t_buffer);

    dwTime = t_buffer.tms_utime + t_buffer.tms_stime;
    dwTime *= 1000;
    clk_tick = sysconf(_SC_CLK_TCK);
    dwTime /= (DWORD)clk_tick;

    return dwTime;
}
#else

#include <times.h>
static DWORD
DrvGetSystemTime(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
    DWORD dwTime;
    tbuffer_t t_buffer;

    /* if times fails, this will return 0   */
    /* we do not need to return -1 as no one */
    /* examines it anyway                   */
    t_buffer.proc_user_time = 0;
    t_buffer.proc_system_time = 0;
    times(&t_buffer);

    dwTime = t_buffer.proc_user_time + t_buffer.proc_system_time;
    dwTime *= 10;  /* x1000 then /100 = 10 millisecond clock ticks */
    return dwTime;

}
#endif







static DWORD
DrvEventsUnblockSelectWait(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
	if (DriverInSelectWait())
	{
		PRIVATEDISPLAY *dp = GETDP();
		/* send a fake X event to force driver out of select() wait */
    		LOCK_SECTION( _MTLOCK_VIDEO);
		XChangeProperty(dp->display,
			XRootWindow(dp->display, dp->screen),
			dp->XFormatAtom, XA_STRING, 
			8, PropModeAppend, "", 0);
		XFlush(dp->display);
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
	}
	return 0L;
}

