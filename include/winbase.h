/*  Win_Base.h	1.19
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

#ifndef Win_Base__h
#define Win_Base__h

#include <stdarg.h>

/*
** Define API decoration for direct importing of DLL references.
*/

#if !defined(_ADVAPI32_)
#define WINADVAPI DECLSPEC_IMPORT
#else
#define WINADVAPI
#endif

#if !defined(_KERNEL32_)
#define WINBASEAPI DECLSPEC_IMPORT
#else
#define WINBASEAPI
#endif

#define STD_INPUT_HANDLE    (DWORD)-10
#define STD_OUTPUT_HANDLE   (DWORD)-11
#define STD_ERROR_HANDLE      (DWORD)-12

typedef struct _OVERLAPPED {
    DWORD   Internal;
    DWORD   InternalHigh;
    DWORD   Offset;
    DWORD   OffsetHigh;
    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef VOID (WINAPI *LPOVERLAPPED_COMPLETION_ROUTINE)(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped
    );

#ifndef _FILETIME_
#define _FILETIME_
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
#endif /* _FILETIME_ */

#ifndef _SYSTEMTIME_
#define _SYSTEMTIME_
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
#endif /* _SYSTEMTIME_ */

typedef struct _WIN32_FIND_DATA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR   cFileName[ _MAX_PATH ];
    CHAR   cAlternateFileName[ 14 ];
} WIN32_FIND_DATA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA;

/*
 *  Internal (TWIN-only) structure used for passing file information
 *  between the lowest driver levels, up through the MFS layer, and
 *  finally to the library itself, as well in the reverse direction.
 *
 *  These fields are mainly driven by the contents/needs for the
 *  Win32 structures WIN32_FIND_INFO and BY_HANDLE_FILE_INFORMATION,
 *  as well as any other structures that need stat-type information.
 *  Feel free to add fields as necessary here, but make sure that
 *  the lower Driver levels are changed to support them.
 */
typedef struct tagTWIN_FILEINFO
{
    DWORD       dwFileAttributes;
    FILETIME    ftCreationTime;
    FILETIME    ftLastAccessTime;
    FILETIME    ftLastWriteTime;
    DWORD       dwVolumeSerialNumber;
    DWORD       nFileSizeHigh;
    DWORD       nFileSizeLow;
    DWORD       nNumberOfLinks;
    DWORD       nFileIndexHigh;
    DWORD       nFileIndexLow;
} TWIN_FILEINFO, *LPTWIN_FILEINFO;

typedef struct _TIME_ZONE_INFORMATION {
    LONG Bias;
    WCHAR StandardName[ 32 ];
    SYSTEMTIME StandardDate;
    LONG StandardBias;
    WCHAR DaylightName[ 32 ];
    SYSTEMTIME DaylightDate;
    LONG DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;

#ifdef TWIN32
UINT WINAPI GetDriveType( LPTSTR );
UINT WINAPI GetTempFileName( LPCTSTR, LPCTSTR, UINT, LPTSTR );
#endif /* TWIN32 */

#define INVALID_HANDLE_VALUE (HANDLE)-1

#define FILE_BEGIN          0
#define FILE_CURRENT        1
#define FILE_END            2

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define FILE_TYPE_UNKNOWN   0x0000
#define FILE_TYPE_DISK      0x0001
#define FILE_TYPE_CHAR      0x0002
#define FILE_TYPE_PIPE      0x0003
#define FILE_TYPE_REMOTE    0x0004

/* GetBinaryType() return values */
#define SCS_32BIT_BINARY    0
#define SCS_DOS_BINARY      1
#define SCS_WOW_BINARY      2
#define SCS_PIF_BINARY      3
#define SCS_POSIX_BINARY    4
#define SCS_OS216_BINARY    5

#define MOVEFILE_REPLACE_EXISTING   0x00000001
#define MOVEFILE_COPY_ALLOWED       0x00000002
#define MOVEFILE_DELAY_UNTIL_REBOOT 0x00000004

#define FS_CASE_IS_PRESERVED            FILE_CASE_PRESERVED_NAMES
#define FS_CASE_SENSITIVE               FILE_CASE_SENSITIVE_SEARCH
#define FS_UNICODE_STORED_ON_DISK       FILE_UNICODE_ON_DISK
#define FS_PERSISTENT_ACLS              FILE_PERSISTENT_ACLS
#define FS_VOL_IS_COMPRESSED            FILE_VOLUME_IS_COMPRESSED
#define FS_FILE_COMPRESSION             FILE_FILE_COMPRESSION

/* File32 function prototypes */
#ifdef TWIN32
#ifndef NOAPIPROTO

HANDLE 
CreateFile(LPCTSTR lpszName, DWORD fdwAccess, DWORD fdwShare,
           LPSECURITY_ATTRIBUTES lpsa, DWORD fdwCreate, 
           DWORD fdwAttr, HANDLE hTemplate);

BOOL 
DeleteFile(LPCTSTR lpszName);

BOOL 
CopyFile(LPTSTR lpExisting, LPTSTR lpNew, BOOL fFail);

BOOL
MoveFile(LPCTSTR lpExisting, LPCTSTR lpNew);

BOOL
MoveFileEx(LPCTSTR lpExisting, LPCTSTR lpNew, DWORD dwFlags);

BOOL
CloseHandle(HANDLE hObject);

BOOL
DuplicateHandle(HANDLE, HANDLE, HANDLE, LPHANDLE, DWORD, BOOL, DWORD);

BOOL
ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nBytesToRead,
         LPDWORD lpBytesRead, LPOVERLAPPED lpOverlapped);

BOOL
ReadFileEx(HANDLE hFile, LPVOID lpBuffer, DWORD nBytesToRead,
           LPOVERLAPPED lpOverlapped, 
           LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

BOOL
WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nBytesToWrite,
          LPDWORD lpBytesWritten, LPOVERLAPPED lpOverlapped);

