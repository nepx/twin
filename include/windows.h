
/*  windows.h -   SDK Structure definitions, and API's.
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

The maintainer of the Willows TWIN Libraries may be reached (Email) 
at the address twin@willows.com	

*/

/**********************************************************************
*
*   Symbol exclusion controls, #define any of the following to 
*    exclude the symbols:
*
*   NOAPIPROTO      Exclude API prototypes, for cross platform compatibility.
*
*   NOMINMAX          min() and max() macros
*
*   NOKERNEL        Kernel definitions and API's
*       NOLOGERROR        LogError() and related definitions
*       NOPROFILER        Profiler APIs
*       NOMEMMGR          Local and global memory management
*       NOLFILEIO         _l* file I/O routines
*       NOOPENFILE        OpenFile and related definitions
*       NORESOURCE        Resource management
*       NOATOM            Atom management
*       NOLANGUAGE        Character test routines
*       NOLSTRING         lstr* string management routines
*       NODBCS            Double-byte character set routines
*       NOKEYBOARDINFO    Keyboard driver routines
*
*   NOGDI           GDI definitions and API's
*       NOGDICAPMASKS     GDI device capability constants
*       NOCOLOR           COLOR_* color values
*       NOGDIOBJ          GDI pens, brushes, fonts
*       NODRAWTEXT        DrawText() and related definitions
*       NOTEXTMETRIC      TEXTMETRIC and related APIs
*       NOSCALABLEFONT    Truetype scalable font support
*       NOBITMAP          Bitmap support
*       NORASTEROPS       GDI Raster operation definitions
*       NOMETAFILE        Metafile support
*
*   NOUSER          USER definitions and API's
*       NOSOUND         Sound definitions and API's
*       NOCOMM          Comm driver definitions and API's
*       NODRIVERS       Installable driver definitions and API's
*       NOSYSMETRICS      GetSystemMetrics() and related SM_* definitions
*       NOSYSTEMPARAMSINFO SystemParametersInfo() and SPI_* definitions
*       NOMSG             APIs and definitions that use MSG structure
*       NOWINSTYLES       Window style definitions
*       NOWINOFFSETS      Get/SetWindowWord/Long offset definitions
*       NOSHOWWINDOW      ShowWindow and related definitions
*       NODEFERWINDOWPOS  DeferWindowPos and related definitions
*       NOVIRTUALKEYCODES VK_* virtual key codes
*       NOKEYSTATES       MK_* message key state flags
*       NOWH              SetWindowsHook and related WH_* definitions
*       NOMENUS           Menu APIs
*       NOSCROLL          Scrolling APIs and scroll bar control
*       NOCLIPBOARD       Clipboard APIs and definitions
*       NOICONS           IDI_* icon IDs
*       NOMB              MessageBox and related definitions
*       NOSYSCOMMANDS     WM_SYSCOMMAND SC_* definitions
*       NOMDI             MDI support
*       NOCTLMGR          Control management and controls
*       NOWINMESSAGES     WM_* window messages
*       NOHELP            Help support
*
*   NOCOMMDLG       COMMDLG definitions and API's
*
********************************************************************/

#ifndef windows__h
#define windows__h
#define __WINDOWS_H   /* OWL tests for this extensively */

#define WINVER 0x400
#define _INC_WINDOWS

#include "platform.h"	/* platform specific */

#include "winerror.h"   /* system error codes */

