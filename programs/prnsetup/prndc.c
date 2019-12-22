/*	Hunter Systems - Apr 1994	*/

/*============================================================================
	prndc.c

	GetDevDrvPort sets 3 pointers (device name, driver name, port name
	from win.ini).
	GetPrinterDC creates default printer DC, using the settings in win.ini.
	The DC can be initialized with data pointed by lpData parameter.
  ============================================================================*/
#include "windows.h"

int
GetDevDrvPort(char **pDev, char **pDrv, char **pPort)
{
	static char	*p1 = 0, *p2 = 0, *p3 = 0;
	static char	szPrn[128] = { '\0' };

	if ( !szPrn[0] )
		if ( !GetProfileString ("windows","device",",,,",szPrn,sizeof(szPrn)) )
			return FALSE;

	if ( !p3 ) {
		p1 = (char*)strtok(szPrn, ",");
		p2 = (char*)strtok(NULL,  ", ");
		p3 = (char*)strtok(NULL,  ", ");
	}
	if ( !p1  ||  !p2  ||  !p3  ||  !*p1  ||  !*p2  ||  !*p3 )
		return FALSE;

	if ( pDev )
		*pDev = p1;
	if ( pDrv )
		*pDrv = p2;
	if ( pPort )
		*pPort = p3;
	return TRUE;
}


HDC
GetPrinterDC(LPVOID lpData)
{
	char	*pDev, *pDrv, *pPort;

	if ( !GetDevDrvPort(&pDrv, &pDev, &pPort) )
		return (HDC)0;

	return CreateDC(pDev, pDrv, pPort, lpData);
}