BOOL
WriteFileEx(HANDLE hFile, LPCVOID lpBuffer, DWORD nBytesToWrite,
            LPOVERLAPPED lpOverlapped,
            LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

DWORD
SetFilePointer(HANDLE hFile, LONG lDistance, PLONG lpDistance,
               DWORD dwMoveMethod);

BOOL
SetEndOfFile(HANDLE hFile);

BOOL
FlushFileBuffers(HANDLE hFile);

BOOL
LockFile(HANDLE hFile, DWORD dwOffsetLow, DWORD dwOffsetHigh,
         DWORD nBytesToLockLow, DWORD nBytesToLockHigh);

BOOL
UnlockFile(HANDLE hFile, DWORD dwOffsetLow, DWORD dwOffsetHigh,
           DWORD nBytesToUnlockLow, DWORD nBytesToUnlockHigh);

BOOL
UnlockFileEx(HANDLE hFile, DWORD dwReserved,
             DWORD nBytesToUnlockLow, DWORD nBytesToUnlockHigh,
             LPOVERLAPPED lpOverlapped);
#endif
#endif

#ifndef NOAPIPROTO
HANDLE
FindFirstFile(LPCTSTR lpszSearchFile, LPWIN32_FIND_DATA lpffd);

BOOL
FindNextFile(HANDLE hFindFile, LPWIN32_FIND_DATA lpffd);

BOOL
FindClose(HANDLE hFindFile);
#endif

#ifdef TWIN32
UINT
GetTempFileName(LPCTSTR lpszPath, LPCTSTR lpszPrefix, 
                UINT uUnique, LPTSTR lpszTempFile);
#endif /* TWIN32 */

#ifndef NOAPIPROTO
DWORD
GetTempPath(DWORD cchBuffer, LPTSTR lpszTempFile);

DWORD
GetFileAttributes(LPCTSTR lpszFile);

BOOL
SetFileAttributes(LPCTSTR lpFileName, DWORD dwAttributes);

DWORD
GetFileType(HANDLE hFile);

DWORD
GetFileSize(HANDLE hFile, LPDWORD lpdwFileSizeHigh);

DWORD
GetFullPathName(LPCTSTR lpszFile, DWORD cchPath, 
                LPTSTR lpszPath, LPTSTR *ppszFilePart);

DWORD
GetShortPathName(LPCTSTR, LPTSTR, DWORD);

BOOL
CreateDirectory(LPTSTR lpszPath, LPSECURITY_ATTRIBUTES lpsa);

BOOL
CreateDirectoryEx(LPCTSTR lpTemplate, LPCTSTR lpNew, LPSECURITY_ATTRIBUTES lpsa);

BOOL
RemoveDirectory(LPCTSTR lpPath);

DWORD
GetCurrentDirectory(DWORD cchCurDir, LPTSTR lpszCurDir);

BOOL
SetCurrentDirectory(LPCTSTR lpPath);

#ifdef LATER
BOOL
GetBinaryType(LPCTSTR lpAppName, LPDWORD lpBinType);

HANDLE
FindFirstChangeNotification(LPTSTR lpszPath, BOOL fWatchSubTree, DWORD fdwFilter);

BOOL
FindNextChangeNotification(HANDLE hChange);

BOOL
FindCloseChangeNotification(HANDLE hChange);
#endif /* LATER */

BOOL
GetVolumeInformation(LPCTSTR lpRootPath, LPTSTR lpVolumeName,
                     DWORD nVolumeNameSize, LPDWORD lpSerialNo,
                     LPDWORD lpMaxLength, LPDWORD lpFlags,
                     LPTSTR lpFileSysName, DWORD nFileSysNameSize);

BOOL
GetDiskFreeSpace(LPCTSTR lpszRootPath, LPDWORD lpSectorsPerCluster,
                 LPDWORD lpBytesPerSector, LPDWORD lpFreeClusters,
                 LPDWORD lpClusters);
#endif
#ifdef TWIN32
UINT
GetDriveType(LPTSTR lpszRootPath);
#endif /* TWIN32 */

#ifndef NOAPIPROTO
DWORD
GetLogicalDrives(void);

DWORD
GetLogicalDriveStrings(DWORD cchBuffer, LPTSTR lpszBuffer);

BOOL
SetFileTime(HANDLE hFile, LPFILETIME lpftCreation, LPFILETIME lpftAccess,
	    LPFILETIME lpftWrite);

BOOL	WINAPI
GetFileTime(HANDLE hFile, LPFILETIME lpftCreation, LPFILETIME lpftAccess,
	    LPFILETIME lpftWrite);

DWORD	WINAPI
SearchPath(LPCTSTR, LPCTSTR, LPCTSTR, DWORD, LPTSTR, LPTSTR *);

/* Win32 time functions */
BOOL FileTimeToSystemTime(CONST FILETIME *lpft, LPSYSTEMTIME lpst);
BOOL SystemTimeToFileTime(CONST SYSTEMTIME *lpst, LPFILETIME lpft);
BOOL FileTimeToLocalFileTime(CONST FILETIME *lpft, LPFILETIME lpftLocal);
BOOL LocalFileTimeToFileTime(CONST FILETIME *lpftLocal, LPFILETIME lpft);
void GetSystemTime(LPSYSTEMTIME lpst);
void GetLocalTime(LPSYSTEMTIME lpst);
BOOL SetSystemTime(LPSYSTEMTIME lpst);
BOOL SetLocalTime(LPSYSTEMTIME lpst);
BOOL SetTimeZoneInformation(LPTIME_ZONE_INFORMATION lptzi);
BOOL GetTimeZoneInformation(LPTIME_ZONE_INFORMATION lptzi);
BOOL DosDateTimeToFileTime(WORD wDOSDate, WORD wDOSTime, LPFILETIME lpft);
BOOL FileTimeToDosDateTime(LPFILETIME lpft, LPWORD lpwDOSDate, LPWORD lpwDOSTime);
LONG CompareFileTime(LPFILETIME lpft1, LPFILETIME lpft2);
BOOL SetSystemTimeAdjustment(DWORD dwTimeAdjustment, BOOL bTimeAdjustmentDisabled);
BOOL GetSystemTimeAdjustment(PDWORD lpTimeAdjustment, PDWORD lpTimeIncrement, PBOOL lpTimeAdjustmentDisabled);
void GetSystemTimeAsFileTime(LPFILETIME lpft);
BOOL SystemTimeToTzSpecificLocalTime(LPTIME_ZONE_INFORMATION lpTimeZoneInformation,
				     LPSYSTEMTIME lpUCT, LPSYSTEMTIME lpst);

#endif

/* PROCESS_INFORMATION */

typedef struct _PROCESS_INFORMATION {
	HANDLE	hProcess;
	HANDLE	hThread;
	DWORD	dwProcessId;
	DWORD	dwThreadId;
} PROCESS_INFORMATION;
typedef PROCESS_INFORMATION		*PPROCESS_INFORMATION;
typedef PROCESS_INFORMATION NEAR	*NPPROCESS_INFORMATION;
typedef PROCESS_INFORMATION FAR		*LPPROCESS_INFORMATION;

/* STARTUP_INFO */

#ifndef UNICODE_ONLY
typedef struct _STARTUPINFOA {
	DWORD	cb;
	LPSTR	lpReserved;
	LPSTR	lpDesktop;
	LPSTR	lpTitle;
	DWORD	dwX;
	DWORD	dwY;
	DWORD	dwXSize;
	DWORD	dwYSize;
	DWORD	dwXCountChars;
	DWORD	dwYCountChars;
	DWORD	dwFillAttribute;
	DWORD	dwFlags;
	WORD	wShowWindow;
	WORD	cbReserved2;
	LPBYTE	lpReserved2;
	HANDLE	hStdInput;
	HANDLE	hStdOutput;
	HANDLE	hStdError;
} STARTUPINFOA;
typedef STARTUPINFOA		*PSTARTUPINFOA;
typedef STARTUPINFOA NEAR	*NPSTARTUPINFOA;
typedef STARTUPINFOA FAR	*LPSTARTUPINFOA;
#endif	/* UNICODE_ONLY */

#ifndef ANSI_ONLY
typedef struct _STARTUPINFOW {
	DWORD	cb;
	LPWSTR	lpReserved;
	LPWSTR	lpDesktop;
	LPWSTR	lpTitle;
	DWORD	dwX;
	DWORD	dwY;
	DWORD	dwXSize;
	DWORD	dwYSize;
	DWORD	dwXCountChars;
	DWORD	dwYCountChars;
	DWORD	dwFillAttribute;
	DWORD	dwFlags;
	WORD	wShowWindow;
	WORD	cbReserved2;
	LPBYTE	lpReserved2;
	HANDLE	hStdInput;
	HANDLE	hStdOutput;
	HANDLE	hStdError;
} STARTUPINFOW;
typedef STARTUPINFOW		*PSTARTUPINFOW;
typedef STARTUPINFOW NEAR	*NPSTARTUPINFOW;
typedef STARTUPINFOW FAR	*LPSTARTUPINFOW;
#endif	/* ANSI_ONLY */

#ifdef UNICODE
typedef STARTUPINFOW		STARTUPINFO;
typedef NPSTARTUPINFOW		NPSTARTUPINFO;
typedef LPSTARTUPINFOW		LPSTARTUPINFO;
#else
typedef STARTUPINFOA		STARTUPINFO;
typedef NPSTARTUPINFOA		NPSTARTUPINFO;
typedef LPSTARTUPINFOA		LPSTARTUPINFO;
#endif	/* UNICODE */

/* OSVERSIONINFO */

#ifndef UNICODE_ONLY
typedef struct _OSVERSIONINFOA {
	DWORD	dwOSVersionInfoSize;
	DWORD	dwMajorVersion;
	DWORD	dwMinorVersion;
	DWORD	dwBuildNumber;
	DWORD	dwPlatformId;
	CHAR	szCSDVersion[128];
} OSVERSIONINFOA;
typedef OSVERSIONINFOA		*POSVERSIONINFOA;
typedef OSVERSIONINFOA NEAR	*NPOSVERSIONINFOA;
typedef OSVERSIONINFOA FAR	*LPOSVERSIONINFOA;
#endif	/* UNICODE_ONLY */

#ifndef ANSI_ONLY
typedef struct _OSVERSIONINFOW {
	DWORD	dwOSVersionInfoSize;
	DWORD	dwMajorVersion;
	DWORD	dwMinorVersion;
	DWORD	dwBuildNumber;
	DWORD	dwPlatformId;
	WCHAR	szCSDVersion[128];
} OSVERSIONINFOW;
typedef OSVERSIONINFOW		*POSVERSIONINFOW;
typedef OSVERSIONINFOW NEAR	*NPOSVERSIONINFOW;
typedef OSVERSIONINFOW FAR	*LPOSVERSIONINFOW;
#endif	/* ANSI_ONLY */

#ifdef UNICODE
typedef OSVERSIONINFOW		OSVERSIONINFO;
typedef POSVERSIONINFOW		POSVERSIONINFO;
typedef NPOSVERSIONINFOW 	NPOSVERSIONINFO;
typedef LPOSVERSIONINFOW	LPOSVERSIONINFO;
#else
typedef OSVERSIONINFOA		OSVERSIONINFO;
typedef POSVERSIONINFOA		POSVERSIONINFO;
typedef NPOSVERSIONINFOA	NPOSVERSIONINFO;
typedef LPOSVERSIONINFOA	LPOSVERSIONINFO;
#endif	/* UNICODE */

/* dwPlatformId member of OSVERSIONINFO */

#define VER_PLATFORM_WIN32s		0
#define VER_PLATFORM_WIN32_WINDOWS	1
#define VER_PLATFORM_WIN32_NT		2

/* SYSTEM_INFO */

typedef struct _SYSTEM_INFO {
    union {
        DWORD		dwOemId;	/* obsolete */
        struct {
            WORD	wProcessorArchitecture;
            WORD	wReserved;
        } used;
    } noname;
    DWORD		dwPageSize;
    LPVOID		lpMinimumApplicationAddress;
    LPVOID		lpMaximumApplicationAddress;
    DWORD		dwActiveProcessorMask;
    DWORD		dwNumberOfProcessors;
    DWORD		dwProcessorType;
    DWORD		dwAllocationGranularity;
    WORD		wProcessorLevel;
    WORD		wProcessorRevision;
} SYSTEM_INFO;
typedef SYSTEM_INFO		*PSYSTEM_INFO;
typedef SYSTEM_INFO NEAR	*NPSYSTEM_INFO;
typedef SYSTEM_INFO FAR		*LPSYSTEM_INFO;

BOOL
CreateProcess(
    LPCSTR lpApplicationName,
    LPSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCSTR lpCurrentDirectory,
    LPSTARTUPINFOA lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
    );

VOID WINAPI ExitProcess(UINT uExitCode);

LPTSTR WINAPI GetCommandLine(VOID);

/* THREAD_PRIORITY */

#define THREAD_PRIORITY_NORMAL		0
#define THREAD_PRIORITY_LOWEST		(THREAD_BASE_PRIORITY_MIN)
#define THREAD_PRIORITY_HIGHEST		(THREAD_BASE_PRIORITY_MAX)
#define THREAD_PRIORITY_BELOW_NORMAL	(THREAD_PRIORITY_LOWEST + 1)
#define THREAD_PRIORITY_ABOVE_NORMAL	(THREAD_PRIORITY_HIGHEST - 1)
#define THREAD_PRIORITY_ERROR_RETURN	(MAXLONG)

#define THREAD_PRIORITY_TIME_CRITICAL	(THREAD_BASE_PRIORITY_LOWRT)
#define THREAD_PRIORITY_IDLE		(THREAD_BASE_PRIORITY_IDLE)

#define THREAD_BASE_PRIORITY_LOWRT	15
#define THREAD_BASE_PRIORITY_MAX	2
#define THREAD_BASE_PRIORITY_MIN	-2
#define THREAD_BASE_PRIORITY_IDLE	-15

/* CRITICAL_SECTION */

typedef RTL_CRITICAL_SECTION		CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION		PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION		LPCRITICAL_SECTION;

/* FORMAT_MESSAGE */

#define FORMAT_MESSAGE_ALLOCATE_BUFFER	0x00000100
#define FORMAT_MESSAGE_IGNORE_INSERTS	0x00000200
#define FORMAT_MESSAGE_FROM_STRING	0x00000400
#define FORMAT_MESSAGE_FROM_HMODULE	0x00000800
#define FORMAT_MESSAGE_FROM_SYSTEM	0x00001000
#define FORMAT_MESSAGE_ARGUMENT_ARRAY	0x00002000
#define FORMAT_MESSAGE_MAX_WIDTH_MASK	0x000000FF

/* TIMEOUT */

#define INFINITE			0xFFFFFFFF

BOOL WINAPI Beep( DWORD dwFreq, DWORD dwDuration );

#if LATER
#define PAGE_NOACCESS           0x01
#define PAGE_READONLY           0x02
#define PAGE_READWRITE          0x04
#define PAGE_WRITECOPY          0x08
#define PAGE_EXECUTE            0x10
#define PAGE_EXECUTE_READ       0x20
#define PAGE_EXECUTE_READWRITE  0x40
#define PAGE_EXECUTE_WRITECOPY  0x80
#define PAGE_GUARD              0x100
#define PAGE_NOCACHE            0x200

#define MEM_COMMIT              0x00001000
#define MEM_RESERVE             0x00002000
#define MEM_DECOMMIT            0x00004000
#define MEM_RELEASE             0x00008000
#define MEM_FREE                0x00010000
#define MEM_PRIVATE             0x00020000
#define MEM_MAPPED              0x00040000
#define MEM_TOP_DOWN            0x00100000
#endif

#define	MAX_COMPUTERNAME_LENGTH	15

/* System Information **********************************************/
#ifndef NOAPIPROTO
DWORD   WINAPI GetVersion(void);
BOOL	WINAPI GetVersionEx(LPOSVERSIONINFO);

DWORD   WINAPI GetFreeSpace(UINT);
UINT    WINAPI GetCurrentPDB(void);

UINT    WINAPI GetWindowsDirectory(LPSTR, UINT);
UINT    WINAPI GetSystemDirectory(LPSTR, UINT);

UINT    WINAPI GetFreeSystemResources(UINT);

DWORD   WINAPI GetWinFlags(void);

LPSTR   WINAPI GetDOSEnvironment(void);

DWORD   WINAPI GetCurrentTime(void);
DWORD   WINAPI GetTickCount(void);
DWORD   WINAPI GetTimerResolution(void);

#endif      /* NOAPIPROTO */

/* GetFreeSystemResources flags */
#define GFSR_SYSTEMRESOURCES   0x0000
#define GFSR_GDIRESOURCES      0x0001
#define GFSR_USERRESOURCES     0x0002

/* Window Flags (GetWinFlags())*/
#define WF_PMODE    0x0001
#define WF_CPU286   0x0002
#define WF_CPU386   0x0004
#define WF_CPU486   0x0008
#define WF_STANDARD 0x0010
#define WF_WIN286   0x0010
#define WF_ENHANCED 0x0020
#define WF_WIN386   0x0020
#define WF_CPU086   0x0040
#define WF_CPU186   0x0080
#define WF_LARGEFRAME   0x0100
#define WF_SMALLFRAME   0x0200
#define WF_80x87    0x0400
#define WF_PAGING   0x0800
#define WF_WLO          0x8000


/* Error logging, debugging ****************************************/
#ifndef NOLOGERROR

/* Debugging support (DEBUG SYSTEM ONLY) */
typedef struct tagWINDEBUGINFO
{
    UINT    flags;
    DWORD   dwOptions;
    DWORD   dwFilter;
    char    achAllocModule[8];
    DWORD   dwAllocBreak;
    DWORD   dwAllocCount;
} WINDEBUGINFO;

#ifndef NOAPIPROTO
void    WINAPI LogError(UINT err, void FAR* lpInfo);
void    WINAPI LogParamError(UINT err, FARPROC lpfn, void FAR* param);

BOOL    WINAPI GetWinDebugInfo(WINDEBUGINFO FAR* lpwdi, UINT flags);
BOOL    WINAPI SetWinDebugInfo(const WINDEBUGINFO FAR* lpwdi);

void    FAR _cdecl DebugOutput(UINT flags, LPCSTR lpsz, ...);
#endif      /* NOAPIPROTO */


/* Error modifier bits from LogError() and LogParamError()*/
#define ERR_WARNING     0x8000
#define ERR_PARAM       0x4000

#define ERR_SIZE_MASK       0x3000
#define ERR_BYTE                0x1000
#define ERR_WORD                0x2000
#define ERR_DWORD               0x3000

/* Generic parameter values, from LogParamError() */
#define ERR_BAD_VALUE           0x6001
#define ERR_BAD_FLAGS           0x6002
#define ERR_BAD_INDEX           0x6003
#define ERR_BAD_DVALUE      0x7004
#define ERR_BAD_DFLAGS      0x7005
#define ERR_BAD_DINDEX      0x7006
#define ERR_BAD_PTR     0x7007
#define ERR_BAD_FUNC_PTR    0x7008
#define ERR_BAD_SELECTOR        0x6009
#define ERR_BAD_STRING_PTR  0x700a
#define ERR_BAD_HANDLE          0x600b

/* KERNEL parameter errors, from LogParamError() */
#define ERR_BAD_HINSTANCE       0x6020
#define ERR_BAD_HMODULE         0x6021
#define ERR_BAD_GLOBAL_HANDLE   0x6022
#define ERR_BAD_LOCAL_HANDLE    0x6023
#define ERR_BAD_ATOM            0x6024
#define ERR_BAD_HFILE           0x6025

/* USER parameter errors, from LogParamError() */
#define ERR_BAD_HWND            0x6040
#define ERR_BAD_HMENU           0x6041
#define ERR_BAD_HCURSOR         0x6042
#define ERR_BAD_HICON           0x6043
#define ERR_BAD_HDWP            0x6044
#define ERR_BAD_CID             0x6045
#define ERR_BAD_HDRVR           0x6046

/* GDI parameter errors, from LogParamError() */
#define ERR_BAD_COORDS      0x7060
#define ERR_BAD_GDI_OBJECT      0x6061
#define ERR_BAD_HDC             0x6062
#define ERR_BAD_HPEN            0x6063
#define ERR_BAD_HFONT           0x6064
#define ERR_BAD_HBRUSH          0x6065
#define ERR_BAD_HBITMAP         0x6066
#define ERR_BAD_HRGN            0x6067
#define ERR_BAD_HPALETTE        0x6068
#define ERR_BAD_HMETAFILE       0x6069


/* KERNEL errors, from LogError() */
#define ERR_GALLOC              0x0001
#define ERR_GREALLOC            0x0002
#define ERR_GLOCK               0x0003
#define ERR_LALLOC              0x0004
#define ERR_LREALLOC            0x0005
#define ERR_LLOCK               0x0006
#define ERR_ALLOCRES            0x0007
#define ERR_LOCKRES             0x0008
#define ERR_LOADMODULE          0x0009

/* USER errors, from LogError() */
#define ERR_CREATEDLG           0x0040
#define ERR_CREATEDLG2          0x0041
#define ERR_REGISTERCLASS       0x0042
#define ERR_DCBUSY              0x0043
#define ERR_CREATEWND           0x0044
#define ERR_STRUCEXTRA          0x0045
#define ERR_LOADSTR             0x0046
#define ERR_LOADMENU            0x0047
#define ERR_NESTEDBEGINPAINT    0x0048
#define ERR_BADINDEX            0x0049
#define ERR_CREATEMENU          0x004a

/* GDI errors, from LogError() */
#define ERR_CREATEDC            0x0080
#define ERR_CREATEMETA          0x0081
#define ERR_DELOBJSELECTED      0x0082
#define ERR_SELBITMAP           0x0083


/* WINDEBUGINFO flags values */
#define WDI_OPTIONS         0x0001
#define WDI_FILTER          0x0002
#define WDI_ALLOCBREAK      0x0004

/* dwOptions values */
#define DBO_CHECKHEAP       0x0001
#define DBO_BUFFERFILL      0x0004
#define DBO_DISABLEGPTRAPPING 0x0010
#define DBO_CHECKFREE       0x0020

#define DBO_SILENT          0x8000

#define DBO_TRACEBREAK      0x2000
#define DBO_WARNINGBREAK    0x1000
#define DBO_NOERRORBREAK    0x0800
#define DBO_NOFATALBREAK    0x0400
#define DBO_INT3BREAK       0x0100

/* DebugOutput flags values */
#define DBF_TRACE           0x0000
#define DBF_WARNING         0x4000
#define DBF_ERROR           0x8000
#define DBF_FATAL           0xc000

/* dwFilter values */
#define DBF_KERNEL          0x1000
#define DBF_KRN_MEMMAN      0x0001
#define DBF_KRN_LOADMODULE  0x0002
#define DBF_KRN_SEGMENTLOAD 0x0004
#define DBF_USER            0x0800
#define DBF_GDI             0x0400
#define DBF_MMSYSTEM        0x0040
#define DBF_PENWIN          0x0020
#define DBF_APPLICATION     0x0008
#define DBF_DRIVER          0x0010

#endif      /* NOLOGERROR */

BOOL    WINAPI ExitWindows(DWORD dwReturnCode, UINT wReserved);

/* Error handling **************************************************/
#ifndef NOAPIPROTO

void    WINAPI FatalExit(int);
void    WINAPI FatalAppExit(UINT, LPCSTR);

BOOL    WINAPI ExitWindowsExec(LPCSTR, LPCSTR);

void    WINAPI DebugBreak(void);
void    WINAPI OutputDebugString(LPCSTR);

UINT    WINAPI SetErrorMode(UINT);

#endif      /* NOAPIPROTO */


#define EW_RESTARTWINDOWS 0x42
#define EW_REBOOTSYSTEM   0x43

/* Flags for SetErrorMode() */
#define SEM_FAILCRITICALERRORS  0x0001
#define SEM_NOGPFAULTERRORBOX   0x0002
#define SEM_NOOPENFILEERRORBOX  0x8000


/* Pointer validation **********************************************/
#ifndef NOAPIPROTO

BOOL    WINAPI IsBadReadPtr(const void FAR* lp, UINT cb);
BOOL    WINAPI IsBadWritePtr(void FAR* lp, UINT cb);
BOOL    WINAPI IsBadHugeReadPtr(const void _huge* lp, DWORD cb);
BOOL    WINAPI IsBadHugeWritePtr(void _huge* lp, DWORD cb);
BOOL    WINAPI IsBadCodePtr(FARPROC lpfn);
BOOL    WINAPI IsBadStringPtr(const void FAR* lpsz, UINT cchMax);
BOOL    WINAPI IsBadStringPtrA(const void FAR* lpsz, UINT cchMax);
BOOL    WINAPI IsBadStringPtrW(const void FAR* lpsz, UINT cchMax);

#endif      /* NOAPIPROTO */


/* Profiling support ***********************************************/
#ifndef NOPROFILER
#ifndef NOAPIPROTO

int     WINAPI ProfInsChk(void);
void    WINAPI ProfSetup(int,int);
void    WINAPI ProfSampRate(int,int);
void    WINAPI ProfStart(void);
void    WINAPI ProfStop(void);
void    WINAPI ProfClear(void);
void    WINAPI ProfFlush(void);
void    WINAPI ProfFinish(void);

#endif      /* NOAPIPROTO */
#endif      /* NOPROFILER */


/* Stack management ************************************************/
typedef int CATCHBUF[9];
typedef int FAR* LPCATCHBUF;

int     TWIN_Catch(LPBYTE, int);


#ifdef WINMALLOC_CHECK

LPVOID	WinMallocCheck(unsigned int, char *,int,int,int);
LPVOID	WinReallocCheck(LPVOID,unsigned int, char *,int,int,int);
void	WinFreeCheck(LPVOID,char *,int);

#define WinMalloc(s) 	WinMallocCheck(s,__FILE__,__LINE__,0,0)
#define WinFree(l)	WinFreeCheck(l,__FILE__,__LINE__)
#define WinRealloc(l,s)	WinReallocCheck(l,s,__FILE__,__LINE__,0,0)

#define WINMALLOC(s,t,h)    WinMallocCheck(s,__FILE__,__LINE__,t,h)
#define WINFREE(l) 	    WinFreeCheck(l,__FILE__,__LINE__)
#define WINREALLOC(l,s,t,h) WinReallocCheck(l,s,__FILE__,__LINE__,t,h)
#else

LPVOID WinMalloc(unsigned int);
void  WinFree(LPVOID);
LPVOID WinRealloc(LPVOID,unsigned int);

#define WINMALLOC(s,t,h)    WinMalloc(s)
#define WINFREE(l) 	    WinFree(l)
#define WINREALLOC(l,s,t,h) WinRealloc(l,s)
#endif

LPSTR WinStrdup(LPCSTR);

#if defined(Linux)
#define TWIN_jmp_buf __jmp_buf
#else
#define TWIN_jmp_buf jmp_buf
#endif
#define Catch(c) TWIN_Catch((LPBYTE)(c), \
			    setjmp((*(TWIN_jmp_buf *)((void *)(c = \
				   (int *)WinMalloc(sizeof(TWIN_jmp_buf)))))))

