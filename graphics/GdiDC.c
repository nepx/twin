/*    
	GdiDC.c	2.41
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

#include <string.h>
#include <stdlib.h>

#include "windows.h"
#include "print.h"
#include "kerndef.h"
#include "Log.h"
#include "GdiDC.h"
#include "DeviceData.h"
#include "Driver.h"

extern LSDENTRY *GetDeviceEntry(LPCSTR);
extern HDC PrinterResetDC(HDC32,const DEVMODE *);
extern LSDENTRY *DisplayDeviceEntry;

BOOL IsMetafileDC(HDC);

static HDC GdiCreateDC(LPLSDS_PARAMS, BOOL);
static void GdiInitDCObjects(HDC32);
static void GdiCleanupDC(HDC32);

void TWIN_InitDriver(void);

static DCINFO DCTemplate =
{
    { 0, 0L, 0, 0, NULL },		/* OBJHEAD never to be used */
    (HWND)0,			/* hWnd		*/
    (DWORD)0,			/* dwDCXFlags	*/
    RGB(0,0,0),			/* TextColor - black */
    RGB(255,255,255),		/* BackColor - white */
    OPAQUE,			/* BackMode	*/
    ALTERNATE,			/* PolyFillMode */
    R2_COPYPEN,			/* ROP2		*/
    STRETCH_DELETESCANS,	/* StretchBltMode */
    ABSOLUTE,			/* RelAbs	*/
    TA_LEFT|TA_TOP|TA_NOUPDATECP, /* TextAlign	*/
    0,				/* nBreakExtra	*/
    0,				/* nBreakCount	*/
    0,				/* nErrorTerm	*/
    0,				/* nCharExtra	*/
    (LPINT)NULL,                /* lpExtraSpace */
    0,				/* dwMapperFlags */
    MM_TEXT,			/* MapMode	*/
    0,0,			/* FOx,FOy	*/
    0,0,			/* DOx,DOy	*/
    0,0,			/* WOx,WOy	*/
    1,1,			/* WEx,WEy	*/
    0,0,			/* VOx,VOy	*/
    1,1,			/* VEx,VEy	*/
    DCB_DISABLE,		/* BoundsFlag	*/
    { 0, 0, 0, 0 },		/* BoundsRect	*/
    (HBRUSH)0,			/* hBrush	*/
    { 0, 0 },			/* ptBrushOrg	*/
    (HFONT)0,			/* hFont	*/
    (LPVOID)0,			/* lpCharWidths */
    (LPNEWTEXTMETRIC)0,		/* lpNTM	*/
    (UINT)0,			/* uiFontType	*/
    (int)0,			/* nDevWidth	*/
    (int)0,			/* nDevHeight	*/
    (HPEN)0,			/* hPen		*/
    (HBITMAP)0,			/* hBitmap	*/
    (HPALETTE)0,		/* hPalette	*/
    FALSE,			/* fPalBack	*/
    (LPUINT)0,			/* lpSystemPaletteIndex */
    (HDC32)NULL,		/* lpSavedDC	*/
    0,				/* nSavedDC	*/
    (LSDENTRY *)0,		/* lpLSDEntry	*/
    { 0L, 0L, 0L },		/* Output[3]	*/
    0L,				/* dwInvalid	*/
    (LPVOID)0,			/* lpDrvData	*/
    /* WIN32 */
    AD_COUNTERCLOCKWISE,	/* nArcDirection */
    GM_COMPATIBLE,		/* nGraphicsMode */
    1.0, 0.0,			/* eM11, eM12 */
    0.0, 1.0,			/* eM21, eM22 */
    0.0, 0.0,			/* eDx, eDy */
    10.0,			/* eMiterLimit */
    0,				/* path state (0=none,1=opened,2=closed) */
    0,				/* path size */
    (LPBYTE)0,			/* path types */
    (LPPOINT)0,			/* path points */
    {				/* HALFTONE stretch mode color adjustment */
	sizeof(COLORADJUSTMENT),	/* caSize */
	(WORD)0,			/* caFlags */
	ILLUMINANT_DEVICE_DEFAULT,	/* caIlluminantIndex */
	10000,				/* caRedGamma */
	10000,				/* caGreenGamma */
	10000,				/* caBlueGamma */
	0,				/* caReferenceBlack */
	10000,				/* caReferenceWhite */
	0,				/* caContrast */
	0,				/* caBrightness */
	0,				/* caColorfulness */
	0,				/* caRedGreenTint */
    },
};

