/*--------------------------------------------------------------------------
**      winspool.h                   Twine           
**              
**------------------------- < License Information > ------------------------
**
**  
**      This file is released under terms of an LGPL.  Readers of 
**  this file may modify or use this source code so long as they make all
**  modifications publically available.  Please refer to TWINE-LICENSE 
**  for further details.
**  
**
**      The original Copyright is in 1999, by CodeWeavers, Inc.
**
**      
**------------------------ < File Content Description > --------------------
**
**  Header:	winspool.h	
**	
**  Description:
**		BLANK TEMPLATE TO MAKE COMPILING MFC GO MORE SMOOTHLY
**    
**--------------------------------------------------------------------------*/

#ifndef __winspool_h__ 
#define __winspool_h__ 

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PRINTER_DEFAULTS {  // pd  
    LPTSTR      pDatatype; 
    LPDEVMODE   pDevMode; 
    ACCESS_MASK DesiredAccess; 
} PRINTER_DEFAULTS, *LPPRINTER_DEFAULTS;
 
BOOL ClosePrinter(
    HANDLE hPrinter 	// handle to printer object 
   );	

BOOL OpenPrinter(
    LPTSTR pPrinterName,	// pointer to printer or server name 
    LPHANDLE phPrinter,		// pointer to printer or server handle 
    LPPRINTER_DEFAULTS pDefault // pointer to printer defaults structure  
   );	
 
LONG DocumentProperties(
    HWND hWnd,			// handle to window that displays dialog box 
    HANDLE hPrinter,		// handle to printer object 
    LPTSTR pDeviceName,		// pointer to device name 
    PDEVMODE pDevModeOutput,	// pointer to modified device mode structure 
    PDEVMODE pDevModeInput,	// pointer to original device mode structure 
    DWORD fMode 		// mode flag 
   );	


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __winspool_h__ */