#ifndef NOAPIPROTO

void    WINAPI Throw(const int FAR*, int);
void    WINAPI SwitchStackBack(void);
void    WINAPI SwitchStackTo(UINT, UINT, UINT);

#endif      /* NOAPIPROTO */


/* Module management ***********************************************/
#define HINSTANCE_ERROR ((HINSTANCE)32)

typedef struct tagSEGINFO
{
    UINT offSegment;
    UINT cbSegment;
    UINT flags;
    UINT cbAlloc;
    HGLOBAL h;
    UINT alignShift;
    UINT reserved[2];
} SEGINFO;
typedef SEGINFO FAR* LPSEGINFO;


#ifndef NOAPIPROTO

HINSTANCE   WINAPI LoadModule(LPCSTR, LPVOID);
BOOL        WINAPI FreeModule(HINSTANCE);

HINSTANCE   WINAPI LoadLibrary(LPCSTR);
void        WINAPI FreeLibrary(HINSTANCE);

UINT        WINAPI WinExec(LPCSTR, UINT);

HMODULE     WINAPI GetModuleHandle(LPCSTR);

int         WINAPI GetModuleUsage(HINSTANCE);
#ifdef TWIN32
DWORD         WINAPI GetModuleFileName(HMODULE, LPTSTR, DWORD);
#else
int         WINAPI GetModuleFileName(HINSTANCE, LPSTR, int);
#endif

