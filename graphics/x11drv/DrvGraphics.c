/*
 	@(#)DrvGraphics.c	2.63
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

#include <limits.h>
#include <math.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "windows.h"

#include "Log.h"
#include "kerndef.h"
#include "GdiDC.h"
#include "DrvCommon.h"
#include "DrvDP.h"
#include "DrvImages.h"
#include "DrvDC.h"
#include "DrvROPCodes.h"
#include "DeviceData.h"
#include "Driver.h"
#include "mtlock.h"

#ifndef M_PI           /* Some math.h's do not define M_PI (Centerline, Mac) */
#if defined(NETWARE)
#define M_PI        3.14159265358979323846      /* pi */
#else
#define M_PI _PI       /* In that case, they usually do define _PI, however. */
#endif
#endif

DWORD		DrvGraphicsInit(LPARAM,LPARAM,LPVOID);
DWORD		DrvGraphicsTab(void);

static DWORD	DrvGraphicsGetDeviceCaps(LPARAM,LPARAM,LPVOID);
static DWORD	DrvGraphicsGetAspectRatio(LPARAM,LPARAM,LPVOID);
static DWORD	DrvGraphicsSetBrushOrg(LPARAM,LPARAM,LPVOID);
static DWORD	DrvGraphicsSelectImage(LPARAM,LPARAM,LPVOID);
static COLORREF DrvGetPixel(LPDRIVERDC,LPPOINT);
static DWORD	DrvGraphicsGetPixel(LPARAM,LPARAM,LPVOID);
static COLORREF DrvSetPixel(LPDRIVERDC,COLORREF,LPPOINT);
static DWORD	DrvGraphicsSetPixel(LPARAM,LPARAM,LPVOID);
static DWORD	DrvGraphicsMoveTo(LPARAM,LPARAM,LPVOID);
static DWORD	DrvLineTo(LPDRIVERDC,LPPOINT,DWORD);
static DWORD 	DrvGraphicsLineTo(LPARAM,LPARAM,LPVOID);
static DWORD	DrvPolyline(LPDRIVERDC,LSDE_POLYPOLY *,DWORD);
static DWORD  	DrvGraphicsPolyLine(LPARAM,LPARAM,LPVOID);
static DWORD	DrvRectangle(LPDRIVERDC,LPRECT);
static DWORD	DrvGraphicsRectangle(LPARAM,LPARAM,LPVOID);
static DWORD	DrvEllipse(LPDRIVERDC,LPRECT);
static DWORD	DrvGraphicsEllipse(LPARAM,LPARAM,LPVOID);
static DWORD	DrvRoundRect(LPDRIVERDC,LPPOINT);
static DWORD	DrvGraphicsRoundRect(LPARAM,LPARAM,LPVOID);
static DWORD	DrvArc(LPDRIVERDC,LPPOINT,DWORD);
static DWORD	DrvGraphicsArc(LPARAM,LPARAM,LPVOID);
static DWORD	DrvPolygon(LPDRIVERDC,int,LPPOINT,DWORD);
static DWORD	DrvGraphicsPolygon(LPARAM,LPARAM,LPVOID);
static DWORD	DrvGraphicsPolyPolygon(LPARAM,LPARAM,LPVOID);
extern DWORD	DrvExtFloodFill(LPDRIVERDC,LPLSDS_PARAMS,LPRECT);
static DWORD	DrvGraphicsExtFloodFill(LPARAM,LPARAM,LPVOID);
static DWORD	DrvGraphicsScrollDC(LPARAM,LPARAM,LPVOID);
static DWORD	DrvGraphicsPutImage(LPARAM,LPARAM,LPVOID);
static DWORD 	DrvGraphicsStretchBlt(LPARAM,LPARAM,LPVOID);
static DWORD	DrvGraphicsDoNothing(LPARAM,LPARAM,LPVOID);

DWORD	DrvValidate(LPDRIVERDC,LPLSDS_PARAMS);

extern int ROPS[];
extern ROPtable ROPTab[];
extern void flip(LPINT,int);
extern int stretch(int,int,LPINT);
extern BOOL ConvertColorDDBToMono(DRVIMAGEDATA *,LPRECT,DWORD);
extern DWORD DrvMakePixel(COLORREF, LPDRIVERDC);
extern XPoint *DrvConvertPoints(LPPOINT,int); /* exported to DrvRegions.c */

static TWINDRVSUBPROC DrvGraphicsEntryTab[] = {
	DrvGraphicsInit, 
	DrvGraphicsDoNothing, /* GETCAPS */
	DrvGraphicsDoNothing, /* EVENTS */
	DrvGraphicsGetDeviceCaps,
	DrvGraphicsDoNothing, /* PGH_VALIDATE */
	DrvGraphicsGetAspectRatio,
	DrvGraphicsSetBrushOrg,
	DrvGraphicsSelectImage,
	DrvGraphicsGetPixel,
	DrvGraphicsSetPixel,
	DrvGraphicsMoveTo,
	DrvGraphicsLineTo,
	DrvGraphicsPolyLine,
	DrvGraphicsRectangle,
	DrvGraphicsRoundRect,
	DrvGraphicsEllipse,
	DrvGraphicsArc,
	DrvGraphicsPolygon,
	DrvGraphicsPolyPolygon,
	DrvGraphicsExtFloodFill,
	DrvGraphicsScrollDC,
	DrvGraphicsPutImage,
	DrvGraphicsDoNothing, /* PGH_PATBLT */
	DrvGraphicsDoNothing, /* PGH_BITBLT */
	DrvGraphicsStretchBlt,
	DrvGraphicsDoNothing, /* PGH_MASKBLT */
	DrvGraphicsDoNothing /* PGH_PLGBLT */
};
	
DWORD
DrvGraphicsMoveTo(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
	LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
	LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;
	POINT oldPt = lpDriverDC->cpt;
        
	if (dwParam2 == LSDM_SET)
        	lpDriverDC->cpt = lpStruct->lsde.point;
        lpStruct->lsde.point = oldPt;
        return PGH_SUCCESS; 
}

DWORD
DrvGraphicsLineTo(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
	LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
	LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;
	
	if (lpStruct->lsde_validate.dwInvalidMask)
		DrvValidate(lpDriverDC, lpStruct);
	return DrvLineTo(lpDriverDC, &lpStruct->lsde.point, dwParam2);
}

static DWORD
DrvLineTo(LPDRIVERDC lpddc, LPPOINT lppt, DWORD dwRelAbs)
{
    POINT oldPt = lpddc->cpt;

    if (dwRelAbs == RELATIVE) {
	lpddc->cpt.x = oldPt.x + lppt->x;
	lpddc->cpt.y = oldPt.y + lppt->y;
    }
    else {	/* ABSOLUTE mode */
	lpddc->cpt.x = lppt->x;
	lpddc->cpt.y = lppt->y;
    }

    if (lpddc->PenStyle != PS_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != FillSolid) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,FillSolid);
	    lpddc->FillStyle = FillSolid;
	}
	if (lpddc->ForeColorPixel != lpddc->PenColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,lpddc->PenColorPixel);
	    lpddc->ForeColorPixel = lpddc->PenColorPixel;
	}
	XDrawLine(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
			oldPt.x,oldPt.y,lpddc->cpt.x,lpddc->cpt.y);
     	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    return PGH_SUCCESS;
}

DWORD
DrvGraphicsPolyLine(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;

	if (lpStruct->lsde_validate.dwInvalidMask)
		DrvValidate(lpDriverDC, lpStruct);
        return DrvPolyline(lpDriverDC, &lpStruct->lsde.polypoly, dwParam2);
}

static DWORD
DrvPolyline(LPDRIVERDC lpddc, LSDE_POLYPOLY *lppp, DWORD dwRelAbs)
{
    if (lpddc->PenStyle != PS_NULL) {
        XPoint *cvtpoints;

	cvtpoints = DrvConvertPoints(lppp->lpPoints,lppp->nTotalCount);

    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != FillSolid) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,FillSolid);
	    lpddc->FillStyle = FillSolid;
	}
	if (lpddc->ForeColorPixel != lpddc->PenColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
				lpddc->PenColorPixel);
	    lpddc->ForeColorPixel = lpddc->PenColorPixel;
	}
	XDrawLines(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
		cvtpoints, lppp->nTotalCount,
		(dwRelAbs == ABSOLUTE) ? CoordModeOrigin : CoordModePrevious);
	WinFree(cvtpoints);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    return PGH_SUCCESS;
}

DWORD           
DrvGraphicsRectangle(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{                       
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;

        if (lpStruct->lsde_validate.dwInvalidMask)
                DrvValidate(lpDriverDC, lpStruct);
        return DrvRectangle(lpDriverDC, &lpStruct->lsde.rect);
}

static DWORD
DrvRectangle(LPDRIVERDC lpddc, LPRECT lpRect)
{
  int iTOffset=0, iWOffset=0;
  if((lpRect->top-lpRect->bottom)!=0)
  {
     iTOffset=1;
  }
  if((lpRect->left-lpRect->right)!=0)
  {
     iWOffset=1;
  } 
    if (lpddc->BrushFlag != BFP_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != lpddc->BrushFillStyle) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,
			lpddc->BrushFillStyle);
	    lpddc->FillStyle = lpddc->BrushFillStyle;
	}
	if (lpddc->ForeColorPixel != lpddc->BrushColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
			lpddc->BrushColorPixel);
	    lpddc->ForeColorPixel = lpddc->BrushColorPixel;
	}
	XFillRectangle(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
		lpRect->left,lpRect->top,
		lpRect->right - lpRect->left - iWOffset,
		lpRect->bottom - lpRect->top - iTOffset);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    if (lpddc->PenStyle != PS_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != FillSolid) {	
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,FillSolid);
	    lpddc->FillStyle = FillSolid;
	}
	if (lpddc->ForeColorPixel != lpddc->PenColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
			lpddc->PenColorPixel);
	    lpddc->ForeColorPixel = lpddc->PenColorPixel;
	}
	XDrawRectangle(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
		lpRect->left,lpRect->top,
                lpRect->right - lpRect->left - iWOffset,
                lpRect->bottom - lpRect->top - iTOffset);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    return PGH_SUCCESS;
}

