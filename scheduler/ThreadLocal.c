
#include "windows.h"
#include "Log.h"

/* (WIN32) Thread Local Storage ******************************************** */

static DWORD dwTlsIndex = 0;
static LPVOID TlsData[512];

DWORD	WINAPI
TlsAlloc(VOID)
{
	APISTR((LF_API, "TlsAlloc: (API)\n"));
	if (dwTlsIndex < sizeof(TlsData) / sizeof(TlsData[0]))
	{
		TlsData[dwTlsIndex] = NULL;
		return (dwTlsIndex++);
	}
	return (0xFFFFFFFFUL);
}

BOOL	WINAPI
TlsFree(DWORD dwTlsIndex)
{
	APISTR((LF_APISTUB, "TlsFree(DWORD=%ld)\n", dwTlsIndex));
	return (TRUE);
}

LPVOID	WINAPI
TlsGetValue(DWORD dwTlsIndex)
{
	APISTR((LF_API, "TlsGetValue: (API) dwTlsIndex %ld\n", dwTlsIndex));
	if (dwTlsIndex < sizeof(TlsData) / sizeof(TlsData[0]))
	{
		LOGSTR((LF_LOG, "TlsGetValue: (LOG) [%ld] = %p\n",
			dwTlsIndex, TlsData[dwTlsIndex]));
		SetLastError(NO_ERROR);
		return (TlsData[dwTlsIndex]);
	}
	SetLastErrorEx(1, 0);
	return (NULL);
}

BOOL	WINAPI
TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue)
{
	APISTR((LF_API, "TlsSetValue: (API) dwTlsIndex %ld lpTlsValue %p\n",
		dwTlsIndex, lpTlsValue));
	if (dwTlsIndex < sizeof(TlsData) / sizeof(TlsData[0]))
	{
		LOGSTR((LF_LOG, "TlsSetValue: (LOG) [%ld] = %p\n",
			dwTlsIndex, lpTlsValue));
		TlsData[dwTlsIndex] = lpTlsValue;
		return (TRUE);
	}
	return (FALSE);
}

