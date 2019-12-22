/*
	@(#)DrvRegions.c	2.10
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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "windows.h"
#include "windowsx.h"
#include "Log.h"
#include "Driver.h"
#include "mtlock.h"

static DWORD DrvRegionsCreateRegion(LPARAM, LPARAM, LPVOID);  
static DWORD DrvRegionsCreatePolyRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsCreateRectRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsCreateEllipticRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsCreateRoundRectRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsDestroyRegion(LPARAM, LPARAM, LPVOID); 
static DWORD DrvRegionsSetRegionEmpty(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsSetRectRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsPtInRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsRectInRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsIsEmptyRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsEqualRegion(LPARAM, LPARAM, LPVOID); 
static DWORD DrvRegionsOffSetRegion(LPARAM, LPARAM, LPVOID); 
static DWORD DrvRegionsUnionRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsCopyRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsIntersectRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsDiffRegion(LPARAM, LPARAM, LPVOID); 
static DWORD DrvRegionsXORRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsUnionRectWithRegion(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsRegionBox(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsGetRegionData(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsDoNothing(LPARAM, LPARAM, LPVOID);
static DWORD DrvRegionsGetCaps(LPARAM, LPARAM, LPVOID);
DWORD DrvRegionsInit(LPARAM, LPARAM, LPVOID);
DWORD DrvRegionsTab(void);

extern XPoint *DrvConvertPoints(LPPOINT,int);

static TWINDRVSUBPROC DrvRegionsEntryTab[] = {
	DrvRegionsInit,
	DrvRegionsGetCaps,
	DrvRegionsDoNothing,
	DrvRegionsCreateRegion,
	DrvRegionsCreatePolyRegion,
	DrvRegionsCreateRectRegion,
	DrvRegionsCreateEllipticRegion,
	DrvRegionsCreateRoundRectRegion,
	DrvRegionsDestroyRegion,
	DrvRegionsSetRegionEmpty,
	DrvRegionsSetRectRegion,
	DrvRegionsPtInRegion,
	DrvRegionsRectInRegion,
	DrvRegionsIsEmptyRegion,
	DrvRegionsEqualRegion,
	DrvRegionsOffSetRegion,
	DrvRegionsUnionRegion,
	DrvRegionsCopyRegion,
	DrvRegionsIntersectRegion,
	DrvRegionsDiffRegion,
	DrvRegionsXORRegion,
	DrvRegionsUnionRectWithRegion,
	DrvRegionsRegionBox,
	DrvRegionsGetRegionData
};

DWORD DrvRegionsInit(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
		return (DWORD)TRUE; 
}

DWORD DrvRegionsTab(void)
{
	return (DWORD)DrvRegionsEntryTab;
}

static DWORD
DrvRegionsDoNothing(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
	return 0L;
}

static DWORD
DrvRegionsGetCaps(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
	return (DWORD) RSUB_CANDO_RECT;
}
	
/* No Parameters */ 
DWORD
DrvRegionsCreateRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	return (DWORD)XCreateRegion();
}

/* dwParm1 - number of point in the polygon */
/* dwParm2 - polygon-filling mode */
/* lpStruct - address of array XPoints */
DWORD
DrvRegionsCreatePolyRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	LPPOINT lppt;
	XPoint  *cvtpoints;
	DWORD   rc;

	if (!(lppt = (LPPOINT)lpStruct))
		return (DWORD)0;

	cvtpoints = DrvConvertPoints(lppt,(int)dwParm1);

 	LOCK_SECTION( _MTLOCK_VIDEO);
	rc = (DWORD)XPolygonRegion(
		cvtpoints,
		(int)dwParm1,
		(int)dwParm2 == WINDING?WindingRule:EvenOddRule);
	WinFree(cvtpoints);
 	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}

/* No Parameters */
DWORD
DrvRegionsCreateEllipticRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	return (DWORD)0;
}

/* No Parameters */
DWORD 
DrvRegionsCreateRoundRectRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	return (DWORD)0;
}