DWORD
DrvGraphicsEllipse(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;

        if (lpStruct->lsde_validate.dwInvalidMask)
                DrvValidate(lpDriverDC, lpStruct);
        return DrvEllipse(lpDriverDC, &lpStruct->lsde.rect);
}

static DWORD
DrvEllipse(LPDRIVERDC lpddc, LPRECT lpRect)
{
    if (lpddc->BrushFlag != BFP_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != lpddc->BrushFillStyle) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,
			lpddc->BrushFillStyle);
	    lpddc->FillStyle = lpddc->BrushFillStyle;
	}
	if (lpddc->ForeColorPixel != lpddc->BrushColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
			lpddc->BrushColorPixel);
	    lpddc->ForeColorPixel = lpddc->BrushColorPixel;
	}
	XFillArc(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
		lpRect->left,lpRect->top,
		lpRect->right - lpRect->left - 1,
		lpRect->bottom - lpRect->top - 1,
		0, 360*64);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    if (lpddc->PenStyle != PS_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != FillSolid) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,FillSolid);
	    lpddc->FillStyle = FillSolid;
	}
	if (lpddc->ForeColorPixel != lpddc->PenColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
			lpddc->PenColorPixel);
	    lpddc->ForeColorPixel = lpddc->PenColorPixel;
	}
	XDrawArc(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
		lpRect->left,lpRect->top,
		lpRect->right - lpRect->left - 1,
		lpRect->bottom - lpRect->top - 1,
		0, 360*64);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    return PGH_SUCCESS;
}

DWORD
DrvGraphicsRoundRect(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;

        if (lpStruct->lsde_validate.dwInvalidMask)
                DrvValidate(lpDriverDC, lpStruct);
        return DrvRoundRect(lpDriverDC, &lpStruct->lsde.arc[0]);
}

static DWORD
DrvRoundRect(LPDRIVERDC lpddc, LPPOINT lparc)
{
    XArc arcs[8];
    XRectangle rects[4];

    if (lparc[2].x > lparc[1].x-lparc[0].x)
	lparc[2].x = (lparc[1].x-lparc[0].x);
    if (lparc[2].y > lparc[1].y-lparc[0].y)
	lparc[2].y = (lparc[1].y-lparc[0].y);

    if (lpddc->BrushFlag != BFP_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != lpddc->BrushFillStyle) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,
			lpddc->BrushFillStyle);
	    lpddc->FillStyle = lpddc->BrushFillStyle;
	}
	if (lpddc->ForeColorPixel != lpddc->BrushColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
			lpddc->BrushColorPixel);
	    lpddc->ForeColorPixel = lpddc->BrushColorPixel;
	}
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
	arcs[0].x 	= lparc[0].x;
	arcs[0].y 	= lparc[0].y;
	arcs[0].width	= lparc[2].x;
	arcs[0].height 	= lparc[2].y;
	arcs[0].angle1  = 180*64;
	arcs[0].angle2  = -90*64;

	arcs[1].x 	= lparc[1].x - lparc[2].x;
	arcs[1].y 	= lparc[0].y;
	arcs[1].width	= lparc[2].x; 
	arcs[1].height 	= lparc[2].y;
	arcs[1].angle1  =  90*64;
	arcs[1].angle2  = -90*64;

	arcs[2].x 	= lparc[1].x - lparc[2].x;
	arcs[2].y 	= lparc[1].y - lparc[2].y;
	arcs[2].width	= lparc[2].x;
	arcs[2].height 	= lparc[2].y;
	arcs[2].angle1  =      0;
	arcs[2].angle2  = -90*64;

	arcs[3].x 	= lparc[0].x;
	arcs[3].y 	= lparc[1].y - lparc[2].y;
	arcs[3].width	= lparc[2].x;
	arcs[3].height 	= lparc[2].y;
	arcs[3].angle1  = 270*64;
	arcs[3].angle2  = -90*64;

    	LOCK_SECTION( _MTLOCK_VIDEO);
	XFillArcs(lpddc->dp->display,lpddc->Drawable,
		lpddc->gc,arcs,4);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	/* fill it in */
	rects[0].x 	= lparc[0].x + lparc[2].x/2;
	rects[0].y 	= lparc[0].y;
	rects[0].width  = lparc[1].x - lparc[0].x - lparc[2].x;
	if (lparc[2].x % 2)
	    rects[0].width++;
	rects[0].height = lparc[1].y - lparc[0].y;

	rects[1].x 	= lparc[0].x;
	rects[1].y 	= lparc[0].y + lparc[2].y/2;
	rects[1].width  = lparc[1].x - lparc[0].x;
	rects[1].height = lparc[1].y - lparc[0].y - lparc[2].y;
	if (lparc[2].y % 2)
	    rects[1].height++;

    	LOCK_SECTION( _MTLOCK_VIDEO);
	XFillRectangles(lpddc->dp->display,lpddc->Drawable,
			lpddc->gc,rects,2);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    if (lpddc->PenStyle != PS_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != FillSolid) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,FillSolid);
	    lpddc->FillStyle = FillSolid;
	}
	if (lpddc->ForeColorPixel != lpddc->PenColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
			lpddc->PenColorPixel);
	    lpddc->ForeColorPixel = lpddc->PenColorPixel;
	}
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
	arcs[0].x 	= lparc[0].x;
	arcs[0].y 	= lparc[0].y;
	arcs[0].width	= lparc[2].x;
	arcs[0].height 	= lparc[2].y;
	arcs[0].angle1  = 180*64;
	arcs[0].angle2  = -90*64;

	arcs[1].x 	= lparc[0].x + lparc[2].x/2;
	arcs[1].y 	= lparc[0].y;
	arcs[1].width	= lparc[1].x - lparc[0].x - lparc[2].x; 
	if (lparc[2].x % 2)
	    arcs[1].width++;
	arcs[1].height 	= 0;
	arcs[1].angle1  = 180*64;
	arcs[1].angle2  = -180*64;

	arcs[2].x 	= lparc[1].x - lparc[2].x;
	arcs[2].y 	= lparc[0].y;
	arcs[2].width	= lparc[2].x; 
	arcs[2].height 	= lparc[2].y;
	arcs[2].angle1  = 90*64;
	arcs[2].angle2  = -90*64;

	arcs[3].x 	= lparc[1].x;
	arcs[3].y 	= lparc[0].y + lparc[2].y/2;
	arcs[3].width	= 0; 
	arcs[3].height 	= lparc[1].y - lparc[0].y - lparc[2].y;
	if (lparc[2].y % 2)
	    arcs[3].height++;
	arcs[3].angle1  = 90*64;
	arcs[3].angle2  = -180*64;

	arcs[4].x 	= lparc[1].x - lparc[2].x;
	arcs[4].y 	= lparc[1].y - lparc[2].y;
	arcs[4].width	= lparc[2].x;
	arcs[4].height 	= lparc[2].y;
	arcs[4].angle1  = 0;
	arcs[4].angle2  = -90*64;

	arcs[5].x 	= lparc[0].x + lparc[2].x/2;
	arcs[5].y 	= lparc[1].y;
	arcs[5].width	= lparc[1].x - lparc[0].x - lparc[2].x; 
	if (lparc[2].x % 2)
	    arcs[5].width++;
	arcs[5].height 	= 0;
	arcs[5].angle1  = 0;
	arcs[5].angle2  = -180*64;

	arcs[6].x 	= lparc[0].x;
	arcs[6].y 	= lparc[1].y - lparc[2].y;
	arcs[6].width	= lparc[2].x;
	arcs[6].height 	= lparc[2].y;
	arcs[6].angle1  = 270*64;
	arcs[6].angle2  = -90*64;

	arcs[7].x 	= lparc[0].x;
	arcs[7].y 	= lparc[0].y + lparc[2].y/2;
	arcs[7].width	= 0; 
	arcs[7].height 	= lparc[1].y - lparc[0].y - lparc[2].y;
	if (lparc[2].y % 2)
	    arcs[7].height++;
	arcs[7].angle1  = 270*64;
	arcs[7].angle2  = -180*64;

    	LOCK_SECTION( _MTLOCK_VIDEO);
	XDrawArcs(lpddc->dp->display,lpddc->Drawable,
			lpddc->gc,arcs,8);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    return PGH_SUCCESS;
}

