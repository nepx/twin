/*
	@(#)DrvText.c	2.72
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
 
/****************************************************************************
 *
 * 	The following features should be added at some point.
 *	1: 	should be able to alias a LOGFONT to a specific any fontstring,
 *	        even one that is not caught in our pattern matching
 *	2:	systemfont.fontmapper codes are not supported we support ...
 *			minimum and maximum font sizes
 *			aliases for fonts
 *			alternate system font and size
 *			need to specify that system font is not scalable
 *	3:	additional font mapping based on LOGFONT information, including
 * 			determine if pitch and family specify a fixed-width font
 *			character sets		
 *	4: 	should have a way to get new font strings, in the event the
 *	        server gets additional fonts. Do this by comparing new strings
 *	        against all current strings, and then adding them, see the 
 *	        example in the current list...
 *
 *	char fontspec[256];
 *
 *	if (minsize && height < minsize)
 *		height = minsize;
 * 	if (maxsize && height > maxsize)
 *		height = maxsize;
 *
 *	sprintf(fontspec, fml->format, abs(height), 0); 
 *	
 *	1 for a given font, use min/max to return in correct range
 *	2 additional value for type of font, swiss/roman,plus delta
 *		delta is amount we can be off, before rescale
 *		flag says dont scale
 *		flag says what font class
 *	3 additional value for family preference
 *
 *****************************************************************************/


/************************************************/
/* Turn this on for verbose debugging output 	*/
/*#define SEVERE 				*/
/************************************************/

#include <string.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "windows.h"
#include "kerndef.h"
#include "Driver.h"
#include "Log.h"
#include "GdiDC.h"
#include "DrvDP.h"
#include "DrvDC.h"
#include "DeviceData.h"
#include "WinConfig.h"
#include "WinMalloc.h"
#include "compat.h"
#include "mtlock.h"

/****************************************************************/
#include "compat.h"
/****************************************************************/

static DWORD  DrvBuildFontCache(LPNEWLOGFONT, LPNEWTEXTMETRIC, LPSTR, DWORD);
static DWORD  DrvGetFontCacheSize(LPSTR);
static DWORD  DrvFreeFont(LPDRIVERDC, LPDRVFONTDATA);
static int ParseXFont(LPSTR, LPSTR, int, int);
static LPSTR XFontAlias(LPSTR);
static LPSTR WinFontAlias(LPSTR);
static void BuildMetrics(LPLOGFONT, LPDRVFONTDATA, LPNEWTEXTMETRIC);


DWORD DrvTextTab(LPARAM, LPARAM, LPVOID);
static LPDRVFONTDATA DrvRealizeFont(LPDRIVERDC, LSDE_REALIZEFONT *, DWORD, BOOL);
static DWORD DrvGetTextMetrics(LPDRIVERDC, LPNEWTEXTMETRIC);
static DWORD DrvGetCharWidth(LPDRIVERDC, LPINT, DWORD);
static DWORD DrvExtTextOut(LPDRIVERDC,LPLSDS_PARAMS,BOOL);
static DWORD DrvGetTextFace(LPDRIVERDC, LPLSDS_PARAMS);
static void DrvInitFonts(LPDRIVERDC);
static void DrvParseFont(LPSTR, BOOL);

