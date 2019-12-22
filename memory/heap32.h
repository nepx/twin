/*************************************************************************
 * This source is derived from the Linux wine project. see www.winehq.com.
 * Bug fixes and patches to this file should be made in the original and then 
 * reflected into Twine.  Please see WINE in this distribution for more 
 * information.  This file is from wine-981129/memory/heap32.h
 ************************************************************************/

/*
 * Win32 heap definitions
 *
 * Copyright 1996 Alexandre Julliard
 */

#ifndef __WINE_HEAP_H
#define __WINE_HEAP_H

#include "winnt.h"

extern HANDLE SystemHeap;
extern HANDLE SegptrHeap;
extern CRITICAL_SECTION *HEAP_SystemLock;

extern int HEAP_IsInsideHeap( HANDLE heap, DWORD flags, LPCVOID ptr );

/* System heap locking macros */

#define SYSTEM_LOCK()       (EnterCriticalSection(HEAP_SystemLock))
#define SYSTEM_UNLOCK()     (LeaveCriticalSection(HEAP_SystemLock))
/* Use this one only when you own the lock! */
#define SYSTEM_LOCK_COUNT() (HEAP_SystemLock->RecursionCount)


typedef struct
{
    LPVOID lpData;
    DWORD cbData;
    BYTE cbOverhead;
    BYTE iRegionIndex;
    WORD wFlags;
    union {
        struct {
            HANDLE hMem;
            DWORD dwReserved[3];
        } Block;
        struct {
            DWORD dwCommittedSize;
            DWORD dwUnCommittedSize;
            LPVOID lpFirstBlock;
            LPVOID lpLastBlock;
        } Region;
    } Foo;
} PROCESS_HEAP_ENTRY, *LPPROCESS_HEAP_ENTRY;

BOOL WINAPI HeapUnlock(HANDLE );
BOOL WINAPI HeapLock( HANDLE );

#endif  /* __WINE_HEAP_H */