DCCACHE *TWIN_DCCache = NULL;
int TWIN_DCCacheSize = 0;

static BOOL TWIN_AllocDCCache(int nGrowSize)
{
	int i;

	TWIN_DCCacheSize += nGrowSize;

	if (TWIN_DCCache) {
		TWIN_DCCache = (DCCACHE *)
		  WinRealloc(TWIN_DCCache, TWIN_DCCacheSize * sizeof(DCCACHE));
	} else {
		TWIN_DCCache = (DCCACHE *)
		  WinMalloc(TWIN_DCCacheSize * sizeof(DCCACHE));
	}

	if (!TWIN_DCCache)
	{
		return (FALSE);
	}

	for (i = TWIN_DCCacheSize - nGrowSize; i < TWIN_DCCacheSize; i++)
	{
		TWIN_DCCache[i].fBusy = FALSE;
		TWIN_DCCache[i].lpDC = 0;
	}

	return (TRUE);

}

DCCACHE *
TWIN_GetDCCache(int i)
{
	if(i < TWIN_DCCacheSize) {
		return &TWIN_DCCache[i];
	}
	return 0;
}

BOOL
GdiInitDC(void)
{
	return (TWIN_AllocDCCache(5));
}

/* This function gets one of the cache DCs, if so asked, or	   */
/* creates a new object. In the future as for support WIN32 DCX_,  */
/* extensions, this function may want to know DCX_ flags, hWnd and */
/* the likes of them to find the exact match in the cache.	   */

HDC32
GdiCreateHDC(BOOL bCache)
{
    HDC32 hDC32 = 0;
    HDC hDC;
    int i;
    static int init_done = FALSE;

    if (!init_done)
    {
	TWIN_InitDriver();
	init_done = TRUE;
    }

    if (!bCache) {
	hDC32 = CREATEHDC(hDC);
    } else {
	int found = FALSE;

	while (found == FALSE) {
		for (i = 0; i < TWIN_DCCacheSize; i++) {
		    if (!TWIN_DCCache[i].fBusy) {
			TWIN_DCCache[i].fBusy = TRUE;

			TWIN_DCCache[i].lpDC = CREATEHDC(hDC);
			hDC32 = TWIN_DCCache[i].lpDC;
			hDC32->dwDCXFlags |= DCX_CACHE_DC;
			found = TRUE;
			break;
		    }
		}

		if(found)
			break;

		/* out of DC cache - enlarge and try again */
		if (!TWIN_AllocDCCache(8))
		{
			return (NULL);
		}
	}
    }
    if (hDC32) {
	memcpy((LPSTR)&hDC32->hWnd,
		(LPSTR)&DCTemplate.hWnd,
		sizeof(DCINFO) - sizeof(OBJHEAD));
    }

    RELEASEHDC(hDC);
    return hDC32;
}

BOOL
IsMetafileDC(HDC hDC)
{
	HDC32 hDC32;
	BOOL bIsMetafileDC;

	ASSERT_HDC(hDC32, hDC, FALSE);

	bIsMetafileDC = (hDC32->dwDCXFlags & DCX_METAFILE_DC) ? TRUE : FALSE;

	RELEASEDCINFO(hDC32);

	return (bIsMetafileDC);

}

static HDC
GdiCreateDC(LPLSDS_PARAMS lpStruct, BOOL bCreateDC)
{
	HDC hDC;
	HDC32 hDC32;
	GDIPROC lpfnOutput;

	if (!(hDC32 = GdiCreateHDC(FALSE)))
	{
		return ((HDC) 0);
	}
	hDC = GETHDC16(hDC32);

	/* find driver entry */
	if (!(hDC32->lpLSDEntry =
		GetDeviceEntry(lpStruct->lsde.create.lpszDriver)))
	{
		RELEASEDCINFO(hDC32);
		FREEDC(hDC);
		return ((HDC) 0);
	}
	lpfnOutput = hDC32->lpLSDEntry->OutputProc;
	hDC32->Output[0] = lpfnOutput;
	hDC32->Output[1] = lpfnOutput;
	hDC32->Output[2] = lpfnOutput;

	/* driver specific initialization */
	if (DC_OUTPUT(LSD_CREATEDC, hDC32, 0, lpStruct) == 0)
	{
        	RELEASEDCINFO(hDC32);
		FREEDC(hDC);
		return ((HDC) 0);
	}

	/* initialize non-metafile DC */
	if (!(hDC32->dwDCXFlags & DCX_METAFILE_DC))
		GdiInitDCObjects(hDC32);

	RELEASEDCINFO(hDC32);
	/* return DC handle */
	return (hDC);
}

