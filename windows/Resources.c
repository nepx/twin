/*    
	Resources.c	2.43
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


For more information about the Willows Twin Libraries.

	http://www.willows.com	

To send email to the maintainer of the Willows Twin Libraries.

	mailto:twin@willows.com 

 */

#include <stdlib.h>
#include <string.h>

#include "windows.h"
#include "windowsx.h"
#include "Log.h"
#include "kerndef.h"
#include "Resources.h"
#include "Module.h"
#include "Kernel.h"
#include "WImageIO.h"
#include "Strings32.h"

/* external linkage */
extern void LoadResourceEx(HANDLE,NAMEINFO *,LPMEMORYINFO);
extern HBITMAP CreateDIBitmapEx(LPBITMAPIMAGE,LPNAMEINFO);
extern HICON LoadIconResource(LPNAMEINFO);
extern HCURSOR LoadCursorResource(LPNAMEINFO);
extern HMODULE GetModuleFromInstance(HINSTANCE);
extern LPBYTE ConvertResourceToNat(LPBYTE, WORD);

/* static functions */
static LPMEMORYINFO HandleAlloc(LPSTR,WORD);
static LPSTR HandleLock(HANDLE,UINT);
static BOOL HandleUnlock(HANDLE,WORD);
static DWORD HandleSize(HINSTANCE,HANDLE);

static LPMEMORYINFO GlobalFirst;
static GNOTIFYPROC	  slpNotifyProc;

#define MT_MEMORY	0	/* object is globalalloc data   */
#define MT_NAMEINFO	1	/* object is nameinfo structure */
#define MT_RESOURCE	2	/* object is resource data      */

extern HMENU TWIN_LoadMenu(HINSTANCE , LPCSTR ,int );
HICON TWIN_LoadIcon(HINSTANCE , LPCSTR ,int );
int   TWIN_LoadString(HINSTANCE , UINT , LPSTR , int ,int );
HCURSOR TWIN_LoadCursor(HINSTANCE , LPCSTR ,int );
HBITMAP TWIN_LoadBitmap(HINSTANCE , LPCSTR ,int );
HACCEL	TWIN_LoadAccelerators(HINSTANCE , LPCSTR ,int );
HRSRC   TWIN_FindResource(HINSTANCE ,LPCSTR ,LPCSTR ,int );
HGLOBAL TWIN_LoadResource(HINSTANCE ,HRSRC ,int );
DWORD   TWIN_SizeofResource(HINSTANCE , HRSRC,int );
HWND    TWIN_CreateDialogParam(HINSTANCE,LPCSTR,HWND,DLGPROC,LPARAM,int );
int     TWIN_DialogBoxParam(HINSTANCE ,LPCSTR ,HWND ,DLGPROC ,LPARAM,int );
extern FARPROC TWIN_GetProcAddress(HINSTANCE,LPCSTR,int);

TWINRT DefaultTwinRT = {
	TWIN_LoadMenu,
	TWIN_LoadIcon,
	TWIN_LoadString,
	TWIN_LoadCursor,
	TWIN_LoadBitmap,	
	TWIN_LoadAccelerators,
	TWIN_FindResource,	
	TWIN_LoadResource,
	TWIN_SizeofResource,
	TWIN_CreateDialogParam,
	TWIN_DialogBoxParam,
	TWIN_GetProcAddress,
};

/****************************************************************
 *
 *	Internal Memory Object Routines
 *	used by resources and memory allocation
 *	functions include:
 *		HandleAlloc 	allocate handle and point to data
 *		HandleLock	return data pointer given handle
 *		HandleUnlock	decrement lock count of HandleLock
 *		HandleSize	return the size of the object
 *				note: is dependent on what the object
 *				      points to.
 *
 ****************************************************************/		
void
TWIN_HandleCleanup()
{
        LPMEMORYINFO 	lpMemory,lpnext;
	int i;
	
	for(i=0,lpMemory = GlobalFirst;lpMemory;lpMemory = lpnext) {
		lpnext = lpMemory->lpNext;
		
		if(lpMemory->wType == MT_MEMORY) {
			logstr(LF_LOG,"%d: ptr=%x handle=%x size=%d\n",
				i++,
				lpMemory->lpCore, 
				lpMemory->hMemory,
				lpMemory->dwSize);
			if (lpMemory->hMemory) {
				GlobalUnlock(lpMemory->hMemory);
				GlobalFree(lpMemory->hMemory);
			}
			if (lpMemory->lpCore) {
				WinFree(lpMemory->lpCore);
				lpMemory->lpCore = 0;
				lpMemory->dwSize = 0;
			}
		}
	}
}

static LPMEMORYINFO
HandleAlloc(LPSTR lpData,WORD wType)
{
        LPMEMORYINFO 	lpMemory;
	HANDLE 		hResData;

	if((lpMemory = CREATEHANDLE(hResData))) {

		/* insert into the list */
		lpMemory->lpNext  = GlobalFirst;
		GlobalFirst = lpMemory;

		/* assign a pointer to the data */
		lpMemory->lpCore = lpData;
		lpMemory->hMemory = hResData;
		lpMemory->wType   = wType;
		lpMemory->wIndex = 0;
		return lpMemory;
	}
	return 0;
}

HANDLE
ResourceAlloc(LPSTR lpstr)
{
    	LPMEMORYINFO lpMemory;
	HANDLE	     hResource = 0;
	lpMemory = HandleAlloc(lpstr,MT_RESOURCE);
	if(lpMemory) {
		hResource = lpMemory->hMemory;	
		RELEASEHANDLEINFO(lpMemory);
	}
	return hResource;
}

static LPSTR 
HandleLock(HANDLE hMem,UINT wFlag)
{
    	LPMEMORYINFO lpMemory;
	LPSTR	     lpCore = NULL;

	if(HIWORD(hMem))
		return (LPSTR) hMem;

	if((lpMemory  = GETHANDLEINFO(hMem))) {

		/* does handle check */
		if(lpMemory->hMemory != hMem) {
		        RELEASEHANDLEINFO(lpMemory);
			return 0;	
		}

		if(wFlag && lpMemory->lpCore) 
			lpMemory->lockcount++;
		lpCore = lpMemory->lpCore;
		RELEASEHANDLEINFO(lpMemory);
	}

	return lpCore;
}

static BOOL
HandleUnlock(HANDLE hMem,WORD wFlag)
{
	LPMEMORYINFO 	lpMemory;

	if(HIWORD(hMem))
		return TRUE;
	if((lpMemory  = GETHANDLEINFO(hMem))) {

		/* does handle check */
		if(lpMemory->hMemory != hMem) {
		        RELEASEHANDLEINFO(lpMemory);
			return TRUE;	
		}

		if(wFlag && lpMemory->lockcount) 
			lpMemory->lockcount--;
		if (lpMemory->lockcount == 0) {
			RELEASEHANDLEINFO(lpMemory);
			return FALSE;
		}
		RELEASEHANDLEINFO(lpMemory);
		return TRUE;
	}
	return TRUE;
}

