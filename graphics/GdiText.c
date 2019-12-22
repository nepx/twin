/*    
	GdiText.c	2.30
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

#include "Log.h"
#include "GdiDC.h"
#include "GdiText.h"
#include "DeviceData.h"
#include "Driver.h"

static LONG TabbedTextOutExt(HDC,int,int,LPCSTR,int,int,int *,int,BOOL);
static int FindNextTab(HDC,int,int,int,int *,int);
static int GetCharAveWidth(HDC);

LPSTR GdiDumpString(LPSTR,int);

BOOL GdiSelectFont(HDC32, HFONT);

BOOL WINAPI
CreateScalableFontResource(UINT dwHidden, LPCSTR lpszFontRes, LPCSTR lpszFontFile, LPCSTR lpszPath)
{
   return 0;
}

BOOL GetCharABCWidthsFloat(
    HDC hdc,	// handle of device context 
    UINT iFirstChar,	// first character in range to query 
    UINT iLastChar,	// last character in range to query 
    LPABCFLOAT lpABCF 	// address of character-width structure 
   )
{
    APISTR((LF_APISTUB,"GetCharABCWidthsFloat(HDC=%x,UINT=%x,UINT=%x,int *%p)\n",
	hdc,iFirstChar,iLastChar,lpABCF));
    return FALSE;
}

BOOL GetCharWidthFloat(
    HDC hdc,	// handle of device context 
    UINT iFirstChar,	// first-character code point 
    UINT iLastChar,	// last-character code point 
    PFLOAT pxBuffer 	// address of buffer that receives width-values 
   )
{
    APISTR((LF_APISTUB,"GetCharWidthFloat(HDC=%x,UINT=%x,UINT=%x,int *%p)\n",
	hdc,iFirstChar,iLastChar,pxBuffer));
    return FALSE;
}

HFONT
GdiCreateFont(LPLOGFONT lpLogFont)
{
    HFONT hFont;
    HFONT32 hFont32;

    if (!(hFont32 = CREATEHFONT(hFont))) {
	return (HFONT)0;
    }

    hFont32->LogFont = *lpLogFont;
    RELEASEFONTINFO(hFont32);
    return hFont;
}

BOOL
GdiDeleteFont(HFONT hFont)
{
    HFONT32 hFont32;

    ASSERT_HFONT(hFont32,hFont,FALSE);
    /*
    **   THE FOLLOWING IS FOR SCREEN FONTS ONLY - WE HAVEN'T DECIDED
    **   WHAT TO DO WITH PRINTER FONTS YET... (Lana)
    **
    **   Also, we pass 0L instead of hDC32->lpDrvData, so the driver
    **   must call GETDP() to retrieve display pointer.
    */
    if (hFont32->lpCachedFont)
       {
       /* first word is uiFontType */
       if (*hFont32->lpCachedFont != RASTER_FONTTYPE) 
          {
          DRVCALL_TEXT(PTH_DELETEFONT, 0L, 0, hFont32->lpCachedFont);
          hFont32->lpCachedFont = NULL;
          }
       }

    if (hFont32->lpNTM)
    {
	WinFree((LPSTR)hFont32->lpNTM);
	hFont32->lpNTM = 0;
    }
    if (hFont32->lpCharWidths)
    {
	WinFree((LPSTR)hFont32->lpCharWidths);
	hFont32->lpCharWidths = 0;
    }

    RELEASEFONTINFO(hFont32);
    return TRUE;
}

BOOL
GdiSelectFont(HDC32 hDC32, HFONT hFont)
{
    HFONT32 hFont32,hOldFont32;

    ASSERT_HFONT(hFont32,hFont,FALSE);

    if (hDC32->hFont == hFont) {
	/* we are selecting the same hFont, so just rescale it, if needed */
	hDC32->dwInvalid |= IM_FONTSCALEMASK;
    }
    else {
	if (hDC32->hFont) {
	    ASSERT_HFONT(hOldFont32,hDC32->hFont,FALSE);
	    /* let's check whether the new font has an identical LOGFONT */
	    if (memcmp((LPSTR)&hFont32->LogFont,
			(LPSTR)&hOldFont32->LogFont,
			sizeof(LOGFONT)))
		/* the fonts are different; need to realize */
		hDC32->dwInvalid |= IM_FONTMASK;
	    else
		hDC32->dwInvalid |= IM_FONTSCALEMASK;

	    RELEASEFONTINFO(hOldFont32);
	}
	else
	    hDC32->dwInvalid |= IM_FONTMASK;
    }

    RELEASEFONTINFO(hFont32);
    return TRUE;
}