FARPROC     WINAPI GetProcAddress(HINSTANCE, LPCSTR);

int         WINAPI GetInstanceData(HINSTANCE, BYTE*, int);

HGLOBAL     WINAPI GetCodeHandle(FARPROC);

void    WINAPI GetCodeInfo(FARPROC lpProc, SEGINFO FAR* lpSegInfo);

FARPROC WINAPI MakeProcInstance(FARPROC, HINSTANCE);
void    WINAPI FreeProcInstance(FARPROC);

LONG    WINAPI SetSwapAreaSize(UINT);
void    WINAPI SwapRecording(UINT);
void    WINAPI ValidateCodeSegments(void);

#endif      /* NOAPIPROTO */

/* Windows Exit Procedure flag values */
#define WEP_SYSTEM_EXIT 1
#define WEP_FREE_DLL    0



/* Task management *************************************************/

#ifndef NOAPIPROTO

UINT    WINAPI GetNumTasks(void);
BOOL    WINAPI IsTask(HTASK);

HTASK   WINAPI GetCurrentTask(void);

void    WINAPI Yield(void);
void    WINAPI DirectedYield(HTASK);

#endif      /* NOAPIPROTO */

/* Thread management ***********************************************/

#ifndef NOAPIPROTO

HANDLE	WINAPI GetCurrentProcess(void);
HANDLE	WINAPI GetCurrentThread(void);
DWORD	WINAPI GetCurrentThreadId(void);

