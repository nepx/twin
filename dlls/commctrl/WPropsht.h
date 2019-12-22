/*  WPropsht.h	1.2 TAB CONTROL 
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

The maintainer of the Willows TWIN Libraries may be reached (Email) 
at the address twin@willows.com	
 
*/

#ifndef __WPROPSHEET_H__
#define __WPROPSHEET_H__
#include "WTab.h"

/* Usefull macros */
#define TAB_MARGIN	          10
#define BTN_MARGIN	          5
#define TABICON_CX	          16
#define TABICON_CY	          16
#define MAX_PAGECAPTION	          40

#define	PAGEDIALOGDATA	          DLGTEMPLATE
/* We define ID_PS_TABCONTROL to be the same as AFX_IDC_TAB_CONTROL to please
 * MFC */
#define ID_PS_TABCONTROL          0x3020

/* Usefull types */
typedef int ( *LPPROPSHCALLBACK )( HWND, UINT, LPARAM );
typedef UINT ( *LPPROPSHPAGECALLBACK )( HWND, UINT, LPPROPSHEETPAGE );
typedef enum { btnOK, 
	       btnBack = btnOK, 
	       btnNext, 
	       btnCancel, 
	       btnApply,
	       btnHelp, 
	       btnFinish = btnHelp } enBtn;
typedef struct {
	DWORD	style;
	BYTE    bNumberOfItems;
	WORD	x,y;
	WORD    cx,cy;
	LPSTR    szMenuName;
	LPSTR    szClassName;
	LPSTR 	szCaption;
	WORD	wPointSize;
	LPSTR    szFaceName;
} PAGEDLGDATA;

/*
 * hMem : handle to a memory block containing this _PSP structure, to be used in
 *	  DestroyPropertyPage () to free allocated memory.
 * hwPage : handle to a modeless dialogbox implemented as a page.
 */
typedef struct 
{
	PROPSHEETPAGE	psPage;
	int		nTab;
	BOOL		bInit; /*whether the page is initialized */
	HWND		hwPage;
	HGLOBAL		hMem; 
} _PSP, *PPSP;

typedef struct 
{
	PPSP		hPage; 
} HPSPAGE;


typedef struct 
{
	RECT		rcDisplay; /*the display area for the tab control*/
	BOOL		bOkIsClose; /* whether the OK button changed to Close*/
	BOOL		bImageList; /* whether the image list is existent */
	UINT		nChanges; /* ==0 => Apply btn disabled, 
				       incremented when a PSM_CHANGED is received,
				       decremented when a PSM_UNCHANGED received */
	int		nRet;	/* return status passed to EndDialog*/

	HWND		hwBtn[5];
	HWND		hwPageSel; /* window handle to a selected page */

	int		PageHeight;
	int		PageWidth;
	int		PageCount;

	HIMAGELIST	hImgList;

	WLIST		PageList; /*array of Property Pages*/
	PROPSHEETHEADER psh;
} PROPSHEETDATA;


/* Prototypes */

/* Messages handlers */
static BOOL 
PropSh_OnInitDialog(HWND hwPropSht, HWND hwndFocus, LPARAM lParam);
static VOID 
PropSh_OnWMNotify(HWND hDlg, UINT uCtlID, LPARAM lParam);
static void 
PropSh_OnDestroy(HWND hwnd);
static void 
PropSh_OnCancel(HWND hwnd, PROPSHEETDATA* pPropShtData, int id, HWND hwndCtl, UINT codeNotify);
static void 
PropSh_OnApply(HWND hwnd, PROPSHEETDATA* pPropShtData, int id, HWND hwndCtl, UINT codeNotify, BOOL bOKClicked);
static void 
PropSh_OnHelp(HWND hwnd, PROPSHEETDATA* pPropShtData, int id, HWND hwndCtl, UINT codeNotify);
static void 
PropSh_OnGoBack(HWND hwnd, PROPSHEETDATA* pPropShtData, int id, HWND hwndCtl, UINT codeNotify);
static void 
PropSh_OnGoFinish(HWND hwnd, PROPSHEETDATA* pPropShtData, int id, HWND hwndCtl, UINT codeNotify);
static void 
PropSh_OnGoNext(HWND hwnd, PROPSHEETDATA* pPropShtData, int id, HWND hwndCtl, UINT codeNotify);
static void 
PropSh_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
#ifdef LATER
static void 
PropSh_OnHScroll(HWND hwnd, HWND hwndCtl, UINT codeNotify, int Pos);
static void 
PropSh_OnVScroll(HWND hwnd, HWND hwndCtl, UINT codeNotify, int Pos);
static void 
PropSh_OnOK(HWND hwnd, HWND hwndCtl, UINT codeNotify, int id);
#endif

