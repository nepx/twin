/*    
	Palette.c	2.29
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

#include "windows.h"
#include "windowsx.h"
#include "GdiDC.h"
#include "GdiObjects.h"
#include "Kernel.h"
#include "Log.h"
#include "Driver.h"
#include "DeviceData.h"

	/* enable new palette code */

HPALETTE32 TWIN_hPalRealized32;

COLORREF TWIN_PaletteRGB(HDC32,COLORREF);
LPRGBQUAD TWIN_MapPaletteColors(HDC,LPBITMAPINFO);
BOOL TWIN_IsColorInPalette(HPALETTE,COLORREF);

HPALETTE	WINAPI
CreatePalette(const LOGPALETTE *lpLogPalette)
{
	HPALETTE hPalette;
	LPPALETTE lpPalette;
	UINT u;

	APISTR((LF_APICALL, "CreatePalette(LOGPALETTE *%p)\n",
		lpLogPalette));

	if (!lpLogPalette)
	{
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "CreatePalette: returns HPALETTE 0\n"));
		return ((HPALETTE) 0);
	}

	if (!(lpPalette = CREATEHPALETTE(hPalette)))
	{
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "CreatePalette: returns HPALETTE 0\n"));
		return ((HPALETTE) 0);
	}

	if (!(lpPalette->lpLogPalette = (LPLOGPALETTE)
		WinMalloc(sizeof(LOGPALETTE)
		+ lpLogPalette->palNumEntries * sizeof(PALETTEENTRY))))
	{
#ifdef LATER
		/* DELETE uninitialized HPALETTE here */
#endif
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "CreatePalette: returns HPALETTE 0\n"));
		return ((HPALETTE) 0);
	}

	lpPalette->lpLogPalette->palVersion = lpLogPalette->palVersion;
	lpPalette->lpLogPalette->palNumEntries = lpLogPalette->palNumEntries;
	for (u = 0; u < lpLogPalette->palNumEntries; u++)
	{
		lpPalette->lpLogPalette->palPalEntry[u]
			= lpLogPalette->palPalEntry[u];
	}

	lpPalette->fIsRealized = FALSE;

	RELEASEPALETTEINFO(lpPalette);

	APISTR((LF_APIRET, "CreatePalette: returns HPALETTE %x\n",hPalette));
	return (hPalette);

}

HPALETTE	WINAPI
SelectPalette(HDC hDC, HPALETTE hPalette, BOOL bForceBackground)
{
	HDC32 hDC32;
	HPALETTE hOldPalette;

	APISTR((LF_APICALL, "SelectPalette(HDC=%x,HPALETTE=%x,BOOL=%d)\n",
		hDC, hPalette, bForceBackground));

	ASSERT_HDC(hDC32, hDC, (HPALETTE) 0);

	hOldPalette = (HPALETTE) DC_OUTPUT(LSD_SELECTPALETTE, hDC32, hPalette,
		bForceBackground);

	RELEASEDCINFO(hDC32);

	APISTR((LF_APIRET, "SelectPalette: returns HPALETTE %x\n",hOldPalette));
	return (hOldPalette);

}

UINT	WINAPI
RealizePalette(HDC hDC)
{
	HDC32 hDC32;
	UINT uNumRealizePalette;

	APISTR((LF_APICALL, "RealizePalette(HDC=%x)\n", hDC));

	ASSERT_HDC(hDC32, hDC, GDI_ERROR);

	uNumRealizePalette = (UINT) DC_OUTPUT(LSD_REALIZEPALETTE, hDC32, 0, 0);

	RELEASEDCINFO(hDC32);

	APISTR((LF_APIRET, "RealizePalette: returns UINT %x\n",
		uNumRealizePalette));
	return (uNumRealizePalette);

}

BOOL	WINAPI
ResizePalette(HPALETTE hPalette, UINT uPaletteSize)
{
	LPPALETTE lpPalette;
	LPLOGPALETTE lpLogPalette;
	UINT u;

	APISTR((LF_APICALL, "ResizePalette(HPALETTE=%x,UINT=%x)\n",
		hPalette, uPaletteSize));

	if((lpPalette = GETPALETTEINFO(hPalette)) == 0) {
		APISTR((LF_APIRET, "ResizePalette: returns BOOL 0\n"));
		return FALSE;
	}

	if (!(lpLogPalette = (LPLOGPALETTE) WinRealloc(lpPalette->lpLogPalette,
		sizeof(LOGPALETTE) + uPaletteSize * sizeof(PALETTEENTRY))))
	{
		RELEASEPALETTEINFO(lpPalette);
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "ResizePalette: returns BOOL 0\n"));
		return (FALSE);
	}

	for (u = lpLogPalette->palNumEntries; u < uPaletteSize; u++)
	{
		lpLogPalette->palPalEntry[u].peRed = 0;
		lpLogPalette->palPalEntry[u].peGreen = 0;
		lpLogPalette->palPalEntry[u].peBlue = 0;
		lpLogPalette->palPalEntry[u].peFlags = 0;
	}
	lpLogPalette->palNumEntries = uPaletteSize;
	lpPalette->lpLogPalette = lpLogPalette;

	RELEASEPALETTEINFO(lpPalette);

	APISTR((LF_APIRET, "ResizePalette: returns BOOL 1\n"));
	return (TRUE);

}

