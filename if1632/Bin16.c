/*    
 * 	Bin16.c
 *	stubs for native only versions of twin libraries.
 *	these functions are referenced, but will/should
 *	never be called.
 */

#include <string.h>

#include "windows.h"

#include "kerndef.h"
#include "Endian.h"
#include "Log.h"
#include "DPMI.h"
#include "BinTypes.h"
#include "KrnTask.h"
#include "Resources.h"
#include "Module.h"

typedef	unsigned short TWIN_HLOCAL;

BOOL WINAPI
TWIN_LocalInit(UINT uSegment, UINT uStartAddr, UINT uEndAddr)
{
	return FALSE;
}

TWIN_HLOCAL WINAPI
TWIN_LocalAlloc(UINT fuAllocFlags, UINT fuAlloc)
{
	return (TWIN_HLOCAL) 0;
}

char NEAR * WINAPI
TWIN_LocalLock(TWIN_HLOCAL hloc)
{
	return NULL;
}

BOOL WINAPI
TWIN_LocalUnlock(TWIN_HLOCAL hloc)
{
	return FALSE;
}

UINT WINAPI
TWIN_LocalCompact(UINT uMinFree)
{
	return 0;
}

TWIN_HLOCAL WINAPI
TWIN_LocalReAlloc(TWIN_HLOCAL hloc, UINT fuNewSize, UINT fuFlags)
{
	return 0;
}

UINT WINAPI
TWIN_LocalFlags(TWIN_HLOCAL hloc)
{
	return 1;
}

UINT WINAPI
TWIN_LocalSize(TWIN_HLOCAL hloc)
{
	return 0;
}

UINT WINAPI
TWIN_LocalShrink(TWIN_HLOCAL hloc, UINT cbNewSize)
{
	return cbNewSize;
}

TWIN_HLOCAL WINAPI
TWIN_LocalHandle(UINT uMemOffset)
{
	return 0;
}

WORD WINAPI
TWIN_LocalHandleDelta(int nNewDelta)
{
	return 0;
}

TWIN_HLOCAL WINAPI
TWIN_LocalFree(TWIN_HLOCAL hloc)
{
	return (TWIN_HLOCAL)0;
}
			
BOOL
IsBadHugePtr(const void *lp, DWORD cb)
{
	return TRUE;
}

BOOL 
IsBadPtr(const void *lp, UINT cb)
{
	return TRUE;
}

void
invoke_binary()
{}

BINADDR
make_binary_thunk(LPDWORD targ, DWORD conv)
{
	return 0;
}

DWORD
make_native_thunk(DWORD targ, DWORD conv)
{
	return 0;
}

BOOL
TWIN_ThunkManager(int allocate)
{
	return 1;
}

LRESULT
hsw_common_nat_to_bin(HWND hWnd, UINT message, WPARAM w, LPARAM l)
{
	return 0;
}

LRESULT
hsw_common_bin_to_nat(HWND hWnd, UINT uiMsg, WPARAM w, LPARAM l)
{
	return 0;
}

LRESULT
EditMemoryBin(HWND hWnd,UINT uiMsg,WPARAM w,LPARAM l)
{
	return 0;
}

FARPROC
OEM_GetProcAddress(LPMODULEINFO lpModuleInfo, UINT uOrdinal)
{
	return 0;
}