DWORD DrvTextInit(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextRescale(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextRealize(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextGetMetrics(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextGetCharWidth(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextGetTextFace(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextGetEnumCnt(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextEnumFonts(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextDeleteFont(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextExtTextOut(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextDoNothing(LPARAM,LPARAM,LPVOID);
static DWORD DrvTextGetGlyphOutline(LPARAM,LPARAM,LPVOID);

static DPYFONTINFO * DrvGetFP(LPSTR, BOOL);
static LPSTR DrvFontAlias(LPLOGFONT, LPSTR, LPINT, LPINT);
static BOOL DrvInitFontAliases(void);

extern DWORD DrvValidate(LPDRIVERDC,LPLSDS_PARAMS);
extern char * WinSaveString(LPSTR);
extern int    SetSystemMetrics(int,int);
 
int DrvTrapHookFontError(int,int);

LPDRVFONTDATA DrvReleaseFont(LPDRIVERDC ,LPDRVFONTDATA );

/****************************************************************************
 * Driver Dependent Font Declaration
 * Default System Font Information
 ****************************************************************************/

typedef struct {
	ATOM	FaceAtom;
	char	*oldname;
	char 	*newname;
        int      minsize; /* minimal size we can map to FOR SCALABLE FONTS ONLY! */
        int      maxsize; /* maximal size we can map to FOR SCALABLE FONTS ONLY! */
	int	style;	  /* pitch and family of the newname font */
} FONTALIAS;

static struct	{
   char	 fontface[LF_FACESIZE];
   int	 fontsize;
   int   fontmapper;
   DPYFONTINFO *system;		/* for system font */
   DPYFONTINFO	*fontinfo;	/* to all fonts    */
   FONTALIAS    *fontalias;	/* alias table of fonts */
}	systemfont;

/****************************************************************************
 * Driver Dependent Font Declaration
 * Default Font Information
 ****************************************************************************/

static TWINDRVSUBPROC DrvTextEntryTab[] = {
	DrvTextInit, 
	DrvTextDoNothing,
	DrvTextDoNothing,
	DrvTextRealize,
	DrvTextRescale,
	DrvTextGetMetrics,
	DrvTextGetCharWidth,
	DrvTextExtTextOut,
	DrvTextGetTextFace,
	DrvTextEnumFonts,
	DrvTextGetEnumCnt,
	DrvTextDeleteFont,
	DrvTextGetGlyphOutline
};

static DWORD
DrvGetTextMetrics(LPDRIVERDC lpddc , LPNEWTEXTMETRIC lpNTM)
{
    LPLOGFONT lplf;
    XFontStruct *xfs;
    int direction,ascent,descent,n;
    XCharStruct overall;
    char *alphabet;

#ifdef LATER
    int nLogPixelsY;
    unsigned long property;

	do it right...
	We should put any saved data from the font itself in terms of
 	what we thought the height was...
#endif
    if ( lpNTM )
	memset((LPSTR)lpNTM,'\0',sizeof(NEWTEXTMETRIC));
    else
	return (DWORD) 0;

    if(lpddc->lpFontData) {
	    lplf = &lpddc->lpFontData->LogFont;
	    if ( !(xfs = lpddc->lpFontData->finfo) )
		return (DWORD) 0;
    } else {
	return (DWORD) 0;
    }

    lpNTM->tmAscent = xfs->ascent;
    lpNTM->tmDescent = xfs->descent;
    lpNTM->tmHeight = lpNTM->tmAscent + lpNTM->tmDescent;

    lpNTM->tmInternalLeading = abs(lplf->lfHeight / 5);
    lpNTM->tmExternalLeading = lpNTM->tmInternalLeading / 4;

    lpNTM->tmFirstChar = xfs->min_char_or_byte2;
    lpNTM->tmLastChar = xfs->max_char_or_byte2;

    if (xfs->max_bounds.width)
	/* maximal width available */
	lpNTM->tmMaxCharWidth = xfs->max_bounds.width;
    else {
	for (n = 0, lpNTM->tmMaxCharWidth = 0;
	     n <= (int)(lpNTM->tmLastChar - lpNTM->tmFirstChar);
	     n++) {
	    if (lpNTM->tmMaxCharWidth < xfs->per_char[n].width)
		lpNTM->tmMaxCharWidth = xfs->per_char[n].width;
	}
    }

    lpNTM->tmDefaultChar = xfs->default_char;
    if ( lpNTM->tmDefaultChar < lpNTM->tmFirstChar ||
        lpNTM->tmDefaultChar > lpNTM->tmLastChar )
        lpNTM->tmDefaultChar = lpNTM->tmFirstChar;

    lpNTM->tmWeight = lplf->lfWeight;	/* temporarily */
    lpNTM->tmItalic = lplf->lfItalic;
    lpNTM->tmUnderlined = lplf->lfUnderline;
    lpNTM->tmStruckOut = lplf->lfStrikeOut;

    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    LOCK_SECTION( _MTLOCK_VIDEO);
    XTextExtents(xfs,alphabet,52,
		&direction,&ascent,&descent,&overall);
    UNLOCK_SECTION( _MTLOCK_VIDEO);

    /* round up .333 to 1 */
    lpNTM->tmAveCharWidth = (overall.width+36)/52;
    lpNTM->tmBreakChar = ' ';
    if(xfs->max_bounds.width == xfs->min_bounds.width)
    	lpNTM->tmPitchAndFamily = FF_MODERN|FIXED_PITCH;
    else
    	lpNTM->tmPitchAndFamily = FF_SWISS|VARIABLE_PITCH;

    lpNTM->tmCharSet = ANSI_CHARSET;
    lpNTM->tmOverhang = 0;
    lpNTM->tmDigitizedAspectX = lpNTM->tmAveCharWidth;
    lpNTM->tmDigitizedAspectY = 15;
    lpNTM->ntmFlags = 0;
    lpNTM->ntmSizeEM = 0;
    lpNTM->ntmCellHeight = 0;
    lpNTM->ntmAvgWidth = 0;

    /*  I needed a non-zero return value, so we'll put in the height/width 
     *  of the font as the return.
     */
    return (DWORD) MAKELONG(lpNTM->tmHeight, lpNTM->tmAveCharWidth);
}

/**************************************************************************/
static DWORD
DrvGetCharWidth(LPDRIVERDC lpddc, LPINT lpWidths, DWORD dwLength)
{
    LPABC lpABC;
    LPINT lpInt;
    LPDRVFONTDATA lpFontData;
    XFontStruct *xfs;
    int nCount,i;

    if ( !(lpFontData = lpddc->lpFontData) )
	return (DWORD) 0;

    if ( !(xfs = lpddc->lpFontData->finfo) )
	return (DWORD) 0;

    if ( lpWidths )
	memset((LPSTR)lpWidths,'\0',dwLength);
    else
	return (DWORD) 0;

    if (lpFontData->uiFontType == TRUETYPE_FONTTYPE) {
	nCount = min(dwLength / sizeof(ABC),
		(xfs->max_char_or_byte2 - xfs->min_char_or_byte2 + 1));

	for (i = 0,lpABC = (LPABC)lpWidths; i < nCount; i++,lpABC++) {

	    lpABC->abcA = xfs->per_char ? 
		xfs->per_char[i].lbearing : 0;

	    lpABC->abcB = xfs->per_char ? 
		(xfs->per_char[i].rbearing - xfs->per_char[i].lbearing) : 8;

	    lpABC->abcC = xfs->per_char ? 
            	(xfs->per_char[i].width  - xfs->per_char[i].rbearing) : 0;
	}
    }
    else {
	nCount = min(dwLength / sizeof(int),
		(xfs->max_char_or_byte2 - xfs->min_char_or_byte2 + 1));
	for (i = 0,lpInt = lpWidths; i < nCount; i++,lpInt++)
	    *lpInt = xfs->per_char ? xfs->per_char[i].width : 10;
    }

    return (DWORD) 1;
}

typedef	void (*LPFNDRAWSTRING)(Display *,Drawable,GC,int,int,char *,int);
typedef	int (*LPFNTEXTWIDTH)(XFontStruct *,char *,int);

/**************************************************************************/
static DWORD
DrvExtTextOut(LPDRIVERDC lpddc,LPLSDS_PARAMS lpStruct,BOOL bExtended)
{
    LSDE_TEXT *lptxt = &lpStruct->lsde.text;
    LPINT lpDX;
    LPFNDRAWSTRING  lpfnDrawString;
    LPFNTEXTWIDTH   lpfnTextWidth;
    int	x,y,i,j,xx;
    int nLen;
    UINT xExtent;
    XFontStruct *xfs;
    char *textout = 0;
    int   textsize = 0;

    if(lpddc->lpFontData == 0) {
	LOGSTR((LF_CONSOLE,"DrvExtTextOut: bad font information in dc\n"));
	DrvTrapHookFontError(0,2);
	return 0;
    }
	
    if ( !(xfs = lpddc->lpFontData->finfo) )
	return 0;

    if (lpStruct->lsde_validate.dwInvalidMask)
	DrvValidate(lpddc,lpStruct);

    if (lpddc->ForeColorPixel != lpddc->TextColorPixel) {
        lpddc->ForeColorPixel = lpddc->TextColorPixel;
        LOCK_SECTION( _MTLOCK_VIDEO);
    	XSetForeground(lpddc->dp->display,lpddc->gc,lpddc->TextColorPixel);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    lpDX = (bExtended)?lptxt->lpDX:0;

    if (lptxt->TextAlign & TA_UPDATECP) 
       {
       x = lpddc->cpt.x;
       y = lpddc->cpt.y;
       } 
    else {
         x = lptxt->x;
         y = lptxt->y;
         }

    if ( xfs->max_byte1 ) {
	lpfnDrawString = (lpddc->BkFillMode == OPAQUE)?
	    (LPFNDRAWSTRING)XDrawImageString16:(LPFNDRAWSTRING)XDrawString16;
	lpfnTextWidth = (LPFNTEXTWIDTH)XTextWidth16;
	nLen = lptxt->nCnt / 2;
    }
    else {
	lpfnDrawString = (lpddc->BkFillMode == OPAQUE)?
               	(LPFNDRAWSTRING)XDrawImageString:(LPFNDRAWSTRING)XDrawString;
	lpfnTextWidth = (LPFNTEXTWIDTH)XTextWidth;
	nLen = lptxt->nCnt;
    }

    /* TA_LEFT == 0, so check for others */ 
    /* also if TA_UPDATECP get extents now */
    if (lptxt->TextAlign & (TA_CENTER|TA_RIGHT|TA_UPDATECP)) 
       {
       if (lpDX)
          {
	  for (i = xExtent = 0; i < nLen; i++)
		xExtent += abs(lpDX[i]);
          }
       else {
	    LOCK_SECTION( _MTLOCK_VIDEO);
            xExtent = lpfnTextWidth(xfs,lptxt->lpStr,nLen);
	    UNLOCK_SECTION( _MTLOCK_VIDEO);
            if (lptxt->lpExtraSpace)
               {
               /*   Adjust xExtent to account text justification...
               */  
               for (i = j = 0;    i < lptxt->nCnt;   i++)
                   {
                   if (lptxt->lpStr[i] == lptxt->chBreak && j < lptxt->nBreakCount)
                      xExtent += lptxt->lpExtraSpace[j++];
                   }
               }
            }

	if ((lptxt->TextAlign & TA_CENTER) == TA_CENTER)
	    x -= xExtent/2;
	if((lptxt->TextAlign & TA_CENTER) == TA_RIGHT)
	    x -= xExtent;
	if(lptxt->TextAlign & TA_UPDATECP)
	  lpddc->cpt.x += xExtent;
    }

    /* TA_TOP == 0, so check for others */
    if (lptxt->TextAlign & (TA_BASELINE|TA_BOTTOM)) {
	if ((lptxt->TextAlign & TA_BASELINE) == TA_BOTTOM)
	    y -= xfs->descent - 1; 
    } else
	y += xfs->ascent;

    if (lpddc->FillStyle != FillSolid) {
    	LOCK_SECTION( _MTLOCK_VIDEO);
	XSetFillStyle(lpddc->dp->display,lpddc->gc,FillSolid);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
	lpddc->FillStyle = FillSolid;
    }

    if (lpDX)
       {
       xx = x;
       LOCK_SECTION( _MTLOCK_VIDEO);
       for (i = 0; i < nLen; x += lpDX[i++]) 
           lpfnDrawString(lpddc->dp->display,lpddc->Drawable,
	                  lpddc->gc, x,y,&lptxt->lpStr[i],1);
       UNLOCK_SECTION( _MTLOCK_VIDEO);
       x = xx;
       }
    else if (lptxt->lpExtraSpace)
            {
/* BEGIN -- DOES NOT WORK FOR DOUBLE BYTE */
            /*   Justified TextOut...
            */
            register int ii;
            char    *lpsz;

	    if (textsize < lptxt->nCnt + 1) {
		textout = WinMalloc(lptxt->nCnt + 1);
	        textsize = lptxt->nCnt + 1;
	    }
		 
            lpsz = textout;
          
            xx = x;
            for (xExtent = ii = i = j = 0;    i < nLen;   i++)
                {
                lpsz[ii++] = lptxt->lpStr[i];  lpsz[ii] = '\0';
		LOCK_SECTION( _MTLOCK_VIDEO);
                xExtent += lpfnTextWidth(xfs, &lptxt->lpStr[i], 1);
		UNLOCK_SECTION( _MTLOCK_VIDEO);
                if (lptxt->lpStr[i] == lptxt->chBreak && j < lptxt->nBreakCount)
                   {
                   xExtent += lptxt->lpExtraSpace[j++];
                   /*
                   **   Draw current word, including trailing break character,
                   **   then reset xExtent and character counter.
                   */
    		   LOCK_SECTION( _MTLOCK_VIDEO);
                   lpfnDrawString(lpddc->dp->display,lpddc->Drawable,
	                          lpddc->gc, x, y, lpsz, lstrlen(lpsz));
    		   UNLOCK_SECTION( _MTLOCK_VIDEO);
                   x += xExtent;
                   xExtent = ii = 0;
                   }
                }
            if (ii > 0) { /* draw the rest of the string (if any) */
    	       LOCK_SECTION( _MTLOCK_VIDEO);
               lpfnDrawString(lpddc->dp->display,lpddc->Drawable,
	                      lpddc->gc, x, y, lpsz, lstrlen(lpsz));
    	       UNLOCK_SECTION( _MTLOCK_VIDEO);
	    }
            x = xx;
            
/* END -- DOES NOT WORK FOR DOUBLE BYTE */
            }
         else {
    	      LOCK_SECTION( _MTLOCK_VIDEO);
	      lpfnDrawString(lpddc->dp->display,lpddc->Drawable,
	                     lpddc->gc, x,y,lptxt->lpStr,nLen);
    	      UNLOCK_SECTION( _MTLOCK_VIDEO);
              }

    /*** STRIKEOUT ***/
    if (lpddc->lpFontData->LogFont.lfStrikeOut) 
       {
       unsigned long thick;

	LOCK_SECTION( _MTLOCK_VIDEO);
       xExtent = lpfnTextWidth(xfs,lptxt->lpStr,nLen);
	UNLOCK_SECTION( _MTLOCK_VIDEO);

       if (lptxt->lpExtraSpace)
          {
          for (i = j = 0;   i < lptxt->nCnt;  i++)
              if (lptxt->lpStr[i] == lptxt->chBreak && j < lptxt->nBreakCount)
                 xExtent += lptxt->lpExtraSpace[j++];
          }
       thick = abs(lpddc->lpFontData->LogFont.lfHeight) / 40 + 1;
        LOCK_SECTION( _MTLOCK_VIDEO);
        XFillRectangle(lpddc->dp->display, lpddc->Drawable, lpddc->gc, 
		x, y-xfs->descent-thick/2, xExtent, thick);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
       }

    /*** UNDERLINE ***/
    if (lpddc->lpFontData->LogFont.lfUnderline) 
       {
       unsigned long pos, thick;

	LOCK_SECTION( _MTLOCK_VIDEO);
       if (!XGetFontProperty(xfs, XA_UNDERLINE_THICKNESS, &thick)) {
          /*
          **   No underline thickness info from X, use our own
          **   calculations, based on font height and boldness.
          */
          if (lpddc->lpFontData->LogFont.lfWeight >= FW_BOLD)
             thick = abs(lpddc->lpFontData->LogFont.lfHeight) / 18 + 1;
          else thick = abs(lpddc->lpFontData->LogFont.lfHeight) / 24 + 1;
       }
	UNLOCK_SECTION( _MTLOCK_VIDEO);

	LOCK_SECTION( _MTLOCK_VIDEO);
       if (!XGetFontProperty(xfs, XA_UNDERLINE_POSITION, &pos))
          pos = xfs->descent - thick; /* Make sure underline doesn't step out of character cell */

       xExtent = lpfnTextWidth(xfs,lptxt->lpStr,nLen);
	UNLOCK_SECTION( _MTLOCK_VIDEO);

       if (lptxt->lpExtraSpace)
          {
          for (i = j = 0;   i < lptxt->nCnt;  i++)
              if (lptxt->lpStr[i] == lptxt->chBreak && j < lptxt->nBreakCount)
                 xExtent += lptxt->lpExtraSpace[j++];
          }
    	LOCK_SECTION( _MTLOCK_VIDEO);
        XFillRectangle(lpddc->dp->display, lpddc->Drawable, lpddc->gc, 
		x, y+pos, xExtent, thick);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);
       }

    LOCK_SECTION( _MTLOCK_VIDEO);
    if(textout) WinFree(textout);
    UNLOCK_SECTION( _MTLOCK_VIDEO);

    return 1L;
}

/**************************************************************************
 *
 *	Array of font alias
 *	atom to atom substitution...
 *	
 *************************************************************************/
/*
 *	DEFAULT_PITCH	
 *	FIXED_PITCH		fixed pitch, duh.
 *	VARIABLE_PITCH		proportional
 *--------------------------------------------------------------------
 *	DONTCARE		we don't have info, or don' care
 *	DECORATIVE		novelty fonts
 *	MODERN			fixed pitch, with or without serifs 
 *	ROMAN			variable pitch, with serifs
 *	SWISS			variable pitch, without serifs
 *	SCRIPT			handwriting like
 */
static FONTALIAS facemap[] = {
	{ 0,"Arial","Helvetica", 	0, 0,  	FF_SWISS|VARIABLE_PITCH },
	{ 0,"Courier New","Courier",    0, 0, 	FF_MODERN|FIXED_PITCH   },
	{ 0,"Times New Roman","Times",  0, 0, 	FF_ROMAN|VARIABLE_PITCH },
	{ 0,"System","helvetica",	0, 0,	FF_SWISS|VARIABLE_PITCH }, 
	{ 0,"Symbol","Symbol", 		0, 0,	FF_DECORATIVE|VARIABLE_PITCH }, 
	{ 0,"Tms Rmn","Times",  	0, 0,	FF_ROMAN|VARIABLE_PITCH },
	{ 0,"MS Sans Serif","Helvetica",0, 0, 	FF_SWISS|VARIABLE_PITCH },
	{ 0,"MS Serif","Charter",	0, 0,  	FF_SWISS|VARIABLE_PITCH },
	{ 0,"Fixedsys","Fixed",         0, 0,   FF_ROMAN|VARIABLE_PITCH },
	{ 0,"Helv","Helvetica", 	0, 0,  	FF_SWISS|VARIABLE_PITCH },
	{ 0,"Terminal","Courier", 	0, 0,  	FF_MODERN|FIXED_PITCH },
	{ 0, 0, 0, 			0, 0,   0 }
};

static BOOL
DrvInitFontAliases(void)
{
    ATOM atmFace;
    FONTALIAS  *fp,*fm,*fn;
    char        keynames[1024];
    char        keycolor[128];
    char       *token;
    LPSTR       key;
    char       *lpszFileName;
    int inicnt,cnt,rv;
 
    lpszFileName = GetTwinFilename();

    systemfont.fontmapper = GetConfigOption(WD_HIRESFONTS);
 
    GetTwinString(WCP_FONTFACE,systemfont.fontface,LF_FACESIZE);

    GetPrivateProfileString((LPSTR)"windows",
                "fontface",
                systemfont.fontface,
                keynames,
                1024,
		lpszFileName);
 
    /* check to see if name is new */
    if (strcmp(keynames,systemfont.fontface)) {
	LOGSTR((LF_LOG,"system font set to %s\n",keynames));
	strcpy(systemfont.fontface,WinSaveString(keynames));
    }

    /* go load default size */
    systemfont.fontsize = GetPrivateProfileInt("windows",
		"fontsize",
		-12,
		lpszFileName);

    /* now go get font substitutions... */
    cnt = GetPrivateProfileString((LPSTR)"Font Alias",
	        (LPSTR) 0,
		(LPSTR) 0,
		keynames,
		1024,
		lpszFileName);

    inicnt = 0;

    if(cnt) {
    	for (key = keynames;*key != 0;key += strlen(key) + 1)
		inicnt++;
    }

    rv  = sizeof(facemap)/sizeof(FONTALIAS) + inicnt;

    LOCK_SECTION( _MTLOCK_VIDEO);
    systemfont.fontalias = (FONTALIAS *) WINMALLOC(rv*sizeof(FONTALIAS),7,0);
    UNLOCK_SECTION( _MTLOCK_VIDEO);
    memset((LPSTR)systemfont.fontalias,'\0',rv*sizeof(FONTALIAS));
    fp = systemfont.fontalias;

    if(cnt) {		
        for (key = keynames;   *key != 0;   key += strlen(key)+1) {
	if(key[0] == '#')
		continue;
	GetPrivateProfileString(
		(LPSTR)"Font Alias",
		(LPSTR) key,
		(LPSTR) 0,		/* default... */
		keycolor,
		128,
		lpszFileName);
	/* 
        **   See if it is set to anything... 
        */
	if (strlen(keycolor)) 
           {
	   fp->FaceAtom = AddAtom(key);
	   fp->oldname  = (char *) WinSaveString(key);
           /*
           **   Parse keycolor to extract newname, minsize and maxsize. The
           **   twin config file entry may look as follows (note than we can
           **   do this trick with scalable font only!):
           **
           **   [Font Alias]
           **   Fixedsys=XFixed[,min][,max]
           */
           token = strtok(keycolor, ",");
           if (token)
              {
              fp->newname = (char *)WinSaveString(token); 
              token = strtok(NULL, ",");
              if (token)
                 {
                 fp->minsize = atoi(token);
                 token = strtok(NULL, ",");
                 if (token)
                    fp->maxsize = atoi(token);
                 }
              }
	   else fp->newname  = (char *)WinSaveString(keycolor); 

	   LOGSTR((LF_LOG,"Mapping Font '%s' to '%s', size range %d..%d\n",
                     fp->oldname, fp->newname, fp->minsize, fp->maxsize));
	   fp++;
	   }
	fp->FaceAtom = 0;
        }
    }

    /*
    **   Now tack on the builtin table...
    **   (this could be optionally ignored via compatibility)
    */
    for (fm = facemap;   fm->oldname;   fm++) 
        {
	atmFace = AddAtom(fm->oldname);
	for (fn = systemfont.fontalias, rv = 1;   fn->FaceAtom && rv;  fn++) 
            {
	    if (atmFace == fn->FaceAtom)
		rv = 0;
	    }
	if (rv == 0)
	   continue;
	fp->FaceAtom = atmFace;
	fp->newname  = fm->newname;
	fp->oldname  = fm->oldname;
	fp++;

	LOGSTR((LF_LOG,"Builtin Alias\t%s to %s\n",
		fm->oldname,fm->newname));
        }

    fp->FaceAtom = 0;
    return TRUE;
}

/******************************************************************/
static int
DrvCountXAlias(LPCSTR lpszXFontName)
{
    FONTALIAS  *lpfa;
    int nCount = 0;
    /* Retrieves the number of windows alias of a Xfont : a given
     * Xfont can be used as alias for more than one Windows font.
     */
    if (systemfont.fontalias) {
	for (lpfa = systemfont.fontalias;  lpfa->oldname && lpfa->newname; lpfa++) {
	    if (strnicmp(lpszXFontName, lpfa->newname, strlen(lpfa->newname)) == 0)
		nCount++;
	}
    }
    return nCount;
}

/******************************************************************/
static LPSTR
DrvFontAlias(LPLOGFONT lplogfont, LPSTR userfont, LPINT lpMin, LPINT lpMax)
{
	ATOM	faceatom;
	char	*face;
	FONTALIAS  *fp;

	if(strlen(lplogfont->lfFaceName)) {
		face = lplogfont->lfFaceName;
	/* can we use the name we were given */
	} else if(strcmp(lplogfont->lfFaceName,"?") == 0) {
		face = systemfont.fontface;
	/* if no name, check if we are fixed? */
	} else if ((lplogfont->lfPitchAndFamily & 3) == FIXED_PITCH) {
		face = "fixed";
	/* if no name, default and variable implies system font */
	} else {
		face = systemfont.fontface;
	}

	faceatom = AddAtom(face);

	/* look for an alias to face, return it in userfont */
	for (fp = systemfont.fontalias;   fp->FaceAtom;   fp++) 
            {
	    if (fp->FaceAtom == faceatom) 
               {
	       strcpy(userfont,fp->newname);
               *lpMin = fp->minsize;
               *lpMax = fp->maxsize;
	       return userfont;
	       }
	}

	/* did not alias the font, return what we got... */
	strcpy(userfont,lplogfont->lfFaceName);
        *lpMin = *lpMax = 0;
	return userfont;
}


/******************************************************************/
static void
DrvInitFonts(LPDRIVERDC lpddc)
{
	int             i;
	char          **fs;
	DPYFONTINFO       *fp;
	char          **fontlist;
	int    		count = 0,nfonts = 0;
	char		fontstring[256];
	int		last = 0;

	/*    
        **   	Go load all known fonts
  	**	Start with scalable fonts
        */
        if (systemfont.fontmapper)
	      	strcpy(fontstring,"-*-*-*-*-*-*-0-0-100-100-*-*-*-*");
	else 
		strcpy(fontstring,"-*-*-*-*-*-*-0-0-75-75-*-*-*-*");

    	LOCK_SECTION( _MTLOCK_VIDEO);
	fontlist = XListFonts(lpddc->dp->display,fontstring,512,&nfonts);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	for (fs = fontlist, i = 0;   i < nfonts;   i++, fs++)
	       DrvParseFont(*fs, 1);
	XFreeFontNames(fontlist);

        /*
        **   	Now go find all nonscalable fonts...
        */
	if (systemfont.fontmapper)
    	      	strcpy(fontstring,"-*-*-*-*-*--*-*-100-100-*-*-*-*");
    	else 
		strcpy(fontstring,"-*-*-*-*-*--*-*-75-75-*-*-*-*");

    	LOCK_SECTION( _MTLOCK_VIDEO);
    	fontlist = XListFonts(lpddc->dp->display,fontstring,512,&count);
    	UNLOCK_SECTION( _MTLOCK_VIDEO);

	for (fs = fontlist, i = 0;   i < count;   i++, fs++)
	    DrvParseFont(*fs, 0);
	XFreeFontNames(fontlist);

	/*    
        **   	Walk the font tree 
	**	break down the fonts into family and styles
        */
	for (fp = systemfont.fontinfo;fp;fp = fp->fontnext) {
                for (i = 0;  i < 4;  i++)
                   if (fp->fontinfo[i])
                      { last = i;  break; }

		for (i = last;  i < 4;  i++)  { /* Remap fonts */
			/* fall back to better version... */
			if(fp->fontinfo[i] == 0) {
				fp->fontinfo[i] = fp->fontinfo[last];
				continue;
			}

			last = i;
		}
	}
}

/******************************************************************/
static DPYFONTINFO *
DrvGetFP(LPSTR fstring, BOOL ynScalable)
{
	DPYFONTINFO *fp;
        char         face[64];
	int	     found;

#ifdef LATER
        if (!ynScalable)
           {
	   strcpy(face, fstring);
           if (face[0] >= 'a' && face[0] <= 'z')
              face[0] -= 0x20;
           }
	else {
             strcpy(face, "X");
             strcat(face, fstring);
             if (face[1] >= 'a' && face[1] <= 'z')
                face[1] -= 0x20;
             }
#else
     	strcpy(face,fstring);
#endif

	/****************************************************/
	/* search for the font family matching the fontface */
	/* the face has already been de-aliased             */
	/****************************************************/

	for(fp = systemfont.fontinfo,found = 0;   fp;   fp = fp->fontnext) {
		if(strcasecmp(fp->fontface,face) == 0) { 
			found++;  
			break; 
		}
	}

	/****************************************************/
	/* allocate a new family if none found */
	/****************************************************/
	if(found == 0) {
		LOCK_SECTION( _MTLOCK_VIDEO);
		fp = (DPYFONTINFO *) WINMALLOC(sizeof(DPYFONTINFO),7,0);
		UNLOCK_SECTION( _MTLOCK_VIDEO);

		/* add a new fontface group */
		fp->uiAliasNb = -1;
		fp->fontnext = systemfont.fontinfo;
		fp->fontflag = 0;
		fp->fontminmax   = 0;
       		fp->fontface = (char *) WinSaveString(face);
                
		/* initialize the tables to NULL */
		fp->fontinfo[0] = 0;
		fp->fontinfo[1] = 0;
		fp->fontinfo[2] = 0;
		fp->fontinfo[3] = 0;
		systemfont.fontinfo = fp;
	}	

	return fp;
}

/****************************************************************************
 * Driver Dependent Font Code
 * Parse a fontstring, and add to system tables...
 ****************************************************************************/
static void
DrvParseFont(LPSTR s, BOOL ynScalable)
{
	char	fontcopy[256];
	char	fontstring[512];
	char	*fields[16];
	char    *p;
	int	nStyle;
	int	nCount;
	DPYFONTINFO *fp;
	DRVFONTDATA *fm,*fml,*fmn;
	int	bold,italics;
	char   *fs;

	/* we are going to mess with the string, so save it */
	strcpy(fontcopy,s);
	
	/* break the string into components... */
	for (p=fontcopy,nStyle=0;*p;) {
		if( *p != '-') {
			p++;			/* we can safely advance... */
			continue;
		}

		*p = 0;				/* terminate previous token  */
		p++;				/* we now point to the token */
		fields[nStyle++] = p;		/* new token is ...          */
		fields[nStyle] = 0;		/* next token starts as null */
		if( *p != '-')			/* we may have null token..  */
			p++;
	}

	/* last entry encountered */
	nCount = nStyle;

	/* this field is the nominal size, in pixels, with leading - */
	fields[5]++;

        /*
        **   Don't count scalable fonts anymore, since they all are
        **   already in here...
        */
        if (!ynScalable && atoi(fields[5]) == 0)
           return; 
        /*
        **   Michael Dvorkin  28-DEC-1995  
        **   Ignore non-scalable fonts where average character width == 0.
        **   XLoadFont generates BadName trap on Caldera!
        */
        if (!ynScalable && atoi(fields[11]) == 0)
           return;
        /*
        **   Skip japanese/chinese character sets, since we don't
        **   use them anyway (too expensive to scale)...
        */
#ifdef NO_ORIENTAL
	if (isdigit(fields[13][0]) && atoi(fields[13]) == 0)
           return; 
#endif
        /*
        **   If it's scalable font, store it only if it's the same resolution
        */
        if (ynScalable)
           {
           if (systemfont.fontmapper)
              {
              /* 100-100 requested... */
              if (atoi(fields[8]) != 100 || atoi(fields[9]) != 100)
                 return;
              }
           else { /* 75-75 requested */
                if (atoi(fields[8]) != 75 || atoi(fields[9]) != 75)
                   return;
                }
           }

	/* get the family pointer */
	fp = DrvGetFP(fields[1], ynScalable);
	if (fp == NULL) {
           LOGSTR((LF_CONSOLE,"Ignoring font %s\n", s));
	   DrvTrapHookFontError(0,3);
           return;
        }

	/* build which index into family we have ... */
	if (strcmp("medium",fields[2]) == 0)
		bold = 0;
	else if (strcmp("bold",fields[2]) == 0)
		bold = 1;
	else if (strcmp("demibold",fields[2]) == 0)
		bold = 1;
	else bold = 0;
		
	if (strcmp("i",fields[3]) == 0)
		italics = 1;
	else if (strcmp("o",fields[3]) == 0)
		italics = 1;
	else if (strcmp("r",fields[3]) == 0)
		italics = 0;
	else italics = 0;
	
	/* now put into a style specific list... */
	nStyle  = italics? 1:0;
	nStyle |= bold?    2:0;

	/* now create this font metric definitition */
	fm = (DRVFONTDATA *) WINMALLOC(sizeof(DRVFONTDATA),7,0);

	/* initialize the structure */
	memset(fm,0,sizeof(DRVFONTDATA));

	fm->fmnext   = 0;
	fm->fmheight = atoi(fields[5]);
	fm->fmstring = (char *) WinSaveString(s);
    	fm->uiFontType = RASTER_FONTTYPE;
    	fm->uiFontType = ynScalable ? TRUETYPE_FONTTYPE : RASTER_FONTTYPE;

	/* assembly the fontstring required to get scalable versions later */
	if(fm->fmheight == 0) {
		sprintf(fontstring,"-%s-%s-%s-%s-%s--%%d-%%d-%s-%s-%s-%s-%s-%s",
			fields[0],
			fields[1],
			fields[2],
			fields[3],
			fields[4],

			fields[8],
			fields[9],
			fields[10],
			fields[11],
			fields[12],
			fields[13]);
		fs = fontstring;
	} else  fs = 0;

	/* check first to see if we have a null list */
	if(fp->fontinfo[nStyle] == 0) {
		fp->fontinfo[nStyle] = fm;
	} else  {

		/* lets sort in increasing order of height */
		fml = fp->fontinfo[nStyle];

		/* is it less than the first one in the list */
		if(fm->fmheight < fml->fmheight) {
			fm->fmnext = fml;
			fp->fontinfo[nStyle] = fm;
		} else {

			/* fm needs to be inserted into font list */
		        for(fml = fp->fontinfo[nStyle];fml;fml = fml->fmnext) {

				/* we already have this one? */
				if(fm->fmheight == fml->fmheight) {
					WinFree((LPSTR)fm);
					fm = 0;
					break;
				}

				/* do we have a next one... */
				fmn = fml->fmnext;
				if(fmn) {
					if(fm->fmheight < fmn->fmheight) {
						fm->fmnext = fmn;
						fml->fmnext = fm;
						break;
					}
					continue;
				} else {
					fml->fmnext = fm;
					break;
				}
			}
		}
	}

	/* build scalable version */
	/* the 0 case says to LoadFont that this is scalable... */
	if(fm && fm->fmheight == 0) {
		
		/* this is the string to generate size N */
		fm->format = (char *) WinSaveString(fs);
	}
}

/******************************************************************/
static DWORD
DrvFreeFont(LPDRIVERDC lpddc, LPDRVFONTDATA lpMagic)
{
   	return 0;
}

static int FontError;

int
DrvTrapHookFontError(int opcode,int code)
{
	if(opcode)
		FontError = code;	
	else	FontError = 0;
	return code;
}

/******************************************************************
**
**   int DrvGetFontCacheSize(LPSTR lpszFamily)
**
**   Returns number of LOGFONTs for the given lpszFamily. The
**   calling function then can allocate count * sizeof LOGFONT
**   font cache and call DrvBuildFontCache to fill it out.
**
******************************************************************/
static DWORD DrvGetFontCacheSize(LPSTR lpszFamily)
{
   int          i;
   char         szAlias[256] = "";
   DWORD        dwCount = 0L;
   DRVFONTDATA *lpdata;
   DPYFONTINFO *lpinfo;
   /*
   **   Try to find an alias for the given font name.
   **   When lpszFamily is empty, count just one style of
   **   all fonts (because EnumFonts with empty family
   **   expects just font name).
   */
   if (lpszFamily && *lpszFamily)
      {
      strcpy(szAlias, WinFontAlias(lpszFamily));
      /*
      **   How many LOGFONTs exist for the given family?
      */
      for (lpinfo = systemfont.fontinfo;  lpinfo;  lpinfo = lpinfo->fontnext)
          {
          if (strcasecmp(szAlias, lpinfo->fontface) == 0)
             {
             for (i = 0;  i < 4;  i++)
                 {
                 if (lpinfo->fontinfo[i])
                    {
                    for (lpdata = lpinfo->fontinfo[i];  lpdata;  lpdata = lpdata->fmnext)
                        {
                        dwCount++;
                        if (lpdata->fmheight == 0)
                           break;
                        }
                    if (lpinfo->fontinfo[i]->fmheight != 0)
                       break;
                    }
                 }
             }
          }
      return dwCount;
      }
   /*
   **   How many different X font families exist at ALL, including aliases?
   */
   for (lpinfo = systemfont.fontinfo;  lpinfo;  lpinfo = lpinfo->fontnext)
       {
       for (i = 0;  i < 4;  i++)
           {
           if (lpinfo->fontinfo[i])
              {
              dwCount++;
              if (lpinfo->uiAliasNb == -1)
                  lpinfo->uiAliasNb = DrvCountXAlias(lpinfo->fontface);
              dwCount += lpinfo->uiAliasNb;
              break;
              }
           }
       }
   return dwCount;
}

/******************************************************************
**
**   LPLOGFONT DrvBuildLogfontCache(LPNEWLOGFONT lplfCache, LPNEWTEXTMETRIC lpntmCache, LPSTR lpszFamily, int nCount)
**
**   Stores LOGFONT info for each font in the given lpszFamily. The
**   memory block, pointed by lpLogFont should already be allocated
**   by calling function (it must call DrvGetFontCacheSize to determine,
**   how many LOGFONTs have been requested).
**
**   WE ASSUME, THAT FONTS SUBSYSTEM HAS BEEN INITIALIZED
**   BY DrvInitFonts(), i.e. fontinfo != NULL
**
******************************************************************/
static DWORD DrvBuildFontCache(LPNEWLOGFONT lplfCache,
	LPNEWTEXTMETRIC lpntmCache, LPSTR lpszFamily, DWORD dwCount)
{
   int             i;
   char            szAlias[256] = ""; /* Unix font alias for the given Win font family */
   char            szXParam[64];      /* X font parameter */
   DPYFONTINFO    *lpinfo;
   DRVFONTDATA    *lpdata;
   NEWLOGFONT     *lplf = lplfCache;
   NEWTEXTMETRIC  *lpntm = lpntmCache;
   DWORD           dwTotal = 0L;

   if (lplfCache == NULL || lpntmCache == NULL || dwCount == 0)
      return 0L;

   /*
   **   Try to find an alias for the given font name.
   */
   if (lpszFamily && *lpszFamily)
      strcpy(szAlias, WinFontAlias(lpszFamily));
   /*
   **   Now initialize NEWLOGFONT cache.
   */
   lplf = lplfCache;  lpntm = lpntmCache;
   for (lpinfo = systemfont.fontinfo;  lpinfo;  lpinfo = lpinfo->fontnext)
       {
       /*
       **  If lpszFamily != NULL we build the cache for this family only
       **  (include possible regular, bold, italic and bold-italic styles).
       **  Otherwise we're requested to put one style of ALL FAMILIES.
       */
       if (lpszFamily && *lpszFamily)
          if (strcasecmp(szAlias, lpinfo->fontface) != 0)
             continue; /* skip non-matching family */

       for (i = 0;  i < 4;  i++)
          if (lpinfo->fontinfo[i])
             {
             for (lpdata = lpinfo->fontinfo[i];  lpdata;  lpdata = lpdata->fmnext)
                 {
                 /*   Initialize NEWLOGFONT pointed by lplf.
                 */
                 lplf->lf.lfHeight = lpdata->fmheight;

                 ParseXFont(lpdata->fmstring, szXParam, 12, sizeof(szXParam));
                 lplf->lf.lfWidth = atoi(szXParam) / 10;

                 lplf->lf.lfEscapement = 0;          
                 lplf->lf.lfOrientation = 0;          
                 /*
                 **   The DRVFONTDATA array stores the font as follows:
                 **       [0] - regular;
                 **       [1] - italic;
                 **       [2] - bold;
                 **       [3] - italic bold;   (see DrvParseFont above)
                 */
                 lplf->lf.lfWeight = (i < 2 ? FW_REGULAR : FW_BOLD);       
                 lplf->lf.lfItalic = i % 2;                 
                 lplf->lf.lfUnderline = 0;               
                 lplf->lf.lfStrikeOut = 0;                

                 ParseXFont(lpdata->fmstring, szXParam, 13, sizeof(szXParam));
                 lplf->lf.lfCharSet = (strcmp(szXParam, "iso8859") == 0 ? ANSI_CHARSET : OEM_CHARSET);

                 lplf->lf.lfOutPrecision = OUT_DEFAULT_PRECIS;  
                 lplf->lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
                 lplf->lf.lfQuality = DEFAULT_QUALITY;          

                 if (lplf->lf.lfHeight == 0)
                    lplf->lf.lfPitchAndFamily = VARIABLE_PITCH; 
                 else lplf->lf.lfPitchAndFamily = FIXED_PITCH;

                 ParseXFont(lpdata->fmstring, szXParam, 11, sizeof(szXParam));

                 if (szXParam[0] == 'p')
                    lplf->lf.lfPitchAndFamily |= VARIABLE_PITCH;
                 else { 
			if (szXParam[0] == 'm')
                         lplf->lf.lfPitchAndFamily |= FIXED_PITCH;
                      	else lplf->lf.lfPitchAndFamily |= FF_DONTCARE;
		 }

                 if (lpszFamily)
                    strcpy(lplf->lf.lfFaceName, lpszFamily);
                 else strcpy(lplf->lf.lfFaceName, XFontAlias(lpinfo->fontface));

#ifdef SEVERE
        LOGSTR((LF_LOG,"BuildFontCache Metrics [%x] '%s': height '%d' width '%d' weight '%d' italic '%d' charset '0x%02X' pitch '0x%02X'\n",
	i,
	lplf->lf.lfFaceName,
	lplf->lf.lfHeight,
        lplf->lf.lfWidth,
	lplf->lf.lfWeight,
	lplf->lf.lfItalic,
        lplf->lf.lfCharSet,
        lplf->lf.lfPitchAndFamily));
#endif
                 /*
                 **   2 more field in NEWLOGFONT structure (for scalable and TT fonts only).
                 */
                 if (lplf->lf.lfWeight == FW_BOLD)
                    {
                    strcpy( (LPSTR)lplf->lfStyle, "Bold");
                    if (lplf->lf.lfItalic)
                       strcat( (LPSTR)lplf->lfStyle, " Italic");
                    }
                 else if (lplf->lf.lfItalic)
                         strcpy( (LPSTR)lplf->lfStyle, "Italic");
                      else strcpy( (LPSTR)lplf->lfStyle, "Regular");
                 strcpy( (LPSTR)lplf->lfFullName,  (LPSTR)lplf->lf.lfFaceName);
                 strcat( (LPSTR)lplf->lfFullName, " ");
                 strcat( (LPSTR)lplf->lfFullName,  (LPSTR)lplf->lfStyle);

                 BuildMetrics((LPLOGFONT)lplf, lpdata, lpntm); /* stores NEWTEXTMETRIC into memory block, specified by lpntm */
                 dwTotal++;  lplf++;  lpntm++; /* point to next slot in cache */
                 /*
                 **   If no family name, the only one font style is enough.
                 */
                 if (lpszFamily == NULL || *lpszFamily == '\0' || lpinfo->fontinfo[i]->fmheight == 0) 
                    break; /* select next font */
                 }
             if (lpszFamily == NULL || *lpszFamily == '\0' || lpinfo->fontinfo[i]->fmheight != 0)
                 break;
             } /* for i = 0..4 loop */
       } 
   return dwTotal;
}

/******************************************************************
**
**   static int ParseXFont(LPSTR lpszXFont, LPSTR lpszRet, int nParamNo, int nMax)
**
**   Parses nParam parameter of lpszXFont and stores the result
**   in lpszRet. Returns the length of lpszRet.
**
*****************************************************************/
static int ParseXFont(LPSTR lpszXFont, LPSTR lpszRet, int nParam, int nMax)
{
   int    i;
   LPSTR  lpsz;

   if (lpszXFont == NULL || lpszRet == NULL)
      return 0;
   /*
   **   Count '-' characters up to nParam.
   */
   for (i = 0, lpsz = lpszXFont;  *lpsz && i != nParam;  lpsz++)
       if (*lpsz == '-')
          i++;

   if (i == nParam)
      {
      if (*lpsz == '-')
         lpsz++;
      for (i = 0;  *lpsz && *lpsz != '-' && i < nMax;  i++)
          *(lpszRet + i) = *lpsz++;
      *(lpszRet + i) = '\0';
      return i;
      }
   else {
        lpszRet[0] = '\0';
        return 0;
        }
}


/******************************************************************
**
**   static LPSTR XFontAlias(LPSTR lpszXFont)
**
**   returns Windows equivalentn for the given X font (if any)
**
*******************************************************************/
static LPSTR XFontAlias(LPSTR lpszXFont)
{
   FONTALIAS  *lpfa;
   char *lpszfont = lpszXFont;

   if (systemfont.fontalias)
      for (lpfa = systemfont.fontalias;  lpfa->oldname && lpfa->newname; lpfa++) 
          if (strnicmp(lpszXFont, lpfa->newname, strlen(lpfa->newname)) == 0) {
	       lpszfont = lpfa->oldname;
	       break;
          //   return lpfa->oldname;
	  }

   if(stricmp(lpszXFont,lpszfont)) {
	LOGSTR((LF_LOG,"aliasing %s to %s\n",lpszXFont,lpszfont));
   }
   return lpszfont;
}

/******************************************************************
**
**   static LPSTR WinFontAlias(LPSTR lpszWinFont)
**
**   returns X equivalent for the given Windows font (if any)
**
*******************************************************************/
static LPSTR WinFontAlias(LPSTR lpszWinFont)
{
   FONTALIAS  *lpfa;

   if (systemfont.fontalias)
      for (lpfa = systemfont.fontalias;  lpfa->oldname && lpfa->newname; lpfa++) 
          if (strnicmp(lpszWinFont, lpfa->oldname, strlen(lpfa->oldname)) == 0)
             return lpfa->newname;

   return lpszWinFont;
}


/******************************************************************
**
**   BuildMetrics(LPLOGFONT lplf, DRVFONTDATA lpdata, LPNEWTEXTMETRIC lpNTM)
**
**   Fill in lpNTM based on lplf and lpdata. Basically it's the same
**   code as in DrvGetTextMetrics, so all modifications should reflect
**   in both places.
**
*******************************************************************/
static void
BuildMetrics(LPLOGFONT lplf, LPDRVFONTDATA lpdata, LPNEWTEXTMETRIC lpNTM)
{
    XFontStruct *xfs;
    int direction,ascent,descent,n;
    XCharStruct overall;
    char *alphabet;
    PRIVATEDISPLAY   *dp = GETDP();

    if (lpdata->Font == (Font)0) {
    	  LOCK_SECTION( _MTLOCK_VIDEO);
          lpdata->Font = XLoadFont(dp->display, lpdata->fmstring); 
    	  UNLOCK_SECTION( _MTLOCK_VIDEO);
	  LOGSTR((LF_LOG,"Load Font: %s\n",lpdata->fmstring));
    }

    if (lpdata->Font && lpdata->finfo == NULL) {
       	LOCK_SECTION( _MTLOCK_VIDEO);
       	lpdata->finfo = XQueryFont(dp->display, lpdata->Font);
       	UNLOCK_SECTION( _MTLOCK_VIDEO);
    }

    if (lpdata->Font == 0 || lpdata->finfo == 0) {
        LOGSTR((LF_ERROR,"Font not present: '%s'\n", lpdata->fmstring));
	return;
     }

    if ( !(xfs = lpdata->finfo) )
	return;

    lpNTM->tmAscent = xfs->ascent;
    lpNTM->tmDescent = xfs->descent;
    lpNTM->tmHeight = lpNTM->tmAscent + lpNTM->tmDescent;

    lpNTM->tmInternalLeading = abs(lplf->lfHeight / 5);
    lpNTM->tmExternalLeading = lpNTM->tmInternalLeading / 4;
    lpNTM->tmFirstChar = xfs->min_char_or_byte2;
    lpNTM->tmLastChar = xfs->max_char_or_byte2;

    if (xfs->max_bounds.width)
	/* maximal width available */
	lpNTM->tmMaxCharWidth = xfs->max_bounds.width;
    else {
	for (n = 0, lpNTM->tmMaxCharWidth = 0;
	     n <= (int)(lpNTM->tmLastChar - lpNTM->tmFirstChar);
	     n++) {
                  if (xfs->per_char)
	             if (lpNTM->tmMaxCharWidth < xfs->per_char[n].width)
		        lpNTM->tmMaxCharWidth = xfs->per_char[n].width;
	}
    }
    lpNTM->tmDefaultChar = xfs->default_char;
    if ( lpNTM->tmDefaultChar < lpNTM->tmFirstChar ||
        lpNTM->tmDefaultChar > lpNTM->tmLastChar )
        lpNTM->tmDefaultChar = lpNTM->tmFirstChar;

    lpNTM->tmWeight = lplf->lfWeight;	/* temporarily */
    lpNTM->tmItalic = lplf->lfItalic;
    lpNTM->tmUnderlined = lplf->lfUnderline;
    lpNTM->tmStruckOut = lplf->lfStrikeOut;

    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    LOCK_SECTION( _MTLOCK_VIDEO);
    XTextExtents(xfs,alphabet,52,
		&direction,&ascent,&descent,&overall);
    UNLOCK_SECTION( _MTLOCK_VIDEO);

    /* round up .333 to 1 */
    lpNTM->tmAveCharWidth = (overall.width+36)/52;
    lpNTM->tmBreakChar = ' ';
    lpNTM->tmPitchAndFamily = lplf->lfPitchAndFamily;
    lpNTM->tmCharSet = lplf->lfCharSet;
    lpNTM->tmOverhang = 0;
    lpNTM->tmDigitizedAspectX = lpNTM->tmAveCharWidth;
    lpNTM->tmDigitizedAspectY = 15;
    lpNTM->ntmFlags = 0;
    lpNTM->ntmSizeEM = 0;
    lpNTM->ntmCellHeight = 0;
    lpNTM->ntmAvgWidth = 0;
}


/**************************************************************************/
static DWORD
DrvGetTextFace(LPDRIVERDC lpddc, LPLSDS_PARAMS lpStruct)
{
    LSDE_TEXT *lptxt = &lpStruct->lsde.text;
    LPLOGFONT lplf = &lpddc->lpFontData->LogFont;

    if (lpStruct->lsde_validate.dwInvalidMask)
	DrvValidate(lpddc,lpStruct);

    strcpy(lptxt->lpStr, lplf->lfFaceName);

#ifdef SEVERE
LOGSTR((LF_LOG,"GetTextFace: return '%s', length '%d'\n", lptxt->lpStr, strlen(lptxt->lpStr)));
#endif
    return (DWORD)strlen(lptxt->lpStr);
}

/******************************************************************/
static DWORD
DrvTextRescale(LPARAM lp, LPARAM dwParam, LPVOID lpVoid)
{
	LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpVoid;
	LPDRIVERDC lpddc = (LPDRIVERDC)lp;
	DWORD dwReturn;

        dwReturn = (DWORD) DrvRealizeFont(lpddc, &lpStruct->lsde.realizefont
, dwParam, 1);
        return (DWORD) dwReturn;
}

static DWORD
DrvTextRealize(LPARAM lp, LPARAM dwParam, LPVOID lpVoid)
{
	LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpVoid;
	LPDRIVERDC lpddc = (LPDRIVERDC)lp;
	DWORD dwReturn;

        dwReturn = (DWORD)DrvRealizeFont(lpddc, &lpStruct->lsde.realizefont,
 dwParam, 0);
        return (DWORD) dwReturn;
}

static DWORD
DrvTextGetMetrics(LPARAM lp, LPARAM dwParam, LPVOID lpVoid)
{
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpVoid;
        LPDRIVERDC lpddc = (LPDRIVERDC)lp;
        DWORD dwReturn;

	dwReturn = DrvGetTextMetrics(lpddc,
			(LPNEWTEXTMETRIC)lpStruct->lsde.lpmetric);
	return dwReturn;
}

static DWORD
DrvTextGetCharWidth(LPARAM lp, LPARAM dwParam, LPVOID lpVoid)
{
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpVoid;
        LPDRIVERDC lpddc = (LPDRIVERDC)lp;
        DWORD dwReturn;

	dwReturn = DrvGetCharWidth(lpddc, lpStruct->lsde.text.lpDX,dwParam);
	return dwReturn;
}

static DWORD
DrvTextExtTextOut(LPARAM lp, LPARAM dwParam, LPVOID lpVoid)
{
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpVoid;
        LPDRIVERDC lpddc = (LPDRIVERDC)lp;
        DWORD dwReturn;

	dwReturn = DrvExtTextOut(lpddc, lpStruct, (BOOL)dwParam);
	return dwReturn;
}

static DWORD
DrvTextGetTextFace(LPARAM lp, LPARAM dwParam, LPVOID lpVoid)
{
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpVoid;
        LPDRIVERDC lpddc = (LPDRIVERDC)lp;
        DWORD dwReturn;

        dwReturn = DrvGetTextFace(lpddc,lpStruct);
        return dwReturn;
}

static DWORD
DrvTextGetEnumCnt(LPARAM lp, LPARAM dwParam, LPVOID lpVoid)
{
        DWORD dwReturn;

        dwReturn = DrvGetFontCacheSize((LPSTR)lpVoid); /* family name */
        return dwReturn;
}

static DWORD
DrvTextEnumFonts(LPARAM lp, LPARAM dwParam, LPVOID lpVoid)
{
        LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpVoid;
        DWORD dwReturn;

	dwReturn = DrvBuildFontCache(
		(LPNEWLOGFONT)lpStruct->lsde.enumfonts.lplfCache,
                (LPNEWTEXTMETRIC)lpStruct->lsde.enumfonts.lpntmCache,
                lpStruct->lsde.enumfonts.lpszFamily,
                lpStruct->lsde.enumfonts.dwCacheSize);
        return dwReturn;
}

static DWORD
DrvTextDeleteFont(LPARAM lp, LPARAM dwParam, LPVOID lpVoid)
{
        LPDRIVERDC lpddc = (LPDRIVERDC)lp;
        DWORD dwReturn;

	dwReturn = DrvFreeFont(lpddc, (LPDRVFONTDATA)lpVoid);
	return dwReturn;
}

static LPDRVFONTDATA
DrvRealizeFont(LPDRIVERDC lpddc, LSDE_REALIZEFONT *lpRealizeFont, DWORD dwMapperFlags, BOOL ynRescale)
{
	DPYFONTINFO 	*fplist;

	DRVFONTDATA	*wfm,		/* working font entry   */
			*sfm = 0,	/* scalable font entry  */
			*cfm = 0,	/* candidate font entry */
			*pfm = 0,	/* previous font entry  */
			*bfm = 0;	/* backup font entry    */

	char	        facealias[256];
	int		nStyle;
	int		found,
			height, 	/* height we want 	*/
			minsize = 0, 	/* minimum acceptable 	*/
			maxsize = 0;	/* maximum acceptable 	*/

        LPDRVFONTDATA   lpMagic = (LPDRVFONTDATA)lpRealizeFont->lpMagic;
	static DRVFONTDATA *dfm;	/* one that works       */

	/*******************************************************/
	/* start by making sure that we have initialized fonts */
	/*******************************************************/

	if (systemfont.fontinfo == 0)
	   DrvInitFonts(lpddc);

	/********************************************************/
	/* did we get a pointer to last realized font 		*/
	/********************************************************/

	if (lpMagic) {

	        if (abs(lpRealizeFont->LogFont.lfHeight) == 
		    	abs(lpMagic->fmheight)){

    		   LOCK_SECTION( _MTLOCK_VIDEO);
	      	   XSetFont(lpddc->dp->display, lpddc->gc, lpMagic->Font);
    		   UNLOCK_SECTION( _MTLOCK_VIDEO);

              	   lpddc->lpFontData = lpMagic;

		   /* verify that we have what we want */
		   /* force test with results print */	
	           return lpMagic;
              }
	} 

	/*************************************************************/
	/* we either did not have previous font, or not close enough */
	/* use logical font info to set nStyle index into fm array   */
	/*************************************************************/

	nStyle  = (lpRealizeFont->LogFont.lfItalic      )? 1:0;
	nStyle |= (lpRealizeFont->LogFont.lfWeight > 400)? 2:0;

	/********************************************************/
	/* adjust the height of the font we get... 		*/
	/********************************************************/

	height = lpRealizeFont->LogFont.lfHeight;

        if (height < 0)
           height = abs(height);
        else if (height == 0)
                height = systemfont.fontsize;
             else height -= (height / 5);

	/* get the alias for the font */
    	DrvFontAlias((LPLOGFONT)&lpRealizeFont->LogFont, facealias, 
		&minsize, &maxsize);

	/********************************************************/
	/* search the font family list, 			*/
	/* 	first time for desired,				*/
	/* 	second time for system 				*/
	/********************************************************/

	found = 0;
	for (;found <= 0;) {
		for (fplist = systemfont.fontinfo; fplist; fplist = fplist->fontnext) {
	    		if (strcasecmp(facealias, fplist->fontface) == 0) {
				found = 1;
	       			break;
	    		}
		}
		if (found > 0)
		   break;

		if (strcasecmp(facealias,systemfont.fontface) == 0) {
			if(lpMagic) {
              	  		lpddc->lpFontData = lpMagic;
    				LOCK_SECTION( _MTLOCK_VIDEO);
				XSetFont(lpddc->dp->display, 
					lpddc->gc, lpMagic->Font);
    				UNLOCK_SECTION( _MTLOCK_VIDEO);
				return lpMagic;
			}
			if(dfm) {
              	  		lpddc->lpFontData = dfm;
    				LOCK_SECTION( _MTLOCK_VIDEO);
				XSetFont(lpddc->dp->display, 
					lpddc->gc, dfm->Font);
    				UNLOCK_SECTION( _MTLOCK_VIDEO);
				return dfm;
			}
			LOGSTR((LF_ERROR,"no system font\n"));
		}
	
		if(found < 0)
			break;
		/* did not find desired font, so 	*/
		/* try again, using system font 	*/
		strcpy(facealias,systemfont.fontface);
		found = -1;
	}

	if(found < 0)
		return 0;

	/********************************************************/
	/* we know which font we want, now find height 		*/
	/********************************************************/

	if(fplist->fontinfo[nStyle] == 0) {
		if(fplist->fontinfo[0])
			nStyle = 0;
		else if(fplist->fontinfo[2])
			nStyle = 2;
		     else if(fplist->fontinfo[1])
				nStyle = 1;
		           if(fplist->fontinfo[3])
				nStyle = 3;
	}

	/****************************************/
	/* fplist points to correct family 	*/
	/* found bits tell what we found out    */
	/* found |= 1	font is scalable        */
	/* found |= 2	font has previous entry */
	/* found |= 4	found the one we want   */
	/****************************************/
	found = 0;
	for (wfm = fplist->fontinfo[nStyle]; wfm;  wfm = wfm->fmnext) {

		/* is the font scalable */
		if (wfm->fmheight == 0) {
		   found = 1;
		   sfm   = wfm;
		   continue;
		}
			
		/* is this one smaller than desired */
	       	if (wfm->fmheight < height) {
		   found |= 2;
		   pfm    = wfm;
		   continue;
                }
		
		/* found the right height */
		if (wfm->fmheight == height) {
		   found |= 4;
		   cfm    = wfm;
		   break; 
		}

		/* found one bigger than wanted */
		if (wfm->fmheight > height) {
		   bfm 	= wfm;
		   break; 
		}
        }

	/********************************************************/
	/*	Using the results of the search, load the 	*/
	/*	font that matches best				*/
	/********************************************************/

	switch (found) {
	/* not scalable   */
	/* no previous entry */
	/* no match*/
	case 0:
		/* did we find one bigger? */
		/* bfm is closest one, that is bigger */
		/* BUT might be 0 */
		cfm = bfm;

		/* none found in this family, does default exist */
		/* see if we have any font at all... */
		/* dfm is the catch all */
		if (cfm == 0)
		   cfm = dfm;
		break;

	/* scalable   */
	/* no previous entry */
	/* no match*/
	case 1:
		/* sfm holds largest one less than desired height */
		cfm = sfm;
		break;

	/* not scalable   */
	/* previous entry */
	/* no match*/
	case 2:
		/* found a smaller entry */
		cfm = pfm;
		break;

	/* scalable   */
	/* previous entry */
	/* no match*/
	case 3:
		/* found a smaller entry */
		cfm = pfm;
		break;

	/* found match */
	case 4: 	/* not scalable, no previous */
	case 5:		/*     scalable, no previous */
	case 6:		/* not scalable,    previous */  
	case 7:		/*     scalable,    previous */
		break;
	}

	if ( !cfm )
		return 0;

        lpddc->lpFontData = cfm;

	/********************************************************/
	/* load the font, if we haven't yet 			*/
	/********************************************************/

	if (cfm->Font == 0) {
    	   LOCK_SECTION( _MTLOCK_VIDEO);
           cfm->Font = XLoadFont(lpddc->dp->display, cfm->fmstring);
    	   UNLOCK_SECTION( _MTLOCK_VIDEO);
	   LOGSTR((LF_LOG,"Load Font: %s\n",cfm->fmstring));
	}

	/********************************************************/
	/* load the fontinfo, if we haven't yet 		*/
	/********************************************************/

	if (cfm->Font && cfm->finfo == 0) {
    	   LOCK_SECTION( _MTLOCK_VIDEO);
	   cfm->finfo = XQueryFont(lpddc->dp->display,cfm->Font);
    	   UNLOCK_SECTION( _MTLOCK_VIDEO);
	}

	if ( cfm->Font == 0 || cfm->finfo == 0) {
           LOGSTR((LF_CONSOLE,"ERROR: Font not available: '%s'\n", cfm->fmstring));
	}

	/********************************************************/
	/* what about truetype vs. rasterfont 			*/
	/********************************************************/

    	cfm->uiFontType = RASTER_FONTTYPE;
        cfm->uiFontType = TRUETYPE_FONTTYPE;

	/********************************************************/ 
	/* point back to the family list 			*/ 
	/********************************************************/

	cfm->fp         = fplist;

	/********************************************************/
	/* save the logfont that we matched 			*/
	/********************************************************/

    	memcpy((void *)&cfm->LogFont, 
		(void *)&lpRealizeFont->LogFont, sizeof(LOGFONT));

	/********************************************************/
	/* set the font 					*/
	/* now the x server knows which font we want...		*/
	/********************************************************/

	if(cfm->Font) {
    		LOCK_SECTION( _MTLOCK_VIDEO);
		XSetFont(lpddc->dp->display, lpddc->gc, cfm->Font);
    		UNLOCK_SECTION( _MTLOCK_VIDEO);
	}

	/* Save as a default, (ie. the system font) */
	/* to be used on unmatching fonts later */
	if(dfm == 0)
		dfm = cfm;

	return cfm;
}

/*
 * 
 */

DWORD
DrvTextInit(LPARAM uiCompat, LPARAM binit, LPVOID lpStruct)
{
    DPYFONTINFO *fp,*fpnext;
    DRVFONTDATA *fml,*fmnext;
    PRIVATEDISPLAY *dp = GETDP();

    if (binit) {
	DrvInitFontAliases();
	return 0;
    } else {
	/* free the alias table */
	WinFree(systemfont.fontalias);	

	/* free the fontinfo structures */
	fpnext = 0;
	for (fp = systemfont.fontinfo;fp;fp = fpnext) {
		fpnext = fp->fontnext;

		fmnext = 0;
		for(fml = fp->fontinfo[0];fml;fml = fmnext) {
			fmnext = fml->fmnext;
			if (fml->finfo)
				XFreeFont(dp->display, fml->finfo);
			WinFree(fml);
		}
		fmnext = 0;
		if(fp->fontinfo[0] != fp->fontinfo[1])
		for(fml = fp->fontinfo[1];fml;fml = fmnext) {
			fmnext = fml->fmnext;
			if (fml->finfo)
				XFreeFont(dp->display, fml->finfo);
			WinFree(fml);
		}
		fmnext = 0;
		if(fp->fontinfo[1] != fp->fontinfo[2])
		for(fml = fp->fontinfo[2];fml;fml = fmnext) {
			fmnext = fml->fmnext;
			if (fml->finfo)
				XFreeFont(dp->display, fml->finfo);
			WinFree(fml);
		}
		fmnext = 0;
		if(fp->fontinfo[2] != fp->fontinfo[3])
		for(fml = fp->fontinfo[3];fml;fml = fmnext) {
			fmnext = fml->fmnext;
			if (fml->finfo)
				XFreeFont(dp->display, fml->finfo);
			WinFree(fml);
		}

		WinFree(fp);
	}
    }
    return 0L;

}

static DWORD
DrvTextDoNothing(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
	return 0L;
}

DWORD
DrvTextTab(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
	return (DWORD)DrvTextEntryTab;
}

static DWORD
DrvTextGetGlyphOutline(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpVoid)
{
	LPLSDS_PARAMS lpStruct = (LPLSDS_PARAMS)lpVoid;

	if (lpStruct->lsde.getglyphoutline.cbBuffer == 0 
		|| lpStruct->lsde.getglyphoutline.lpvBuffer == (void *)NULL) 
	{
		return (DWORD)(16+20);
		/* 1 TTPOLYHEADER & 1 TTPOLYCURVE with 2 pts */
	}	
    return (DWORD)-1;
}