BOOL WINAPI
GetCharWidth(HDC hDC, UINT uFirst, UINT uLast, int *lpnWidths)
{
    HDC32 hDC32;
    BOOL bResult; 
    LSDS_PARAMS argptr;

    APISTR((LF_APICALL,"GetCharWidth(HDC=%x,UINT=%x,UINT=%x,int *%p)\n",
		hDC,uFirst,uLast,lpnWidths));

    ASSERT_HDC(hDC32,hDC,FALSE);

    argptr.lsde_validate.dwInvalidMask = 0;

    /* these two fields are overloaded */
    argptr.lsde.text.x    = (int)uFirst;
    argptr.lsde.text.y    = (int)uLast;
    argptr.lsde.text.lpDX = lpnWidths;
    bResult = (DC_OUTPUT(LSD_GETCHARWIDTH,hDC32,0L,&argptr))?TRUE:FALSE;

    RELEASEDCINFO(hDC32);    

    APISTR((LF_APIRET,"GetCharWidth: returns BOOL %d\n",bResult));
    return bResult;
}

BOOL WINAPI
GetCharABCWidths(HDC hDC, UINT uFirst, UINT uLast, LPABC lpabc)
{
    HDC32 hDC32;
    BOOL bResult;
    LSDS_PARAMS argptr;

    APISTR((LF_API,"GetCharABCWidths(HDC=%x,UINT=%x,UINT=%x,LPABC=%p)\n",
		hDC,uFirst,uLast,lpabc));

    ASSERT_HDC(hDC32,hDC,FALSE);

    argptr.lsde_validate.dwInvalidMask = 0;

    /* these two fields are overloaded */
    argptr.lsde.text.x    = (int)uFirst;
    argptr.lsde.text.y    = (int)uLast;
    argptr.lsde.text.lpDX = (LPINT)lpabc;
    bResult = (DC_OUTPUT(LSD_GETCHARWIDTH,hDC32,1L,&argptr))?TRUE:FALSE;
    RELEASEDCINFO(hDC32);

    APISTR((LF_APIRET,"GetCharABCWidths: returns BOOL %d\n",bResult));
    return bResult;
}

BOOL WINAPI
GetTextMetrics(HDC hDC, LPTEXTMETRIC lpMetrics)
{
    HDC32 hDC32;
    BOOL bResult;
    LSDS_PARAMS argptr;

    APISTR((LF_APICALL,"GetTextMetrics(HDC=%x,LPTEXTMETRIC=%p)\n",hDC,lpMetrics));

    ASSERT_HDC(hDC32,hDC,FALSE);

    argptr.lsde_validate.dwInvalidMask = 0;

    argptr.lsde.lpmetric = lpMetrics;
    bResult = (DC_OUTPUT(LSD_GETTEXTMETRICS,hDC32,0,&argptr))?TRUE:FALSE;
    RELEASEDCINFO(hDC32);

    APISTR((LF_APIRET,"GetTextMetrics: returns BOOL %d\n",bResult));

    return bResult;
}

DWORD WINAPI
GetTextExtent(HDC hDC, LPCSTR lpStr, int nLen)
{
    SIZE sz;

    if (GetTextExtentPoint(hDC,lpStr,nLen,&sz))
	return MAKELONG(sz.cx,sz.cy);
    else
	return 0;
}
#ifdef TWIN32