static DWORD
HandleSize(HINSTANCE hInst,HANDLE hMem)
{
	LPMEMORYINFO 	lpMemory;
	LPNAMEINFO 	p;
	DWORD		size;
	DWORD		dwResult = 0;

	if((lpMemory  = GETHANDLEINFO(hMem))) {
		switch(lpMemory->wType) {
		case MT_MEMORY:
			dwResult = lpMemory->dwSize;
			break;
		case MT_NAMEINFO:
			p = (LPNAMEINFO) lpMemory->lpCore;
			dwResult = p->rcslength;
			break;
		case MT_RESOURCE:	
			if((size = lpMemory->dwSize) == 0)
			   dwResult = lpMemory->dwBinSize;
			break;
		}
		RELEASEHANDLEINFO(lpMemory);
		return dwResult;
	}
	return(0);
}

#ifdef LATER
	for completeness, we should add 
	HandleFree
	HandleRealloc

#endif

/*********************************************************/
/***** API Binary Interface Routines *********************/
/*********************************************************/

HGLOBAL
GlobalHandle32(LPCVOID lpaddress)
{
	LPMEMORYINFO p;	

	for(p = GlobalFirst;p;p = p->lpNext) {
		if(((DWORD) p->lpCore <= (DWORD) lpaddress) && 
		   (DWORD) lpaddress < (DWORD)(p->lpCore + p->dwSize))
			return p->hMemory;
	}
	return 0;
}


/*********************************************************/
/***** API Memory Routines *******************************/
/*********************************************************/

#ifdef	STRICT
LPVOID	WINAPI
#else
LPSTR	WINAPI
#endif
GlobalLock(HGLOBAL hMem)
{
	LPSTR  lpstr;
	APISTR((LF_APICALL,"GlobalLock(HGLOBAL=%x)\n",hMem));
	lpstr = HandleLock(hMem,1);
	if (! lpstr && GlobalHandle32((LPCVOID) hMem))
	    lpstr = (LPSTR) hMem;
	APISTR((LF_APIRET,"GlobalLock: returns LPSTR %x\n",lpstr));
	return lpstr;
}

BOOL	WINAPI
GlobalUnlock(HGLOBAL hMem)
{
	BOOL rv;
	APISTR((LF_APICALL,"GlobalUnlock(HGLOBAL=%x)\n",hMem));
	rv = HandleUnlock(hMem,1);
	if (! rv && GlobalHandle32((LPCVOID) hMem))
	    rv = TRUE;
	APISTR((LF_APIRET,"GlobalUnlock: returns BOOL %x\n",rv));
	return rv;
}

HGLOBAL	WINAPI
GlobalHandle(LPCVOID lpGlobalMem)
{
	HGLOBAL rc;
	APISTR((LF_APICALL, "GlobalHandle(LPCVOID=%p)\n",
		lpGlobalMem));
 	rc = GlobalHandle32(lpGlobalMem);
     	APISTR((LF_APIRET,"GlobalHandle: returns HGLOBAL %x\n",rc));
 
 	return rc;
}

UINT	WINAPI
GlobalPageLock(HGLOBAL hGlobalMem)
{
	APISTR((LF_APISTUB, "GlobalPageLock(HGLOBAL=%p)\n",hGlobalMem));
	return (1);
}

UINT	WINAPI
GlobalPageUnlock(HGLOBAL hGlobalMem)
{
	APISTR((LF_APISTUB, "GlobalPageUnlock(HGLOBAL=%p)\n",hGlobalMem));
	return (1);
}

HGLOBAL	WINAPI
GlobalAlloc(UINT wFlags,DWORD dwBytes)
{
        LPMEMORYINFO 	lpMemory;
	LPSTR	     	lpmem;
	HGLOBAL		hMem;
	
    	APISTR((LF_APICALL,"GlobalAlloc(UINT=%x,DWORD=%x)\n",
		wFlags,dwBytes));

	dwBytes = (dwBytes+31) & 0xffffffe0;
	if (dwBytes) {
	    lpmem  = (LPSTR) WinMalloc(dwBytes);
	    if(!lpmem) {
    		APISTR((LF_APIFAIL,"GlobalAlloc: returns HGLOBAL %x\n",0));
		return((HGLOBAL)NULL);
	    }
	 } else
	    lpmem = 0;

	/* allocate an object using this data */
	if((lpMemory = HandleAlloc(lpmem,MT_MEMORY))) {

		HGLOBAL hMemory;

		/* initialize the MEMORYINFO block */
		lpMemory->wFlags  = wFlags;
		lpMemory->dwSize  = dwBytes;

		lpMemory->lockcount = 0;

		/* global memory flags */
		if(dwBytes && wFlags & GMEM_ZEROINIT)
			memset(lpMemory->lpCore,0,dwBytes);

		hMem = lpMemory->hMemory;
		hMemory = (HGLOBAL) lpMemory->lpCore;
		RELEASEHANDLEINFO(lpMemory);

                if (wFlags & GMEM_MOVEABLE) {
    	            APISTR((LF_APIRET,"GlobalAlloc: returns HGLOBAL %x\n", hMem));
		    return hMem;
		}
    	        APISTR((LF_APIRET,"GlobalAlloc: returns HGLOBAL %x\n", hMemory));
		return hMemory;
	}
    	APISTR((LF_APIFAIL,"GlobalAlloc: returns HGLOBAL %x\n",0));
	return (HGLOBAL)NULL;
}