static int
Pt2Angle(int cx,int cy,int width,int height,int x3,int y3)
{
	double Y,X;
	double theta,rtoa;
	int adjust = 1;
	
	if (cy == y3)
	{
	    if (cx > x3)
	    	return 180 * 64;
	    else
	    	return 0;
	}

	if (cx == x3)
	{
	    if (cy > y3)
	    	return 90 * 64;
	    else
	    	return -90 * 64;
	}
	
	Y = (double)(cy - y3);
	X = (double)(x3 - cx);

	theta = atan2(Y,X);

/* NOTE: this change compensates for limitations in some math libs */
	if ( theta < 0.0 )
	    theta *= (adjust = -1);

	Y = (double)(height);
	X = (double)(width);

	rtoa  = adjust * atan(tan(theta) * X / Y);

/****************************************************************************

       "skewed-angle" = atan(tan("normal-angle"))*(width/height) + adjust

       The skewed-angle and normal-angle are expressed in radians
       (rather than in degrees scaled by 64) in the range [ 0 , 2 pi  ] 
	and where atan returns a value in the range [ - pi / 2 , pi / 2] 
	and adjust is:

       0       for normal-angle in the range [ 0 , pi / 2  ]
       pi      for normal-angle in the range [ pi / 2 , 3 pi / 2  ]
       2 pi    for normal-angle in the range [ 3 pi / 2 , 2 pi  ]

	Note:  we don't use x3,y3 directly, we need to use deltax,deltay,
	to see the coordinate relative to the center of the rectangle.

*****************************************************************************/

	if(x3<cx)
		adjust = 1;
	else	adjust = 0;

	if(y3>cy)
		adjust += 2;

	switch(adjust) {
		case 0:
			break;
		case 1:
		case 3:
			rtoa += 3.1415927;
			break;
		case 2:
			rtoa += 2 * 3.1415927;
	}

	rtoa *= 180.0 * 64;
	rtoa /= 3.1415927;

	return (int)(rtoa);
}	

DWORD
DrvGraphicsArc(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;

        if (lpStruct->lsde_validate.dwInvalidMask)
                DrvValidate(lpDriverDC, lpStruct);
        return DrvArc(lpDriverDC, &lpStruct->lsde.arc[0], dwParam2);
}

static DWORD
DrvArc(LPDRIVERDC lpddc, LPPOINT lparc, DWORD dwFunc)
{
    int angle1,angle2;
    int cx,cy;
    int width,height;
    int a1,a2;
    int x1,y1,x2,y2;
	
    width   = (lparc[1].x - lparc[0].x)/2;
    height  = (lparc[1].y - lparc[0].y)/2;

    cx = lparc[0].x + width;
    cy = lparc[0].y + height;
	
    angle1 = Pt2Angle(cx,cy, width,height, lparc[2].x,lparc[2].y);	
    a1 = angle1 > 0 ? (angle1 + 32) / 64 : (angle1 - 32) / 64;
    angle2 = Pt2Angle(cx,cy, width,height, lparc[3].x,lparc[3].y);	
    a2 = angle2 > 0 ? (angle2 + 32) / 64 : (angle2 - 32) / 64;
	
    angle2 -= angle1;
    if(angle2 < 0) angle2 += 360*64;

    if (dwFunc != LSD_ARC) {	/* LSD_CHORD or LSD_PIE */
	if (lpddc->BrushFlag != BFP_NULL) {
	    if (lpddc->FillStyle != lpddc->BrushFillStyle) {
    	        LOCK_SECTION( _MTLOCK_VIDEO);
		XSetFillStyle(lpddc->dp->display,lpddc->gc,
			lpddc->BrushFillStyle);
    	        UNLOCK_SECTION( _MTLOCK_VIDEO);
		lpddc->FillStyle = lpddc->BrushFillStyle;
	    }
	    if (lpddc->ForeColorPixel != lpddc->BrushColorPixel) {
    	        LOCK_SECTION( _MTLOCK_VIDEO);
		XSetForeground(lpddc->dp->display,lpddc->gc,
				lpddc->BrushColorPixel);
    	        UNLOCK_SECTION( _MTLOCK_VIDEO);
		lpddc->ForeColorPixel = lpddc->BrushColorPixel;
	    }
	    if (dwFunc == LSD_CHORD) {
    	        LOCK_SECTION( _MTLOCK_VIDEO);
		XSetArcMode(lpddc->dp->display,lpddc->gc,ArcChord);
    	        UNLOCK_SECTION( _MTLOCK_VIDEO);
	    }

    	    LOCK_SECTION( _MTLOCK_VIDEO);
	    XFillArc(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
			lparc[0].x,lparc[0].y,
			lparc[1].x-lparc[0].x-1,lparc[1].y-lparc[0].y-1,
			angle1,
                        angle2);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);

	    if (dwFunc == LSD_CHORD) {
    	        LOCK_SECTION( _MTLOCK_VIDEO);
		XSetArcMode(lpddc->dp->display,lpddc->gc,ArcPieSlice);
    	        UNLOCK_SECTION( _MTLOCK_VIDEO);
	    }
	}
    }

    if (lpddc->PenStyle != PS_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != FillSolid) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,FillSolid);
	    lpddc->FillStyle = FillSolid;
	}
	if (lpddc->ForeColorPixel != lpddc->PenColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
			lpddc->PenColorPixel);
	    lpddc->ForeColorPixel = lpddc->PenColorPixel;
	}

	XDrawArc(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
			lparc[0].x,lparc[0].y,
			lparc[1].x-lparc[0].x-1,lparc[1].y-lparc[0].y-1,
			angle1,
                        angle2);


    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	if (dwFunc != LSD_ARC) {	/* LSD_CHORD or LSD_PIE */
	    x1 = (double)width * cos((double)a1 * M_PI/180.);
	    x2 = (double)width * cos((double)a2 * M_PI/180.);
	    y1 = -(double)height * sin((double)a1 * M_PI/180.);
	    y2 = -(double)height * sin((double)a2 * M_PI/180.);

	    x1 += cx;
	    y1 += cy;
	    x2 += cx;
	    y2 += cy;

    	    LOCK_SECTION( _MTLOCK_VIDEO);
	    if (dwFunc == LSD_CHORD)
		XDrawLine(lpddc->dp->display,lpddc->Drawable,lpddc->gc,	
				x1,y1,x2,y2);
	    else {
		XDrawLine(lpddc->dp->display,lpddc->Drawable,lpddc->gc,	
				x1,y1,cx,cy);
		XDrawLine(lpddc->dp->display,lpddc->Drawable,lpddc->gc,	
				cx,cy,x2,y2);
	    }
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	}
    }
    return PGH_SUCCESS;
}

DWORD
DrvGraphicsPolyPolygon(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
        int i, nCount;
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;
            
	if (lpStruct->lsde_validate.dwInvalidMask)
       		DrvValidate(lpDriverDC, lpStruct);
        for (i=0, nCount=0; i < lpStruct->lsde.polypoly.nCount; i++)
        {
        	DrvPolygon(lpDriverDC, lpStruct->lsde.polypoly.lpCounts[i],
			&lpStruct->lsde.polypoly.lpPoints[nCount], dwParam2);
                nCount += lpStruct->lsde.polypoly.lpCounts[i];
        } 
        return PGH_SUCCESS;
}

static DWORD
DrvPolygon(LPDRIVERDC lpddc, int nCount, LPPOINT lpPoints, DWORD dwRelAbs)
{
    int i,x,y;
    XPoint *lpXPoints;

    lpXPoints = DrvConvertPoints(lpPoints,nCount);

    if (lpddc->BrushFlag != BFP_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != lpddc->BrushFillStyle) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,
			lpddc->BrushFillStyle);
	    lpddc->FillStyle = lpddc->BrushFillStyle;
	}
	if (lpddc->ForeColorPixel != lpddc->BrushColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
			lpddc->BrushColorPixel);
	    lpddc->ForeColorPixel = lpddc->BrushColorPixel;
	}
	XFillPolygon(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
		lpXPoints,
		nCount,
		Complex,
		(dwRelAbs == ABSOLUTE)?CoordModeOrigin:CoordModePrevious);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    if (lpddc->PenStyle != PS_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->FillStyle != FillSolid) {
	    XSetFillStyle(lpddc->dp->display,lpddc->gc,FillSolid);
	    lpddc->FillStyle = FillSolid;
	}
	if (lpddc->ForeColorPixel != lpddc->PenColorPixel) {
	    XSetForeground(lpddc->dp->display,lpddc->gc,
			lpddc->PenColorPixel);
	    lpddc->ForeColorPixel = lpddc->PenColorPixel;
	}
	XDrawLines(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
	    	lpXPoints,nCount,
	    	(dwRelAbs == ABSOLUTE)?CoordModeOrigin:CoordModePrevious);
	if (dwRelAbs == ABSOLUTE)
	    XDrawLine(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
			lpXPoints[nCount-1].x,lpXPoints[nCount-1].y,
			lpXPoints[0].x,lpXPoints[0].y);
	else {
	    for (i = 0, x = 0, y = 0; i < nCount; i++) {
		x += lpXPoints[i].x;
		y += lpXPoints[i].y;
	    }
	    XDrawLine(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
		x,y,lpXPoints[0].x,lpXPoints[0].y);
	}
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    LOCK_SECTION( _MTLOCK_VIDEO);
    WinFree(lpXPoints);
    UNLOCK_SECTION( _MTLOCK_VIDEO);
    return PGH_SUCCESS;
}

DWORD
DrvGraphicsExtFloodFill(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;

        if (lpStruct->lsde_validate.dwInvalidMask)
                DrvValidate(lpDriverDC, lpStruct);
	return DrvExtFloodFill(lpDriverDC, lpStruct, (LPRECT)dwParam2);
}