#endif      /* NOAPIPROTO */

/* Global memory management ****************************************/
#ifndef NOMEMMGR

#ifdef STRICT
typedef BOOL (CALLBACK* GNOTIFYPROC)(HGLOBAL);
#else
typedef FARPROC GNOTIFYPROC;
#endif

/* API Prototypes **************/
#ifndef NOAPIPROTO

#define GlobalDiscard(h)    GlobalReAlloc(h, 0L, GMEM_MOVEABLE)

HGLOBAL WINAPI GlobalAlloc(UINT, DWORD);
HGLOBAL WINAPI GlobalReAlloc(HGLOBAL, DWORD, UINT);
HGLOBAL WINAPI GlobalFree(HGLOBAL);

DWORD   WINAPI GlobalDosAlloc(DWORD);
UINT    WINAPI GlobalDosFree(UINT);

#ifdef STRICT
void FAR* WINAPI GlobalLock(HGLOBAL);
#else
char FAR* WINAPI GlobalLock(HGLOBAL);
#endif

BOOL    WINAPI GlobalUnlock(HGLOBAL);

DWORD   WINAPI GlobalSize(HGLOBAL);

HGLOBAL	WINAPI GlobalHandle(LPCVOID);
HGLOBAL GlobalHandle32(LPCVOID);

UINT    WINAPI GlobalFlags(HGLOBAL);

#ifdef STRICT
void FAR* WINAPI GlobalWire(HGLOBAL);
#else
char FAR* WINAPI GlobalWire(HGLOBAL);
#endif

BOOL    WINAPI GlobalUnWire(HGLOBAL);

UINT    WINAPI GlobalPageLock(HGLOBAL);
UINT    WINAPI GlobalPageUnlock(HGLOBAL);

void    WINAPI GlobalFix(HGLOBAL);
void    WINAPI GlobalUnfix(HGLOBAL);

HGLOBAL WINAPI GlobalLRUNewest(HGLOBAL);
HGLOBAL WINAPI GlobalLRUOldest(HGLOBAL);

DWORD   WINAPI GlobalCompact(DWORD);

void    WINAPI GlobalNotify(GNOTIFYPROC);

HGLOBAL WINAPI LockSegment(UINT);
void    WINAPI UnlockSegment(UINT);

#define LockData(dummy)     LockSegment((UINT)-1)
#define UnlockData(dummy)   UnlockSegment((UINT)-1)

UINT    WINAPI AllocSelector(UINT);
UINT    WINAPI FreeSelector(UINT);
UINT    WINAPI AllocDStoCSAlias(UINT);
UINT    WINAPI PrestoChangoSelector(UINT sourceSel, UINT destSel);
DWORD   WINAPI GetSelectorBase(UINT);
UINT    WINAPI SetSelectorBase(UINT, DWORD);
DWORD   WINAPI GetSelectorLimit(UINT);
UINT    WINAPI SetSelectorLimit(UINT, DWORD);

void    WINAPI LimitEmsPages(DWORD);

void    WINAPI ValidateFreeSpaces(void);

/* Local Memory Management */
#define LocalDiscard     GlobalDiscard

#define LocalLock    GlobalLock
#define LocalUnlock  GlobalUnlock
#define LocalFree    GlobalFree
#define LocalAlloc   GlobalAlloc
#define LocalReAlloc GlobalReAlloc
#define LocalSize    GlobalSize

#define LocalHandle  GlobalHandle
#define LocalFlags   GlobalFlags
#define LocalCompact GlobalCompact
#define LocalShrink  GlobalShrink


#endif      /* NOAPIPROTO */


/* Flags and definitions *******/

/* Global Memory Flags */