BOOL WINAPI
GetTextExtentExPoint(HDC hDC, LPCSTR lpStr, int nLen,
                                       int maxExt, LPINT lpnFit,
                                       LPINT alpDx, LPSIZE lpSize )
{
	int n,fit,width;
	LPCSTR lpstr = lpStr;
    	SIZE  lsize;

    	lpSize->cx = 0;
	lpSize->cy = 0;
	fit        = 0;
	width     = 0;

    	APISTR((LF_APICALL,
		"GetTextExtentExPoint(HDC=%x,LPCSTR=%s,int=%d,LPSIZE=%p)\n",
		hDC,GdiDumpString((LPSTR)lpStr,nLen),nLen,lpSize));


    	for(n = 0; n < nLen; n++,lpstr++) {
    		if (GetTextExtentPoint(hDC,lpstr,1,&lsize)  == 0) {
    			APISTR((LF_APIRET,"GetTextExtentExPoint: returns BOOL 0\n"));
			return FALSE;
		}
	
        	if( width+lsize.cx < maxExt ) {
            		width += lsize.cx;
            		fit++;

            		if( alpDx ) 
				alpDx[n] = width;

            		if( lsize.cy > lpSize->cy ) 
				lpSize->cy = lsize.cy;
        	} else 
			break;
    	}
    
	lpSize->cx = width;
   
	*lpnFit = fit;

    	APISTR((LF_APIRET,"GetTextExtentExPoint: returns BOOL %d\n",TRUE));

	return TRUE;
}

#endif

BOOL WINAPI
GetTextExtentPoint(HDC hDC, LPCSTR lpStr, int nLen, LPSIZE lpSize)
{
    HDC32 hDC32;
    int i, j, nWidth, nBreakExtra;
    BYTE ch,tmFirstChar,tmLastChar,tmDefaultChar;
    LPABC lpABC;

    APISTR((LF_APICALL,
	"GetTextExtentPoint(HDC=%x,LPCSTR=%s,int=%d,LPSIZE=%p)\n",
	hDC,GdiDumpString((LPSTR)lpStr,nLen),nLen,lpSize));

    if (!lpSize) {
    	APISTR((LF_APIRET,"GetTextExtentPoint: returns BOOL %d\n",FALSE));
	return FALSE;
    }

    ASSERT_HDC(hDC32,hDC,FALSE);

    /* if the char width array is not in the DC or invalid, force getting it */
    /* this also guarantees that TEXTMETRIC is present in the DC */
    /* hDC32->lpCharWidths is either LPINT or LPABC, and is in device units */
    if (!hDC32->lpCharWidths ||
	(hDC32->dwInvalid & (IM_FONTMASK|IM_FONTSCALEMASK)))
	if (!GetCharWidth(hDC,0,0,&i)) {
	    RELEASEDCINFO(hDC32);
    	    APISTR((LF_APIRET,"GetTextExtentPoint: returns BOOL %d\n",FALSE));
	    return FALSE;
	}

    /* GetTextExtentPoint accounts for intercharacter spacing */
    /* (as in GetTextCharExtra), and is affected by text */
    /* justification. */

    for (i = j = nBreakExtra = nWidth = 0,
		tmDefaultChar = hDC32->lpNTM->tmDefaultChar -
				hDC32->lpNTM->tmFirstChar,
		tmLastChar = hDC32->lpNTM->tmLastChar,
		tmFirstChar = hDC32->lpNTM->tmFirstChar;
	(i < nLen) && (ch = (BYTE)lpStr[i]); i++) 
      {
      if (hDC32->uiFontType != TRUETYPE_FONTTYPE) 
         {
	 if (ch < tmFirstChar || ch > tmLastChar)
	    nWidth += ((LPINT)hDC32->lpCharWidths)[tmDefaultChar];
  	 else nWidth += ((LPINT)hDC32->lpCharWidths)[ch-tmFirstChar];
	 }
      else {
           if (ch < tmFirstChar || ch > tmLastChar) 
              {
	      lpABC = &(((LPABC)hDC32->lpCharWidths)[tmDefaultChar]);
	      nWidth += lpABC->abcA + lpABC->abcB + lpABC->abcC;
	      }
	   else {
		lpABC = &(((LPABC)hDC32->lpCharWidths)[ch-tmFirstChar]);
		nWidth += lpABC->abcA + lpABC->abcB + lpABC->abcC;
	        }
	   }

#ifdef	LATER
      /* test the current selected font for TT_ flag; */
      /* if present, go get kerning pairs for the font from */
      /* the font subsystem and account for them */
#endif
      /*
      ** hDC32->nBreakExtra is set in SetTextJustification (GdiDCAttr.c)
      */
      if (ch == hDC32->lpNTM->tmBreakChar && hDC32->nBreakExtra > 0 && j < hDC32->nBreakCount)
         nBreakExtra += hDC32->lpExtraSpace[j++];
      }
    nWidth += hDC32->nCharExtra * (min(nLen,(int)strlen(lpStr))-1) + nBreakExtra;

    lpSize->cx = nWidth;
    lpSize->cy = hDC32->lpNTM->tmHeight;
    /* finally convert the whole thing to logical coordinates */
    DEtoLE(hDC32,lpSize);

    RELEASEDCINFO(hDC32);

    APISTR((LF_APIRET,"GetTextExtentPoint: returns BOOL %d\n",TRUE));
    return TRUE;
}