/* lpStruct - pointer to RECT */
DWORD
DrvRegionsCreateRectRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	LPRECT lpRect;
	Region region;
	XRectangle xRect;

	if (!(lpRect = (LPRECT)lpStruct))
		return (DWORD)ERROR;
	LOCK_SECTION( _MTLOCK_VIDEO);
	region = XCreateRegion();
	UNLOCK_SECTION( _MTLOCK_VIDEO);

	xRect.x = lpRect->left;
	xRect.y = lpRect->top;
	xRect.width = lpRect->right - lpRect->left;
	xRect.height = lpRect->bottom - lpRect->top;

	LOCK_SECTION( _MTLOCK_VIDEO);
	XUnionRectWithRegion(&xRect, region, region);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
		
	return (DWORD)region;
}

/* dwParm1 - Region to destroy */
DWORD
DrvRegionsDestroyRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	LOCK_SECTION( _MTLOCK_VIDEO);
	XDestroyRegion((Region)dwParm1);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return (DWORD)SIMPLEREGION;
}

/* dwParm1 - Region to set empty */
DWORD
DrvRegionsSetRegionEmpty(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	Region EmptyRegion;

 	LOCK_SECTION( _MTLOCK_VIDEO);
	EmptyRegion = XCreateRegion();
	XUnionRegion(EmptyRegion, EmptyRegion, (Region)dwParm1);
	XDestroyRegion(EmptyRegion);
 	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return (DWORD)NULLREGION;
}

/* dwParm1 - Region to set */
/* lpStruct - Pointer to RECT */
DWORD
DrvRegionsSetRectRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	LPRECT lpRect;
	XRectangle xRect;	
	Region EmptyRegion;
	
	if (!(lpRect = (LPRECT)lpStruct))
		return (DWORD)ERROR;

	xRect.x = lpRect->left;
	xRect.y = lpRect->top;
	xRect.width = lpRect->right - lpRect->left;
	xRect.height = lpRect->bottom - lpRect->top;

 	LOCK_SECTION( _MTLOCK_VIDEO);
	EmptyRegion = XCreateRegion();
	XUnionRectWithRegion(&xRect, EmptyRegion, (Region)dwParm1);
	XDestroyRegion(EmptyRegion);
 	UNLOCK_SECTION( _MTLOCK_VIDEO);

	return (DWORD)SIMPLEREGION;
}

/* dwParm1 - Region to test */
/* lpStruct - pointer to POINT */
DWORD
DrvRegionsPtInRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	LPPOINT lppt;
	DWORD   rc;

	if (!(lppt = (LPPOINT)lpStruct))
		return (DWORD)FALSE;
	LOCK_SECTION( _MTLOCK_VIDEO);
	rc = (DWORD)XPointInRegion((Region)dwParm1, lppt->x, lppt->y);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}


/* dwParm1 - Region to test */
/* lpStruct - Pointer to RECT */
DWORD
DrvRegionsRectInRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	LPRECT lpRect;
	DWORD  rc;

	if (!(lpRect = (LPRECT)lpStruct))
		return (DWORD)FALSE;
	LOCK_SECTION( _MTLOCK_VIDEO);
	rc = (DWORD)XRectInRegion((Region)dwParm1,
			lpRect->left,
			lpRect->top,
			lpRect->right-lpRect->left,
			lpRect->bottom-lpRect->top);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}

/* dwParm1 - dwParm1 - (REGION32) Region */
DWORD
DrvRegionsIsEmptyRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	DWORD rc;

	LOCK_SECTION( _MTLOCK_VIDEO);
	rc = (DWORD)XEmptyRegion((Region)dwParm1);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}

/* dwParm1 - First Region to compare */
/* dwParm2 - Second Region to compare */
DWORD
DrvRegionsEqualRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	DWORD rc;

	LOCK_SECTION( _MTLOCK_VIDEO);
	rc = (DWORD)XEqualRegion((Region)dwParm1, (Region)dwParm2);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}

/* dwParm1 - Region to offset */
/* lpStruct - pointer to POINT */
DWORD
DrvRegionsOffSetRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	LPPOINT lppt;
	
	if (!(lppt = (LPPOINT)lpStruct))
		return (DWORD)ERROR;
	LOCK_SECTION( _MTLOCK_VIDEO);
	XOffsetRegion((Region)dwParm1, lppt->x, lppt->y);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return (DWORD)COMPLEXREGION;
}

