
#ifndef __winmaptypes__
#define __winmaptypes__

#define _huge
#define _far
#define __far
#define far
#define _pascal
#define _export
#define __export
#define pascal
#define _near
#define near
#define _cdecl
#define __cdecl
#define cdecl
#define	__stdcall
#define _declspec(impexp)
#define __declspec(impexp)

/* Common definitions *****************************************/
#define VOID        void
#define CONST       const

#ifndef FAR
#define FAR         _far
#endif
#ifndef NEAR
#define NEAR        _near
#endif
#ifndef PASCAL
#define PASCAL      _pascal
#endif
#ifndef CDECL
#define CDECL       _cdecl
#endif

#define WINAPI      _far _pascal    /* use this tag for APIs defined by MS */
#define TWINAPI     _far _pascal    /* use this tag for our public extensions */
#define CALLBACK    _far _pascal
#define	APIENTRY

#ifndef TWIN_BOOL_DEFINED
typedef int		BOOL;
#endif	/* TWIN_BOOL_DEFINED */

#ifndef TWIN_SHORT_DEFINED
typedef short int	SHORT;
#endif	/* TWIN_SHORT_DEFINED */

#ifndef TWIN_USHORT_DEFINED
typedef unsigned short	USHORT;
#endif	/* TWIN_USHORT_DEFINED */

#ifndef TWIN_INT_DEFINED
typedef int		INT;
#endif	/* TWIN_INT_DEFINED */

#ifndef TWIN_UINT_DEFINED
typedef unsigned int	UINT;
#endif	/* TWIN_UINT_DEFINED */

#ifndef TWIN_LONG_DEFINED
typedef signed long         LONG;
#endif	/* TWIN_LONG_DEFINED */

#ifndef TWIN_ULONG_DEFINED
typedef unsigned long	ULONG;
#endif	/* TWIN_ULONG_DEFINED */

#endif /* __winmaptypes__ */