BOOL WINAPI
GetTextExtentPoint32(HDC hDC, LPCSTR lpStr, int nLen, LPSIZE lpSize)
{
	return GetTextExtentPoint(hDC,lpStr,nLen,lpSize);
}

int WINAPI
GetTextFace(HDC hDC,int cbBuffer,LPSTR lpszFace)
{
    HDC32 hDC32;
    int nResult;
    LSDS_PARAMS argptr;

    APISTR((LF_APICALL,"GetTextFace(HDC=%x,int=%d,LPSTR=%p)\n",
	hDC,cbBuffer,lpszFace));

    ASSERT_HDC(hDC32,hDC,0);

    argptr.lsde_validate.dwInvalidMask = 0;

    argptr.lsde.text.lpStr = lpszFace;
    argptr.lsde.text.nCnt = cbBuffer;

    nResult = DC_OUTPUT(LSD_GETTEXTFACE,hDC32,cbBuffer,&argptr);
    RELEASEDCINFO(hDC32);
    APISTR((LF_APIRET,"GetTextFace: returns int %d\n",nResult));
    return nResult;
}

DWORD WINAPI
GetGlyphOutline(HDC hDC, UINT uChar, UINT fuFormat,
	GLYPHMETRICS *lpgm, DWORD cbBuffer, void *lpBuffer, const MAT2 *lpmat2)
{
    HDC32 hDC32;
    DWORD nResult;
    LSDS_PARAMS argptr;

    APISTR((LF_APICALL,
	"GetGlyphOutline(HDC=%x,UINT=%x,UINT=%x,GLYPHMETRICS*=%p,DWORD=%x,void *%p,MAT2 *=%p)\n",
	hDC,uChar,fuFormat,lpgm,cbBuffer,lpBuffer,lpmat2));

#ifdef NEVER
    if ( cbBuffer == 0 || lpBuffer == (void *)NULL ) {
        APISTR((LF_APIRET,"GetGlyphOutline: returns DWORD %d\n",16+20));
	return (DWORD)(16+20);	/* 1 TTPOLYHEADER & 1 TTPOLYCURVE with 2 pts */
    }
#endif

    ASSERT_HDC(hDC32,hDC,0);

    argptr.lsde_validate.dwInvalidMask = 0;
    argptr.lsde.getglyphoutline.uChar = uChar;
    argptr.lsde.getglyphoutline.fuFormat = fuFormat;
    argptr.lsde.getglyphoutline.lpgm = lpgm;
    argptr.lsde.getglyphoutline.cbBuffer = cbBuffer;
    argptr.lsde.getglyphoutline.lpvBuffer = lpBuffer;
    argptr.lsde.getglyphoutline.lpmat2 = lpmat2;

    nResult = DC_OUTPUT(LSD_GETGLYPHOUTLINE,hDC32,cbBuffer,&argptr);
    RELEASEDCINFO(hDC32);
    APISTR((LF_APIRET,"GetGlyphOutline: returns DWORD %d\n",nResult));
    return nResult;
}

WORD WINAPI
GetOutlineTextMetrics(HDC hDC, UINT cbData, OUTLINETEXTMETRIC *lpotm)
{
    APISTR((LF_APISTUB,
	"GetOutlineTextMetrics(HDC=%x,UINT=%d,OUTLINETEXTMETRIC *=%p)\n",
	hDC,cbData,lpotm));
    return 0;
}

int WINAPI
GetKerningPairs(HDC hDC, int cPairs, KERNINGPAIR *lpkrnpair)
{
    APISTR((LF_APISTUB,
	"GetKerningPairs(hDC=%x,int=%d,KERNINGPAIR * %x)\n",
	hDC,cPairs,lpkrnpair));
    return 0;
}