static void
GdiInitDCObjects(HDC32 hDC32)
{
	HDC hDC = GETHDC16(hDC32);

	SelectObject(hDC,
		(hDC32->dwDCXFlags & DCX_DISPLAY_DC)
		? GetStockObject(SYSTEM_FONT)
		: GetStockObject(DEVICE_DEFAULT_FONT));
	SelectObject(hDC, GetStockObject(WHITE_BRUSH));
	SelectObject(hDC, GetStockObject(BLACK_PEN));
	SelectPalette(hDC, GetStockObject(DEFAULT_PALETTE), FALSE);
	SetBkColor(hDC, RGB(255,255,255));
	SetTextColor(hDC, RGB(0,0,0));
}

static void
GdiCleanupDC(HDC32 hDC32)
{
	HDC hDC;

	/* get DC handle */
	hDC = GETHDC16(hDC32);

	/* (memory DC) unselect non-stock bitmap */
	if(hDC32->hBitmap)
	{
		UNLOCKGDI(hDC32->hBitmap);
		hDC32->hBitmap = 0;
	}

	/* unselect non-stock brush */
	if(hDC32->hBrush)
	{
		UNLOCKGDI(hDC32->hBrush);
		hDC32->hBrush = 0;
	}

	/* unselect non-stock pen */
	if(hDC32->hPen)
	{
		UNLOCKGDI(hDC32->hPen);
		hDC32->hPen = 0;
	}

	/* unselect non-stock font */
	if(hDC32->hFont)
	{
		UNLOCKGDI(hDC32->hFont);
		hDC32->hFont = 0;
	}

	/* unselect non-stock palette */
	if(hDC32->hPalette)
	{
		UNLOCKGDI(hDC32->hPalette);
		hDC32->hPalette = 0;
	}

	/* select empty clip region */
	SelectClipRgn(hDC, (HRGN) 0);

	/* free internal arrays */
	if (hDC32->lpSystemPaletteIndex)
	{
		WinFree((LPSTR) hDC32->lpSystemPaletteIndex);
		hDC32->lpSystemPaletteIndex = NULL;
	}
	if (hDC32->lpNTM) 
	{
		WinFree((LPSTR) hDC32->lpNTM);
		hDC32->lpNTM = NULL;
	}
	if (hDC32->lpCharWidths)
	{
		WinFree((LPSTR) hDC32->lpCharWidths);
		hDC32->lpCharWidths = NULL;
	}
	if (hDC32->lpPathPoints)
	{
		WinFree((LPSTR) hDC32->lpPathPoints);
		hDC32->lpPathPoints = NULL;
	}
	if (hDC32->lpPathTypes)
	{
		WinFree((LPSTR) hDC32->lpPathTypes);
		hDC32->lpPathTypes = NULL;
	}
}

void
GdiInitDisplayDC(HDC32 hDC32)
{
	GDIPROC lpfnOutput;

	hDC32->lpLSDEntry = DisplayDeviceEntry;
	lpfnOutput = hDC32->lpLSDEntry->OutputProc;
	hDC32->Output[0] = lpfnOutput;
	hDC32->Output[1] = lpfnOutput;
	hDC32->Output[2] = lpfnOutput;

	GdiInitDCObjects(hDC32);
}

/*****************************************************************
		Windows APIs
*****************************************************************/