/* APIs */
HPROPSHEETPAGE WINAPI
CreatePropertySheetPageA(LPCPROPSHEETPAGE lppsp);
HPROPSHEETPAGE WINAPI
CreatePropertySheetPageW(LPCPROPSHEETPAGEW lppsp);
HPROPSHEETPAGE WINAPI
CreatePropertySheetPage(LPCPROPSHEETPAGE lppsp);
BOOL WINAPI
DestroyPropertySheetPage(HPROPSHEETPAGE hPSPage);
int 
PropertySheet(LPCPROPSHEETHEADER lppsh);
int
PropertySheetA(LPCPROPSHEETHEADER lppsh);
int
PropertySheetW(LPCPROPSHEETHEADERW lppsh);

/* Local routines */
static DWORD
PropSh_IGetDlgUnits(PROPSHEETDATA* pthis);
static BOOL
PropSh_IInitPage(HWND hwPropSh, PROPSHEETDATA* pthis, PPSP pSrcPage, BOOL bReSize);
static BOOL 
PropSh_IInitSheet(HWND hwPropSh, PROPSHEETDATA* pthis, LPPROPSHEETHEADER ppsh);
static BOOL
PropSh_ISetButtonsLayout(HWND hwPropSht, PROPSHEETDATA* pthis, RECT* prcTab);
static BOOL
PropSh_ISetLayout(HWND hwPropSht, PROPSHEETDATA* pthis);
static void
PropSh_AddPage(HWND hwPS, PROPSHEETDATA* pthis, PPSP hPage, BOOL bResize);
static BOOL CALLBACK 
WPropSh_DlgProc(HWND hwnd, UINT	uMsg, WPARAM wParam, LPARAM lParam);
static BOOL  
PropSh_SetCurSel(HWND hWnd, PROPSHEETDATA* pthis, int PageIndx,HPROPSHEETPAGE hPage);
static void 
PropSh_PressBtn(HWND hWnd, PROPSHEETDATA* pthis, int iBtn);
static int 
PropSh_QuerySiblings(HWND hWnd, PROPSHEETDATA* pthis, WPARAM param1, LPARAM param2);
static void 
PropSh_RemovePage(HWND hWnd, PROPSHEETDATA* pthis, int PageIndx, HPROPSHEETPAGE hPage);
static void 
PropSh_SetFinishTxt(HWND hWnd, PROPSHEETDATA* pthis, LPSTR lpText);
static void 
PropSh_SetTitle(HWND hWnd, PROPSHEETDATA* pthis, DWORD dwStyle, LPSTR lpText);
static void 
PropSh_SetWizBtn(HWND hWnd, PROPSHEETDATA* pthis, DWORD dwFlag);
static LRESULT 
PropSh_PSMWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
static BOOL CALLBACK 
WPropSh_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* Error codes */
#define WPS_ERR_GENERAL        		                -1
#define WPS_ERR_REGISTERCLASS  			        -2
#define WPS_ERR_GLOBALALLOC				-3
#define WPS_ERR_GLOBALLOCK				-4
#define	WPS_ERR_LISTCOUNT				-5
#define WPS_ERR_LISTLOCK				-6
#define WPS_ERR_OUTOFRANGE				-7
#define WPS_ERR_LISTCREATE				-8
#define WPS_ERR_NULLPOINTER				-9
#define WPS_ERR_NOSIZE					-10
#define WPS_ERR_LOCALALLOC				-11
#define WPS_ERR_LOCALLOCK				-12
#define WPS_ERR_OUTOFBOUNDS				-13
#define WPS_ERR_LISTINITIALIZE			        -14
#define WPS_ERR_LISTINSERT				-15
#define WPS_ERR_LISTDESTROY				-16
#define WPS_ERR_GETDC					-17
#define WPS_ERR_DRAWTEXT				-18
#define WPS_ERR_HEADERINSERT		         	-19
#define WPS_ERR_LISTDELETE				-20
#define WPS_ERR_CREATEWINDOW		        	-22
#define WPS_ERR_GETWINPROP		        	-23

#endif