HGLOBAL	WINAPI
GlobalReAlloc(HGLOBAL hMem, DWORD dwBytes, UINT uFlags)
{
	LPHANDLEINFO lpMemory; 	
	LPSTR	     lpmem;
	
    	APISTR((LF_APICALL,"GlobalReAlloc(HGLOBAL=%x,DWORD=%x,UINT=%x)\n",
		hMem,dwBytes,uFlags));

	dwBytes = (dwBytes+31) & 0xffffffe0;
	if((lpMemory  = GETHANDLEINFO(hMem))) { 
  	    if ((lpMemory->wFlags & (GMEM_DISCARDABLE|GMEM_MOVEABLE))
			== (GMEM_DISCARDABLE|GMEM_MOVEABLE)) {

		if((uFlags == GMEM_MOVEABLE) && (dwBytes == 0)) {

			if(lpMemory->lpCore)
				WinFree(lpMemory->lpCore);

			lpMemory->lpCore = 0;
			lpMemory->wFlags = uFlags;

			RELEASEHANDLEINFO(lpMemory);
    			APISTR((LF_APIRET,"GlobalReAlloc: returns HGLOBAL %x\n",
				hMem));
			return hMem;
		}
	    }

	    if (!(uFlags & GMEM_MODIFY)) {
		lpmem = lpMemory->lpCore; 
		if (lpmem)
			lpmem = WinRealloc(lpmem,dwBytes); 
		else
			lpmem = WinMalloc(dwBytes);

		lpMemory->lpCore = lpmem;

		if(uFlags & GMEM_ZEROINIT) {
			if(dwBytes > lpMemory->dwSize)
				memset(lpMemory->lpCore+lpMemory->dwSize,0,
					dwBytes-lpMemory->dwSize);
		}
		lpMemory->dwSize = dwBytes;
		lpMemory->wFlags = uFlags;
	    }
	    else {
		if(uFlags & GMEM_DISCARDABLE)
			uFlags |= lpMemory->wFlags;
		lpMemory->wFlags = (uFlags & ~GMEM_MODIFY);
	    }
	    RELEASEHANDLEINFO(lpMemory);
	}

    	APISTR((LF_APIRET,"GlobalReAlloc: returns HGLOBAL %x\n",
		hMem));
	return hMem;
}

/* this should be HandleFree... */
HANDLE	WINAPI
GlobalFree(HANDLE hMem)
{
	LPMEMORYINFO p;
	LPHANDLEINFO lpMemory; 	

    	APISTR((LF_APICALL,"GlobalFree(HANDLE=%x)\n", hMem));

	if((lpMemory = GETHANDLEINFO(hMem))) {

		/* deallocate any allocated memory */
		if(lpMemory->wType == MT_MEMORY && lpMemory->lpCore) {
			WinFree((LPSTR)lpMemory->lpCore);
			lpMemory->lpCore = 0;
		}

		/* unlink the object from the global list */
		for(p = GlobalFirst;p;p = p->lpNext) {
			if((p == GlobalFirst) && (p == lpMemory)) {
				GlobalFirst = lpMemory->lpNext;
				break;
			}
			if(p->lpNext == lpMemory) {
				p->lpNext = lpMemory->lpNext;
				break;
			}
		}
		RELEASEHANDLEINFO(lpMemory);
	}
	else {			/* failed to find handle, return parameter */
	    	APISTR((LF_APIRET,"GlobalFree: returns HANDLE %x\n", hMem));
		return hMem;
	}

	/* now get rid of the handle */
	FREEHANDLE(hMem);

    	APISTR((LF_APIRET,"GlobalFree: returns HANDLE %x\n", 0));
	return 0;
}

DWORD	WINAPI
GlobalSize(HANDLE hMem)
{
	HGLOBAL hLocal = hMem;

	if(HIWORD(hLocal))
		hLocal = GlobalHandle32((LPCVOID) hLocal);
	return HandleSize(0,hMem);
}

UINT	WINAPI
GlobalFlags(HANDLE hMem)
{
	UINT		flags;
	LPHANDLEINFO lpMemory; 	

    	APISTR((LF_APICALL,"GlobalFlags(HANDLE=%x)\n", hMem));
	if ((lpMemory  = GETHANDLEINFO(hMem))) { 
		flags = lpMemory->wFlags;
		flags &= ~GMEM_LOCKCOUNT;
		flags |= lpMemory->lockcount & GMEM_LOCKCOUNT;

		RELEASEHANDLEINFO(lpMemory);
    		APISTR((LF_APIRET,"GlobalFlags: returns UINT %x\n", flags));
		return(flags);
	}
    	APISTR((LF_APIFAIL,"GlobalFlags: returns UINT %x\n", 0));
	return(0);
}

void	WINAPI
GlobalNotify(GNOTIFYPROC lpNotifyProc)
{
	slpNotifyProc = lpNotifyProc;
}

HGLOBAL	WINAPI
GlobalLRUOldest(HGLOBAL hglb)
{
    APISTR((LF_APISTUB, "GlobalLRUOldest(HGLOBAL=%p)\n",hglb));
    return hglb;
}

HGLOBAL	WINAPI
GlobalLRUNewest(HGLOBAL hglb)
{
    APISTR((LF_APISTUB, "GlobalLRUNewest(HGLOBAL=%p)\n",hglb));
    return hglb;
}

DWORD	WINAPI
GlobalCompact(DWORD dwMinFree)
{
    APISTR((LF_APISTUB, "GlobalCompact(DWORD=%p)\n",dwMinFree));
    return 4*1024*1024;
}

#if 0
typedef int (*RESOURCEHOOKPROC)( HINSTANCE , LPCSTR , HANDLE *);

static RESOURCEHOOKPROC lpfnResourceHook[16];

RESOURCEHOOKPROC
TWIN_SetResourceHookProc(int id,RESOURCEHOOKPROC lpfnHook)
{
	RESOURCEHOOKPROC last = lpfnResourceHook[id];

	lpfnResourceHook[id] = lpfnHook;

	return last;
}

RESOURCEHOOKPROC
TWIN_GetResourceHookProc(int id)
{
	RESOURCEHOOKPROC last = lpfnResourceHook[id];

	return last;
}
#endif

/*********************************************************/
/***** API Resource Dependent Routines *******************/
/*********************************************************/

HACCEL	WINAPI
TWIN_LoadAccelerators(HINSTANCE hInstance, LPCSTR lpTableName,int Style)
{
	HANDLE hResInfo;
	HACCEL rc;

    	APISTR((LF_APICALL,"LoadAccelerators(HINSTANCE=%x,LPCSTR=%x)\n",
		hInstance,lpTableName));

	hResInfo = FindResource(hInstance,lpTableName,RT_ACCELERATOR);

	if(hResInfo == 0) {
    		APISTR((LF_APIRET,"LoadAccelerators: returns HACCEL 0\n"));
		return 0;
	}

	rc =  LoadResource(hInstance,hResInfo);
    	APISTR((LF_APIRET,"LoadAccelerators: returns HACCEL %x\n",rc));
	return rc;
}

int	WINAPI
CopyAcceleratorTable(HACCEL hAccelTable, LPACCEL lpAccelTable,
	int nAccelTableSize)
{
     	APISTR((LF_APISTUB, "CopyAcceleratorTable(HACCEL=%x,LPACCEL=%p)\n",
		hAccelTable,lpAccelTable));
	return (0);
}

