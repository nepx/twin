/*  Win_NT.h	1.12
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
 
#ifndef Win_NT__h
#define Win_NT__h

#include <stddef.h>

#define HEAP_NO_SERIALIZE               0x00000001      
#define HEAP_GROWABLE                   0x00000002      
#define HEAP_GENERATE_EXCEPTIONS        0x00000004      
#define HEAP_ZERO_MEMORY                0x00000008      
#define HEAP_REALLOC_IN_PLACE_ONLY      0x00000010      
#define HEAP_TAIL_CHECKING_ENABLED      0x00000020      
#define HEAP_FREE_CHECKING_ENABLED      0x00000040      
#define HEAP_DISABLE_COALESCE_ON_FREE   0x00000080      
#define HEAP_CREATE_ALIGN_16            0x00010000      
#define HEAP_CREATE_ENABLE_TRACING      0x00020000      

#define DLL_PROCESS_ATTACH 1    
#define DLL_THREAD_ATTACH  2    
#define DLL_THREAD_DETACH  3    
#define DLL_PROCESS_DETACH 0    

#define FILE_MAP_COPY			0x00000001
#define FILE_MAP_WRITE			0x00000002
#define FILE_MAP_READ			0x00000004
#define FILE_MAP_ALL_ACCESS		0x0000000F

#define FILE_SHARE_READ			0x00000001
#define FILE_SHARE_WRITE		0x00000002
#define FILE_ATTRIBUTE_READONLY         0x00000001  
#define FILE_ATTRIBUTE_HIDDEN           0x00000002  
#define FILE_ATTRIBUTE_SYSTEM           0x00000004  
#define FILE_ATTRIBUTE_DIRECTORY        0x00000010  
#define FILE_ATTRIBUTE_ARCHIVE          0x00000020  
#define FILE_ATTRIBUTE_NORMAL           0x00000080  
#define FILE_ATTRIBUTE_TEMPORARY        0x00000100  
#define FILE_ATTRIBUTE_COMPRESSED       0x00000800  
#define FILE_CASE_SENSITIVE_SEARCH      0x00000001  
#define FILE_CASE_PRESERVED_NAMES       0x00000002  
#define FILE_UNICODE_ON_DISK            0x00000004  
#define FILE_PERSISTENT_ACLS            0x00000008  
#define FILE_FILE_COMPRESSION           0x00000010  
#define FILE_VOLUME_IS_COMPRESSED       0x00008000  


#define GENERIC_READ                    0x80000000
#define GENERIC_WRITE                   0x40000000
#define GENERIC_EXECUTE                 0x20000000
#define GENERIC_ALL                     0x10000000

/* UNICODE ( Wide Character ) types */
typedef unsigned short  WCHAR;

typedef WCHAR  *PWCHAR;
typedef WCHAR  *LPWCH, *PWCH;
typedef const WCHAR *LPCWCH, *PCWCH;
typedef WCHAR *NWPSTR;
typedef WCHAR *LPWSTR, *PWSTR;

typedef CONST WCHAR *LPCWSTR, *PCWSTR;

/* ANSI ( Multi_byte Character ) types */
typedef CHAR *PCHAR;
typedef CHAR *LPCH, *PCH;

typedef const CHAR *LPCCH, *PCCH;

#define TEXT(quote)	__TEXT(quote)

/* Neutral ANSI/UNICODE types and macros */
#ifdef UNICODE

#ifndef _TCHAR_DEFINED
typedef WCHAR TCHAR, *PTCHAR;
typedef WCHAR TBYTE, *PTBYTE;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

typedef LPWSTR LPTCH, PTCH;
typedef LPWSTR PTSTR, LPTSTR;
typedef LPCWSTR LPCTSTR;
typedef LPWSTR LP;
#define __TEXT(quote) L##quote

#else /* UNICODE */

#ifndef _TCHAR_DEFINED
typedef unsigned char TBYTE, *PTBYTE;
#define _TCHAR_DEFINED
#endif /* _TCHAR_DEFINED */

typedef LPSTR LPTCH, PTCH;
#define __TEXT(quote) quote
#endif /* UNICODE */

/* LCID */

typedef DWORD LCID;
typedef PDWORD PLCID;

/* LIST_ENTRY */

typedef struct _LIST_ENTRY
{
   struct _LIST_ENTRY	*Flink;
   struct _LIST_ENTRY	*Blink;
} LIST_ENTRY;
typedef LIST_ENTRY	*PLIST_ENTRY;
typedef LIST_ENTRY NEAR	*NPLIST_ENTRY;
typedef LIST_ENTRY FAR	*LPLIST_ENTRY;

/* CRITICAL_SECTION */

#define RTL_CRITSECT_TYPE	0
#define RTL_RESOURCE_TYPE	1

typedef struct _RTL_CRITICAL_SECTION
{
    void       *RTL_DEBUG_INFO;
    LONG	LockCount;
    LONG	RecursionCount;
    HANDLE	OwningThread;
    HANDLE	LockSemaphore;
    DWORD	SpinCount;
} RTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION		*PRTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION NEAR	*NPRTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION FAR	*LPRTL_CRITICAL_SECTION;