HDC WINAPI
CreateCompatibleDC(HDC hDC)
{
    HDC32 hDC32 = 0,hCompatDC32;
    HDC	hCompatDC;
    GDIPROC lpfnOutput;

    APISTR((LF_APICALL, "CreateCompatibleDC(HDC=%x)\n",hDC));

    if (hDC && !(hDC32 = GETDCINFO(hDC))) {
    	APISTR((LF_APIFAIL, "CreateCompatibleDC: returns HDC %x\n",0));
	return (HDC)0;
    }

    if (hDC && hDC32->dwDCXFlags & DCX_METAFILE_DC) {
	if(hDC32)
		RELEASEDCINFO(hDC32);
	hDC32 = 0;
	/* fall through as though hDC was NULL */
    }

    if (!(hCompatDC32 = GdiCreateHDC(FALSE))) {
    	APISTR((LF_APIFAIL, "CreateCompatibleDC: returns HDC %x\n",0));
	if(hDC32)
        	RELEASEDCINFO(hDC32);
	return (HDC)0;
    }

    hCompatDC = GETHDC16(hCompatDC32);

    if (hDC32) {
	hCompatDC32->dwDCXFlags = (hDC32->dwDCXFlags & DCXDEVICE_MASK) |
				DCX_COMPATIBLE_DC;
	hCompatDC32->lpLSDEntry = hDC32->lpLSDEntry;
    }
    else {
	hCompatDC32->dwDCXFlags = DCX_DISPLAY_DC|DCX_COMPATIBLE_DC;
	hCompatDC32->lpLSDEntry = DisplayDeviceEntry;
    }

    lpfnOutput = hCompatDC32->lpLSDEntry->OutputProc;
    hCompatDC32->Output[0] = lpfnOutput;
    hCompatDC32->Output[1] = lpfnOutput;
    hCompatDC32->Output[2] = lpfnOutput;

    if (DC_OUTPUT(LSD_CREATEDC,hCompatDC32,hDC32,0) == 0) {
        RELEASEDCINFO(hCompatDC32);
	FREEDC(hCompatDC);
	if(hDC32) {
	    /* release lock on original hDC */
	    RELEASEDCINFO(hDC32);
	}
    	APISTR((LF_APIFAIL, "CreateCompatibleDC: returns HDC %x\n",0));
	return (HDC)0;
    }

    /* release lock on original hDC */
    if(hDC32) {
	    RELEASEDCINFO(hDC32);
    }

    GdiInitDCObjects(hCompatDC32);
    SelectObject(hCompatDC,GetStockObject(SYSTEM_BITMAP));

    RELEASEDCINFO(hCompatDC32);
    APISTR((LF_APIRET, "CreateCompatibleDC: returns HDC %x\n",hCompatDC));
    return hCompatDC;
}

HDC WINAPI
CreateIC(LPCSTR lpszDriver, LPCSTR lpszDevice,
	 LPCSTR lpszOutput, const void *lpvInitData)
{
    LSDS_PARAMS argptr;
    HDC hDC;

    APISTR((LF_APICALL,"CreateIC(LPCSTR=%s, LPCSTR=%s, LPCSTR=%s, void *=%p)\n",
		(lpszDriver) ? lpszDriver : "0",
		(lpszDevice) ? lpszDevice : "0",
		(lpszOutput) ? lpszOutput : "0",
		lpvInitData));

    argptr.lsde.create.lpszDriver = (LPSTR)lpszDriver;
    argptr.lsde.create.lpszDevice = (LPSTR)lpszDevice;
    argptr.lsde.create.lpszOutput = (LPSTR)lpszOutput;
    argptr.lsde.create.lpvInitData = (LPVOID)lpvInitData;

    hDC =  GdiCreateDC(&argptr, FALSE);
    APISTR((LF_APIRET, "CreateIC: returns HDC %x\n",hDC));
    return hDC;
}


HDC WINAPI
CreateDC(LPCSTR lpszDriver, LPCSTR lpszDevice,
	 LPCSTR lpszOutput, const void *lpvInitData)
{
    LSDS_PARAMS argptr;
    HDC hDC;

    APISTR((LF_APICALL,"CreateDC(LCSTR=%s, LCSTR=%s, LCSTR=%s, void *=%p)\n",
		(lpszDriver) ? lpszDriver : "0",
		(lpszDevice) ? lpszDevice : "0",
		(lpszOutput) ? lpszOutput : "0",
		lpvInitData));

    argptr.lsde.create.lpszDriver = (LPSTR)lpszDriver;
    argptr.lsde.create.lpszDevice = (LPSTR)lpszDevice;
    argptr.lsde.create.lpszOutput = (LPSTR)lpszOutput;
    argptr.lsde.create.lpvInitData = (LPVOID)lpvInitData;

    hDC = GdiCreateDC(&argptr, TRUE);

    APISTR((LF_APIRET, "CreateDC: returns HDC %x\n",hDC));
    return hDC;
}

