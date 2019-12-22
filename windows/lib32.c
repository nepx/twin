
#include <stdio.h>

#include "windows.h"
#include "Log.h"
#include "winnt.h"
#include "heap.h"
#include "RscData.h"
#include "stdarg.h"
#include "Resources.h"
#include "Module.h"
#include "WinConfig.h"
#include "Strings32.h"
#include "Log.h"

LPWSTR lstrcpyAtoW( LPWSTR , LPCSTR );
char *GetTwinMsgCode(HWND, UINT);
static short wcmdln[256];

#ifdef TWIN_BIN32
/* based on current running application */
int
GetStartupInfo(LPSTARTUPINFOA lp)
{
	APISTR((LF_APICALL,"GetStartupInfo(%p) sizeof=%x\n",
		lp,sizeof(STARTUPINFOA)));
	memset(lp,0,sizeof(STARTUPINFOA));
	lp->dwFlags = 1;
	lp->wShowWindow = 1;	
	APISTR((LF_APIRET,"GetStartupInfo returns %d\n",0));
	return 0;
}

BOOL
CharToOemA(LPCSTR instr,LPSTR outstr)
{
	APISTR((LF_API,"CharToOemA(%p,%p)\n",instr,outstr));
	strcpy(outstr,instr);	
	return TRUE;
}
DWORD
GetFileType32(HANDLE hFile)
{
	APISTR((LF_API,"GetFileType32(%x)\n",hFile));
	return 0;	
}
#endif

HANDLE
GetStdHandle(DWORD nStdHandle)
{
	APISTR((LF_API,"GetStdHandle(%x)\n",nStdHandle));
	if(nStdHandle == STD_INPUT_HANDLE)
		return INVALID_HANDLE_VALUE;
	if(nStdHandle == STD_OUTPUT_HANDLE)
		return INVALID_HANDLE_VALUE;
	if(nStdHandle == STD_ERROR_HANDLE)
		return INVALID_HANDLE_VALUE;

	return INVALID_HANDLE_VALUE;
}

UINT
GetACP(void)
{
	/* return ansi code page 1252 */
	return 0x4e4;  
}

typedef struct {
	UINT   MaxCharSize;
	BYTE   DefaultChar[2];
	BYTE   LeadByte[8];
} CPINFO;

typedef CPINFO *LPCPINFO;

UINT
GetCPInfo(UINT codepage,LPCPINFO lpcpinfo)
{
	APISTR((LF_API,"GetCPInfo(%x,%p)\n",codepage,lpcpinfo));

	lpcpinfo->DefaultChar[0] = '?';
	switch (codepage)
	{
	case 932 : /* Shift JIS (japan) */
		lpcpinfo->MaxCharSize = 2;
		lpcpinfo->LeadByte[0]= 0x81; lpcpinfo->LeadByte[1] = 0x9F;
		lpcpinfo->LeadByte[2]= 0xE0; lpcpinfo->LeadByte[3] = 0xFC;
		lpcpinfo->LeadByte[4]= 0x00; lpcpinfo->LeadByte[5] = 0x00;
		break;
	case 936 : /* GB2312 (Chinese) */
	case 949 : /* KSC5601-1987 (Korean) */
	case 950 : /* BIG5 (Chinese) */
		lpcpinfo->MaxCharSize = 2;
		lpcpinfo->LeadByte[0]= 0x81; lpcpinfo->LeadByte[1] = 0xFE;
		lpcpinfo->LeadByte[2]= 0x00; lpcpinfo->LeadByte[3] = 0x00;
		break;
	case 1361 : /* Johab (Korean) */
		lpcpinfo->MaxCharSize = 2;
		lpcpinfo->LeadByte[0]= 0x84; lpcpinfo->LeadByte[1] = 0xD3;
		lpcpinfo->LeadByte[2]= 0xD8; lpcpinfo->LeadByte[3] = 0xDE;
		lpcpinfo->LeadByte[4]= 0xE0; lpcpinfo->LeadByte[5] = 0xF9;
		lpcpinfo->LeadByte[6]= 0x00; lpcpinfo->LeadByte[7] = 0x00;
	break;
	default :
		lpcpinfo->MaxCharSize = 1;
		lpcpinfo->LeadByte[0]= 0x00; lpcpinfo->LeadByte[1] = 0x00;
	break;
	}

	return codepage;  
}