int	WINAPI
TWIN_LoadString(HINSTANCE hInstance, UINT wID, LPSTR lpBuffer, int nBufferMax,int Style)
{
	HANDLE hResInfo;
	HANDLE hResData;
	STRINGENTRY *lpString;
	int 	count = 0;
	int	nIndex;

    	APISTR((LF_APICALL,"LoadString(HINSTANCE=%x,UINT=%x,LPSTR=%p,int=%x)\n",
		hInstance,wID,lpBuffer,nBufferMax));

	hResInfo = FindResource(hInstance,
				(LPCSTR)(((UINT)(wID & 0x0000fff0)>>4)+1),
				RT_STRING);
	if(hResInfo == 0) {
    		APISTR((LF_APIRET,"LoadString: returns int 0\n"));
		return count;
	}

	hResData = LoadResource(hInstance,hResInfo);
	if(hResData == 0) {
    		APISTR((LF_APIFAIL,"LoadString: returns int 0\n"));
		return count;
	}

	lpString = (STRINGENTRY *) HandleLock(hResData,0);

	nIndex = wID & 0xf;
	if (lpString[nIndex]) {
	    count = min(nBufferMax,(int)strlen(lpString[nIndex])+1);
	    strncpy(lpBuffer,lpString[nIndex],count);
	}
	else {
	    count = 1;
	    lpBuffer[0] = '\0';
	}

	RELEASEHANDLE(hResData);

    	APISTR((LF_APIRET,"LoadString: returns int %d\n",count-1));
	return count-1;
}

#ifdef TWIN32
/*
 * LoadImage() Win32 API
 */
HANDLE	WINAPI
LoadImage
(
	HINSTANCE			hInstance,
	LPCSTR				pstrImage,
	UINT				ImageType,
	int				cxDesired,
	int				cyDesired,
	UINT				LoadFlags
)

{
	POINT			    	ptSize;
	HBITMAP				hbImage		  = ( HBITMAP )NULL;
	HBITMAP				hbImageMask	  = ( HBITMAP )NULL;
	HDC				hDeskDC		  = GetDC ( GetDesktopWindow () );
	HBITMAP				hBitmap		  = ( HBITMAP )NULL;
	HBITMAP				hbmpMask	  = ( HBITMAP )NULL;
	HDC				hSrcDC		  = ( HDC )NULL;
	HDC				hDstDC		  = ( HDC )NULL;
	HANDLE				hImage	  	  = ( HANDLE )NULL;

	BITMAPINFOHEADER		bi;
	HBITMAP			    	hbmpOld;

	BITMAP			    	bmColor;
	BITMAP			    	Bitmap;
	ICONINFO		    	IconInfo;


	memset ( &bi, 0, sizeof ( BITMAPINFOHEADER ) );
	memset ( &bmColor, 0, sizeof ( BITMAP ) );
	memset ( &IconInfo, 0, sizeof ( ICONINFO ) );

	if ( LoadFlags & LR_LOADFROMFILE )
	{
		if ( ImageType == IMAGE_BITMAP )
			hImage = W_IReadBmpFile ( pstrImage );
		else
		if ( ImageType == IMAGE_ICON )
			hImage = W_IReadIconFile ( pstrImage );
		else
			hImage = W_IReadCursorFile ( pstrImage );

	}
	else
	{
		switch ( ImageType )
		{
			
			case IMAGE_BITMAP   :
				hImage = LoadBitmap ( hInstance, pstrImage ); 
				break;

			case IMAGE_ICON     :
				if ( ! ( hImage = LoadIcon ( hInstance, pstrImage ) ) )
					break;
				if ( GetIconInfo ( hImage, &IconInfo ) )
				{
					hbImage = IconInfo.hbmColor;
					hbImageMask = IconInfo.hbmMask;
				}
				break;
			
			case IMAGE_CURSOR   :
				if ( ! (hImage = LoadCursor ( hInstance, pstrImage ) ) )
					break;
				if ( GetIconInfo (hImage, &IconInfo ) )
				{
					hbImage = IconInfo.hbmColor;
					hbImageMask = IconInfo.hbmMask;
				}
				break;
		}

	}
#if 1
	GetObject ( hbImage, sizeof ( BITMAP ), &Bitmap ); 
	ptSize.x = Bitmap.bmWidth;
	ptSize.y = Bitmap.bmHeight;
	DPtoLP ( hDeskDC, &ptSize, 1 );

	if ( ( hSrcDC = CreateCompatibleDC ( hDeskDC ) ) &&
	     ( hDstDC = CreateCompatibleDC ( hDeskDC ) )
	   ) 
	{
		hBitmap = CreateCompatibleBitmap ( hDeskDC, cxDesired, cyDesired );
		hbmpOld = SelectObject ( hDstDC, hBitmap );
		SelectObject ( hSrcDC, hbImage );
		StretchBlt ( hDstDC , 0, 0, cxDesired, cyDesired, hSrcDC,
			0, 0, ptSize.x, ptSize.y, SRCCOPY );
		SelectObject ( hDstDC, hbmpOld );
		GetObject ( hBitmap, sizeof ( BITMAP ), &Bitmap ); 

		if ( hbImageMask )
		{
			hbmpMask = CreateCompatibleBitmap ( hDeskDC, cxDesired, cyDesired );
			hbmpOld = SelectObject ( hDstDC, hbmpMask );
			SelectObject ( hSrcDC, hbImageMask );
			StretchBlt ( hDstDC, 0, 0, cxDesired, cyDesired, hSrcDC,
				0, 0, ptSize.x, ptSize.y, SRCCOPY );
			SelectObject ( hDstDC, hbmpOld );
		}
	}

	if ( ImageType	== IMAGE_ICON || ImageType == IMAGE_CURSOR )
	{
		IconInfo.hbmColor = hBitmap;
		IconInfo.hbmMask = hbmpMask;
		return CreateIconIndirect ( &IconInfo );
	}

	DeleteObject ( hBitmap );
	DeleteObject ( hbmpMask );
	if ( hDstDC )
		DeleteDC ( hDstDC );
	if ( hSrcDC )
		DeleteDC ( hSrcDC );
	ReleaseDC ( GetDesktopWindow (), hDeskDC );
#endif
	return hImage;

}
#endif