#ifdef __cplusplus
extern "C" {    /* Force C declarations for C++ */
#endif          /* __cplusplus */

#ifdef WIN32
#ifndef TWIN32
#define TWIN32
#endif
#endif /* WIN32 */

#ifdef RC_INVOKED
/* Don't include definitions that RC.EXE can't parse */
#define NOATOM
#define NOGDI
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOWH
#define NODBCS
#define NOSYSTEMPARAMSINFO
#define NOCOMM
#define NOOEMRESOURCE
#define NOCONSOLE
#endif  /* RC_INVOKED */

#include "winmaptypes.h"

#include "windef.h"

/* Usefull macros ********************************************/
#define LOBYTE(w)       ((BYTE)(w))
#define HIBYTE(w)       ((BYTE)(((UINT)(w) >> 8) & 0xFF))

/* #define LOWORD(l)       ((WORD)(DWORD)(l)) */
#define LOWORD(l)       ((WORD)((DWORD)(l) & 0x0FFFF))
#define HIWORD(l)       ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))

#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))
#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))

#ifndef NOMINMAX
#ifndef max
#define max(a,b)        (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)        (((a) < (b)) ? (a) : (b))
#endif
#define __max max
#define __min min
#endif  /* NOMINMAX */

#define	UNREFERENCED_PARAMETER(x)

/* Pointer macros */
#define MAKELP(sel, off)    ((void FAR*)MAKELONG((off), (sel)))
#define SELECTOROF(lp)      HIWORD(lp)
#define OFFSETOF(lp)        LOWORD(lp)

#define FIELDOFFSET(type, field)    ((int)(&((type NEAR*)1)->field)-1)

#define MAKELPARAM(low, high)   ((LPARAM)MAKELONG(low, high))
#define MAKELRESULT(low, high)  ((LRESULT)MAKELONG(low, high))

#ifdef TWIN32
#define MAKEWPARAM(low, high)   ((WPARAM)MAKELONG(low, high))
#endif /* TWIN32 */


/* System inclusions ******************************************/

/* some WIN32 definitions */

#ifndef _MAX_PATH
#define _MAX_PATH   260
#endif
#ifdef MAX_PATH
#undef MAX_PATH
#endif
#define MAX_PATH	_MAX_PATH
#ifndef _MAX_DRIVE
#define _MAX_DRIVE  3
#endif
#ifndef _MAX_DIR
#define _MAX_DIR    256
#endif
#ifndef _MAX_FNAME
#define _MAX_FNAME  256
#endif
#ifndef _MAX_EXT
#define _MAX_EXT    256
#endif

#ifndef RC_INVOKED
#include "winnt.h"
#include "winbase.h"
#endif

#ifndef NOVERSION
#include "winver.h"
#endif

#ifndef NOCONSOLE
#include "wincon.h"
#endif

#ifndef NOGDI
#include "wingdi.h"
#endif      /* NOGDI */

#ifndef NOUSER
#include "winuser.h"
#endif      /* NOUSER */

/* Spooler status notification message */
#define WM_SPOOLERSTATUS        0x002A

#ifdef __cplusplus
} /* Leave 'extern "C"' mode when switching header files */
#endif

#include "winmapclib.h"	/* standard C library functions and definitions */

#ifdef __cplusplus
extern "C" {
#endif


#include "dlgs.h"

#ifndef RC_INVOKED

#include "shellapi.h"

#ifndef NO_WINSOCK
#include "winsock.h"
#endif

#ifndef NO_COMMDLG
#include "commdlg.h"
#endif

#include "ddeml.h"
#include "mmsystem.h"
#if defined(TWIN32)
#include "print.h"
#endif /* TWIN32 */
#endif /* RC_INVOKED */

#include "commctrl.h"

#define _TUCHAR UCHAR


extern int _doserrno;
extern int __argc;
extern char **__argv;

#if ! defined(PRINTFLAG_DEFINED)
#define PRINTFLAG_DEFINED
typedef enum __MIDL_IPrint_0001 {
	PRINTFLAG_MAYBOTHERUSER  	= 1,
	PRINTFLAG_PROMPTUSER  		= 2,
	PRINTFLAG_USERMAYCHANGEPRINTER  = 4,
	PRINTFLAG_RECOMPOSETODEVICE  	= 8,
	PRINTFLAG_DONTACTUALLYPRINT  	= 16,
	PRINTFLAG_FORGETPROPERTIES  	= 32,
	PRINTFLAG_PRINTTOFILE  		= 64
} PRINTFLAG;
#endif

#define T2CA(x)	x
#if ! defined(T2COLE)
    #define T2COLE(x) (LPCOLESTR)(x)
#endif

#ifdef __cplusplus
}           /* End of extern "C" { */
#endif      /* __cplusplus */

/*
 *  Hook to allow an application to include something after every
 *  inclusion of windows.h.  To use this, they must define the
 *  value, and then provide the header somewhere in their include
 *  path.
 */
#if defined(OPENAPI_USE_APPLICATION_HEADER)
#include "openapi_application.h"
#endif

#define __WIN32__
#define __FLAT__
#define wchar_t WCHAR
typedef WORD CLIPFORMAT;

// Common macros gleamed from COMPOBJ.H

#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif

#endif      /* windows__h */