UINT	WINAPI
SetPaletteEntries(HPALETTE hPalette,
	UINT uPaletteIndex, UINT uNumPaletteEntries,
	const PALETTEENTRY *lpPaletteEntries)
{
	LPPALETTE lpPalette;
	LPLOGPALETTE lpLogPalette;
	UINT u;

	APISTR((LF_APICALL, 
	   "SetPaletteEntries(HPALETTE=%x,UINT=%x,UINT=%x,PALETTENTRY *%x)\n",
		hPalette, uPaletteIndex, uNumPaletteEntries,
		lpPaletteEntries));

	if((lpPalette = GETPALETTEINFO(hPalette)) == 0) {
		APISTR((LF_APIRET, "SetPaletteEntries: returns UINT 0\n"));
		return FALSE;
	}

	if (!(lpLogPalette = lpPalette->lpLogPalette)
	 || (uPaletteIndex >= lpLogPalette->palNumEntries)
	 || (uPaletteIndex + uNumPaletteEntries > lpLogPalette->palNumEntries)
	 || !lpPaletteEntries)
	{
		RELEASEPALETTEINFO(lpPalette);
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "SetPaletteEntries: returns UINT 0\n"));
		return (0);
	}

	for (u = 0; u < uNumPaletteEntries; u++)
	{
		lpLogPalette->palPalEntry[uPaletteIndex + u]
			= lpPaletteEntries[u];
	}

	RELEASEPALETTEINFO(lpPalette);

	APISTR((LF_APIRET, "SetPaletteEntries: returns UINT %x\n",
		uNumPaletteEntries));
	return (uNumPaletteEntries);

}

UINT	WINAPI
GetPaletteEntries(HPALETTE hPalette,
	UINT uPaletteIndex, UINT uNumPaletteEntries,
	LPPALETTEENTRY lpPaletteEntries)
{
	LPPALETTE lpPalette;
	LPLOGPALETTE lpLogPalette;
	UINT u;

	APISTR((LF_APICALL, 
	   "GetPaletteEntries(HPALETTE=%x,UINT=%x,UINT=%x,PALETTENTRY *%x)\n",
		hPalette, uPaletteIndex, uNumPaletteEntries,
		lpPaletteEntries));

	if((lpPalette = GETPALETTEINFO(hPalette)) == 0) {
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "GetPaletteEntries: returns UINT 0\n"));
		return FALSE;
	}

	if (!(lpLogPalette = lpPalette->lpLogPalette))
	{
		RELEASEPALETTEINFO(lpPalette);
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "GetPaletteEntries: returns UINT 0\n"));
		return (0);
	}

	if (!lpPaletteEntries)
	{
		uNumPaletteEntries = lpLogPalette->palNumEntries;
		RELEASEPALETTEINFO(lpPalette);
		APISTR((LF_APIRET, "GetPaletteEntries: returns UINT %x\n",
			uNumPaletteEntries));
		return (uNumPaletteEntries);
	}

	if (uPaletteIndex >= lpLogPalette->palNumEntries)
	{
		RELEASEPALETTEINFO(lpPalette);
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "GetPaletteEntries: returns UINT 0\n"));
		return (0);
	}

	uNumPaletteEntries = min(uNumPaletteEntries,
		lpLogPalette->palNumEntries - uPaletteIndex);

	for (u = 0; u < uNumPaletteEntries; u++)
	{
		lpPaletteEntries[u]
			= lpLogPalette->palPalEntry[uPaletteIndex + u];
	}

	RELEASEPALETTEINFO(lpPalette);

	APISTR((LF_APIRET, "GetPaletteEntries: returns UINT %x\n",
		uNumPaletteEntries));
	return (uNumPaletteEntries);

}