/* dwParm1 - Source Region 1 */
/* dwParm2 - Source Region 2 */
/* lpStruct - Destination Region */
DWORD
DrvRegionsUnionRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	DWORD rc;
	
	LOCK_SECTION( _MTLOCK_VIDEO);
	XUnionRegion((Region)dwParm1, (Region)dwParm2, (Region)lpStruct);
	rc = (DWORD)(XEmptyRegion((Region)lpStruct)?
			NULLREGION:COMPLEXREGION);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}

/* dwParm1 - Source Region */
/* dwParm2 - Destination Region */
DWORD
DrvRegionsCopyRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	Region EmptyRegion;
	DWORD  result;

 	LOCK_SECTION( _MTLOCK_VIDEO);
	EmptyRegion = XCreateRegion();
	XUnionRegion((Region)dwParm1, EmptyRegion,(Region)dwParm2);
	result = (DWORD)(XEmptyRegion((Region)dwParm2)?
			NULLREGION:COMPLEXREGION);
	XDestroyRegion(EmptyRegion);
 	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return result;
}

/* dwParm1 - Source Region 1 */
/* dwParm2 - Source Region 2 */
/* lpStruct - Destination Region */
DWORD
DrvRegionsIntersectRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	DWORD rc;
	LOCK_SECTION( _MTLOCK_VIDEO);
	XIntersectRegion((Region)dwParm1, (Region)dwParm2, (Region)lpStruct);
	rc = (DWORD)(XEmptyRegion((Region)lpStruct)?
				NULLREGION:COMPLEXREGION);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}

/* dwParm1 - Source Region 1 */
/* dwParm2 - Source Region 2 */
/* lpStruct - Destination Region */
DWORD
DrvRegionsDiffRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	DWORD rc;
	LOCK_SECTION( _MTLOCK_VIDEO);
	XSubtractRegion((Region)dwParm1, (Region)dwParm2, (Region)lpStruct);
	rc = (DWORD)(XEmptyRegion((Region)lpStruct)?
				NULLREGION:COMPLEXREGION);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}

/* dwParm1 - (REGION32) Source Region 1 */
/* dwParm2 - (REGION32) Source Region 2 */
/* lpStruct - (REGION32) Destination Region */
DWORD
DrvRegionsXORRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	DWORD rc;
	LOCK_SECTION( _MTLOCK_VIDEO);
	XXorRegion((Region)dwParm1, (Region)dwParm2, (Region)lpStruct);
	rc = (DWORD)(XEmptyRegion((Region)lpStruct)?
				NULLREGION:COMPLEXREGION);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}

/* dwParm1 - Region to union */
/* lpStruct - Pointer to RECT */
DWORD
DrvRegionsUnionRectWithRegion(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	LPRECT lpRect;
	DWORD  rc;
	XRectangle xRect;

	if (!(lpRect = (LPRECT)lpStruct))
		return (DWORD)ERROR;
	xRect.x = lpRect->left;
	xRect.y = lpRect->top;
	xRect.width = lpRect->right - lpRect->left;
	xRect.height = lpRect->bottom - lpRect->top;

	LOCK_SECTION( _MTLOCK_VIDEO);
	XUnionRectWithRegion(&xRect, (Region)dwParm1, (Region)dwParm1);
	rc = (DWORD)(XEmptyRegion((Region)dwParm1)?
			NULLREGION:COMPLEXREGION);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;

}

/* dwParm1 - Region to get bounds rectangle for */
/* lpStruct - Pointer to RECT */
DWORD
DrvRegionsRegionBox(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	XRectangle xRect;
	LPRECT lpRect;
	DWORD rc;
	
	if (!(lpRect = (LPRECT)lpStruct))
		return (DWORD)ERROR;

	LOCK_SECTION( _MTLOCK_VIDEO);
	XClipBox((Region)dwParm1, &xRect);

	lpRect->left = xRect.x;
	lpRect->top = xRect.y;
	lpRect->right = xRect.x + xRect.width;
	lpRect->bottom = xRect.y + xRect.height;


	rc = (DWORD)(IsRectEmpty(lpRect)?
			NULLREGION:COMPLEXREGION);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return rc;
}

/* dwParm1 - (REGION32) Region */
/* dwParm2 - (DWORD) Size of region data buffer (in bytes) */
/* lpStruct - (LPRGNDATA) Region data bugger */
DWORD
DrvRegionsGetRegionData(LPARAM dwParm1, LPARAM dwParm2, LPVOID lpStruct)
{
	return ((DWORD)0);
}
	