/* access types */

#define DELETE				(0x00010000L)
#define READ_CONTROL			(0x00020000L)
#define WRITE_DAC			(0x00040000L)
#define WRITE_OWNER			(0x00080000L)
#define SYNCHRONIZE			(0x00100000L)
#define SPECIFIC_RIGHTS_ALL		(0x0000FFFFL)
#define STANDARD_RIGHTS_REQUIRED	(0x000F0000L)
#define STANDARD_RIGHTS_READ		(READ_CONTROL)
#define STANDARD_RIGHTS_WRITE		(READ_CONTROL)
#define STANDARD_RIGHTS_EXECUTE		(READ_CONTROL)
#define STANDARD_RIGHTS_ALL		(0x001F0000L)

/* registry specific access rights */

#define KEY_QUERY_VALUE			(0x0001)
#define KEY_SET_VALUE			(0x0002)
#define KEY_CREATE_SUB_KEY		(0x0004)
#define KEY_ENUMERATE_SUB_KEYS		(0x0008)
#define KEY_NOTIFY			(0x0010)
#define KEY_CREATE_LINK			(0x0020)

#define KEY_READ\
	((STANDARD_RIGHTS_READ |\
	  KEY_QUERY_VALUE |\
	  KEY_ENUMERATE_SUB_KEYS |\
	  KEY_NOTIFY)\
	& (~SYNCHRONIZE))

#define KEY_WRITE\
	((STANDARD_RIGHTS_WRITE |\
	  KEY_SET_VALUE |\
	  KEY_CREATE_SUB_KEY)\
	& (~SYNCHRONIZE))

#define KEY_EXECUTE\
	((KEY_READ) & (~SYNCHRONIZE))

#define KEY_ALL_ACCESS\
	((STANDARD_RIGHTS_ALL |\
	  KEY_QUERY_VALUE |\
	  KEY_SET_VALUE |\
	  KEY_CREATE_SUB_KEY |\
	  KEY_ENUMERATE_SUB_KEYS |\
	  KEY_NOTIFY |\
	  KEY_CREATE_LINK)\
	& (~SYNCHRONIZE))

/* registry open/create options */

#define REG_OPTION_RESERVED		(0x00000000L)
#define REG_OPTION_NON_VOLATILE		(0x00000000L)
#define REG_OPTION_VOLATILE		(0x00000001L)
#define REG_OPTION_CREATE_LINK		(0x00000002L)
#define REG_OPTION_BACKUP_RESTORE	(0x00000004L)

#define REG_LEGAL_OPTION\
	(REG_OPTION_RESERVED |\
	 REG_OPTION_NON_VOLATILE |\
	 REG_OPTION_VOLATILE |\
	 REG_OPTION_CREATE_LINK |\
	 REG_OPTION_BACKUP_RESTORE)

/* registry key creation/open disposition */

#define REG_CREATED_NEW_KEY		(0x00000001L)
#define REG_OPENED_EXISTING_KEY		(0x00000002L)

/* registry key restore flags */

#define REG_WHOLE_HIVE_VOLATILE		(0x00000001L)
#define REG_REFRESH_HIVE		(0x00000002L)

/* registry notify filter values */

#define REG_NOTIFY_CHANGE_NAME		(0x00000001L)
#define REG_NOTIFY_CHANGE_ATTRIBUTES	(0x00000002L)
#define REG_NOTIFY_CHANGE_LAST_SET	(0x00000004L)
#define REG_NOTIFY_CHANGE_SECURITY	(0x00000008L)

#define REG_LEGAL_CHANGE_FILTER\
	(REG_NOTIFY_CHANGE_NAME |\
	 REG_NOTIFY_CHANGE_ATTRIBUTES |\
	 REG_NOTIFY_CHANGE_LAST_SET |\
	 REG_NOTIFY_CHANGE_SECURITY)

/* registry value types */

#define REG_NONE			(0)
#define REG_SZ				(1)
#define REG_EXPAND_SZ			(2)
#define REG_BINARY			(3)
#define REG_DWORD			(4)
#define REG_DWORD_LITTLE_ENDIAN		(4)
#define REG_DWORD_BIG_ENDIAN		(5)
#define REG_LINK			(6)
#define REG_MULTI_SZ			(7)
#define REG_RESOURCE_LIST		(8)
#define REG_RESOURCE_REQUIREMENTS_LIST	(10)

/* file access */

#define DUPLICATE_CLOSE_SOURCE		0x00000001
#define DUPLICATE_SAME_ACCESS		0x00000002

typedef PVOID PACCESS_TOKEN;

typedef struct _ACL {
    BYTE AclRevision;
    BYTE Sbz1;
    WORD AclSize;
    WORD AceCount;
    WORD Sbz2;
} ACL, *LPACL;

typedef struct {
    BYTE Value[6];
} SID_IDENTIFIER_AUTHORITY,
 *PSID_IDENTIFIER_AUTHORITY,
 *LPSID_IDENTIFIER_AUTHORITY;