HICON FAR PASCAL
TWIN_LoadIcon(HINSTANCE hInstance, LPCSTR lpIconName,int Style)
{
	HANDLE hResNameInfo;
	HICON hIcon;
	LPNAMEINFO  lpTemplate;
	LPMEMORYINFO    lpMemory = (LPMEMORYINFO)NULL;
	ICONDIRENTRY *picon;
	ICONDIRENTRY *plast;

    	APISTR((LF_APICALL,"LoadIcon(HINSTANCE=%x,LPCSTR==%x)\n",
		hInstance,lpIconName));

	hResNameInfo = FindResource(hInstance, lpIconName, RT_GROUP_ICON);

	if(hResNameInfo == 0) {
    		APISTR((LF_APIRET,"LoadIcon: returns HICON 0\n"));
		return 0;
	}

	lpTemplate = (LPNAMEINFO) HandleLock(hResNameInfo,0);

	if(lpTemplate == 0) {
    		APISTR((LF_APIRET,"LoadIcon: returns HICON 0\n"));
		return 0;
	}

	if (lpTemplate->hGlobal == 0) {
	    lpMemory = HandleAlloc((LPSTR)lpTemplate->rcsdata,MT_RESOURCE);
	    lpTemplate->hGlobal = lpMemory->hMemory;
	}
	if (lpTemplate->rcsdata == 0)
	    LoadResourceEx(hInstance,lpTemplate,lpMemory);
	    
	/* we are now pointing to the ICONDIRENTRY */
	picon = (ICONDIRENTRY *) lpTemplate->rcsdata;

	for(plast=0;picon->wOrdinalNumber;picon++){
		if(plast && plast->bColorCount > picon->bColorCount)
			continue;
		plast = picon;
	}
	if(plast == 0) {
    		APISTR((LF_APIRET,"LoadIcon: returns HICON 0\n"));
		return 0;
	}

	hResNameInfo = FindResource(hInstance, 
		(LPSTR) (unsigned) plast->wOrdinalNumber, RT_ICON);

	if(hResNameInfo == 0) {
    		APISTR((LF_APIRET,"LoadIcon: returns HICON 0\n"));
		return 0;
	}

	lpTemplate = (LPNAMEINFO) HandleLock(hResNameInfo,0);

	if(lpTemplate == 0) {
    		APISTR((LF_APIRET,"LoadIcon: returns HICON 0\n"));
		return 0;
	}
	if(lpTemplate->hObject) {
    	    	APISTR((LF_APIRET,"LoadIcon: returns HICON %x\n",
			lpTemplate->hObject));
	    	return (HICON)lpTemplate->hObject;
	}

	if (lpTemplate->rcsdata == 0)
	    LoadResourceEx(hInstance,lpTemplate,lpMemory);

	hIcon = LoadIconResource(lpTemplate);
		
	lpTemplate->hObject = (HGDIOBJ)hIcon;

	if(lpMemory)
		RELEASEHANDLEINFO(lpMemory);
    	APISTR((LF_APIRET,"LoadIcon: returns HICON %x\n",hIcon));
	return hIcon;
}

HCURSOR 
TWIN_LoadCursor(HINSTANCE hInst, LPCSTR lpCursorRsrc,int Style)
{
	HCURSOR		hCursor;
	HRSRC		hRsrc;
	LPNAMEINFO  lpTemplate;
	LPMEMORYINFO    lpMemory = (LPMEMORYINFO)NULL;
	CURSORDIRENTRY	*lpcursor;

	APISTR((LF_APICALL, "TWIN_LoadCursor(HINSTANCE=%x,LPCSTR=%p)\n", 
		hInst, lpCursorRsrc));

	if ( !(hRsrc = FindResource(hInst, lpCursorRsrc, RT_GROUP_CURSOR)) ) {
    		APISTR((LF_APIFAIL,"TWIN_LoadCursor: returns HCURSOR 0 #1\n"));
		return (HCURSOR)0;
	}

	if (!(lpTemplate = (LPNAMEINFO) HandleLock(hRsrc,0))) {
    		APISTR((LF_APIFAIL,"TWIN_LoadCursor: returns HCURSOR 0 #2\n"));
		return (HCURSOR)0;
	}

	if (!lpTemplate->hGlobal) {
	    lpMemory = (LPMEMORYINFO)HandleAlloc
			((LPSTR)lpTemplate->rcsdata,MT_RESOURCE);
	    lpTemplate->hGlobal = lpMemory->hMemory;
	}
	if (!lpTemplate->rcsdata)
	    LoadResourceEx(hInst,lpTemplate,lpMemory);

	/* we are now pointing to the CURSORDIRENTRY */
	lpcursor = (CURSORDIRENTRY *)lpTemplate->rcsdata;

	hRsrc = FindResource(hInst,
		(LPSTR)(unsigned)lpcursor->wOrdinalNumber,RT_CURSOR);
	if (!hRsrc) {
    		APISTR((LF_APIFAIL,"TWIN_LoadCursor: returns HCURSOR 0 #3\n"));
		return (HCURSOR)0;
	}

	if (!(lpTemplate = (LPNAMEINFO) HandleLock(hRsrc,0))) {
    		APISTR((LF_APIFAIL,"TWIN_LoadCursor: returns HCURSOR 0 #4\n"));
		return (HCURSOR)0;
	}

	if (!lpTemplate->hGlobal) {
	    lpMemory = (LPMEMORYINFO)HandleAlloc
			((LPSTR)lpTemplate->rcsdata,MT_RESOURCE);
	    lpTemplate->hGlobal = lpMemory->hMemory;
	}

	if (lpTemplate->hObject) {
    		APISTR((LF_APIRET,"TWIN_LoadCursor: returns HCURSOR %x\n",
			lpTemplate->hObject));
	    	return (HCURSOR)lpTemplate->hObject;
	}

	if (lpTemplate->rcsdata == 0)
	    LoadResourceEx(hInst,lpTemplate,lpMemory);

 	hCursor = LoadCursorResource(lpTemplate);

	lpTemplate->hObject = (HGDIOBJ)hCursor;

	/* this puts a lock on the resource, so it cannot be destroyed */
	/* we should probably also do this for the case where */
	/* hInst == GetInstanceFromModule("USER") */
	if (hInst == 0) {
		lpTemplate->rcsflags = 1;	
	}

	if(lpMemory)
		RELEASEHANDLEINFO(lpMemory);
    	APISTR((LF_APIRET,"TWIN_LoadCursor: returns HCURSOR %x\n",hCursor));
	return hCursor;
}

HBITMAP
TWIN_LoadBitmap(HINSTANCE hInstance, LPCSTR lpszBitmap,int bUnicode)
{
	HANDLE hResNameInfo;
	LPNAMEINFO  lpTemplate;
	LPMEMORYINFO    lpMemory = (LPMEMORYINFO)NULL;

    	APISTR((LF_APICALL,"LoadBitmap(HINSTANCE=%x,LPCSTR=%p)\n",
		hInstance,lpszBitmap));

	hResNameInfo = FindResource(hInstance, lpszBitmap, RT_BITMAP);

	if(hResNameInfo == 0) {
    		APISTR((LF_APIFAIL,"LoadBitmap: returns HBITMAP %x\n",0));
		return 0;
	}

	lpTemplate = (LPNAMEINFO) HandleLock(hResNameInfo,0);

	if(lpTemplate == 0) {
    		APISTR((LF_APIFAIL,"LoadBitmap: returns HBITMAP %x\n",0));
		return 0;
	}
	if (lpTemplate->hGlobal == 0) {
	    lpMemory = HandleAlloc((LPSTR)lpTemplate->rcsdata,MT_RESOURCE);
	    lpTemplate->hGlobal = lpMemory->hMemory;
	}
	if (lpTemplate->rcsdata == 0)
	    LoadResourceEx(hInstance,lpTemplate,lpMemory);

	if(!lpTemplate->hObject)
	    lpTemplate->hObject = (HGDIOBJ)CreateDIBitmapEx(
			(LPBITMAPIMAGE)lpTemplate->rcsdata,lpTemplate);
	else
	    LOCKGDI(lpTemplate->hObject);

    	APISTR((LF_APIRET,"LoadBitmap: returns HBITMAP %x\n",lpTemplate->hObject));

	if(lpMemory)
		RELEASEHANDLEINFO(lpMemory);
	return (HBITMAP)lpTemplate->hObject;
}