BOOL WINAPI
DeleteDC(HDC hDC)
{
    HDC32 hDC32;

    APISTR((LF_APICALL, "DeleteDC(HDC=%x)\n",hDC));

    if (!(hDC32 = GETHDC32(hDC))) {
        APISTR((LF_APIFAIL, "DeleteDC: returns BOOL %x\n",FALSE));
	return FALSE;
    }

    if (hDC32->hWnd) {
        APISTR((LF_APIFAIL, "DeleteDC: returns BOOL %x\n",FALSE));
	return FALSE;
    }

    if (--hDC32->ObjHead.wRefCount) {
        APISTR((LF_APIRET, "DeleteDC: returns BOOL %x (FIXME %d)\n",FALSE,hDC32->ObjHead.wRefCount));
	return FALSE;
    }

    while (hDC32->lpSavedDC) {
        if (!RestoreDC(hDC, -1))
            break;
    }

    if (!(hDC32->dwDCXFlags & DCX_METAFILE_DC))
	GdiCleanupDC(hDC32);

    DC_OUTPUT(LSD_DELETEDC, hDC32, 0, 0);

    if ((hDC32->dwDCXFlags & DCX_CACHE_DC)) {
	int i;

	for (i = 0; i < TWIN_DCCacheSize; i++) {
	    if (TWIN_DCCache[i].lpDC == hDC32) {
		TWIN_DCCache[i].fBusy = FALSE;
		hDC32->lpNTM = 0;
		hDC32->lpCharWidths = 0;
		hDC32->hFont = 0;
		hDC32->hBrush = 0;
		hDC32->hBitmap = 0;
		hDC32->dwInvalid = (DWORD)-1;

		TWIN_DCCache[i].lpDC = 0;
		break;
	    }
	}
    }

    RELEASEDCINFO(hDC32);
    FREEDC(hDC);
    APISTR((LF_APIRET, "DeleteDC: returns BOOL %x\n",TRUE));
    return TRUE;
}

int WINAPI
SaveDC(HDC hDC)
{
    HDC32 hDC32;
    HDC32 hSavedDC32;
    int	  rc;

    APISTR((LF_APICALL, "SaveDC(HDC=%x)\n",hDC));

    if (!(hDC32 = GETDCINFO(hDC))) {
    	APISTR((LF_APIFAIL, "SaveDC: returns int %x\n",0));
	return 0;
    }

    hSavedDC32 = (HDC32)WinMalloc(sizeof(DCINFO));
    memcpy((LPSTR)hSavedDC32,(LPSTR)hDC32,sizeof(DCINFO));

    /*
     *  Make private copies of some fields that must be reallocated
     *  in distinct instances of the DC structure.
     */
    if (hSavedDC32->lpCharWidths)
    {
	int nCharRange, nElementSize;
	DWORD dwSize;

        nCharRange = hDC32->lpNTM->tmLastChar - hDC32->lpNTM->tmFirstChar + 1;
        if (hDC32->uiFontType == TRUETYPE_FONTTYPE)
	    nElementSize = sizeof(ABC);
	else
	    nElementSize = sizeof(int);
	dwSize = nCharRange * nElementSize;

	hSavedDC32->lpCharWidths = WinMalloc(dwSize);
	memcpy(hSavedDC32->lpCharWidths, hDC32->lpCharWidths, dwSize);
    }
    if (hSavedDC32->lpNTM)
    {
	hSavedDC32->lpNTM = WinMalloc(sizeof(NEWTEXTMETRIC));
	memcpy(hSavedDC32->lpNTM, hDC32->lpNTM, sizeof(NEWTEXTMETRIC));
    }

    hDC32->lpSavedDC = hSavedDC32;
    hSavedDC32->nSavedID = hDC32->nSavedID++;
    DC_OUTPUT(LSD_SAVEDC,hDC32,0,0);

    rc = hSavedDC32->nSavedID+1;
    APISTR((LF_APIRET, "SaveDC: returns int %x\n",rc));
    return rc;
}