#define GMEM_FIXED          0x0000
#define GMEM_MOVEABLE       0x0002
#define GMEM_NOCOMPACT      0x0010
#define GMEM_NODISCARD      0x0020
#define GMEM_ZEROINIT       0x0040
#define GMEM_MODIFY         0x0080
#define GMEM_DISCARDABLE    0x0100
#define GMEM_NOT_BANKED     0x1000
#define GMEM_SHARE          0x2000
#define GMEM_DDESHARE       0x2000
#define GMEM_NOTIFY         0x4000
#define GMEM_LOWER          GMEM_NOT_BANKED
#define GMEM_VALID_FLAGS    0x7F72
#define GMEM_INVALID_HANDLE 0x8000

#define GHND            (GMEM_MOVEABLE | GMEM_ZEROINIT)
#define GPTR            (GMEM_FIXED | GMEM_ZEROINIT)

/* Flags returned from GlobalFlags() */
/* (in addition to GMEM_DISCARDABLE) */
#define GMEM_DISCARDED      0x4000
#define GMEM_LOCKCOUNT      0x00FF

/* Low system memory notification message */
#define WM_COMPACTING       0x0041


/* Local memory management flags */
#define LMEM_FIXED          0x0000
#define LMEM_MOVEABLE       0x0002
#define LMEM_NOCOMPACT      0x0010
#define LMEM_NODISCARD      0x0020
#define LMEM_ZEROINIT       0x0040
#define LMEM_MODIFY         0x0080
#define LMEM_DISCARDABLE    0x0F00

#define LHND            (LMEM_MOVEABLE | LMEM_ZEROINIT)
#define LPTR            (LMEM_FIXED | LMEM_ZEROINIT)

#define NONZEROLHND     (LMEM_MOVEABLE)
#define NONZEROLPTR     (LMEM_FIXED)

/* Flags returned from LocalFlags() */
/* (in addition to LMEM_DISCARDABLE) */
#define LMEM_DISCARDED      0x4000
#define LMEM_LOCKCOUNT      0x00FF

#endif      /* NOMEMMGR */


/* File I/O Support ************************************************/

#ifndef NOAPIPROTO

#ifdef TWIN32
UINT    TWINAPI GetDriveType16(int);
int	TWINAPI GetTempFileName16(BYTE, LPCSTR, UINT, LPSTR);
#else
UINT    WINAPI GetDriveType(int);
int     WINAPI GetTempFileName(BYTE, LPCSTR, UINT, LPSTR);
BYTE    WINAPI GetTempDrive(char);
#endif

UINT    WINAPI SetHandleCount(UINT);

#endif      /*NOAPIPROTO */

/* GetTempFileName() Flags */
#define TF_FORCEDRIVE       (BYTE)0x80

/* GetDriveType return values */
#define DRIVE_UNKNOWN   0
#define DRIVE_NO_ROOT_DIR 1
#define DRIVE_REMOVABLE 2
#define DRIVE_FIXED     3
#define DRIVE_REMOTE    4
#define DRIVE_CDROM     5
#define DRIVE_RAMDISK   6


#ifndef NOLFILEIO

typedef UINT HFILE;      /* Polymorphic with C runtime file handle type */
#define HFILE_ERROR ((HFILE)-1)


#ifndef NOAPIPROTO

HFILE   WINAPI _lopen(LPCSTR, int);
HFILE   WINAPI _lcreat(LPCSTR, int);
int	TWINAPI _ldelete(LPCSTR);

HFILE   WINAPI _lclose(HFILE);

LONG    WINAPI _llseek(HFILE, LONG, int);

UINT    WINAPI _lread(HFILE, void _huge*, UINT);
UINT    WINAPI _lwrite(HFILE, const void _huge*, UINT);

long    WINAPI _hread(HFILE, void _huge*, long);
long    WINAPI _hwrite(HFILE, const void _huge*, long);

#endif      /* NOAPIPROTO */


/* _lopen() flags */
#define READ        0
#define WRITE       1
#define READ_WRITE  2

/* _llseek origin values */
#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif


/* File Input ******************************************************/
#ifndef NOOPENFILE

/* OpenFile() Structure */
typedef struct tagOFSTRUCT
{
    BYTE cBytes;
    BYTE fFixedDisk;
    UINT nErrCode;
    BYTE reserved[4];
    char szPathName[128];
} OFSTRUCT;
typedef OFSTRUCT*       POFSTRUCT;
typedef OFSTRUCT NEAR* NPOFSTRUCT;
typedef OFSTRUCT FAR*  LPOFSTRUCT;


/* API Prototypes **************/
#ifndef NOAPIPROTO

HFILE   WINAPI OpenFile(LPCSTR, OFSTRUCT FAR*, UINT);

#endif      /* NOAPIPROTO */


/* Flags and definitions *******/

/* Flags for OpenFile() */
#define OF_READ         0x0000
#define OF_WRITE        0x0001
#define OF_READWRITE        0x0002
#define OF_SHARE_COMPAT     0x0000
#define OF_SHARE_EXCLUSIVE  0x0010
#define OF_SHARE_DENY_WRITE 0x0020
#define OF_SHARE_DENY_READ  0x0030
#define OF_SHARE_DENY_NONE  0x0040
#define OF_PARSE        0x0100
#define OF_DELETE       0x0200
#define OF_VERIFY       0x0400      /* Used with OF_REOPEN */
#define OF_SEARCH       0x0400      /* Used without OF_REOPEN */
#define OF_CANCEL       0x0800
#define OF_CREATE       0x1000
#define OF_PROMPT       0x2000
#define OF_EXIST        0x4000
#define OF_REOPEN       0x8000


#endif  /* NOOPENFILE */

#endif  /* NOLFILEIO */


/* Network support *************************************************/
#ifndef NOAPIPROTO

UINT WINAPI WNetAddConnection(LPSTR, LPSTR, LPSTR);
UINT WINAPI WNetGetConnection(LPSTR, LPSTR, UINT FAR*);
UINT WINAPI WNetCancelConnection(LPSTR, BOOL);

#endif      /* NOAPIPROTO */

/* Errors */
#define WN_SUCCESS              0x0000
#define WN_NOT_SUPPORTED        0x0001
#define WN_NET_ERROR            0x0002
#define WN_MORE_DATA            0x0003
#define WN_BAD_POINTER          0x0004
#define WN_BAD_VALUE            0x0005
#define WN_BAD_PASSWORD         0x0006
#define WN_ACCESS_DENIED        0x0007
#define WN_FUNCTION_BUSY        0x0008
#define WN_WINDOWS_ERROR        0x0009
#define WN_BAD_USER             0x000A
#define WN_OUT_OF_MEMORY        0x000B
#define WN_CANCEL               0x000C
#define WN_CONTINUE             0x000D

/* Connection errors */
#define WN_NOT_CONNECTED        0x0030
#define WN_OPEN_FILES           0x0031
#define WN_BAD_NETNAME          0x0032
#define WN_BAD_LOCALNAME        0x0033
#define WN_ALREADY_CONNECTED    0x0034
#define WN_DEVICE_ERROR         0x0035
#define WN_CONNECTION_CLOSED    0x0036


/* Resource management *********************************************/
#define MAKEINTRESOURCE(i)  ((LPCSTR)MAKELP(0, (i)))

#ifdef STRICT
typedef HGLOBAL (CALLBACK* RSRCHDLRPROC)(HGLOBAL, HINSTANCE, HRSRC);
#else
typedef FARPROC RSRCHDLRPROC;
#endif

#ifndef NOAPIPROTO

HRSRC   WINAPI FindResource(HINSTANCE, LPCSTR, LPCSTR);
HGLOBAL WINAPI LoadResource(HINSTANCE, HRSRC);
BOOL    WINAPI FreeResource(HGLOBAL);

HANDLE  WINAPI LoadImage(HINSTANCE, LPCSTR, UINT, int, int, UINT);

#ifdef STRICT
void FAR* WINAPI LockResource(HGLOBAL);
#else
char FAR* WINAPI LockResource(HGLOBAL);
#endif

#define     UnlockResource(h)       GlobalUnlock(h)