static DWORD 
DrvGraphicsStretchBlt(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
    LPDRIVERDC lpddc = (LPDRIVERDC)dwParam1; 
    LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;
    LSDE_STRETCHDATA *lpsd = &lpStruct->lsde.stretchdata;
    int nWidthSrc,nHeightSrc,nWidthDest,nHeightDest;
    DWORD rop;
    GC gc = 0;
    XImage *imSrc,*imDest;
    BOOL fMirrorX = FALSE,fMirrorY = FALSE;
    LPWORD lpRops;
    Pixmap srcPix = 0;
    int	n;
    int	x,y,fg,bg;
    Pixmap tmpPix = 0;
    int nROP2;
    LPDRIVERDC lpSrcDC,lpDestDC = lpddc;
    DWORD dwTemp;
    LPBYTE lpDest;
    LPBYTE lpSrc;
    int last_x;
    int step;
    int	bytes_per_pixel = 0;
	

    LPINT xbits = 0, ybits = 0;
    int nxbits = 0,nybits = 0;

    if (!lpsd || !lpsd->nWidthDest || !lpsd->nHeightDest || 
			!lpsd->nWidthSrc || !lpsd->nHeightSrc)
	return (DWORD)FALSE;

    if (lpsd->hSrcDC32) {
	lpSrcDC = (LPDRIVERDC)lpsd->hSrcDC32;
	if (lpStruct->lsde_validate.dwInvalidMask & IM_SRCBKCOLORMASK) {
	    dwTemp = lpStruct->lsde_validate.dwInvalidMask;
	    lpStruct->lsde_validate.dwInvalidMask = IM_SRCBKCOLORMASK;
	    DrvValidate(lpSrcDC,lpStruct);
	    lpStruct->lsde_validate.dwInvalidMask =
				dwTemp & ~IM_SRCBKCOLORMASK;
	}
    }
    else
	lpSrcDC = 0;

    if (lpStruct->lsde_validate.dwInvalidMask)
	DrvValidate(lpddc,lpStruct);

    /* If nothing is selected into a compatible DC --  fail   */
    if (lpSrcDC && (lpSrcDC->lpDrvImage == 0) &&
		(lpSrcDC->dwDCFlags & DCX_COMPATIBLE_DC)) {
	    return (DWORD)FALSE;
    }
    if ((lpDestDC->lpDrvImage == 0) &&
		(lpDestDC->dwDCFlags & DCX_COMPATIBLE_DC)) {
	return (DWORD)FALSE;
    }

    nWidthSrc = lpsd->nWidthSrc;
    nWidthDest = lpsd->nWidthDest;
    nHeightSrc = lpsd->nHeightSrc;
    nHeightDest = lpsd->nHeightDest;

    /* If parameters have different signs, we have to mirror the image */

    if ((nWidthSrc < 0) || (nWidthDest < 0)) {
	if ((nWidthSrc < 0) ^ (nWidthDest < 0)) /* mirror along X-axis */
	    fMirrorX = TRUE;
	if (nWidthSrc < 0) {
	    nWidthSrc = -nWidthSrc;
	    lpsd->xSrc -= nWidthSrc - 1;
	}
	if (nWidthDest < 0) {
	    nWidthDest = -nWidthDest;
	    lpsd->xDest -= nWidthDest - 1;
	}
    }
    if ((nHeightSrc < 0) || (nHeightDest < 0)) {
	if ((nHeightSrc < 0) ^ (nHeightDest < 0)) /* mirror along Y-axis */
	    fMirrorY = TRUE;
	if (nHeightSrc < 0) {
	    nHeightSrc = -nHeightSrc;
	    lpsd->ySrc -= nHeightSrc - 1;
	}
	if (nHeightDest < 0) {
	    nHeightDest = -nHeightDest;
	    lpsd->yDest -= nHeightDest - 1;
	}
    }

    rop = lpsd->dwRop;
    if ((ROPTab[HIWORD(rop)].bRops[0] == 0) || 
		(ROPTab[HIWORD(rop)].dwROP != rop)) {
    }

    if (lpSrcDC && lpSrcDC->lpDrvImage && lpSrcDC->lpDrvImage->fMono ) {
	if (lpSrcDC->lpDrvImage->pixmap) {
	    if (lpSrcDC->lpDrvImage->fInvalid ||
			(lpSrcDC->lpDrvImage->image == 0) ||
			(lpSrcDC->lpDrvImage->image->depth != 1)) {
    		LOCK_SECTION( _MTLOCK_VIDEO);
		if (lpSrcDC->lpDrvImage->image) {
		    XDestroyImage((XImage *)lpSrcDC->lpDrvImage->image);
		}
		lpSrcDC->lpDrvImage->image = XGetImage(lpSrcDC->dp->display,
				lpSrcDC->Drawable,
				0,0,
				lpSrcDC->lpDrvImage->nWidth,
				lpSrcDC->lpDrvImage->nHeight,
				(DWORD)1,XYPixmap);
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
		lpSrcDC->lpDrvImage->fInvalid = FALSE;
	    }
	}
	else {
#ifdef LATER
	does this code ever work???
#endif
    	    LOCK_SECTION( _MTLOCK_VIDEO);
	    lpSrcDC->lpDrvImage->pixmap =
			XCreatePixmap(lpSrcDC->dp->display,
				DefaultRootWindow(lpSrcDC->dp->display),
				lpSrcDC->lpDrvImage->nWidth,
				lpSrcDC->lpDrvImage->nHeight,
				lpSrcDC->dp->Depth);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	    lpSrcDC->Drawable = (Drawable)lpSrcDC->lpDrvImage->pixmap;
	}

    	LOCK_SECTION( _MTLOCK_VIDEO);
	gc = XCreateGC(lpSrcDC->dp->display,
			DefaultRootWindow(lpSrcDC->dp->display),
			0,(XGCValues *)0);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	bg = lpDestDC->BackColorPixel;
	fg = lpDestDC->TextColorPixel;

    	LOCK_SECTION( _MTLOCK_VIDEO);
	XSetBackground(lpSrcDC->dp->display,gc,fg); /* Windows and X treat  */
	XSetForeground(lpSrcDC->dp->display,gc,bg); /* fg and bg different  */
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
	lpSrcDC->lpDrvImage->image->format = XYBitmap;

    	LOCK_SECTION( _MTLOCK_VIDEO);
	srcPix = XCreatePixmap(lpSrcDC->dp->display,
			DefaultRootWindow(lpSrcDC->dp->display),
			lpSrcDC->lpDrvImage->nWidth,
			lpSrcDC->lpDrvImage->nHeight,
			lpSrcDC->dp->Depth);
	XPutImage(lpSrcDC->dp->display,srcPix,gc,
			lpSrcDC->lpDrvImage->image,
			0,0,0,0,
			lpSrcDC->lpDrvImage->nWidth,
			lpSrcDC->lpDrvImage->nHeight);	
	XFreeGC(lpSrcDC->dp->display,gc);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	gc = 0;
    }
    if (lpDestDC->Drawable == (Drawable)0) {
	if (lpDestDC->lpDrvImage) {
      	    LOCK_SECTION( _MTLOCK_VIDEO);
	    lpDestDC->lpDrvImage->pixmap =
		    XCreatePixmap(lpDestDC->dp->display,
			    DefaultRootWindow(lpDestDC->dp->display),
			    lpDestDC->lpDrvImage->nWidth,
			    lpDestDC->lpDrvImage->nHeight,
			    lpDestDC->dp->Depth);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	    lpDestDC->Drawable = (Drawable)lpDestDC->lpDrvImage->pixmap;
	}
	else
	    return (DWORD)FALSE;
    }

    if (nWidthSrc != nWidthDest || nHeightSrc != nHeightDest ||
			fMirrorX || fMirrorY) {

	if((nxbits < nWidthDest) || fMirrorX) {
    	        LOCK_SECTION( _MTLOCK_VIDEO);
		xbits = (LPINT)DRVMEM_MALLOC(nWidthDest*sizeof(int));
    	        UNLOCK_SECTION( _MTLOCK_VIDEO);
		nxbits = nWidthDest;
	}
				
	if((nybits < nHeightDest) || fMirrorY) {
    	        LOCK_SECTION( _MTLOCK_VIDEO);
		ybits = (LPINT)DRVMEM_MALLOC(nHeightDest*sizeof(int));
    	        UNLOCK_SECTION( _MTLOCK_VIDEO);
		nybits = nHeightDest;
	}
			
	stretch(nWidthSrc,nWidthDest,xbits);
	stretch(nHeightSrc,nHeightDest,ybits);

	if (fMirrorX)
	    flip(xbits,nWidthDest);

	if (fMirrorY)
	    flip(ybits,nHeightDest);

        LOCK_SECTION( _MTLOCK_VIDEO);
	/* obtain the original image */
	if (!(imSrc = XGetImage(lpSrcDC->dp->display,
				(srcPix) ? srcPix : lpSrcDC->Drawable,
				lpsd->xSrc,lpsd->ySrc,
				nWidthSrc,nHeightSrc,
				(unsigned long)0xffffffff,
				ZPixmap))) {
	    if(xbits) DRVMEM_FREE(xbits);
	    if(ybits) DRVMEM_FREE(ybits);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO); 
	    return (DWORD)FALSE;
	}
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	if (srcPix) {
            LOCK_SECTION( _MTLOCK_VIDEO);
	    XFreePixmap(lpSrcDC->dp->display,srcPix);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	    srcPix = 0;
	}

    	LOCK_SECTION( _MTLOCK_VIDEO);
	/* this will hold the stretched image */
	if (!(imDest = XCreateImage(lpSrcDC->dp->display,
			DefaultVisual(lpSrcDC->dp->display,
					lpSrcDC->dp->screen),
			imSrc->depth,ZPixmap,0,0,
			nWidthDest,nHeightDest,
			imSrc->bitmap_pad,0))) {
	    XDestroyImage(imSrc);
	    if(xbits) DRVMEM_FREE(xbits);
	    if(ybits) DRVMEM_FREE(ybits);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	    return (DWORD)FALSE;
	}
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (!(imDest->data = DRVMEM_MALLOC
			(imDest->bytes_per_line*nHeightDest))) {
	    XDestroyImage(imSrc);
	    XDestroyImage(imDest);
	    if(xbits) DRVMEM_FREE(xbits);
	    if(ybits) DRVMEM_FREE(ybits);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	    return (DWORD)FALSE;
	}

	/* this is where we put the stretched image */
	if (!(srcPix = XCreatePixmap(lpSrcDC->dp->display,
			RootWindow(lpSrcDC->dp->display,lpSrcDC->dp->screen),
			nWidthDest,nHeightDest,
			imDest->depth))) {
	    DRVMEM_FREE(imDest->data);
	    imDest->data = 0;
	    XDestroyImage(imSrc);
	    XDestroyImage(imDest);
	    if(xbits) DRVMEM_FREE(xbits);
	    if(ybits) DRVMEM_FREE(ybits);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	    return (DWORD)FALSE;
	}
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	bytes_per_pixel = imSrc->bits_per_pixel / 8;
	for (y = 0; y < nHeightDest; y ++) {
	    lpSrc = imSrc->data + ybits[y] * imSrc->bytes_per_line;
	    lpDest= imDest->data + y * imDest->bytes_per_line;
	    last_x = 0;
	    for (x = 0; x < nWidthDest; x++) {
		lpSrc += (xbits[x] - last_x) * bytes_per_pixel;
		for (step = 0; step < bytes_per_pixel; step ++)
		     ((char *)lpDest)[step] = ((char *)lpSrc)[step];
		last_x = xbits[x];
		lpDest += bytes_per_pixel;
	    }
	}

	/* the offset in the source has already been accounted for */
	lpsd->xSrc = lpsd->ySrc = 0;
    	LOCK_SECTION( _MTLOCK_VIDEO);
	XPutImage(lpSrcDC->dp->display,
		srcPix,
		DefaultGC(lpSrcDC->dp->display,lpSrcDC->dp->screen),
		imDest,
		0,0,0,0,
		nWidthDest,nHeightDest);

	DRVMEM_FREE(imDest->data);
	imDest->data = 0;
	XDestroyImage(imDest);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	if (lpSrcDC && lpSrcDC->lpDrvImage && lpSrcDC->lpDrvImage->image == 0)
	    lpSrcDC->lpDrvImage->image = imSrc;
	else {
	    LOCK_SECTION( _MTLOCK_VIDEO);
	    XDestroyImage(imSrc);
	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	}
    }
    else {
	if (lpSrcDC && lpSrcDC->lpDrvImage) {
	    if ((nWidthDest+lpsd->xSrc) > lpSrcDC->lpDrvImage->nWidth)
		nWidthDest = lpSrcDC->lpDrvImage->nWidth - lpsd->xSrc;

	    if (nHeightDest+lpsd->ySrc > lpSrcDC->lpDrvImage->nHeight)
		nHeightDest = lpSrcDC->lpDrvImage->nHeight - lpsd->ySrc;
	}
    }

    /* We might use the brush here, so go validate brush attributes */
    if (lpDestDC->BrushFlag != BFP_NULL) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpDestDC->FillStyle != lpDestDC->BrushFillStyle) {
	    XSetFillStyle(lpDestDC->dp->display,lpDestDC->gc,
			lpDestDC->BrushFillStyle);
	    lpDestDC->FillStyle = lpDestDC->BrushFillStyle;
	}
	if (lpDestDC->ForeColorPixel != lpDestDC->BrushColorPixel) {
	    XSetForeground(lpDestDC->dp->display,lpDestDC->gc,
			lpDestDC->BrushColorPixel);
	    lpDestDC->ForeColorPixel = lpDestDC->BrushColorPixel;
	}
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    /* now we can actually perform the logical blit */

    lpRops = &(ROPTab[HIWORD(rop)].bRops[0]);

    LOCK_SECTION( _MTLOCK_VIDEO);
    for(n=0;n<4 && *lpRops;n++,lpRops++) {
	if (*lpRops & 0x8000) {		/* temporary pixmap needed */
	    if (!(tmpPix = XCreatePixmap(lpDestDC->dp->display,
			RootWindow(lpDestDC->dp->display,
				lpDestDC->dp->screen),
			nWidthDest,
			nHeightDest,
			lpDestDC->dp->Depth))) 
		break;	/* out of the loop -- failure */
	    if (lpDestDC->lpDrvImage == 0) { /* screen DC */
		gc = XCreateGC(lpDestDC->dp->display,
			DefaultRootWindow(lpDestDC->dp->display),
			0,(XGCValues *)0);
		XCopyGC(lpDestDC->dp->display,lpDestDC->gc,
			GCPlaneMask|GCForeground|GCBackground|GCFillStyle|
			GCTile|GCStipple|GCTileStipXOrigin|GCTileStipYOrigin,
			gc);
	    }
	    else
		gc = lpDestDC->gc;
	    XSetFunction(lpDestDC->dp->display,gc,GXcopy);
	    switch (*lpRops & 0xff00) {
		case ROP_DB:
		case ROP_DS:
		    XCopyArea(lpDestDC->dp->display,
				lpDestDC->Drawable,
				(Drawable)tmpPix,
				gc,
				lpsd->xDest,lpsd->yDest,
				nWidthDest,nHeightDest,
				0,0);
		    break;
		case ROP_SB:
		    XFillRectangle(lpDestDC->dp->display,
				(Drawable)tmpPix,
				gc,
				0,0,
				nWidthDest,nHeightDest);
		    break;
		default:	/* we should never get here */
		    break;
	    }
	}
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	nROP2 = ROPS[LOBYTE(*lpRops)];

    	LOCK_SECTION( _MTLOCK_VIDEO);
	XSetFunction(lpDestDC->dp->display,lpDestDC->gc,nROP2);
	switch (*lpRops & 0xff00) {
	    case ROP_BD:
		if (lpDestDC->BrushFlag != BFP_NULL) {
		    if (nROP2 != GXcopy)
			XSetPlaneMask(lpDestDC->dp->display,
					lpDestDC->gc,
					lpDestDC->dp->pixel_mask);
			XFillRectangle(lpDestDC->dp->display,
					lpDestDC->Drawable,lpDestDC->gc,
			    		lpsd->xDest,lpsd->yDest,
					nWidthDest,nHeightDest);
			if (nROP2 != GXcopy &&
				lpDestDC->RopFunction == GXcopy)
			    XSetPlaneMask(lpDestDC->dp->display,
					lpDestDC->gc,AllPlanes);
		}
		break;
	    case ROP_FD:
		if (rop == BLACKNESS || rop == WHITENESS) {
		    XSetFunction(lpDestDC->dp->display,lpDestDC->gc,GXcopy);
		    XSetForeground(lpDestDC->dp->display, lpDestDC->gc,
				DrvMakePixel((rop == WHITENESS)?
					RGB(255,255,255):RGB(0,0,0), NULL));
		}
		else
		    if (nROP2 != GXcopy)
			XSetPlaneMask(lpDestDC->dp->display,
				lpDestDC->gc,lpDestDC->dp->pixel_mask);

		XFillRectangle(lpDestDC->dp->display,
				lpDestDC->Drawable,lpDestDC->gc,
				lpsd->xDest,lpsd->yDest,
				nWidthDest,nHeightDest);

		if (rop == WHITENESS || rop == BLACKNESS)
		    XSetForeground(lpDestDC->dp->display, lpDestDC->gc,
				lpDestDC->ForeColorPixel);
		else
		    if (nROP2 != GXcopy && lpDestDC->RopFunction == GXcopy)
			    XSetPlaneMask(lpDestDC->dp->display,
					lpDestDC->gc,AllPlanes);
		break;
	    case ROP_SD:
		if (nROP2 != GXcopy)
		    XSetPlaneMask(lpDestDC->dp->display,
				lpDestDC->gc,lpDestDC->dp->pixel_mask);
		if(srcPix || lpSrcDC->Drawable) 	/* sanity check */
		    XCopyArea(lpDestDC->dp->display,
				(Drawable)(srcPix?srcPix:lpSrcDC->Drawable),
				lpDestDC->Drawable,
				lpDestDC->gc,
				lpsd->xSrc,lpsd->ySrc,
				nWidthDest,nHeightDest,
				lpsd->xDest,lpsd->yDest);
		if (nROP2 != GXcopy && lpDestDC->RopFunction == GXcopy)
		    XSetPlaneMask(lpDestDC->dp->display,
				lpDestDC->gc,AllPlanes);
		break;
	    case ROP_DB:
	    case ROP_BP:
		if (lpDestDC->BrushFlag != BFP_NULL) {
		    if (gc != lpDestDC->gc) 
		        XSetFunction(lpDestDC->dp->display,gc,nROP2);
		    if (nROP2 != GXcopy)
			XSetPlaneMask(lpDestDC->dp->display,
				gc,lpDestDC->dp->pixel_mask);
		    XFillRectangle(lpDestDC->dp->display,
				(Drawable)tmpPix,gc,
				0,0,nWidthDest,nHeightDest);
		    if (nROP2 != GXcopy && lpDestDC->RopFunction == GXcopy)
			XSetPlaneMask(lpDestDC->dp->display,
				gc,AllPlanes);
		}
		break;
	    case ROP_DS:
	    case ROP_SP:
	    case ROP_SB:
		if (!tmpPix)
		    break;
		XSetFunction(lpDestDC->dp->display,gc,nROP2);
		if (nROP2 != GXcopy)
		    XSetPlaneMask(lpDestDC->dp->display,
				gc,lpDestDC->dp->pixel_mask);
		if (srcPix || lpSrcDC->Drawable)
		    XCopyArea(lpDestDC->dp->display,
				(Drawable)(srcPix?srcPix:lpSrcDC->Drawable),
				(Drawable)tmpPix,
				gc,
				lpsd->xSrc,lpsd->ySrc,
				nWidthDest,nHeightDest,
				0,0);
		if (nROP2 != GXcopy && lpDestDC->RopFunction == GXcopy)
		    XSetPlaneMask(lpDestDC->dp->display,gc,AllPlanes);
		break;
	    case ROP_PD:
		if (!tmpPix)
		    break;
		if (nROP2 != GXcopy)
		    XSetPlaneMask(lpDestDC->dp->display,
				lpDestDC->gc,lpDestDC->dp->pixel_mask);
		XCopyArea(lpDestDC->dp->display,
				(Drawable)tmpPix,
				(Drawable)(lpDestDC->Drawable),
				lpDestDC->gc,0,0,
				nWidthDest,nHeightDest,
				lpsd->xDest,lpsd->yDest);
		if (nROP2 != GXcopy && lpDestDC->RopFunction == GXcopy)
		    XSetPlaneMask(lpDestDC->dp->display,
				lpDestDC->gc,AllPlanes);
		break;
	    default:
		break;
	}
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    if (tmpPix) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	XFreePixmap(lpDestDC->dp->display,tmpPix);
	if (lpDestDC->lpDrvImage == 0) /* screen DC */
	    XFreeGC(lpDestDC->dp->display,gc);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    /* Restore the function on the DC previously set by SetROP2 */
    LOCK_SECTION( _MTLOCK_VIDEO);
    XSetFunction(lpDestDC->dp->display,lpDestDC->gc,
		lpDestDC->RopFunction);
    UNLOCK_SECTION( _MTLOCK_VIDEO);

    /* now destroy the stretched pixmap, if it exists */
    if (srcPix) {
	LOCK_SECTION( _MTLOCK_VIDEO);
	XFreePixmap(lpDestDC->dp->display,srcPix);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }


    if (lpDestDC->lpDrvImage && lpDestDC->lpDrvImage->fMono ) {
	if (lpSrcDC && (!(lpSrcDC->lpDrvImage &&
			lpSrcDC->lpDrvImage->fMono))) {
	    RECT rc;

	    SetRect(&rc,lpsd->xDest,lpsd->yDest,nWidthDest,nHeightDest);
	    ConvertColorDDBToMono(lpDestDC->lpDrvImage,&rc,
				lpSrcDC->BackColorPixel);
	}
	lpDestDC->lpDrvImage->fInvalid = TRUE;
    }

    LOCK_SECTION( _MTLOCK_VIDEO);
    if(xbits) DRVMEM_FREE(xbits);
    if(ybits) DRVMEM_FREE(ybits);
    UNLOCK_SECTION( _MTLOCK_VIDEO);

    return (DWORD)PGH_SUCCESS;
}