BOOL WINAPI
RestoreDC(HDC hDC,int nSavedDC)
{
    HDC32 hDC32, hSavedDC32, hTempDC32;
    int NumToPop;

    APISTR((LF_APICALL, "RestoreDC(HDC=%x,int=%x\n",hDC,nSavedDC));

    ASSERT_HDC(hDC32,hDC,FALSE);

    if (nSavedDC == 0) {
    	APISTR((LF_APIFAIL, "RestoreDC: returns int %x\n",FALSE));
	return FALSE;
    }

    hSavedDC32 = hDC32->lpSavedDC;
    if (nSavedDC < 0) {
	/*
	 *  Negative number means to pop that many off the stack.
	 *  We already have one popped by presetting hSavedDC32,
	 *  so setup the remaining count, and pop them until we
	 *  reach the count, or until we run out of items to pop.
	 */
	NumToPop = -nSavedDC - 1;
	while (NumToPop-- && hSavedDC32)
	    hSavedDC32 = hSavedDC32->lpSavedDC;
    }
    else {
	/*
	 *  Go through looking for a match.  The ID in the saved
	 *  DC structure is actually one less than what we were
	 *  passed.
	 */
	while (hSavedDC32 && hSavedDC32->nSavedID != nSavedDC-1) 
	    hSavedDC32 = hSavedDC32->lpSavedDC;
    }
    if (!hSavedDC32)
	return FALSE;
    hTempDC32 = hDC32->lpSavedDC;
    while (hTempDC32 && hTempDC32 != hSavedDC32) {
	/*
	 *  If there are intervening saved DCs, pop them off
	 *  the stack now.
	 */
	hDC32->lpSavedDC = hTempDC32->lpSavedDC;
	DC_OUTPUT(LSD_RESTOREDC,hTempDC32,0,0);
	if (hTempDC32->lpCharWidths)
	{
	    WinFree(hTempDC32->lpCharWidths);
	    hTempDC32->lpCharWidths = 0;
	}
	if (hTempDC32->lpNTM)
	{
	    WinFree(hTempDC32->lpNTM);
	    hTempDC32->lpNTM = 0;
	}
	WinFree((LPSTR)hTempDC32);
	hTempDC32 = hDC32->lpSavedDC;
    }

    hSavedDC32->dwInvalid = IM_INVALID;

    DC_OUTPUT(LSD_RESTOREDC,hDC32,(DWORD)hSavedDC32,0);
    if (hDC32->lpCharWidths)
	WinFree(hDC32->lpCharWidths);
    if (hDC32->lpNTM)
	WinFree(hDC32->lpNTM);

    memcpy((LPSTR)hDC32,(LPSTR)hSavedDC32,sizeof(DCINFO));

    if (hDC32->lpSavedDC == NULL)
	hDC32->nSavedID = 0;
    
    WinFree((LPSTR)hSavedDC32);

    APISTR((LF_APIRET, "RestoreDC: returns int %x\n",TRUE));
    return TRUE;
}

HDC WINAPI
ResetDC(HDC hDC, const DEVMODE *lpdm)
{
    HDC32 hDC32;
    HDC hRetDC;

    APISTR((LF_APICALL,"ResetDC(HDC=%x)\n",hDC));

    ASSERT_HDC(hDC32,hDC,0);

    /* as this API makes sense only for printer DCs, we do not need */
    /* to pass thru LSD interface, but will call the func inside */
    /* the printer logic directly */
    if (!(hDC32->dwDCXFlags & DCX_PRINTER_DC))
	hRetDC = (HDC)0;
    else
	hRetDC = (HDC)PrinterResetDC(hDC32,lpdm);

    RELEASEDCINFO(hDC32);

    APISTR((LF_APIRET, "ResetDC: returns HDC %x\n",hRetDC));
    return (hRetDC);
}

int
GetEnvironment(LPCSTR lpszPort, void FAR *lpvEnviron, UINT cbMaxCopy)
{
    	APISTR((LF_APISTUB, "GetEnvironment(LPCSTR=%s,void *=%p,UINT=%x) returns int %x\n",
		lpszPort,lpvEnviron,cbMaxCopy,0));
	return 0;	/* No environment found */
}


int
SetEnvironment(LPCSTR lpszPort, const void FAR *lpvEnviron, UINT cbMaxCopy)
{
    	APISTR((LF_APISTUB, "SetEnvironment(LPCSTR=%s,void *=%p,UINT=%x) returns int %x\n",
		lpszPort,lpvEnviron,cbMaxCopy,0));
	return 0;
}