DWORD   WINAPI SizeofResource(HINSTANCE, HRSRC);

int     WINAPI AccessResource(HINSTANCE, HRSRC);

HGLOBAL WINAPI AllocResource(HINSTANCE, HRSRC, DWORD);

RSRCHDLRPROC WINAPI SetResourceHandler(HINSTANCE, LPCSTR, RSRCHDLRPROC);

#endif      /* NOAPIPROTO */


#ifndef NORESOURCE

/* Predefined Resource Types */
#define RT_CURSOR       MAKEINTRESOURCE(1)
#define RT_BITMAP       MAKEINTRESOURCE(2)
#define RT_ICON         MAKEINTRESOURCE(3)
#define RT_MENU         MAKEINTRESOURCE(4)
#define RT_DIALOG       MAKEINTRESOURCE(5)
#define RT_STRING       MAKEINTRESOURCE(6)
#define RT_FONTDIR      MAKEINTRESOURCE(7)
#define RT_FONT         MAKEINTRESOURCE(8)
#define RT_ACCELERATOR  MAKEINTRESOURCE(9)
#define RT_RCDATA       MAKEINTRESOURCE(10)

#define RT_GROUP_CURSOR MAKEINTRESOURCE(12)
#define RT_GROUP_ICON   MAKEINTRESOURCE(14)


#endif  /* NORESOURCE */


#ifdef OEMRESOURCE

/* OEM Resource Ordinal Numbers */
#define OBM_CLOSE           32754
#define OBM_UPARROW         32753
#define OBM_DNARROW         32752
#define OBM_RGARROW         32751
#define OBM_LFARROW         32750
#define OBM_REDUCE          32749
#define OBM_ZOOM            32748
#define OBM_RESTORE         32747
#define OBM_REDUCED         32746
#define OBM_ZOOMD           32745
#define OBM_RESTORED        32744
#define OBM_UPARROWD        32743
#define OBM_DNARROWD        32742
#define OBM_RGARROWD        32741
#define OBM_LFARROWD        32740
#define OBM_MNARROW         32739
#define OBM_COMBO           32738

#define OBM_UPARROWI        32737
#define OBM_DNARROWI        32736
#define OBM_RGARROWI        32735
#define OBM_LFARROWI        32734

#define OBM_OLD_CLOSE       32767
#define OBM_SIZE            32766
#define OBM_OLD_UPARROW     32765
#define OBM_OLD_DNARROW     32764
#define OBM_OLD_RGARROW     32763
#define OBM_OLD_LFARROW     32762
#define OBM_BTSIZE          32761
#define OBM_CHECK           32760
#define OBM_CHECKBOXES      32759
#define OBM_BTNCORNERS      32758
#define OBM_OLD_REDUCE      32757
#define OBM_OLD_ZOOM        32756
#define OBM_OLD_RESTORE     32755

#define OCR_NORMAL          32512
#define OCR_IBEAM           32513
#define OCR_WAIT            32514
#define OCR_CROSS           32515
#define OCR_UP              32516
#define OCR_SIZE            32640
#define OCR_ICON            32641
#define OCR_SIZENWSE        32642
#define OCR_SIZENESW        32643
#define OCR_SIZEWE          32644
#define OCR_SIZENS          32645
#define OCR_SIZEALL         32646
#define OCR_ICOCUR          32647

#define OIC_SAMPLE          32512
#define OIC_HAND            32513
#define OIC_QUES            32514
#define OIC_BANG            32515
#define OIC_NOTE            32516

#endif  /* OEMRESOURCE */



/* Atom management *************************************************/
#define MAKEINTATOM(i)      ((LPCSTR)MAKELP(0, (i)))

#ifndef NOATOM
#ifndef NOAPIPROTO

BOOL    WINAPI InitAtomTable(int);
ATOM    WINAPI AddAtom(LPCSTR);
ATOM    WINAPI DeleteAtom(ATOM);
ATOM    WINAPI FindAtom(LPCSTR);
UINT    WINAPI GetAtomName(ATOM, LPSTR, int);
ATOM    WINAPI GlobalAddAtom(LPCSTR);
ATOM    WINAPI GlobalDeleteAtom(ATOM);
ATOM    WINAPI GlobalFindAtom(LPCSTR);
UINT    WINAPI GlobalGetAtomName(ATOM, LPSTR, int);
HLOCAL  WINAPI GetAtomHandle(ATOM);

#endif      /* NOAPIPROTO */
#endif      /* NOATOM */



/* win.ini support *************************************************/
#ifndef NOAPIPROTO

/* User Profile Routines */
UINT    WINAPI GetProfileInt(LPCSTR, LPCSTR, int);
int     WINAPI GetProfileString(LPCSTR, LPCSTR, LPCSTR, LPSTR, int);
BOOL    WINAPI WriteProfileString(LPCSTR, LPCSTR, LPCSTR);

UINT    WINAPI GetPrivateProfileInt(LPCSTR, LPCSTR, int, LPCSTR);
int     WINAPI GetPrivateProfileString(LPCSTR, LPCSTR, LPCSTR, LPSTR, int, LPCSTR);
BOOL    WINAPI WritePrivateProfileString(LPCSTR, LPCSTR, LPCSTR, LPCSTR);

#endif      /* NOAPIPROTO */

#define WM_WININICHANGE     0x001A



/* International and character translation support *****************/

#ifndef NOAPIPROTO

void    WINAPI AnsiToOem(const char _huge*, char _huge*);
void    WINAPI OemToAnsi(const char _huge*, char _huge*);

void    WINAPI AnsiToOemBuff(LPCSTR, LPSTR, UINT);
void    WINAPI OemToAnsiBuff(LPCSTR, LPSTR, UINT);

LPSTR   WINAPI AnsiNext(LPCSTR);
LPSTR   WINAPI AnsiPrev(LPCSTR, LPCSTR);

LPSTR   WINAPI AnsiUpper(LPSTR);
LPSTR   WINAPI AnsiLower(LPSTR);

UINT    WINAPI AnsiUpperBuff(LPSTR, UINT);
UINT    WINAPI AnsiLowerBuff(LPSTR, UINT);

#ifndef  NOLANGUAGE
BOOL    WINAPI IsCharAlpha(char);
BOOL    WINAPI IsCharAlphaNumeric(char);
BOOL    WINAPI IsCharUpper(char);
BOOL    WINAPI IsCharLower(char);
#endif

#ifndef NOLSTRING
int     WINAPI lstrcmp(LPCSTR, LPCSTR);
int     WINAPI lstrcmpi(LPCSTR, LPCSTR);
LPSTR   WINAPI lstrcpy(LPSTR, LPCSTR);
LPSTR   WINAPI lstrcat(LPSTR, LPCSTR);
int     WINAPI lstrlen(LPCSTR);

int     WINAPI lstrcmpA(LPCSTR, LPCSTR);
int     WINAPI lstrlenA(LPCSTR);

LPSTR   WINAPI lstrcpyn(LPSTR, LPCSTR, int);
void    WINAPI hmemcpy(void _huge*, const void _huge*, long);

#endif  /* NOLSTRING */

#ifndef NODBCS
BOOL    WINAPI IsDBCSLeadByte(BYTE);
#endif  /* NODBCS */

int     WINAPI LoadString(HINSTANCE, UINT, LPSTR, int);

#endif      /* NOAPIPROTO */



/* Keyboard driver support *****************************************/
#ifndef NOKEYBOARDINFO
#ifndef NOAPIPROTO

DWORD   WINAPI OemKeyScan(UINT);
UINT    WINAPI VkKeyScan(UINT);
int     WINAPI GetKeyboardType(int);
UINT    WINAPI MapVirtualKey(UINT, UINT);
int     WINAPI GetKBCodePage(void);
int     WINAPI GetKeyNameText(LONG, LPSTR, int);
int     WINAPI ToAscii(UINT, UINT, CONST PBYTE, LPWORD, UINT);

#endif      /* NOAPIPROTO */
#endif      /* NOKEYBOARDINFO */

/* (WIN32) Profile support ************************************************* */