static DWORD
DrvGraphicsSelectImage(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
    LPDRIVERDC lpddc = (LPDRIVERDC)dwParam1;
    LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;
    LSDE_PUTIMAGEDATA *lppid = &lpStruct->lsde.imagedata;

    LPDRVIMAGEDATA lpid = lppid->lpimagedata;

    if (!lpddc || !lpid || !lpid->image)
	return 0;

    if (lpid->pixmap == 0 ) {
	
        if(!lpid->nWidth || !lpid->nHeight)
	   return 0;

    	LOCK_SECTION( _MTLOCK_VIDEO);
	lpid->pixmap = (Drawable)XCreatePixmap(lpddc->dp->display,
			DefaultRootWindow(lpddc->dp->display),
			lpid->nWidth,
			lpid->nHeight,
			lpddc->dp->Depth);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	if (lpid->fMono) {
	    GC gc;
	    XGCValues values;

	    values.foreground = DrvMakePixel(RGB(255,255,255), NULL);
	    values.background = DrvMakePixel(RGB(0,0,0), NULL);
    	    LOCK_SECTION( _MTLOCK_VIDEO);
	    gc = XCreateGC(lpddc->dp->display,
			lpid->pixmap,
			GCForeground|GCBackground,&values);
	    XPutImage(lpddc->dp->display,lpid->pixmap,
			gc,lpid->image,
			0,0,0,0,
			lpid->nWidth,lpid->nHeight);
	    XFreeGC(lpddc->dp->display,gc);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	}
	else
	{
    	    LOCK_SECTION( _MTLOCK_VIDEO);
	    XPutImage(lpddc->dp->display,lpid->pixmap,
			lpddc->gc,lpid->image,
			0,0,0,0,
			lpid->nWidth,lpid->nHeight);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	}
    }
    lpddc->Drawable = lpid->pixmap;
    lpddc->lpDrvImage = lpid;
    return PGH_SUCCESS;
}