HBITMAP	WINAPI
LoadBitmap(HINSTANCE hInstance, LPCSTR lpszBitmap)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HBITMAP	      rc;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt)
    		rc = modinfo->lptwinrt->lpfbitmap(hInstance,lpszBitmap,0);
	else
		rc = DefaultTwinRT.lpfbitmap(hInstance,lpszBitmap,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return rc;
}

BOOL	WINAPI
FreeResource(HGLOBAL hResData)
{
	BOOL	rc;
	/* Decrement the count on the resdata resource */
    	APISTR((LF_APICALL,"FreeResource(HGLOBAL=%x)\n",hResData));

	rc = HandleUnlock(hResData,1);
    	APISTR((LF_APIRET,"FreeResource: returns BOOL %x\n",rc));
	return rc;
}

#ifdef	STRICT
LPVOID	WINAPI
#else
LPSTR	WINAPI
#endif
LockResource(HANDLE hResData)
{
	LPSTR lp;
    	APISTR((LF_APICALL,"LockResource(HGLOBAL=%x)\n",hResData));
	lp = HandleLock(hResData,1);
    	APISTR((LF_APIRET,"LockResource: returns LPSTR %p\n",lp));
	return lp;
}

HGLOBAL WINAPI
AllocResource(HINSTANCE hInst, HRSRC hRsrc, DWORD bytes)
{
    	APISTR((LF_APISTUB, "AllocResource(HINSTANCE=%x,HRSRC=%x,DWORD=%x)\n",
		hInst, hRsrc, bytes));
	return((HGLOBAL)NULL);
}

RSRCHDLRPROC WINAPI
SetResourceHandler(HINSTANCE hInst, LPCSTR lpszRsrc, RSRCHDLRPROC proc)
{
    	APISTR((LF_APISTUB, "SetResourceHandler(HINSTANCE=%x,LPCSTR=%p,RSRCHDLRPROC=%x)\n",
		hInst,lpszRsrc,proc));
	return((RSRCHDLRPROC)NULL);
}

/* Some of this code is copied from LoadResource() */
HICON	WINAPI
CreateIconFromResource(PBYTE presbits, DWORD dwResSize, BOOL fIcon, DWORD dwVer)
{
  LPHANDLEINFO lpHandle;
  LPMEMORYINFO lpMemory;
  LPNAMEINFO pNameInfo;
  LPVOID lpData;

  pNameInfo = (LPNAMEINFO)WinMalloc(sizeof(NAMEINFO));
  lpHandle = HandleAlloc((LPSTR)pNameInfo,MT_NAMEINFO);

  pNameInfo->rcsdata = presbits;
  pNameInfo->rcslength = dwResSize;
  pNameInfo->wType = (WORD)(DWORD)(fIcon ? RT_ICON : RT_CURSOR);
  lpData = (LPVOID)ConvertResourceToNat(pNameInfo->rcsdata,
					pNameInfo->wType);
  lpMemory = HandleAlloc((LPSTR)lpData,MT_RESOURCE);
  lpMemory->wIndex = pNameInfo->wType;
  lpMemory->dwBinSize = pNameInfo->rcslength;
  pNameInfo->hGlobal = lpMemory->hMemory;

  RELEASEHANDLEINFO(lpHandle);
  RELEASEHANDLEINFO(lpMemory);

  return LoadIconResource(pNameInfo);
}

/*********************************************************/
/***** API Internal Resource Routines ********************/
/*********************************************************/


#define LRT_GET	0
#define LRT_SET 1
#define LRT_PUT 2

typedef struct {
	HINSTANCE	hInstance;
	TYPEINFO	*lpResource;
} INTERNALRESOURCE;

extern TYPEINFO hsmt_resource_USER[];
extern MODULETAB TWIN_ModuleTable[];

TYPEINFO *
LoadResourceTable(int nFunc,HINSTANCE hInst,TYPEINFO *lpResource)
{
	HANDLE hModule;
	LPMODULEINFO modinfo = NULL;

	switch(nFunc) {
	case LRT_SET:
		if(hInst == 0) 
		  lpResource = hsmt_resource_USER;
		else 
		  lpResource = (TYPEINFO*) TWIN_ModuleTable[0].dscr->resource;
		break;

	case LRT_GET:	/* get resource table for given hinstance 	  */
	case LRT_PUT:	/* get/replace resource table for given hinstance */
		if (!hInst) {
		    	hModule = GetModuleHandle("USER");
		} else if (!GETMODULEINFO(hInst)) /* might be hModule already */
		    	hModule = GetModuleFromInstance(hInst);
		else
			hModule = hInst;

		if (!(modinfo = GETMODULEINFO(hModule)))
		    lpResource = (hInst)?(TYPEINFO *)NULL:hsmt_resource_USER;
		else
	            lpResource = modinfo->ResourceTable;

	    	if (modinfo) {
			RELEASEMODULEINFO(modinfo);
	    	}
	    	break;

	default:
	    	break;
	}
	return lpResource;
}

/*********************************************************/
/***** API Resource Routines *****************************/
/*********************************************************/

typedef BOOL (CALLBACK * ENUMRESPROC)(NAMEINFO *, DWORD, LPARAM);