BOOL WINAPI
GetRasterizerCaps(RASTERIZER_STATUS *lpraststat, int cb)
{
	APISTR((LF_APISTUB,"GetRasterizerCaps(RASTERIZER_STATS *=%p,int=%d)\n",
		lpraststat,cb));
	lpraststat->nSize = sizeof(RASTERIZER_STATUS);
	lpraststat->wFlags = 0;
	lpraststat->nLanguageID = 0x409;
	return TRUE;
}

BOOL WINAPI
TextOut(HDC hDC, int x, int y, LPCSTR lpStr, int nCnt)
{
    HDC32 hDC32;
    LSDS_PARAMS argptr;
    BOOL rc;

    APISTR((LF_APICALL,"TextOut(HDC=%x,int=%d,int=%d,LPCSTR=%s,int=%d)\n",
	hDC,x,y,GdiDumpString((LPSTR)lpStr,nCnt),nCnt));

    ASSERT_HDC(hDC32,hDC,FALSE);

    argptr.lsde_validate.dwInvalidMask = 0;

    if (!lpStr || nCnt == 0) {
	RELEASEDCINFO(hDC32);
    	APISTR((LF_APIRET,"TextOut: returns BOOL 0 (no data)\n"));
	return FALSE;
    }

    argptr.lsde.text.x       = x;
    argptr.lsde.text.y       = y;
    argptr.lsde.text.lpStr   = (LPSTR)lpStr;
    argptr.lsde.text.nCnt    = nCnt;
    argptr.lsde.text.uiFlags = 0;
    argptr.lsde.text.lpRect  = NULL;
    argptr.lsde.text.lpDX    = NULL;

    rc =  (DC_OUTPUT(LSD_TEXTOUT,hDC32,0,&argptr))?TRUE:FALSE;
    RELEASEDCINFO(hDC32);
    APISTR((LF_APIRET,"TextOut: returns BOOL %d\n",rc));
    return rc;
}

BOOL WINAPI
ExtTextOut(HDC hDC, int x, int y, UINT uiFlags,
	   const RECT *lpRect, LPCSTR lpStr, UINT nCnt, LPINT lpDX)
{
    HDC32 hDC32;
    BOOL bResult;
    LSDS_PARAMS argptr;

    APISTR((LF_APICALL,
"ExtTextOut(HDC=%x,int=%d,int=%d,UINT=%x,RECT*=%p,LPCSTR=%s,UINT=%x,LPINT=%p)\n",
	hDC,x,y,uiFlags,lpRect,GdiDumpString((LPSTR)lpStr,nCnt),nCnt,nCnt,lpDX));

    ASSERT_HDC(hDC32,hDC,FALSE);

    argptr.lsde_validate.dwInvalidMask = 0;

    argptr.lsde.text.x       = x;
    argptr.lsde.text.y       = y;
    argptr.lsde.text.lpStr   = (LPSTR)lpStr;
    argptr.lsde.text.nCnt    = nCnt;
    argptr.lsde.text.uiFlags = uiFlags;
    argptr.lsde.text.lpRect  = (LPRECT)lpRect;
    argptr.lsde.text.lpDX    = lpDX;

    bResult =  (DC_OUTPUT(LSD_EXTTEXTOUT,hDC32,0,&argptr))?TRUE:FALSE;

    RELEASEDCINFO(hDC32);
    APISTR((LF_APIRET,"ExtTextOut: returns BOOL %d\n",bResult));
    return bResult;
}

DWORD WINAPI
GetTabbedTextExtent(HDC hDC,LPCSTR lpstr,int cChars,
			int cTabs,int FAR *lpnTabs)
{
    DWORD ret;

    APISTR((LF_APICALL,
	"GetTabbedTextExtent(HDC=%x,LPCSTR=%s,int=%d,int=%d,int *%p)\n",
		hDC,lpstr,cChars,cTabs,lpnTabs));

    ret =  TabbedTextOutExt(hDC,0,0,lpstr,cChars,cTabs,lpnTabs,0,FALSE);

    APISTR((LF_APIRET,"GetTabbedTextExtent: returns DWORD %d\n",ret));
    return ret;
}