typedef struct _SID {
    BYTE Revision;
    BYTE SubAuthorityCount;
    SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
    DWORD SubAuthority[1];
} SID,*PSID,*LPSID;

/* Security desciptors - Taken from wine-990110 */
#define	SECURITY_DESCRIPTOR_REVISION	1
#define	SECURITY_DESCRIPTOR_REVISION1	1

typedef WORD SECURITY_DESCRIPTOR_CONTROL;

#define	SE_OWNER_DEFAULTED	0x0001
#define	SE_GROUP_DEFAULTED	0x0002
#define	SE_DACL_PRESENT		0x0004
#define	SE_DACL_DEFAULTED	0x0008
#define	SE_SACL_PRESENT		0x0010
#define	SE_SACL_DEFAULTED	0x0020
#define	SE_SELF_RELATIVE	0x8000

typedef struct {
	BYTE	Revision;
	BYTE	Sbz1;
	SECURITY_DESCRIPTOR_CONTROL Control;
	LPSID	Owner;
	LPSID	Group;
	LPACL	Sacl;
	LPACL	Dacl;
} SECURITY_DESCRIPTOR,*PSECURITY_DESCRIPTOR,*LPSECURITY_DESCRIPTOR;

/* security information */
typedef DWORD				SECURITY_INFORMATION;
typedef SECURITY_INFORMATION		*PSECURITY_INFORMATION;
typedef SECURITY_INFORMATION NEAR	*NPSECURITY_INFORMATION;
typedef SECURITY_INFORMATION FAR	*LPSECURITY_INFORMATION;

#define OWNER_SECURITY_INFORMATION	(0X00000001L)
#define GROUP_SECURITY_INFORMATION	(0X00000002L)
#define DACL_SECURITY_INFORMATION	(0X00000004L)
#define SACL_SECURITY_INFORMATION	(0X00000008L)

/* access mask */

typedef DWORD ACCESS_MASK;
typedef ACCESS_MASK *PACCESS_MASK;

/* win32 status */

#ifndef WIN32_NO_STATUS

#define STATUS_WAIT_0			((DWORD)0x00000000L)
#define STATUS_ABANDONED_WAIT_0		((DWORD)0x00000080L)
#define STATUS_USER_APC			((DWORD)0x000000C0L)
#define STATUS_TIMEOUT			((DWORD)0x00000102L)
#define STATUS_PENDING			((DWORD)0x00000103L)

#define MAXIMUM_WAIT_OBJECTS		64

#endif	/* WIN32_NO_STATUS */

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
	DWORD	Data1;
	WORD	Data2;
	WORD	Data3;
	BYTE    Data4[8];
} GUID;
#endif

#define LPOLESTR	LPSTR

/* (WIN32) Virtual Memory ************************************************** */

/* page flags */

#define PAGE_NOACCESS          0x01
#define PAGE_READONLY          0x02
#define PAGE_READWRITE         0x04
#define PAGE_WRITECOPY         0x08
#define PAGE_EXECUTE           0x10
#define PAGE_EXECUTE_READ      0x20
#define PAGE_EXECUTE_READWRITE 0x40
#define PAGE_EXECUTE_WRITECOPY 0x80
#define PAGE_GUARD            0x100
#define PAGE_NOCACHE          0x200

#define MEM_COMMIT              0x00001000
#define MEM_RESERVE             0x00002000
#define MEM_DECOMMIT            0x00004000
#define MEM_RELEASE             0x00008000
#define MEM_FREE                0x00010000
#define MEM_PRIVATE             0x00020000
#define MEM_MAPPED              0x00040000
#define MEM_TOP_DOWN            0x00100000
/* function prototypes */

#ifndef NOAPIPROTO

LPVOID    WINAPI VirtualAlloc(LPVOID,DWORD,DWORD,DWORD);
BOOL      WINAPI VirtualFree(LPVOID,DWORD,DWORD);

BOOL      WINAPI VirtualLock(LPVOID,DWORD);
BOOL      WINAPI VirtualUnlock(LPVOID,DWORD);

BOOL	WINAPI
VirtualProtect(LPVOID lpAddress, DWORD dwSize,
	DWORD dwAccessFlags, PDWORD pdwOldAccessFlags);

BOOL	WINAPI
VirtualProtectEx(HANDLE hProcess, LPVOID lpAddress, DWORD dwSize,
	DWORD dwAccessFlags, LPDWORD pdwOldAccessFlags);

#endif	/* NOAPIPROTO */

#if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#define DECLSPEC_IMPORT __declspec(dllimport)
#else
#define DECLSPEC_IMPORT
#endif

/*  Time zone definitions  */
#define TIME_ZONE_ID_UNKNOWN	0
#define TIME_ZONE_ID_STANDARD	1
#define TIME_ZONE_ID_DAYLIGHT	2

#define PROCESSOR_INTEL_PENTIUM		586
#define PROCESSOR_ARCHITECTURE_INTEL  	0

#endif	/* Win_NT__h */