UINT	WINAPI
GetSystemPaletteEntries(HDC hDC,
	UINT uPaletteIndex, UINT uNumPaletteEntries, 
	LPPALETTEENTRY lpPaletteEntries)
{
	HDC32 hDC32;
	HPALETTE hSystemPalette;
	LPPALETTE lpSystemPalette = 0;
	UINT uNumSystemPaletteEntries;

	APISTR((LF_APICALL, 
	"GetSystemPaletteEntries(HDC=%x,UINT=%x,UINT=%x,PALETTEENTRY *%x)\n",
		hDC, uPaletteIndex, uNumPaletteEntries, lpPaletteEntries));

	ASSERT_HDC(hDC32, hDC, 0);

	if (!(hSystemPalette = (HPALETTE)
		DC_OUTPUT(LSD_GETSYSTEMPALETTE, hDC32, 0, 0))
	 || !(lpSystemPalette = GETPALETTEINFO(hSystemPalette)))
	{
		RELEASEDCINFO(hDC32);
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "GetSystemPaletteEntries: returns UINT 0\n"));
		return (0);
	}

	uNumSystemPaletteEntries = GetPaletteEntries(hSystemPalette,
		uPaletteIndex, uNumPaletteEntries, lpPaletteEntries);

	if(lpSystemPalette)
		RELEASEPALETTEINFO(lpSystemPalette);

	RELEASEDCINFO(hDC32);

	APISTR((LF_APIRET, "GetSystemPaletteEntries: returns UINT %x\n",
		uNumSystemPaletteEntries));
	return (uNumSystemPaletteEntries);

}

UINT	WINAPI
GetSystemPaletteUse(HDC hDC)
{
	HDC32 hDC32;
	UINT uSystemPaletteUse;

	APISTR((LF_APICALL, "GetSystemPaletteUse(HDC=%x)\n", hDC));

	ASSERT_HDC(hDC32, hDC, SYSPAL_ERROR);

	uSystemPaletteUse = (UINT)
		DC_OUTPUT(LSD_SYSTEMPALETTEUSE, hDC32, SYSPAL_ERROR, FALSE);

	RELEASEDCINFO(hDC32);

	APISTR((LF_APIRET, "GetSystemPaletteUse: returns UINT %x\n", 
		uSystemPaletteUse));
	return (uSystemPaletteUse);

}

UINT	WINAPI
SetSystemPaletteUse(HDC hDC, UINT uSystemPaletteUse)
{
	HDC32 hDC32;
	UINT uOldSystemPaletteUse;

	APISTR((LF_APICALL, "SetSystemPaletteUse(HDC=%x,UINT=%d)\n",
		hDC, uSystemPaletteUse));

	ASSERT_HDC(hDC32, hDC, SYSPAL_ERROR);

	uOldSystemPaletteUse = (UINT)
		DC_OUTPUT(LSD_SYSTEMPALETTEUSE, hDC32, uSystemPaletteUse, TRUE);

	RELEASEDCINFO(hDC32);

	APISTR((LF_APIRET, "SetSystemPaletteUse: returns UINT %x\n", 
		uOldSystemPaletteUse));

	return (uOldSystemPaletteUse);

}

UINT	WINAPI
GetNearestPaletteIndex(HPALETTE hPalette, COLORREF crColor)
{
	LPPALETTE lpPalette;
	LPLOGPALETTE lpLogPalette;
	UINT u, uNearestPaletteIndex;
	long delta0, r0, g0, b0, delta, r, g, b;

	APISTR((LF_APICALL, 
		"GetNearestPaletteIndex(HPALETTE=%x,COLORREF=%x)\n",
		hPalette, crColor));

	if((lpPalette = GETPALETTEINFO(hPalette)) == 0) {
		APISTR((LF_APIRET, "GetNearestPaletteIndex: returns UINT %x\n",
			CLR_INVALID));
		return FALSE;
	}

	if (!(lpLogPalette = lpPalette->lpLogPalette))
	{
		RELEASEPALETTEINFO(lpPalette);

		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "GetNearestPaletteIndex: returns UINT %x\n",
			CLR_INVALID));
		return (CLR_INVALID);
	}

	r0 = GetRValue(crColor);
	g0 = GetGValue(crColor);
	b0 = GetBValue(crColor);
	delta0 = 0x030000L;
	uNearestPaletteIndex = CLR_INVALID;
		
	for (u = 0; u < lpLogPalette->palNumEntries; u++)
	{
		if ((lpLogPalette->palPalEntry[u].peRed == r0)
		 && (lpLogPalette->palPalEntry[u].peGreen == g0)
		 && (lpLogPalette->palPalEntry[u].peBlue == b0))
		{
			uNearestPaletteIndex = u;
			break;
		}
		r = lpLogPalette->palPalEntry[u].peRed;
		g = lpLogPalette->palPalEntry[u].peGreen;
		b = lpLogPalette->palPalEntry[u].peBlue;
		delta = (r-r0)*(r-r0) + (g-g0)*(g-g0) + (b-b0)*(b-b0);
		if (delta < delta0)
		{
			uNearestPaletteIndex = u;
			delta0 = delta;
		}
	}

	RELEASEPALETTEINFO(lpPalette);

	APISTR((LF_APIRET, "GetNearestPaletteIndex: returns UINT %x\n",
		uNearestPaletteIndex));
	return (uNearestPaletteIndex);

}