static DWORD
DrvGraphicsPutImage(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
    LPDRIVERDC lpddc = (LPDRIVERDC)dwParam1;
    LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;
    LSDE_PUTIMAGEDATA *lppid = &lpStruct->lsde.imagedata;
    XImage *im;

    if(!lppid || !lppid->lpimagedata || !lpddc) 
	return FALSE;

    if (!(im = ((LPDRVIMAGEDATA)lppid->lpimagedata)->image))
	return FALSE;

    if(lpddc->Drawable && lpddc->gc) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	if (lpddc->RopFunction != GXcopy)
	    XSetFunction(lpddc->dp->display,lpddc->gc,GXcopy);
	if (im->format == XYBitmap) {
	    lpddc->ForeColorPixel = DrvMakePixel(RGB(255,255,255), NULL);
	    XSetForeground(lpddc->dp->display,lpddc->gc,lpddc->ForeColorPixel);
	    lpddc->BackColorPixel = DrvMakePixel(RGB(0,0,0), NULL);
	    XSetBackground(lpddc->dp->display,lpddc->gc,lpddc->BackColorPixel);
	}
	XPutImage(lpddc->dp->display,lpddc->Drawable,lpddc->gc,im,
		    lppid->xSrc,lppid->ySrc,
		    lppid->xDest,lppid->yDest,
		    lppid->cx,lppid->cy);
	/* Restore the function */
	if (lpddc->RopFunction != GXcopy)
	    XSetFunction(lpddc->dp->display,lpddc->gc, lpddc->RopFunction);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
	
    return TRUE;
}