TYPEINFO *
EnumResources(HINSTANCE hInstance,LPCSTR lpType,
		FARPROC lpEnumFunc, LPARAM lParam)
{
	ENUMRESPROC   lpCallBack = 0;
	TYPEINFO *lpResTable;	/* resource table pointer */
  	NAMEINFO *lpResInfo;	/* nameinfo table pointer/resource */
	BOOL	  bByName;
	int       i;
	DWORD	  rval;

	if (HIWORD(lpType) == 0) {
		bByName = FALSE;
	}
	else {
		bByName = TRUE;
	}
	
	for (lpResTable = LoadResourceTable(LRT_GET,hInstance,0);
	     lpResTable && lpResTable->rcstypename;
	     lpResTable++) {
		/* if doing a name check, make sure we have one */
		/* this avoids passing an ordinal to strcasecmp */
		if ( bByName && HIWORD(lpResTable->rcstypename) == 0)
			continue;

		/* compare by ordinal value or matching strings */
		if (!bByName && (lpResTable->rcstypename == lpType))
			break;

		if(bByName && (strcasecmp(lpResTable->rcstypename,lpType) == 0))
			break;
	}

	/* if not found, return NULL */
	if (lpResTable == 0 || lpResTable->rcstypename == 0) {
		return 0;
	}

	if (lpEnumFunc)
		lpCallBack = (ENUMRESPROC)
			MakeProcInstance((FARPROC)lpEnumFunc,hInstance);

	/* search the type, for the specific entry */
	/* note: this could be bByName or by value */
	for (i=0,lpResInfo=lpResTable->rcsinfo;
	    i<(int)(lpResTable->rcscount);
	    i++,lpResInfo++) {
		rval = (DWORD) lpResInfo->rcsitemname;
		if (lpCallBack == 0)
			continue;

		if(HIWORD(rval) == 0)
			rval &= 0x7fff;

	        if (!lpCallBack(lpResInfo, rval, lParam)) {
			break;
		}

	}

	if(lpCallBack)
		FreeProcInstance((FARPROC)lpEnumFunc);
	return lpResTable;
}


/*
 * note:
 *	hiword of both lpname and lptype may be 0
 *	so be prepared to search based on ordinals or by name...
 */
HRSRC
TWIN_FindResource(HINSTANCE hInstance,LPCSTR lpName,LPCSTR lpType,int bUnicode)
{
	TYPEINFO *lpResTable;	/* resource table pointer */
  	NAMEINFO *lpResInfo;	/* nameinfo table pointer/resource */
	LPHANDLEINFO lpHandle; /* pointer to nameinfo table entry */
	HRSRC    hResInfo ;  /* handle of nameinfo table pointer */
	BOOL	  bByName;
	BOOL	  bByOrdinal;
	int       i;
	DWORD	  dwOrdinal = 0;

	APISTR((LF_APICALL,"FindResource(HINSTANCE=%x,LPCSTR=%p,LPCSTR=%p)\n",
		hInstance,lpName,lpType));

	if(HIWORD(lpType) == 0) {
		bByName = FALSE;
	} else {
		bByName = TRUE;
	}
	
	if(HIWORD(lpName) == 0) {
		bByOrdinal = TRUE;
		dwOrdinal = (DWORD)lpName;
	} else {
	    if (lpName[0] == '#') {
	 	dwOrdinal = (DWORD)atol(lpName+1);
		bByOrdinal = TRUE;
	    }
	    else {
		bByOrdinal = FALSE;
	    }
	}
	
	lpResTable = LoadResourceTable(LRT_GET,hInstance,0);

	/* search ResourceTable for Resource Type given by lpType */
	/* note: this could be bByName or by value */
	for(; lpResTable && lpResTable->rcstypename;
	    lpResTable++) {

		/* if doing a name check, make sure we have one */
		/* this avoids passing an ordinal to strcasecmp */
		if ( bByName && HIWORD(lpResTable->rcstypename) == 0)
			continue;

		/* compare by ordinal value or matching strings */
		if(!bByName && (lpResTable->rcstypename == lpType))
			break;

		if(bByName && (strcasecmp(lpResTable->rcstypename,lpType) == 0))
			break;
	}

	/* if not found, return NULL */
	if(lpResTable == 0 || (lpResTable->rcstypename == 0)) {
    		APISTR((LF_APIRET,"FindResource: returns HRSRC %x\n",0));
		return 0;
	}

	/* search the type, for the specific entry */
	/* note: this could be bByName or by value */
	for(i=0,lpResInfo=lpResTable->rcsinfo;
	    i<(int)(lpResTable->rcscount);
	    i++,lpResInfo++) {

		if(bByOrdinal) {
		    if(((DWORD)(lpResInfo->rcsitemname) & 0x7fff) == dwOrdinal)
			break;
		} else  {
		    if(HIWORD(lpResInfo->rcsitemname) && 
			    (strcasecmp(lpResInfo->rcsitemname,lpName) == 0))
			break;
		}
	}

	/* if not found return NULL */
	if(i == lpResTable->rcscount) {
    		APISTR((LF_APIRET,"FindResource: returns HRSRC %x\n",0));
		return 0;
	}

	/* see if we have already found this resource */
	if(lpResInfo->hRsrc) {
    		APISTR((LF_APIRET,"FindResource: returns HRSRC %x\n",
			lpResInfo->hRsrc));
		return lpResInfo->hRsrc;
	}

	/* allocate a handle to this resource item */
	lpHandle = HandleAlloc((LPSTR)lpResInfo,MT_NAMEINFO);

	if(lpHandle == 0) {
    		APISTR((LF_APIRET,"FindResource: returns HRSRC %x\n",0));
		return 0;
	}

	hResInfo = (HRSRC)lpHandle->hMemory;

	/* save the handle for later, don't allocate more than one */
	lpResInfo->hRsrc = hResInfo;

    	APISTR((LF_APIRET,"FindResource: returns HRSRC %x\n",hResInfo));
	RELEASEHANDLEINFO(lpHandle);
	return hResInfo;
}


HRSRC
FindResource(HINSTANCE hInstance,LPCSTR lpName,LPCSTR lpType)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HRSRC	      rc;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		rc = modinfo->lptwinrt->lpffind(hInstance,lpName,lpType,0);
	} else
		rc = DefaultTwinRT.lpffind(hInstance,lpName,lpType,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return rc;
}

HGLOBAL
TWIN_LoadResource(HINSTANCE hInstance,HRSRC hResInfo,int bUnicode)
{
  	NAMEINFO *lpResInfo;		/* nameinfo table pointer/resource */
        LPMEMORYINFO 	lpMemory = (LPMEMORYINFO)NULL;
	LPVOID lpData;
	
	if((lpResInfo =  (LPNAMEINFO) HandleLock(hResInfo,0)) == 0)
		return 0;

	if (!lpResInfo->hGlobal) {
#ifdef	LATER
	/* the following call does the right thing only for RT_BITMAP.
	   To support other types (RT_DIALOG,RT_MENU), we have to change
	   the corresponding code in the library
	*/
#endif
	    if (lpResInfo->rcsdata)
		lpData = (LPVOID)ConvertResourceToNat(lpResInfo->rcsdata,
					lpResInfo->wType);
	    else
		lpData = (LPVOID)0;
	    lpMemory = HandleAlloc((LPSTR)lpData,MT_RESOURCE);
	    lpMemory->wIndex = lpResInfo->wType;
	    lpMemory->dwBinSize = lpResInfo->rcslength;
	    lpResInfo->hGlobal = lpMemory->hMemory;
	}

	if (lpResInfo->rcsdata == 0) {
	    LoadResourceEx(hInstance,lpResInfo,lpMemory);
	    if (lpResInfo->rcsdata)
		lpMemory->lpCore =
			(LPVOID)ConvertResourceToNat(lpResInfo->rcsdata,
			lpResInfo->wType);
	}

	if(lpMemory)
		RELEASEHANDLEINFO(lpMemory);
	return lpResInfo->hGlobal;
}