BOOL	WINAPI
AnimatePalette(HPALETTE hPalette,
	UINT uPaletteIndex, UINT uNumPaletteEntries,
	const PALETTEENTRY *lpPaletteEntries)
{
	LPPALETTE lpPalette;
	LPLOGPALETTE lpLogPalette;
	UINT u, uAnimatePaletteIndex;
	BYTE r, g;
	BOOL bSuccess = TRUE;

	APISTR((LF_APICALL, 
	"AnimatePalette(HPALETTE=%x,UINT=%x,UINT=%x,PALETTEENTRY *%p)\n",
		hPalette, uPaletteIndex, uNumPaletteEntries,
		lpPaletteEntries));

	/* (UNDOCUMENTED) null palette handle ==> animate system palette */
	if (!hPalette)
	{
		for (u = uAnimatePaletteIndex = 0; u < uNumPaletteEntries;
		     u++, uAnimatePaletteIndex++)
		{
			if (!(lpPaletteEntries[u].peFlags & PC_RESERVED))
				continue;
			DRVCALL_COLORS(PLH_ANIMATEPALETTE, 0,
				uAnimatePaletteIndex, lpPaletteEntries + u);
		}

		APISTR((LF_APIRET, "AnimatePalette: returns BOOL TRUE\n"));
		return (TRUE);
	}

	if((lpPalette = GETPALETTEINFO(hPalette)) == 0) {
		APISTR((LF_APIRET, "AnimatePalette: returns BOOL FALSE\n"));
		return FALSE;
	}

	if (!(lpLogPalette = lpPalette->lpLogPalette)
	 || (uPaletteIndex >= lpLogPalette->palNumEntries)
	 || (uPaletteIndex + uNumPaletteEntries > lpLogPalette->palNumEntries)
	 || !lpPaletteEntries)
	{
		RELEASEPALETTEINFO(lpPalette);
		SetLastErrorEx(1, 0);
		APISTR((LF_APIRET, "AnimatePalette: returns BOOL FALSE\n"));
		return (FALSE);
	}

	for (u = 0; u < uNumPaletteEntries; u++)
	{
		if (!(lpLogPalette->palPalEntry[uPaletteIndex+u].peFlags &
			PC_RESERVED))
		{
			continue;
		}
		if (!(lpLogPalette->palPalEntry[uPaletteIndex+u].peFlags &
			PC_EXPLICIT))
		{
			continue;
		}
		r = lpLogPalette->palPalEntry[uPaletteIndex+u].peRed;
		g = lpLogPalette->palPalEntry[uPaletteIndex+u].peGreen;
		uAnimatePaletteIndex = r | (g << 8);
		if (uAnimatePaletteIndex == 0)
		{
			continue;
		}
		else
		{
			bSuccess = DRVCALL_COLORS(PLH_ANIMATEPALETTE, 0,
				uAnimatePaletteIndex, lpPaletteEntries + u);
		}
	}

	RELEASEPALETTEINFO(lpPalette);

	APISTR((LF_APIRET, "AnimatePalette: returns BOOL %d\n",bSuccess));
	return (bSuccess);

}

BOOL	WINAPI
UpdateColors(HDC hDC)
{
	HDC32 hDC32;
	BOOL bUpdateColors;

	APISTR((LF_APICALL, "UpdateColors(HDC=%x)\n", hDC));

	ASSERT_HDC(hDC32, hDC, FALSE);

	bUpdateColors = (BOOL) DC_OUTPUT(LSD_UPDATECOLORS, hDC32, 0, 0);

	RELEASEDCINFO(hDC32);

	APISTR((LF_APIRET, "UpdateColors: returns BOOL %d\n",bUpdateColors));
	return (bUpdateColors);
}