LONG WINAPI
TabbedTextOut(HDC hDC,int x,int y,LPCSTR lpstr,
		int cChars,int cTabs,int FAR *lpnTabs,int nTabOrigin)
{
    DWORD ret;

    APISTR((LF_APICALL,
"TabbedTextOut(HDC=%x,int=%d,int=%d,LPCSTR=%s,int=%d,int=%d,int* =%p,int=%d)\n",
		hDC,x,y,lpstr,cChars,cTabs,lpnTabs,nTabOrigin));

    ret =  TabbedTextOutExt(hDC,x,y,lpstr,cChars,cTabs,
			lpnTabs,nTabOrigin,TRUE);

    APISTR((LF_APIRET,"TabbedTextOut: returns DWORD %d\n",ret));
    return ret;
}

static LONG
TabbedTextOutExt(HDC hDC,int x,int y,LPCSTR lpstr,
		int cChars,int cTabs,int FAR *lpnTabs,
		int nTabOrigin,BOOL bWrite)
{
    HDC32 hDC32;
    WORD	width,height;
    DWORD   extent;
    int nChars = cChars;
    int	sx,dx;
    int	startx;
	
    sx = dx = 0;
    extent  = 0;
    width   = 0;
    startx  = x;

    ASSERT_HDC(hDC32,hDC,FALSE);
    RELEASEDCINFO(hDC32);

    if(nChars < 0)
	nChars = strlen(lpstr);

    for(;dx < nChars;) {
	while(dx < nChars && lpstr[dx] != '\t')
		dx++;

	if(dx == nChars)  /* have we reached the end 	     */
		break;

	if(bWrite) {	/* amount upto tab 	     	     */
		TextOut(hDC,x,y,&lpstr[sx],dx-sx); 
        }

	if(dx != sx) {
		extent = GetTextExtent(hDC,&lpstr[sx],dx-sx);  /* its width */
		width = LOWORD(extent);
	} else {
		width = 0;
	}

	x = FindNextTab(hDC,x,width,cTabs,lpnTabs,nTabOrigin);
		
	dx++;		/* skip over the tab 			*/
	sx = dx;	/* this is the new starting index 	*/
    }
    if(sx != dx) {
	if(bWrite) {	/* remaining text    			*/
		TextOut(hDC,x,y,&lpstr[sx],dx-sx);
	}

	extent = GetTextExtent(hDC,&lpstr[sx],dx-sx); /* its width   */
	width = LOWORD(extent);
	x += width;			/* updated width     */
    }
    height = HIWORD(extent);		/* the height 	     */

    /* The old code used width here, but width refers only to */	
    /* last segment.  The non-intuitive x is the running total */	
    /* width of the string, including the initial starting point */
    /* hence we subtract off the initial position to get widht   */
    x -= startx;			/* the overall width */

    return (LONG) MAKELONG(x,height);
}

static int
FindNextTab(HDC hDC,int x,int width,int cTabs,int FAR *lpnTabs,int nTabOrigin)
{
    int	nTab;
    int	nTabStop;
    int	nAveWidth;
	
    if (cTabs == 0 || lpnTabs == 0) {
	nAveWidth = GetCharAveWidth(hDC)*8;
	cTabs     = 1;
    }
    else
	nAveWidth = lpnTabs[0];

    if (cTabs == 1) {
	nTabStop  = nTabOrigin;
	while (x + width >= nTabStop)
	    nTabStop += nAveWidth;
	return nTabStop;
    }

    for (nTab=0;nTab<cTabs;nTab++) {		/* find tab  */
	if (x+width < lpnTabs[nTab] + nTabOrigin)
	    break;
    }	

    if (nTab == cTabs) {
	nAveWidth = GetCharAveWidth(hDC);
	return (x + width + nAveWidth);	
    }

    return (nTabOrigin + lpnTabs[nTab]);	/* move to the tab */
}

static int
GetCharAveWidth(HDC hDC)
{
    static char abc[] = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int nLen;
    SIZE sz;

    nLen    = sizeof(abc);
    if (GetTextExtentPoint(hDC,abc,nLen,&sz))
	return sz.cx/nLen;
    else
	return 0;
}

LPSTR
GdiDumpString(LPSTR lpszString,int nCount)
{
    int len;
    static char buf[80];

    if (lpszString && nCount) {
	if (nCount == -1)
	    len = strlen(lpszString)+1;
	else
	    len = min(nCount,(int)strlen(lpszString));
	len = min(len,79);
    }
    else
	len = 0;

    if (len) {
	strncpy(buf,lpszString,len);
	buf[len] = 0;
    }
    else
	strcpy(buf,"[NULL]");
		
    return buf;
}