#ifndef NOAPIPROTO

DWORD	WINAPI GetPrivateProfileSectionNames(LPTSTR, DWORD, LPCTSTR);
DWORD	WINAPI GetPrivateProfileSection(LPCSTR, LPSTR, DWORD, LPCSTR);
BOOL	WINAPI WritePrivateProfileSection(LPCTSTR, LPCTSTR, LPCTSTR);
BOOL	WINAPI WriteProfileSection(LPCTSTR, LPCTSTR);

BOOL	WINAPI GetPrivateProfileStruct(LPCTSTR, LPCTSTR, LPVOID, UINT,
	LPCTSTR);
BOOL	WINAPI WritePrivateProfileStruct(LPCTSTR, LPCTSTR, LPVOID, UINT,
	LPCTSTR);

#endif  /* NOAPIPROTO */

/* (WIN32) Kernel Error **************************************************** */

#ifndef NOAPIPROTO

DWORD	WINAPI GetLastError(VOID);
VOID	WINAPI SetLastError(DWORD);
VOID	WINAPI SetLastErrorEx(DWORD, DWORD);

#endif	/* NOAPIPROTO */

/* Critical Section ******************************************************** */

#ifndef NOAPIPROTO

VOID	WINAPI InitializeCriticalSection(LPCRITICAL_SECTION );
VOID	WINAPI DeleteCriticalSection(LPCRITICAL_SECTION );
VOID	WINAPI EnterCriticalSection(LPCRITICAL_SECTION );
VOID	WINAPI LeaveCriticalSection(LPCRITICAL_SECTION );
BOOL    WINAPI TryEnterCriticalSection( LPCRITICAL_SECTION );
VOID    WINAPI ReinitializeCriticalSection( LPCRITICAL_SECTION );

#endif	/* NOAPIPROTO */

/* Thread Local Storage **************************************************** */

#ifndef NOAPIPROTO

DWORD	WINAPI TlsAlloc(VOID);
BOOL	WINAPI TlsFree(DWORD dwTlsIndex);
LPVOID	WINAPI TlsGetValue(DWORD dwTlsIndex);
BOOL	WINAPI TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);

#endif	/* NOAPIPROTO */

/* Interlocked Variable **************************************************** */

#ifndef NOAPIPROTO

LONG	WINAPI InterlockedDecrement(LPLONG);
LONG	WINAPI InterlockedIncrement(LPLONG);

#endif	/* NOAPIPROTO */

/* (WIN32) Event *********************************************************** */

/* function prototypes */

#ifndef NOAPIPROTO

HANDLE	WINAPI
CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes,
	BOOL bManualReset,
	BOOL bInitialState,
	LPCSTR lpName);

BOOL	WINAPI
ResetEvent(HANDLE hEvent);

BOOL	WINAPI
SetEvent(HANDLE hEvent);

BOOL	WINAPI
PulseEvent(HANDLE hEvent);

#endif	/* NOAPIPROTO */

/* (WIN32) Process Creation ************************************************ */

/* dwCreationFlag values */

#define DEBUG_PROCESS			0x00000001
#define DEBUG_ONLY_THIS_PROCESS		0x00000002
#define CREATE_SUSPENDED		0x00000004
#define DETACHED_PROCESS		0x00000008
#define CREATE_NEW_CONSOLE		0x00000010
#define NORMAL_PRIORITY_CLASS		0x00000020
#define IDLE_PRIORITY_CLASS		0x00000040
#define HIGH_PRIORITY_CLASS		0x00000080
#define REALTIME_PRIORITY_CLASS		0x00000100
#define CREATE_NEW_PROCESS_GROUP	0x00000200
#define CREATE_UNICODE_ENVIRONMENT	0x00000400
#define CREATE_SEPARATE_WOW_VDM		0x00000800
#define CREATE_SHARED_WOW_VDM		0x00001000
#define CREATE_DEFAULT_ERROR_MODE	0x04000000
#define CREATE_NO_WINDOW		0x08000000
#define PROFILE_USER			0x10000000
#define PROFILE_KERNEL			0x20000000
#define PROFILE_SERVER			0x40000000

typedef DWORD (CALLBACK *LPTHREAD_START_ROUTINE)(LPVOID);

HANDLE CreateThread(
  LPSECURITY_ATTRIBUTES  lpattr,  // pointer to security attributes
  DWORD                  dwstack, // initial thread stack size
  LPTHREAD_START_ROUTINE start,   // pointer to thread function
  LPVOID	         arg,     // argument for new thread
  DWORD			 cflags,  // creation flags
  LPDWORD 		 lptid    // pointer to receive thread ID
);
DWORD  SuspendThread(HANDLE handle);
DWORD  ResumeThread(HANDLE handle);
void   TerminateThread(HANDLE ,DWORD );

/* (WIN32) Synchronization ************************************************* */

/* WaitForSingleObject*() return code */

#define WAIT_FAILED		((DWORD)0xFFFFFFFF)
#define WAIT_OBJECT_0		((STATUS_WAIT_0) + 0)
#define WAIT_ABANDONED		((STATUS_ABANDONED_WAIT_0) + 0)
#define WAIT_ABANDONED_0	((STATUS_ABANDONED_WAIT_0) + 0)
#define WAIT_TIMEOUT		STATUS_TIMEOUT
#define WAIT_IO_COMPLETION	STATUS_USER_APC

/* function prototypes */

#ifndef NOAPIPROTO

DWORD	WINAPI
WaitForSingleObject(HANDLE hObject, DWORD dwTimeOut);

DWORD	WINAPI
WaitForSingleObjectEx(HANDLE hObject, DWORD dwTimeOut, BOOL bAlertable);

DWORD	WINAPI
WaitForMultipleObjects(DWORD dwCount, CONST HANDLE *lpHandles,
	BOOL bWaitAll, DWORD dwMilliseconds);

DWORD	WINAPI
WaitForMultipleObjectsEx(DWORD dwCount, CONST HANDLE *lpHandles,
	BOOL bWaitAll, DWORD dwMilliseconds, BOOL bAlertable);

#endif	/* NOAPIPROTO */

/* (WIN32) File Security *************************************************** */

/* function prototypes */

#ifndef NOAPIPROTO

BOOL	WINAPI
GetFileSecurity(LPCSTR lpszFile,
	SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd,
	DWORD dw, LPDWORD lp);

BOOL	WINAPI
SetFileSecurity(LPCSTR lpszFile,
	SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd);

#endif	/* NOAPIPROTO */

/* (WIN32) Mutex *********************************************************** */

/* function prototypes */

#ifndef NOAPIPROTO

HANDLE	WINAPI
CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes,
	BOOL bInitialOwner,
	LPCSTR lpName);

HANDLE	WINAPI
OpenMutex(DWORD dwDesiredAccess,
	BOOL bInheritHandle,
	LPCSTR lpName);

BOOL	WINAPI
ReleaseMutex(HANDLE hMutex);

#endif	/* NOAPIPROTO */

/* (WIN32) Semaphore ******************************************************* */

/* function prototypes */

#ifndef NOAPIPROTO

HANDLE	WINAPI
CreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
	LONG lInitialCount,
	LONG lMaximumCount,
	LPCSTR lpName);

HANDLE	WINAPI
OpenSemaphore(DWORD dwDesiredAccess,
	BOOL bInheritHandle,
	LPCSTR lpName);

BOOL	WINAPI
ReleaseSemaphore(HANDLE hSemaphore,
	LONG lpReleaseCount, LPLONG lpPreviousCount);

#endif	/* NOAPIPROTO */

int	WINAPI
GetThreadPriority(HANDLE hThread);

BOOL	WINAPI
SetThreadPriority(HANDLE hThread, int nPriority);

BOOL	WINAPI
SendNotifyMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);	

DWORD	WINAPI
FormatMessage(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId,
	DWORD dwLanguageId, LPTSTR lpBuffer, DWORD nSize, va_list *Arguments);

/* Application's entry procedure */
#ifdef STRICT
int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
#endif

#endif      /* Win_Base__h */