COLORREF
TWIN_PaletteRGB(HDC32 hDC32, COLORREF crColor)
{
    LPPALETTE lpPalette;
    UINT uIndex;
    LPPALETTEENTRY pe;

    if((lpPalette = GETPALETTEINFO(hDC32->hPalette)) == 0) {
		return FALSE;
    }

    /* PALETTE_RGB */
    if (crColor & PALETTE_RGB)
    {
	RELEASEPALETTEINFO(lpPalette);
	return (crColor & ~PALETTE_MASK);
    }

    /* PALETTE_INDEX */
    uIndex = crColor & ~PALETTE_MASK;
    if (uIndex >= lpPalette->lpLogPalette->palNumEntries)
	uIndex = 0;
    pe = &(lpPalette->lpLogPalette->palPalEntry[uIndex]);
    crColor = RGB(pe->peRed, pe->peGreen, pe->peBlue);
    RELEASEPALETTEINFO(lpPalette);

    return crColor;
}

LPRGBQUAD
TWIN_MapPaletteColors(HDC hDC, LPBITMAPINFO lpbmi)
{
    int lockpalette = 0;
    HDC32 hDC32 = 0;
    LPPALETTE lpPalette;
    LPRGBQUAD lpRGB;
    int nNumColors,i;
    WORD *lpIndex;

    if (hDC == 0) {
	if (!(lpPalette = TWIN_hPalRealized32)) {
    	    HPALETTE hPalette;

	    hPalette = GetStockObject(DEFAULT_PALETTE);
	    lpPalette = GETPALETTEINFO(hPalette);
	    TWIN_hPalRealized32 = lpPalette;
	}
    }
    else {
	ASSERT_HDC(hDC32,hDC,0);
	lpPalette = GETPALETTEINFO(hDC32->hPalette);
	lockpalette++;
    }
    if (lpPalette == NULL) {
	if(hDC32)
        	RELEASEDCINFO(hDC32);
	return NULL;
    }

    nNumColors = 1 << lpbmi->bmiHeader.biBitCount;
    if (lpbmi->bmiHeader.biClrUsed)
	nNumColors = min(nNumColors,lpbmi->bmiHeader.biClrUsed);

    lpRGB = (LPRGBQUAD)WinMalloc(sizeof(RGBQUAD) * nNumColors);
    memset((LPSTR)lpRGB,'\0',sizeof(RGBQUAD)*nNumColors);
    /* When iUsage is DIB_PAL_COLORS, "bmiColors is an array of 16-bit
     * unsigned integers that specify indices into the currently realized
     * logical palette, instead of explicit RGB values."  See Win32 docs.
     */
    lpIndex = (WORD *)&lpbmi->bmiColors[0];

    for (i=0; i<nNumColors; i++) {
	lpRGB[i].rgbRed
		= lpPalette->lpLogPalette->palPalEntry[*lpIndex].peRed;
	lpRGB[i].rgbGreen
		= lpPalette->lpLogPalette->palPalEntry[*lpIndex].peGreen;
	lpRGB[i].rgbBlue
		 = lpPalette->lpLogPalette->palPalEntry[*lpIndex].peBlue;
	lpIndex++;
    }

    if(hDC32)
    	RELEASEDCINFO(hDC32);
    if(lockpalette)
    	RELEASEPALETTEINFO(lpPalette);
    return lpRGB;
}

BOOL
TWIN_IsColorInPalette(HPALETTE hPalette, COLORREF cr)
{
    LPPALETTE lpPalette;
    int i;

    if((lpPalette = GETPALETTEINFO(hPalette)) == 0) {
		return FALSE;
	}

    for (i=0; i < lpPalette->lpLogPalette->palNumEntries; i++) {
	if ((lpPalette->lpLogPalette->palPalEntry[i].peRed == GetRValue(cr))
	 || (lpPalette->lpLogPalette->palPalEntry[i].peGreen == GetGValue(cr))
	 || (lpPalette->lpLogPalette->palPalEntry[i].peBlue == GetBValue(cr)))
	{
	    RELEASEPALETTEINFO(lpPalette);
	    return TRUE;
	}
    }
    RELEASEPALETTEINFO(lpPalette);
    return FALSE;
}

/* (WIN32) GDI Halftone Palette ******************************************** */

HPALETTE	WINAPI
CreateHalftonePalette(HDC hDC)
{
	HPALETTE hPalette = 0;
	HDC32 hDC32;

	APISTR((LF_APISTUB,"CreateHalftonePalette(HDC=%x)\n", hDC));

	ASSERT_HDC(hDC32, hDC, (HPALETTE) 0);

	RELEASEDCINFO(hDC32);

	return (hPalette);
}