DWORD                   
DrvGraphicsSetPixel(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{       
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;
        
        if (lpStruct->lsde_validate.dwInvalidMask)
                DrvValidate(lpDriverDC, lpStruct);
        return (DWORD)DrvSetPixel(lpDriverDC, (COLORREF)dwParam2, 
		&lpStruct->lsde.point);
}   

static COLORREF
DrvSetPixel(LPDRIVERDC lpddc, COLORREF cr, LPPOINT lppt)
{
    DWORD dwPixel;
    COLORREF crRet;

    crRet = DrvGetPixel(lpddc,lppt);

    dwPixel = DrvMakePixel(cr, lpddc);
    LOCK_SECTION( _MTLOCK_VIDEO);
    XSetForeground(lpddc->dp->display,lpddc->gc,dwPixel);
    XDrawPoint(lpddc->dp->display,lpddc->Drawable,lpddc->gc,
		lppt->x,lppt->y);
    XSetForeground(lpddc->dp->display,lpddc->gc,lpddc->ForeColorPixel);
    UNLOCK_SECTION( _MTLOCK_VIDEO);

    return crRet;
}

DWORD   
DrvGraphicsGetPixel(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{   
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;
    
        if (lpStruct->lsde_validate.dwInvalidMask)
                DrvValidate(lpDriverDC, lpStruct);
        return (DWORD)DrvGetPixel(lpDriverDC, &lpStruct->lsde.point);
}

static COLORREF
DrvGetPixel(LPDRIVERDC lpddc, LPPOINT lppt)
{
    XImage *image;
    DWORD dwPixel;

    lpddc->dp->ErrorCode = 0;
    LOCK_SECTION( _MTLOCK_VIDEO);
    image = XGetImage(lpddc->dp->display,lpddc->Drawable,
			lppt->x,lppt->y,1,1,
			(DWORD)-1,ZPixmap);
    UNLOCK_SECTION( _MTLOCK_VIDEO);

    if (lpddc->dp->ErrorCode) {
	return (COLORREF)-1;
    }
    if (image) {
	LOCK_SECTION( _MTLOCK_VIDEO);
	dwPixel = XGetPixel(image,0,0);
	XDestroyImage(image);
	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }
    else
	return (COLORREF)0L;

    return (COLORREF)DRVCALL_COLORS(PLH_GETCOLORREF,dwPixel,0,0);
}

DWORD
DrvGraphicsSetBrushOrg(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;

    	LOCK_SECTION( _MTLOCK_VIDEO);
	XSetTSOrigin(lpDriverDC->dp->display, lpDriverDC->gc,
		lpStruct->lsde.point.x,lpStruct->lsde.point.y);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return PGH_SUCCESS;
}

DWORD
DrvGraphicsGetAspectRatio(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;

    	LOCK_SECTION( _MTLOCK_VIDEO);
        ((LPSIZE)lpStruct)->cx =
	        (DisplayWidthMM(lpDriverDC->dp->display,
                        lpDriverDC->dp->screen)*1000) /
                DisplayWidth(lpDriverDC->dp->display, lpDriverDC->dp->screen);
        ((LPSIZE)lpStruct)->cy =
                (DisplayHeightMM(lpDriverDC->dp->display,
                        lpDriverDC->dp->screen)*1000) /
                DisplayHeight(lpDriverDC->dp->display, lpDriverDC->dp->screen);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
        return 1L;
}


DWORD
DrvValidate(LPDRIVERDC lpddc, LPLSDS_PARAMS lpStruct)
{
    LSDE_VALIDATE *lpvs = &lpStruct->lsde_validate;
    DWORD dwMask = lpvs->dwInvalidMask;
    int LineStyle;
    int LineCap = CapNotLast;
    int LineJoin = JoinMiter;

    if (dwMask & IM_POLYFILLMODEMASK) {
        LOCK_SECTION( _MTLOCK_VIDEO);
	XSetFillRule(lpddc->dp->display,lpddc->gc,
		(lpvs->PolyFillMode == WINDING)?WindingRule:EvenOddRule);
        UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    if (dwMask & IM_ROP2MASK) {
	lpddc->RopFunction = ROPS[lpvs->ROP2];

        LOCK_SECTION( _MTLOCK_VIDEO);
	XSetFunction(lpddc->dp->display,lpddc->gc,lpddc->RopFunction);
	if (lpvs->ROP2 != R2_COPYPEN)
	    XSetPlaneMask(lpddc->dp->display,lpddc->gc,
				lpddc->dp->pixel_mask);
	else
	    XSetPlaneMask(lpddc->dp->display,lpddc->gc,AllPlanes);
        UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    if (dwMask & IM_STRETCHMODEMASK) 
	lpddc->StretchMode = lpvs->StretchMode;

    if (dwMask & IM_BKMODEMASK) 
	lpddc->BkFillMode = lpvs->BackMode;

    if (dwMask & IM_TEXTCOLORMASK) 
	lpddc->TextColorPixel = DrvMakePixel(lpvs->TextColor, lpddc);

    if (dwMask & (IM_BKCOLORMASK|IM_SRCBKCOLORMASK)) {
        DWORD dwBkPixel;

	dwBkPixel = DrvMakePixel((dwMask & IM_BKCOLORMASK)
			? lpvs->BackColor : lpvs->SrcDCBkColor, lpddc);
	if (lpddc->BackColorPixel != dwBkPixel) {
    	    LOCK_SECTION( _MTLOCK_VIDEO);
	    XSetBackground(lpddc->dp->display,lpddc->gc,dwBkPixel);
    	    UNLOCK_SECTION( _MTLOCK_VIDEO);
	    lpddc->BackColorPixel = dwBkPixel;
  	}
    }

    if (dwMask & IM_PENCOLORMASK) 
	lpddc->PenColorPixel = DrvMakePixel(lpvs->PenColor, lpddc);

    if (dwMask & IM_BRUSHCOLORMASK) 
	lpddc->BrushColorPixel = DrvMakePixel(lpvs->BrushColor, lpddc);

    if (dwMask & IM_BRUSHFILLMASK) {
	lpddc->BrushFlag = lpvs->BrushFlag;
	switch (lpddc->BrushFlag) {
	    case BFP_NULL:
	    case BFP_PIXEL:
		lpddc->BrushFillStyle = FillSolid;
		break;
	    case BFP_BITMAP:
		lpddc->BrushFillStyle = (lpddc->BkFillMode == OPAQUE) ?
				FillOpaqueStippled : FillStippled;
    		LOCK_SECTION( _MTLOCK_VIDEO);
		XSetStipple(lpddc->dp->display,lpddc->gc,
				(Pixmap)lpvs->BrushPrivate);
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
		break;
	    case BFP_PIXMAP:
		lpddc->BrushFillStyle = FillTiled;
    		LOCK_SECTION( _MTLOCK_VIDEO);
		XSetTile(lpddc->dp->display,lpddc->gc,
				(Pixmap)lpvs->BrushPrivate);
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
		break;
	    default:
		break;
	}
    }
    if (dwMask & IM_PENSTYLEMASK) {
	char DashPattern[6];
	int DashCount;

	if ((lpvs->PenStyle & PS_STYLE_MASK) == PS_SOLID ||
			(lpvs->PenStyle & PS_STYLE_MASK) == PS_INSIDEFRAME ||
			(lpvs->PenStyle & PS_STYLE_MASK) == PS_NULL) {
	    LineStyle = LineSolid;
	    DashCount = 0;
	}
	else {
	    LineStyle = (lpddc->BkFillMode == OPAQUE) ?
			LineDoubleDash : LineOnOffDash;
	    if (lpddc->PenStyle != lpvs->PenStyle) {
		switch (lpvs->PenStyle) {
                    case PS_ALTERNATE:
                        DashPattern[0] = 1;
                        DashPattern[1] = 1;
                        DashCount = 2;
                        break;
		    case PS_DASH:
			DashPattern[0] = 17;
			DashPattern[1] = 6;
			DashCount = 2;
			break;
		    case PS_DOT:
			DashPattern[0] = 3;
			DashPattern[1] = 3;
			DashPattern[2] = 3;
			DashPattern[3] = 3;
			DashCount = 4;
			break;
		    case PS_DASHDOT:
			DashPattern[0] = 10;
			DashPattern[1] = 5;
			DashPattern[2] = 3;
			DashPattern[3] = 5;
			DashCount = 4;
			break;
		    case PS_DASHDOTDOT:
			DashPattern[0] = 9;
			DashPattern[1] = 3;
			DashPattern[2] = 3;
			DashPattern[3] = 3;
			DashPattern[4] = 3;
			DashPattern[5] = 3;
			DashCount = 6;
			break;
		    default:
			DashCount = 0;
			break;
		}
		if (DashCount) {
    		    LOCK_SECTION( _MTLOCK_VIDEO);
		    XSetDashes(lpddc->dp->display,lpddc->gc,
			0,DashPattern,DashCount);
    		    UNLOCK_SECTION( _MTLOCK_VIDEO);
		}
	    }
	}

        /* this is only valid for Geometric pens */
        if ((lpvs->PenStyle & PS_TYPE_MASK) == PS_COSMETIC)
        {
            LineCap = CapRound;
            LineJoin = JoinRound;
        }
        else
        {
            /* determine the End Cap Style */
            switch (lpvs->PenStyle & PS_ENDCAP_MASK) {
                case PS_ENDCAP_ROUND: 
                    LineCap = CapRound;
                    break;
                 case PS_ENDCAP_SQUARE:
                     LineCap = CapProjecting;
                     break;
                 case PS_ENDCAP_FLAT:
                     LineCap = CapNotLast;
                     break;
             }
            /* determine the joint Style */
            switch (lpvs->PenStyle & PS_JOIN_MASK) {
                 case PS_JOIN_ROUND: 
                     LineJoin = JoinRound;
                     break;
                 case PS_JOIN_BEVEL:
                     LineJoin = JoinBevel;
                     break;
                 case PS_JOIN_MITER:
                     LineJoin = JoinMiter;
                     break;
             }
        }

	lpddc->PenStyle = lpvs->PenStyle;
        LOCK_SECTION( _MTLOCK_VIDEO);
	XSetLineAttributes(lpddc->dp->display,lpddc->gc,
			lpvs->PenWidth, LineStyle,
			LineCap, LineJoin);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    if (dwMask & IM_PENWIDTHMASK) {
	if ((lpvs->PenStyle & PS_STYLE_MASK) == PS_SOLID ||
			(lpvs->PenStyle & PS_STYLE_MASK) == PS_INSIDEFRAME ||
			(lpvs->PenStyle & PS_STYLE_MASK) == PS_NULL)
	    LineStyle = LineSolid;
	else
	    LineStyle = (lpddc->BkFillMode == OPAQUE) ?
			LineDoubleDash : LineOnOffDash;

	/*  For now let's just ignore lpvs->PenHeight */

        /* this is only valid for Geometric pens */
        if ((lpvs->PenStyle & PS_TYPE_MASK) == PS_COSMETIC)
        {
            LineCap = CapNotLast;
            LineJoin = JoinMiter;
        }
        else
        {
            /* determine the End Cap Style */
            switch (lpvs->PenStyle & PS_ENDCAP_MASK) {
                case PS_ENDCAP_ROUND: 
                    LineCap = CapRound;
                    break;
                 case PS_ENDCAP_SQUARE:
                     LineCap = CapProjecting;
                     break;
                 case PS_ENDCAP_FLAT:
                     LineCap = CapNotLast;
                     break;
             }
            /* determine the joint Style */
            switch (lpvs->PenStyle & PS_JOIN_MASK) {
                 case PS_JOIN_ROUND: 
                     LineJoin = JoinRound;
                     break;
                 case PS_JOIN_BEVEL:
                     LineJoin = JoinBevel;
                     break;
                 case PS_JOIN_MITER:
                     LineJoin = JoinMiter;
                     break;
             }
        }

    	LOCK_SECTION( _MTLOCK_VIDEO);
	XSetLineAttributes(lpddc->dp->display,lpddc->gc,
			lpvs->PenWidth, LineStyle,
			LineCap, LineJoin);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    return 0L;
}

static DWORD 
DrvGraphicsGetDeviceCaps(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
	int nCaps = (int)dwParam2;
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
	
	Display *display;
	int screen;
	Visual *visual;

	int nHorzSize, nVertSize,
	    nHorzRes, nVertRes,
	    nBitsPixel, nPlanes,
	    nNumBrushes, nNumPens, nNumMarkers, nNumFonts, nNumColors,
	    nAspectX, nAspectY, nAspectXY,
	    nLogPixelsX, nLogPixelsY,
	    nSizePalette, nNumReserved, nColorRes;

	display = lpDriverDC->dp->display;
	screen = lpDriverDC->dp->screen;
    	LOCK_SECTION( _MTLOCK_VIDEO);
	visual = XDefaultVisual(display, screen);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	switch (nCaps)
	{
	case DRIVERVERSION:
		return (DWORD)0x30a;

	case TECHNOLOGY:
		return (DWORD)DT_RASDISPLAY;

	case HORZSIZE:
    		LOCK_SECTION( _MTLOCK_VIDEO);
		nHorzSize = XDisplayWidthMM(display, screen);
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
		return (DWORD)(nHorzSize);

	case VERTSIZE:
    		LOCK_SECTION( _MTLOCK_VIDEO);
		nVertSize = XDisplayHeightMM(display, screen);
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
		return (DWORD)(nVertSize);

	case HORZRES:
    		LOCK_SECTION( _MTLOCK_VIDEO);
		nHorzRes = XDisplayWidth(display, screen);
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
		return (DWORD)(nHorzRes);

	case VERTRES:
    		LOCK_SECTION( _MTLOCK_VIDEO);
		nVertRes = XDisplayHeight(display, screen);
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
		return (DWORD)(nVertRes);

	case BITSPIXEL:
		nBitsPixel = lpDriverDC->dp->Depth;
		return (DWORD)(nBitsPixel);

	case PLANES:
		nPlanes = 1;
		return (DWORD)(nPlanes);

	case NUMBRUSHES:
		nNumBrushes = -1;
		return (DWORD)(nNumBrushes);

	case NUMPENS:
		nNumPens = 100;
		return (DWORD)(nNumPens);

	case NUMMARKERS:
		nNumMarkers = 0;
		return (DWORD)(nNumMarkers);

	case NUMFONTS:
		nNumFonts = 0;
		return (DWORD)(nNumFonts);

	case NUMCOLORS:
		nNumColors = 0;
		switch (visual->class)
		{
		case StaticGray:
			nNumColors = visual->map_entries;
			break;
		case StaticColor:
			nNumColors = visual->map_entries;
			break;
		case TrueColor:
			nNumColors = 1 << lpDriverDC->dp->Depth;
			break;
		case GrayScale:
			nNumColors = visual->map_entries;
			break;
		case PseudoColor:
			nNumColors = visual->map_entries;
			break;
		case DirectColor:
			nNumColors = 1 << lpDriverDC->dp->Depth;
			break;
		}
		return (DWORD)(nNumColors);

	case PDEVICESIZE:
		/* on Windows - 35 bytes (SVGA) */
		return (DWORD)0;	/* LATER -- support PDEVICE */

	case CURVECAPS:
		return (DWORD)(CC_CIRCLES |
			CC_PIE |
			CC_CHORD |
			CC_ELLIPSES |
			CC_WIDE |
			CC_STYLED |
			CC_WIDESTYLED |
			CC_INTERIORS);

	case LINECAPS:
		return (DWORD)(LC_POLYLINE |
			LC_WIDE |
			LC_STYLED |
			LC_WIDESTYLED);

	case POLYGONALCAPS:
		return (DWORD)(PC_POLYGON |
			PC_RECTANGLE |
			PC_WINDPOLYGON |
			PC_SCANLINE |
			PC_WIDE |
			PC_STYLED |
			PC_WIDESTYLED);

	case TEXTCAPS:
		return (DWORD)(TC_CP_STROKE | TC_RA_ABLE);

	case CLIPCAPS:
		return (DWORD)CP_RECTANGLE;

	case RASTERCAPS:
		return (DWORD)(RC_BITBLT |
			RC_BITMAP64 |
			RC_GDI20_OUTPUT |
			RC_DI_BITMAP |
			RC_PALETTE |
			RC_DIBTODEV |
			RC_BIGFONT |
			RC_STRETCHBLT);

	case ASPECTX:
		nAspectX = 10;
		return (DWORD)(nAspectX);

	case ASPECTY:
		nAspectY = 10;
		return (DWORD)(nAspectY);

	case ASPECTXY:
		nAspectXY = 14;
		return (DWORD)(nAspectXY);

	/* logical pixels per inch is scaled by scale_factor* to make it easier
	 * to read
	 */
	case LOGPIXELSX:
#ifdef TWIN32
		nLogPixelsX = 0x60;
#else
    		LOCK_SECTION( _MTLOCK_VIDEO);
		nLogPixelsX = (int)
			((254L *
			XDisplayWidth(display,screen)) /
			(10L *
			XDisplayWidthMM(display,screen)));
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
#endif
		return (DWORD)(nLogPixelsX);

	/* logical pixels per inch is scaled by scale_factor* to make it easier
	 * to read
	 */
	case LOGPIXELSY:
#ifdef TWIN32
		nLogPixelsY = 0x60;
#else
    		LOCK_SECTION( _MTLOCK_VIDEO);
		nLogPixelsY = (int)
			((254L *
			XDisplayHeight(display, screen)) /
			(10L *
			XDisplayHeightMM(display, screen)));
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
#endif
		return (DWORD)(nLogPixelsY);

	case SIZEPALETTE:
		nSizePalette = 0;
		switch (visual->class)
		{
		case StaticGray:
			nSizePalette = visual->map_entries;
			break;
		case StaticColor:
			nSizePalette = visual->map_entries;
			break;
		case TrueColor:
			nSizePalette = 0;
			break;
		case GrayScale:
			nSizePalette = visual->map_entries;
			break;
		case PseudoColor:
			nSizePalette = visual->map_entries;
			break;
		case DirectColor:
			nSizePalette = 0;
			break;
		}
		return (DWORD)(nSizePalette);

	case NUMRESERVED:
		nNumReserved = 0;
		switch (visual->class)
		{
		case StaticGray:
			nNumReserved = visual->map_entries;
			break;
		case StaticColor:
			nNumReserved = visual->map_entries;
			break;
		case TrueColor:
			nNumReserved = 0;
			break;
		case GrayScale:
			nNumReserved = min(20, visual->map_entries);
			break;
		case PseudoColor:
			nNumReserved = min(20, visual->map_entries);
			break;
		case DirectColor:
			nNumReserved = 0;
			break;
		}
		return (DWORD)(nNumReserved);

	case COLORRES:
		nColorRes = 0;
		switch (visual->class)
		{
		case StaticGray:
			nColorRes = 3 * visual->bits_per_rgb;
			break;
		case StaticColor:
			nColorRes = 3 * visual->bits_per_rgb;
			break;
		case TrueColor:
			nColorRes = 3 * visual->bits_per_rgb;
			break;
		case GrayScale:
			nColorRes = 3 * visual->bits_per_rgb;
			break;
		case PseudoColor:
			nColorRes = 3 * visual->bits_per_rgb;
			break;
		case DirectColor:
			nColorRes = 3 * visual->bits_per_rgb;
			break;
		}
		return (DWORD)(nColorRes);

	}

	return (0);
}

DWORD
DrvGraphicsScrollDC(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
        LPDRIVERDC lpDriverDC = (LPDRIVERDC)dwParam1;
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;
   
    	LOCK_SECTION( _MTLOCK_VIDEO);
	XCopyArea(lpDriverDC->dp->display, lpDriverDC->Drawable,
		lpDriverDC->Drawable, lpDriverDC->gc,
		lpStruct->lsde.scrolldc.xSrc,
		lpStruct->lsde.scrolldc.ySrc,
		lpStruct->lsde.scrolldc.nWidth,
		lpStruct->lsde.scrolldc.nHeight,
		lpStruct->lsde.scrolldc.xDest,
		lpStruct->lsde.scrolldc.yDest);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
	return (DWORD)TRUE;
}

DWORD
DrvGraphicsTab(void)
{
	return (DWORD)DrvGraphicsEntryTab;
}

DWORD 
DrvGraphicsInit(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
	return (DWORD)TRUE;
}

static DWORD
DrvGraphicsDoNothing(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
	return 0L;
}

static DWORD
DrvGraphicsPolygon(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpParams)
{
	LPDRIVERDC lpDriverDC= (LPDRIVERDC)dwParam1;
	LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpParams;

            /* dwParam2 -- RelAbs flag */
            if (lpStruct->lsde_validate.dwInvalidMask)
                DrvValidate(lpDriverDC, lpStruct);
            return DrvPolygon(lpDriverDC, lpStruct->lsde.polypoly.nTotalCount,
                lpStruct->lsde.polypoly.lpPoints, dwParam2);
}