#ifdef TWIN_BIN32
LPWSTR
GetCommandLineW(void)
{
	APISTR((LF_API,"GetCommandLineW()\n"));
	lstrcpyAtoW((LPWSTR) wcmdln, GetCommandLine());
	return (LPWSTR) wcmdln;
}
#endif


int
IsBadReadPtr32(void *p,int nsize)
{
	APISTR((LF_API,"IsBadReadPtr32(%p,%d)\n",p,nsize));
	return 0;
}

int
IsBadWritePtr32(void *p,int nsize)
{
	APISTR((LF_API,"IsBadWritePtr32(%p,%d)\n",p,nsize));
	return 0;
}

#ifdef TWIN_BIN32
HWND
CreateWindowExA(DWORD dwExStyle, LPCSTR lpClass, 
		LPCSTR lpWindowName, DWORD dwStyle,
		int X, int Y, int nWidth, int nHeight, 
		HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, 
		LPVOID lpParam)
{
	APISTR((LF_API,"CreateWindowExA(...)\n"));
	return CreateWindowEx(dwExStyle,lpClass,lpWindowName,dwStyle,
		X,Y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
}

HWND
CreateWindowExW(DWORD dwExStyle, LPCSTR lpClass, 
		LPCSTR lpWindowName, DWORD dwStyle,
		int X, int Y, int nWidth, int nHeight, 
		HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, 
		LPVOID lpParam)
{
	char lpclassname[80];
	char lpwinname[80];

	lpclassname[0] = 0;
	lpwinname[0] = 0;
	WideCharToMultiByte(0,0,(LPCWSTR)lpClass,0,lpclassname,80,0,0);
	WideCharToMultiByte(0,0,(LPCWSTR)lpWindowName,0,lpwinname,80,0,0);


	APISTR((LF_API,"CreateWindowExW(...)\n"));
	return CreateWindowEx(dwExStyle,lpclassname,lpwinname,dwStyle,
		X,Y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
}

BOOL    
GetMessageA(MSG *lpmsg, HWND hWnd, UINT idlo, UINT idhi)
{
	APISTR((LF_API,"GetMessageA(%p,%x,%x,%x)\n",
		lpmsg,hWnd,idlo,idhi));

	return GetMessage(lpmsg,hWnd,idlo,idhi);
}

BOOL    
GetMessageW(MSG *lpmsg, HWND hWnd, UINT idlo, UINT idhi)
{
	APISTR((LF_API,"GetMessageW(%p,%x,%x,%x)\n",
		lpmsg,hWnd,idlo,idhi));

	return GetMessage(lpmsg,hWnd,idlo,idhi);
}

int
TranslateAcceleratorA(HWND hWnd, HACCEL hAccel, MSG *lpmsg)
{
	APISTR((LF_API,"TranslateAcceleratorA(%x,%x,%p)\n",
		hWnd,hAccel,lpmsg));

	return TranslateAccelerator(hWnd,hAccel,lpmsg);
}

int
TranslateAcceleratorW(HWND hWnd, HACCEL hAccel, MSG *lpmsg)
{
	APISTR((LF_API,"TranslateAcceleratorW(%x,%x,%p)\n",
		hWnd,hAccel,lpmsg));

	return TranslateAccelerator(hWnd,hAccel,lpmsg);
}

LONG    
DispatchMessageA(MSG *lpmsg)
{
	APISTR((LF_API,"DispatchMessageA(%p)\n",lpmsg));
	return DispatchMessage(lpmsg);

}

LONG    
DispatchMessageW(MSG *lpmsg)
{
	APISTR((LF_API,"DispatchMessageW(%p)\n",lpmsg));
	return DispatchMessage(lpmsg);

}

DWORD
DefWindowProcA(HWND hWnd,UINT msg,DWORD wparam,DWORD lparam)
{
	APISTR((LF_API,"DefWindowProcA(%x,%x:%s,%x,%x)\n",
		hWnd,msg,
		GetTwinMsgCode(hWnd, msg),
		wparam,lparam));
	return DefWindowProc(hWnd,msg,wparam,lparam);
}

DWORD
DefWindowProcW(HWND hWnd,UINT msg,DWORD wparam,DWORD lparam)
{
	APISTR((LF_API,"DefWindowProcW(%x,%x:%s,%x,%x)\n",
		hWnd,msg,
		GetTwinMsgCode(hWnd, msg),
		wparam,lparam));
	return DefWindowProc(hWnd,msg,wparam,lparam);
}

DWORD
DefDlgProcA(HWND hDlg,UINT msg,DWORD wparam,DWORD lparam)
{
	APISTR((LF_API,"DefDlgProcA(%x,%x:%s,%x,%x)\n",
		hDlg,msg,
		GetTwinMsgCode(hDlg, msg),
		wparam,lparam));
	return DefDlgProc(hDlg,msg,wparam,lparam);

}

DWORD
DefDlgProcW(HWND hWnd,UINT msg,DWORD wparam,DWORD lparam)
{
	APISTR((LF_API,"DefDlgProcW(%x,%x:%s,%x,%x)\n",
		hWnd,msg,
		GetTwinMsgCode(hWnd, msg),
		wparam,lparam));
	return DefDlgProc(hWnd,msg,wparam,lparam);
}
#endif

static void
FixClassEx(WNDCLASSEX *wc,char *MenuName,char *ClassName)
{
	
	WideCharToMultiByte(0,0,(LPCWSTR)wc->lpszMenuName,0,MenuName,32,0,0);
	wc->lpszMenuName = MenuName;

	WideCharToMultiByte(0,0,(LPCWSTR)wc->lpszClassName,0,ClassName,32,0,0);
	wc->lpszClassName = ClassName;
#if 0
	APISTR((LF_CONSOLE,"WNDCLASSEX(%p)\n",wc));
	APISTR((LF_CONSOLE,"sz=%d Style=%x xtra=%d,%d\n",
		wc->cbSize,wc->style,wc->cbClsExtra,
		wc->cbWndExtra));
	APISTR((LF_CONSOLE,"inst=%x icon=%x cursor=%x\n",
		wc->hInstance,wc->hIcon,wc->hCursor));
	APISTR((LF_CONSOLE,"bg=%x iconsm=%x\n",
		wc->hbrBackground,wc->hIconSm));
	DumpMemory("MenuName",wc->lpszMenuName,8);
	DumpMemory("ClassName",wc->lpszClassName,8);
#endif
}

#ifdef TWIN_BIN32
int
RegisterClassA(WNDCLASS *wc)
{
	APISTR((LF_API,"RegisterClassA(%p)\n",wc));
	return RegisterClass(wc);	
}

int
RegisterClassW(WNDCLASS *wc)
{
	char MenuName[32];
	char ClassName[32];

	APISTR((LF_API,"RegisterClassW(%p)\n",wc));
	WideCharToMultiByte(0,0,(LPCWSTR)wc->lpszMenuName,0,MenuName,32,0,0);
	wc->lpszMenuName = MenuName;

	WideCharToMultiByte(0,0,(LPCWSTR)wc->lpszClassName,0,ClassName,32,0,0);
	wc->lpszClassName = ClassName;

	APISTR((LF_API,"RegisterClassW(%s)(%s)\n",
		MenuName, ClassName));
	return RegisterClass(wc);	
}
int
RegisterClassExA(WNDCLASSEX *wc)
{
	APISTR((LF_API,"RegisterClassExA(%p)\n",wc));
	return RegisterClassEx(wc);	
}

int
RegisterClassExW(WNDCLASSEX *wc)
{
	char MenuName[32];
	char ClassName[32];
	APISTR((LF_API,"RegisterClassExW(%p)\n",wc));
	FixClassEx(wc,MenuName,ClassName);
	return RegisterClassEx(wc);	
}

#endif

DWORD
LocalAlloc32(UINT flags,UINT bytes)
{
	HANDLE hMem;
	DWORD  dwWord;
	APISTR((LF_APICALL,"LocalAlloc(%x,%x)\n", flags,bytes));
	
	hMem = GlobalAlloc(flags,bytes);
	if(hMem && !(flags  & LMEM_MOVEABLE)) {
		dwWord = (DWORD) GlobalLock(hMem);
		APISTR((LF_APIRET,"LocalAlloc returns DWORD (ptr) %x\n", 
			dwWord));
		return dwWord;
	}

	APISTR((LF_APIRET,"LocalAlloc returns DWORD (handle) %x\n", hMem));
	return (DWORD)hMem;
}

DWORD
LocalReAlloc32(DWORD hLocal,UINT flags,UINT bytes)
{
	HANDLE hMem = (HANDLE)hLocal;
	DWORD  dwWord;
	
	APISTR((LF_APICALL,"LocalReAlloc(%x,%x,%x)\n", hLocal,flags,bytes));
	if(HIWORD(hLocal))
		hMem = GlobalHandle32((LPCVOID)hLocal);
		
	hMem =  GlobalReAlloc(hMem,flags,bytes);
	if(HIWORD(hLocal)) {
		dwWord = (DWORD) GlobalLock(hMem);
		APISTR((LF_APIRET,"LocalReAlloc returns DWORD (ptr) %x\n", 
			dwWord));
		return dwWord;
	}	
	APISTR((LF_APIRET,"LocalReAlloc returns DWORD (handle) %x\n", hMem));
	return (DWORD)hMem;
}

HLOCAL
LocalFree32(HLOCAL hLocal)
{
	HANDLE hMem = hLocal;
	HGLOBAL hFree;
	APISTR((LF_APICALL,"LocalFree32(%x)\n", hMem));
	if(HIWORD(hLocal))
		hMem = GlobalHandle32((LPCVOID)hLocal);
		
	hFree =  GlobalFree(hMem);
	APISTR((LF_APIRET,"LocalFree32 returns HLOCAL\n", hFree));
	return (HLOCAL) hFree;
}

LPVOID
LocalLock32(HLOCAL hLocal)
{
	HANDLE hMem = hLocal;
	LPVOID  lpVoid;
	
	APISTR((LF_APICALL,"LocalLock(%x)\n", hLocal));
	lpVoid = (LPVOID) GlobalLock(hMem);
	APISTR((LF_APIRET,"LocalLock returns DWORD (ptr) %p\n", lpVoid));
	return lpVoid;
}

BOOL
GetTextMetricsW(HDC hDC, LPTEXTMETRIC lpMetrics)
{
    BOOL rc;
    APISTR((LF_APICALL,"GetTextMetricsW(HDC=%x,LPTEXTMETRIC=%p)\n",hDC,lpMetrics));
    rc = GetTextMetrics(hDC,lpMetrics);
    APISTR((LF_APIRET,"GetTextMetricsW returns BOOL %d\n",rc));
    return rc;
}

BOOL WINAPI
GetTextMetricsA(HDC hDC, LPTEXTMETRIC lpMetrics)
{
    BOOL rc;
    APISTR((LF_APICALL,"GetTextMetricsA(HDC=%x,LPTEXTMETRIC=%p)\n",hDC,lpMetrics));
    rc = GetTextMetrics(hDC,lpMetrics);
    APISTR((LF_APIRET,"GetTextMetricsA returns BOOL %d\n",rc));
    return rc;
}

BOOL GetTextExtentPoint32A(HDC hDC,LPCSTR lpcstr,int cbstring,LPSIZE lpSize)
{
	APISTR((LF_API,"GetTextExtentPoint32A(HDC=%x,%s,UINT=%x)\n",
		hDC,lpcstr,cbstring));
	return GetTextExtentPoint(hDC,(LPSTR)lpcstr,cbstring,lpSize);
}

BOOL GetTextExtentPoint32W(HDC hDC,LPCWSTR lpcstr,int cbstring,LPSIZE lpSize)
{
	APISTR((LF_API,"GetTextExtentPoint32W(HDC=%x,%s,UINT=%x)\n",
		hDC,lpcstr,cbstring));
	return GetTextExtentPoint(hDC,(LPSTR)lpcstr,cbstring,lpSize);
}

BOOL GetTextExtentPointA(HDC hDC,LPCSTR lpcstr,int cbstring,LPSIZE lpSize)
{
	APISTR((LF_API,"GetTextExtentPointA(HDC=%x,%s,UINT=%x)\n",
		hDC,lpcstr,cbstring));
	return GetTextExtentPoint(hDC,lpcstr,cbstring,lpSize);
}

BOOL GetTextExtentPointW(HDC hDC,LPCWSTR lpcstr,int cbstring,LPSIZE lpSize)
{
	char *lpstr;
	BOOL  rc;
	lpstr = WinStrdupW(lpcstr);
	APISTR((LF_API,"GetTextExtentPointW(HDC=%x,%s,UINT=%x)\n",
		hDC,lpstr,cbstring));
	rc = GetTextExtentPoint(hDC,lpstr,cbstring,lpSize);
	WinFree(lpstr);
	return rc;
}

void
SetDlgItemTextA(HWND hDlg,int nID,char *p)
{
	APISTR((LF_API,"SetDlgItemTextA(%x,%d,%s)\n",
		hDlg,nID,p));
	SetDlgItemText(hDlg,nID,p);
}

void
SetDlgItemTextW(HWND hDlg,int nID,LPCWSTR p)
{
	LPSTR lpstr;
	
	lpstr = WinStrdupW(p);
	APISTR((LF_API,"SetDlgItemTextW(%x,%d,%s)\n",
		hDlg,nID,lpstr));
	SetDlgItemText(hDlg,nID,lpstr);
	WinFree(lpstr);
}

BOOL
TextOutA(HDC hDC,int x,int y,LPCSTR lpstr,int cbstring)
{
	APISTR((LF_API,"TextOutA(HDC=%x,%d,%d],%s,UINT=%x)\n",
		hDC,x,y,lpstr,cbstring));

	return TextOut(hDC,x,y,lpstr,cbstring);
}

BOOL
TextOutW(HDC hDC,int x,int y,LPCWSTR lpwstr,int cbstring)
{	
	char *lpstr;
	BOOL  rc;

	lpstr = WinStrdupW(lpwstr);
	APISTR((LF_API,"TextOutW(HDC=%x,%d,%d,%p[%s],UINT=%x)\n",
		hDC,x,y,lpwstr,lpstr,cbstring));
	rc = TextOut(hDC,x,y,lpstr,cbstring);
	WinFree(lpstr);
	return rc;
}
BOOL
ExtTextOutA(HDC hDC, int x, int y, UINT uiFlags,
	   const RECT *lpRect, LPCSTR lpStr, UINT nCnt, LPINT lpDX)
{
	APISTR((LF_API,"ExtTextOutA(HDC=%x,%d,%d,%d,%p,%s,UINT=%x,%p)\n",
		hDC,x,y,uiFlags,lpRect,lpStr,nCnt,lpDX));
	return ExtTextOut(hDC,x,y,uiFlags,lpRect,lpStr,nCnt,lpDX);
}

BOOL
ExtTextOutW(HDC hDC, int x, int y, UINT uiFlags,
	   const RECT *lpRect, LPCWSTR lpStr, UINT nCnt, LPINT lpDX)
{
	char *tstr;
	BOOL  rc;
	tstr = WinStrdupW(lpStr);
	APISTR((LF_API,"ExtTextOutW(HDC=%x,%d,%d,%d,%p,%s[%s],UINT=%x,%p)\n",
		hDC,x,y,uiFlags,lpRect,lpStr,tstr,nCnt,lpDX));
	rc = ExtTextOut(hDC,x,y,uiFlags,lpRect,tstr,nCnt,lpDX);
	WinFree(tstr);
	return rc;
}

LPARAM
SendMessageA(HWND hWnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	return SendMessage(hWnd,msg,wparam,lparam);
}

LPARAM
SendMessageW(HWND hWnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	return SendMessage(hWnd,msg,wparam,lparam);
}

BOOL
IsDialogMessageA(HWND hWnd,LPMSG lpmsg)
{
	return IsDialogMessage(hWnd,lpmsg);
}

BOOL
IsDialogMessageW(HWND hWnd,LPMSG lpmsg)
{
	return IsDialogMessage(hWnd,lpmsg);
}

HFONT 
CreateFontW(
    int nHeight,	// logical height of font 
    int nWidth,	// logical average character width 
    int nEscapement,	// angle of escapement 
    int nOrientation,	// base-line orientation angle 
    int fnWeight,	// font weight 
    DWORD fdwItalic,	// italic attribute flag 
    DWORD fdwUnderline,	// underline attribute flag 
    DWORD fdwStrikeOut,	// strikeout attribute flag 
    DWORD fdwCharSet,	// character set identifier 
    DWORD fdwOutputPrecision,	// output precision 
    DWORD fdwClipPrecision,	// clipping precision 
    DWORD fdwQuality,	// output quality 
    DWORD fdwPitchAndFamily,	// pitch and family 
    LPCWSTR lpszFace 	// pointer to typeface name string 
   )
{
 	HFONT hFont;
	LPSTR lpstr;

	lpstr = WinStrdupW(lpszFace);
  	hFont = CreateFont(
	    nHeight,
	    nWidth,	
	    nEscapement,	
	    nOrientation,
	    fnWeight,
	    fdwItalic,	
	    fdwUnderline,
	    fdwStrikeOut,	
	    fdwCharSet,	
	    fdwOutputPrecision,	
	    fdwClipPrecision,	
	    fdwQuality,	
	    fdwPitchAndFamily,
	    lpstr);
	WinFree(lpstr);
	return hFont;
}

BOOL
CreateFontIndirectA(LPLOGFONT lpfont)
{
	return (BOOL)CreateFontIndirect(lpfont);
}
BOOL
CreateFontIndirectW(LPLOGFONT lpfont)
{
	return (BOOL)CreateFontIndirect(lpfont);
}

BOOL
WinHelpA( HWND hWnd, LPCSTR lpszHelpFile, UINT fuCommand, DWORD dwData )
{
	APISTR((LF_API,"WinHelpA(%x,%s,%d,%d)\n",
		hWnd,lpszHelpFile,fuCommand,dwData));
	return WinHelp( hWnd, lpszHelpFile, fuCommand, dwData );
}

BOOL
WinHelpW( HWND hWnd, LPCWSTR lpszHelpFile, UINT fuCommand, DWORD dwData )
{
	BOOL  rc;
	char *lpstr;
	lpstr = WinStrdupW(lpszHelpFile);
	APISTR((LF_API,"WinHelpW(%x,%s:%s,%d,%d)\n",
		hWnd,lpszHelpFile,lpstr,fuCommand,dwData));
	rc = WinHelp( hWnd, lpstr, fuCommand, dwData );
	WinFree(lpstr);
	return rc;
}

BOOL GetTextExtentExPointA(HDC hDC,LPCTSTR lpsz,int cch,int nMax,LPINT lpfit,LPINT lpdx, LPSIZE lpsize)
{
	return GetTextExtentExPoint(hDC,lpsz,cch,nMax,lpfit,lpdx,lpsize);
}
BOOL GetTextExtentExPointW(HDC hDC,LPCTSTR lpsz,int cch,int nMax,LPINT lpfit,LPINT lpdx, LPSIZE lpsize)
{
	char *lpstr;
	BOOL rc;
	lpstr = WinStrdupW((LPWSTR)lpsz);
	APISTR((LF_API,"GetTextExtentExPointW(.%s..)\n",lpstr));
	rc = GetTextExtentExPoint(hDC,lpstr,cch,nMax,lpfit,lpdx,lpsize);
	WinFree(lpstr);
	return rc;
}

int
GetWindowLongA(HWND hWnd,int offset)
{
	return GetWindowLong(hWnd,offset);
}

int
GetWindowLongW(HWND hWnd,int offset)
{
	return GetWindowLong(hWnd,offset);
}

int
SetWindowLongA(HWND hWnd,int offset,int value)
{
	return SetWindowLong(hWnd,offset,value);
}
int
SetWindowLongW(HWND hWnd,int offset,int value)
{
	return SetWindowLong(hWnd,offset,value);
}

BOOL SetWindowTextA(HWND hWnd,LPCSTR lpstr)
{
	return SetWindowText(hWnd,lpstr);
}
BOOL
SetWindowTextW(HWND hWnd,LPCWSTR lpstr)
{
	LPSTR tstr;
	BOOL  rc;
	tstr = WinStrdupW(lpstr);
	rc = SetWindowText(hWnd,tstr);
	WinFree(tstr);
	return rc;
}


BOOL
AppendMenuA(HMENU hMenu,int flag,int id,LPWSTR *item)
{
	BOOL rc; 
	APISTR((LF_APICALL,"AppendMenuA(%x,%x,%x,%p)\n",
		hMenu,flag,id,item));
	rc = AppendMenu(hMenu,flag,id,(LPCSTR) item);
	APISTR((LF_APIRET,"AppendMenuA returns BOOL %d\n",rc));
	return rc;
}

BOOL
AppendMenuW(HMENU hMenu,int flag,int id,LPWSTR *item)
{
// TODO - is item WIDE???
	APISTR((LF_APICALL,"AppendMenuW(%x,%x,%x,%p)\n",
		hMenu,flag,id,item));
	return 1;
}

LONG
SendDlgItemMessageA(HWND h,int id,UINT msg,WPARAM wparam,LPARAM lparam)
{
	return SendDlgItemMessage(h,id,msg,wparam,lparam);
}

LONG
SendDlgItemMessageW(HWND h,int id,UINT msg,WPARAM wparam,LPARAM lparam)
{
	return SendDlgItemMessage(h,id,msg,wparam,lparam);
}

int   
DrawTextA(HDC hDC, LPCSTR lpsz, int cb, LPRECT lprc, UINT uFormat)
{
	return DrawText(hDC,lpsz,cb,lprc,uFormat);
}

LRESULT
CallWindowProcA(FARPROC lpfnwndproc,HWND hWnd,UINT message,WPARAM w,LPARAM l)
{
	return CallWindowProc(lpfnwndproc,hWnd,message,w,l);	
}

int
SetClassLongA(HWND hWnd, int nIndex, LONG dwNewLong)
{
	return SetClassLong(hWnd,nIndex,dwNewLong);
}
int
GetCurrentDirectoryA(DWORD cchCurDir, LPTSTR lpszCurDir)
{
	return GetCurrentDirectory(cchCurDir,lpszCurDir);
}

HWND WINAPI
FindWindowA(LPCSTR lpszClass, LPCSTR lpszWindow)
{
	HWND hWnd;
	APISTR((LF_APICALL,"FindWindowA()\n"));
	hWnd =  FindWindowEx((HWND)NULL, (HWND)NULL, lpszClass, lpszWindow);
	APISTR((LF_APIRET,"FindWindowA returns HWND %x\n",hWnd));
	return hWnd;
}

HWND WINAPI
FindWindowW(LPCSTR lpszClass, LPCSTR lpszWindow)
{
	HWND hWnd;
	APISTR((LF_APICALL,"FindWindowW()\n"));
	hWnd = FindWindowEx((HWND)NULL, (HWND)NULL, lpszClass, lpszWindow);
	APISTR((LF_APIRET,"FindWindowW returns HWND %x\n",hWnd));
	return hWnd;
}

HWND WINAPI
FindWindowExA(HWND hwndParent, HWND hwndChildAfter,
	LPCTSTR lpszClass, LPCTSTR lpszWindow)
{
	HWND hWnd;
	APISTR((LF_APICALL,"FindWindowExA()\n"));
	hWnd = FindWindowEx(hwndParent,hwndChildAfter,lpszClass,lpszWindow);
	APISTR((LF_APIRET,"FindWindowExA returns HWND %x\n",hWnd));
	return hWnd;
}

HWND WINAPI
FindWindowExW(HWND hwndParent, HWND hwndChildAfter,
	LPCTSTR lpszClass, LPCTSTR lpszWindow)
{
	HWND hWnd;
	APISTR((LF_APICALL,"FindWindowExW()\n"));
	hWnd =  FindWindowEx(hwndParent,hwndChildAfter,lpszClass,lpszWindow);
	APISTR((LF_APIRET,"FindWindowExW returns HWND %x\n",hWnd));
	return hWnd;
}

HDC 
CreateMetaFileA(LPCSTR filename)
{
	return CreateMetaFile(filename);
}

HDC
CreateDCA(LPCSTR lpszDriver, LPCSTR lpszDevice,
	 LPCSTR lpszOutput, const void *lpvInitData)
{
	return CreateDC(lpszDriver, lpszDevice, lpszOutput, lpvInitData);
}

void *
TWINmemset(void *addr,int fill,int len)
{
	return memset(addr,fill,len);
}

void
Shell32End()
{}
void
User32End()
{}
void
Kernel32End()
{}
void
Gdi32End()
{}