HGLOBAL WINAPI
LoadResource(HINSTANCE hInstance, HRSRC hResInfo)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HGLOBAL	      rc;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		rc = modinfo->lptwinrt->lpfload(hInstance,hResInfo,0);
	} else
		rc = DefaultTwinRT.lpfload(hInstance,hResInfo,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return rc;
}

DWORD
TWIN_SizeofResource(HINSTANCE hinst, HRSRC hrsrc, int bUnicode)
{
	return HandleSize(hinst,hrsrc);
}

DWORD	WINAPI
SizeofResource(HINSTANCE hInstance, HRSRC hResInfo)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	int	      rc;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		rc = modinfo->lptwinrt->lpfsizeof(hInstance,hResInfo,0);
	} else
		rc = DefaultTwinRT.lpfsizeof(hInstance,hResInfo,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return rc;
}

/****************************************************************/

int	WINAPI
LoadString(HINSTANCE hInstance, UINT wID, LPSTR lpBuffer, int nBufferMax)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	int	      rc;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		rc = modinfo->lptwinrt->lpfstring(hInstance,wID,lpBuffer,nBufferMax,0);
	} else
		rc = DefaultTwinRT.lpfstring(hInstance,wID,lpBuffer,nBufferMax,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return rc;
}

#ifdef TWIN_BIN32
int	WINAPI
LoadStringA(HINSTANCE hInstance, UINT wID, LPSTR lpBuffer, int nBufferMax)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	int	     rc;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		rc = modinfo->lptwinrt->lpfstring(hInstance,wID,lpBuffer,nBufferMax,0);
	} else 
		rc = DefaultTwinRT.lpfstring(hInstance,wID,lpBuffer,nBufferMax,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return rc;
}

int	WINAPI
LoadStringW(HINSTANCE hInstance, UINT wID, LPSTR lpBuffer, int nBufferMax)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	int		rc;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		rc = modinfo->lptwinrt->lpfstring(hInstance,wID,lpBuffer,nBufferMax,1);
	} else
		rc =  DefaultTwinRT.lpfstring(hInstance,wID,lpBuffer,nBufferMax,1);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return rc;
}
#endif

HICON FAR PASCAL WINAPI
LoadIcon(HINSTANCE hInstance, LPCSTR lpIconName)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HICON		hIcon;

    	APISTR((LF_APICALL,"LoadIcon(HINSTANCE=%x,LPCSTR==%x)\n",
		hInstance,lpIconName));

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hIcon =  modinfo->lptwinrt->lpficon(hInstance,lpIconName,0);
	} else
		hIcon =  DefaultTwinRT.lpficon(hInstance,lpIconName,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

    	APISTR((LF_APIRET,"LoadIcon returns HICON %x\n",hIcon));
	return hIcon;
}

#ifdef TWIN_BIN32

HICON FAR PASCAL WINAPI
LoadIconW(HINSTANCE hInstance, LPCSTR lpIconName)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HICON		hIcon;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hIcon = modinfo->lptwinrt->lpficon(hInstance,lpIconName,1);
	} else 
		hIcon = DefaultTwinRT.lpficon(hInstance,lpIconName,1);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);
	
	return hIcon;
}

#endif

HCURSOR WINAPI 
LoadCursor(HINSTANCE hInstance, LPCSTR lpCursorRsrc)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HCURSOR	     hCursor  = 0;

	APISTR((LF_APICALL, "LoadCursor(HINSTANCE=%x,LPCSTR=%p)\n", 
		hInstance, lpCursorRsrc));
	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hCursor = modinfo->lptwinrt->lpfcursor(hInstance,lpCursorRsrc,0);
	} else
		hCursor = DefaultTwinRT.lpfcursor(hInstance,lpCursorRsrc,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	APISTR((LF_APIRET, "LoadCursor returns HCURSOR %x\n",hCursor));
	return hCursor;
}

#ifdef TWIN_BIN32
HCURSOR WINAPI 
LoadCursorA(HINSTANCE hInstance, LPCSTR lpCursorRsrc)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HCURSOR	     hCursor;

	APISTR((LF_APICALL, "LoadCursorA(HINSTANCE=%x,LPCSTR=%p)\n", 
		hInstance, lpCursorRsrc));
	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hCursor = modinfo->lptwinrt->lpfcursor(hInstance,lpCursorRsrc,0);
	} else
		hCursor = DefaultTwinRT.lpfcursor(hInstance,lpCursorRsrc,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	APISTR((LF_APIRET, "LoadCursorA returns HCURSOR %d\n",hCursor));
	return hCursor;
}

HCURSOR WINAPI 
LoadCursorW(HINSTANCE hInstance, LPCSTR lpCursorRsrc)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HCURSOR		hCursor;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hCursor = modinfo->lptwinrt->lpfcursor(hInstance,lpCursorRsrc,1);
	} else
		hCursor = DefaultTwinRT.lpfcursor(hInstance,lpCursorRsrc,1);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return hCursor;
}
#endif

HACCEL	WINAPI
LoadAccelerators(HINSTANCE hInstance, LPCSTR lpTableName)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HACCEL		hAccel;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hAccel = modinfo->lptwinrt->lpfaccel(hInstance,lpTableName,0);
	} else 
		hAccel = DefaultTwinRT.lpfaccel(hInstance,lpTableName,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return hAccel;
}

#ifdef TWIN_BIN32
HACCEL	WINAPI
LoadAcceleratorsA(HINSTANCE hInstance, LPCSTR lpTableName)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HACCEL		hAccel;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hAccel = modinfo->lptwinrt->lpfaccel(hInstance,lpTableName,0);
	} else
		hAccel = DefaultTwinRT.lpfaccel(hInstance,lpTableName,0);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return hAccel;
}

HACCEL	WINAPI
LoadAcceleratorsW(HINSTANCE hInstance, LPCSTR lpTableName)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo = 0;
	HACCEL		hAccel;

	if(hInstance) {
		hModule = GetModuleFromInstance(hInstance);
		modinfo = GETMODULEINFO(hModule);
	}

	if(modinfo && modinfo->lptwinrt) {
    		hAccel = modinfo->lptwinrt->lpfaccel(hInstance,lpTableName,1);
	} else
		hAccel = DefaultTwinRT.lpfaccel(hInstance,lpTableName,1);

	if(modinfo)
		RELEASEMODULEINFO(modinfo);

	return hAccel;
}
#endif
