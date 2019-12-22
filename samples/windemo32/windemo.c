/*
 *	windemo.c -- Simple Windows Demonstration Application
 *
 *  @(#)windemo.c	12. 10/10/96 15:24:29 /users/sccs/src/samples/windemo/s.windemo.c	
 *
 *	Copyright (c) 1995-1996, Willows Software Inc.  All rights reserved.
 *
 *	Demonstrates the following:
 *
 *	- use of resource and module definition compilers.
 *	- registering classes, and creating windows.
 *	- standard message loop and message dispatch logic.
 */

//{{NO_DEPENDENCIES}}
// Microsoft Developer Studio generated include file.
// Used by windemo.rc

#define IDD_DIALOG1                     101

// Next default values for new objects
 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NO_MFC                     1
#define _APS_NEXT_RESOURCE_VALUE        102
#define _APS_NEXT_COMMAND_VALUE         40006
#define _APS_NEXT_CONTROL_VALUE         1000
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

/*
 *	- use of resources, menues, bitmaps, stringtables and accelerators.
 *	- calling the ShellAbout API.
 */

#include "windows.h"
#include "windemo.h"

HWND hDialog;

HANDLE	hInstance = (HANDLE)NULL;
char    Program[] = "WinDemo";
char    Title[256] = "My Own Title";

BOOL  CALLBACK
About(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char lpstr[80];
	RECT r;
	LOGFONT lf;

    switch (msg) {
       case WM_INITDIALOG:
		   {
			HDC hDC;			
			HFONT hFont,last;
			TEXTMETRIC tm;

			SetRect(&r,4,8,40,80);
			MapDialogRect(hDlg,&r);
			wsprintf(lpstr,"x,y=%d,%d %d,%d\n",
				r.left,r.top,r.right,r.bottom);
		
			OutputDebugString(lpstr);
			memset(&lf,0,sizeof(LOGFONT));
			
            lf.lfHeight = -MulDiv(8,0x60,72);
            lf.lfWeight = 400;

            lstrcpy(lf.lfFaceName,"MS Sans Serif");

            hFont = CreateFontIndirect(&lf);
			hDC = GetDC(0);
			if(hFont) {
				last = SelectObject(hDC,hFont);
				GetTextMetrics(hDC,&tm);
			}
			if(hFont)
					DeleteObject(hFont);
			ReleaseDC(0,hDC);
		   }
		
		   return TRUE;

       case WM_COMMAND:
           EndDialog(hDlg, TRUE);
           return TRUE;

	   default:
			break;
    } /* endswitch() */

    return FALSE;

} /* About() */

HBRUSH hBrush;

int
showcontrol(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC hDC;
	RECT r;
	int  i;

	hDC = GetDC(hWnd);
	SetRect(&r,20,20,40,40);
	// draw the borders of a raised button
	i = DrawEdge(hDC,&r,BDR_RAISED,BF_SOFT|BF_RECT);
	OffsetRect(&r,20,0);
	// draw the borders of a pushed button
	DrawFrameControl(hDC,&r,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_PUSHED);
	OffsetRect(&r,20,0);

	// draw the borders of a raised button
	i = DrawFrameControl(hDC,&r,DFC_BUTTON,DFCS_BUTTONPUSH);
	
	ReleaseDC(hWnd,hDC);
	return 0;
}

long FAR PASCAL
WinWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //HICON hIcon;
	//FARPROC lpProcDialog;
	static HBRUSH hBG;
	
    switch (message) {

    case WM_COMMAND:
           switch (wParam) {
           case IDM_ABOUT:
//               hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
//  			 ShellAbout(hWnd, Program, Title, hIcon);
//               DestroyIcon(hIcon);

			//   lpProcDialog = MakeProcInstance((FARPROC)About, hInstance);
            //   DialogBox(hInstance, IDD_DIALOG1, hWnd, lpProcDialog);
            //   FreeProcInstance(lpProcDialog);
               hDialog = CreateDialogParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),
				   hWnd,About,0);
			   if(hDialog)
				ShowWindow(hDialog,SW_SHOWNORMAL);
               break;
	    case IDM_EXIT:
			if(hBG) DeleteObject(hBG);
			hBG = 0;
	       DestroyWindow(hWnd);
	       break;
           }
	   break;

    case WM_SIZE:
           switch (wParam) {
              case SIZEICONIC:
				MessageBeep(0);
           }
           break;

	case WM_HOTKEY:
		   ShowWindow(hWnd,SW_NORMAL);
		   break;

    case WM_DESTROY:
           PostQuitMessage(0);
           break;

	case WM_KEYDOWN:
			showcontrol(hWnd,message,wParam,lParam);
			break;
	case WM_CTLCOLOREDIT:
		hBG = hBrush;
		if(hBG == 0) {
			hBG = GetStockObject(LTGRAY_BRUSH);
		}
		
		SetTextColor((HDC)wParam,RGB(0,0,255));
		SetBkColor((HDC)wParam,RGB(0xff,0xff,0));
		
		OutputDebugString("ctlcoloredit\n");
		return (long) hBG;

    default:
           return DefWindowProc(hWnd,message,wParam,lParam);
    }
    
	return (0);
}

void
showstring(char *title,char *str)
{
	OutputDebugString(title);
	OutputDebugString(" [");
	if(str)
		OutputDebugString(str);
	OutputDebugString("]\n");
}

extern int __argc;
extern char **__argv;

BOOL PASCAL 
WinMain(HANDLE hInst, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HACCEL hAccelTable;
    HWND   hWnd,hEdit;
    MSG    Msg;
    HWND  hWin;
	char title[80];
	long  dw;

	showstring("WinMain",lpCmdLine);
    showstring("0",__argv[0]);
	showstring("1",__argv[1]);

	if (!hPrevInstance) {
        WNDCLASS wc;
	    hBrush = CreateSolidBrush(RGB(0xff,0xff,0));
        wc.style         = 0;
        wc.lpfnWndProc   = WinWndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInst;
        wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
        wc.hCursor       = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
        wc.hbrBackground = hBrush;
        wc.lpszMenuName  = (LPSTR) 0;
        wc.lpszClassName = (LPSTR) Program;

        if ( !RegisterClass(&wc) )
            return FALSE;
    }

    LoadString(hInst,IDS_TITLE,Title,256);
    hInstance = hInst;

	showstring("loadstring",Title);
    hAccelTable = LoadAccelerators( hInstance, (LPSTR)"RESOURCE" );

	dw = WS_EX_CLIENTEDGE|WS_EX_ACCEPTFILES;
    hWnd = CreateWindowEx(dw,Program,
			"Emmo",
			WS_POPUP|WS_DLGFRAME|WS_SYSMENU,
			40,240,200,200,
			0,
			(HMENU)NULL,
			hInst,
			NULL);

    if (!hWnd)
       return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

	hWin = GetWindow(hWnd,2);
    GetClassName(hWin,title,80);

	hEdit = CreateWindow("EDIT","",WS_VISIBLE|WS_CHILD|WS_BORDER,40,40,100,24,
		hWnd,0,hInst,0);

	hWin = GetWindow(hEdit,2);

	RegisterHotKey(hWnd,1,MOD_ALT,VK_F10);

	ShowWindow(hWnd,SW_HIDE);

    while (GetMessage(&Msg, (HWND)NULL, 0, 0)) {
      if (IsDialogMessage(hDialog,&Msg))
		continue;

	  if (TranslateAccelerator( hWnd, hAccelTable, (LPMSG)&Msg ) == 0) {
      	TranslateMessage(&Msg);
      	DispatchMessage(&Msg);
      }	
    }
    return Msg.wParam;
}
