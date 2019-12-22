/* 
 * windef.h
 * provide common types of objects
 */


#ifndef __WINDEF_H__
#define __WINDEF_H__

#ifndef FALSE
#define FALSE       0
#define TRUE        1
#endif

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;

typedef	char		CHAR;
#ifndef TWIN_UCHAR_DEFINED
typedef unsigned char   UCHAR;
#endif
typedef	char		TCHAR, *PTCHAR;
typedef float		FLOAT;

typedef UINT		*PUINT;
typedef UINT NEAR	*NPUINT;
typedef UINT FAR	*LPUINT;

typedef FLOAT		*PFLOAT;
typedef FLOAT NEAR	*NPFLOAT;
typedef FLOAT FAR	*LPFLOAT;

typedef unsigned char 	boolean;
typedef unsigned char 	byte;

/* Pointer definitions******************************************/
#if defined(NULL)
#undef NULL
#endif
#define NULL            0

typedef char NEAR*          PSTR;
typedef const char NEAR*    PCSTR;
typedef char NEAR*          NPSTR;

typedef char FAR*           LPSTR;
typedef const char FAR*     LPCSTR;
typedef char FAR*           LPTSTR;
typedef const char FAR*     LPCTSTR;

typedef BYTE NEAR*          PBYTE;
typedef BYTE FAR*           LPBYTE;

typedef int NEAR*           PINT;
typedef int FAR*            LPINT;

typedef WORD NEAR*          PWORD;
typedef WORD FAR*           LPWORD;

typedef long NEAR*          PLONG;
typedef long FAR*           LPLONG;

typedef DWORD NEAR*         PDWORD;
typedef DWORD FAR*          LPDWORD;

typedef void NEAR*          PVOID;
typedef void FAR*           LPVOID;

typedef const void NEAR*    PCVOID;
typedef const void FAR*     LPCVOID;

typedef BOOL NEAR*          PBOOL;
typedef BOOL FAR*           LPBOOL;

/* Types use for passing & returning polymorphic values */
typedef UINT WPARAM;
typedef LONG LPARAM;
typedef LONG LRESULT;

/* Handle definitions ******************************************/
/*
 *  For WIN16, we cannot use the STRICT flag to enable unique
 *  handle types, since for us the underlying pointers are 32-bit 
 *  pointers, not 16-bit pointers.
 */
typedef void *HANDLE;
#ifdef RC_INVOKED
/* FIXME: tools/rc does not seem to support the ## operator currently. */
#define DECLARE_HANDLE(name)   typedef void *name
#define DECLARE_HANDLE32(name) typedef void *name
#else
#define DECLARE_HANDLE(name)   typedef struct name##__ { } *name
#define DECLARE_HANDLE32(name) typedef struct name##32__ { } *name
#endif /* RC_INVOKED */

typedef HANDLE*         PHANDLE;
typedef HANDLE NEAR*    SPHANDLE;
typedef HANDLE FAR*     LPHANDLE;

typedef HANDLE          HGLOBAL;
typedef HANDLE          HLOCAL;

typedef HANDLE          GLOBALHANDLE;
typedef HANDLE          LOCALHANDLE;

typedef WORD            ATOM;
typedef LONG            HRESULT;

typedef void *HANDLE32;           
typedef void *HTASK32;           

typedef HANDLE HTASK16;
typedef HANDLE HQUEUE16;

#ifdef STRICT
typedef void (CALLBACK*     FARPROC)(void);
typedef void (NEAR PASCAL*  NEARPROC)(void);
#else
typedef int (CALLBACK*      FARPROC)();
typedef int (NEAR PASCAL*   NEARPROC)();
#endif
typedef int (WINAPI *PROC)();

/* Core handle declarations */
DECLARE_HANDLE(HSTR);
DECLARE_HANDLE(HTASK);
DECLARE_HANDLE(HRSRC);
DECLARE_HANDLE(HDROP);

DECLARE_HANDLE(HINSTANCE);
typedef HINSTANCE HMODULE;  /* HMODULEs can be used in place of HINSTANCEs */

DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HPEN);
DECLARE_HANDLE(HBRUSH);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HPALETTE);
DECLARE_HANDLE(HFONT);
DECLARE_HANDLE(HWND);
#if 1
typedef HANDLE HGDIOBJ;
#else
DECLARE_HANDLE(HGDIOBJ);
#endif

/* GDI related typedefs */
typedef struct tagRECT
{
    int left;
    int top;
    int right;
    int bottom;
} RECT;
typedef RECT*      PRECT;
typedef RECT NEAR* NPRECT;
typedef RECT FAR*  LPRECT;
typedef const RECT FAR*  LPCRECT;

typedef struct _RECTL
{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECTL;
typedef RECTL		*PRECTL;
typedef RECTL NEAR	*NPRECTL;
typedef RECTL FAR	*LPRECTL;
typedef const RECTL     *LPCRECTL;

typedef struct tagPOINT
{
    int x;
    int y;
} POINT;
typedef POINT*       PPOINT;
typedef POINT NEAR* NPPOINT;
typedef POINT FAR*  LPPOINT;

/* #define MAKEPOINT(l)      (*((POINT FAR*)&(l))) */
POINT MAKEPOINT(DWORD);

typedef struct tagPOINTS
{
    short x;
    short y;
} POINTS;
typedef POINTS*       PPOINTS;
typedef POINTS NEAR* NPPOINTS;
typedef POINTS FAR*  LPPOINTS;

#define MAKEPOINTS(l)      (*((POINTS FAR*)&(l)))

typedef struct tagSIZE
{
    int cx;
    int cy;
} SIZE;
typedef SIZE*       PSIZE;
typedef SIZE NEAR* NPSIZE;
typedef SIZE FAR*  LPSIZE;

typedef SIZE		SIZEL;
typedef SIZE		*PSIZEL;
typedef SIZE NEAR	*NPSIZEL;
typedef SIZE FAR	*LPSIZEL;

typedef DWORD	LCTYPE;

#endif /* __WINDEF_H__ */
