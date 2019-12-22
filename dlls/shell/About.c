/*    
	About.c	2.21
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

#include "windows.h"
#include "windowsx.h"
#include "commdlg.h"
#include "Log.h"
#include "Dialog.h"
#include "About.h"
#include "Strings32.h"
#include "Release.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static BOOL ShellAboutHandler(HWND, unsigned, WPARAM, LPARAM);

void
ShellAbout(HWND hWnd, LPCSTR lpszCaption, LPCSTR lpszAboutText,
		HICON hIcon)
{
	FARPROC	lpProc;
	SHELLABOUTDATA sad;
	HINSTANCE  hInst;
	int     rc;
	

	hInst = LoadLibrary("SHELL");

	lpProc= MakeProcInstance((FARPROC)ShellAboutHandler,hInst);

	sad.lpszText    = (LPSTR)lpszAboutText;
	sad.lpszCaption = (LPSTR) lpszCaption;
	sad.hIcon = hIcon;

	rc = DialogBoxParam(
	        hInst, 
		"ShellAbout",
		hWnd,
		lpProc,
		(LPARAM)&sad		
		);

	FreeProcInstance(lpProc); 
	FreeLibrary(hInst);
}

#ifdef TWIN_BIN32
void
ShellAboutA(HWND hWnd, LPCSTR lpszCaption, LPCSTR lpszAboutText, HICON hIcon)
{
	ShellAbout(hWnd, lpszCaption, lpszAboutText, hIcon);
}

void
ShellAboutW(HWND hWnd, LPWSTR lpszCaption, LPWSTR lpszAboutText, HICON hIcon)
{
	char *lpcap;
	char *lptxt;

	lpcap = WinStrdupW(lpszCaption);
	lptxt = WinStrdupW(lpszAboutText);

	ShellAbout(hWnd, lpcap, lptxt, hIcon);
	WinFree(lpcap);
	WinFree(lptxt);
}
#endif

#ifdef DEBUG
#define  TWINDBG "debug"
#else
#define  TWINDBG "optimized"
#endif

static BOOL FAR PASCAL
ShellAboutHandler(HWND hDlg, unsigned msg, WPARAM wParam, LPARAM lParam)
{
	SHELLABOUTDATA *sad;
	char		abouttext[256];
	DWORD		version;
	char		dirname[256];

	switch(msg) {
	case WM_INITDIALOG:
		sad = (SHELLABOUTDATA *) lParam;
		
		/************************************************/
		/*	Caption first				*/
		/************************************************/

		sprintf(abouttext,"About %s",sad->lpszCaption);
	    	SetWindowText(hDlg,abouttext);

		/************************************************/
		/*	Default first 2 lines			*/
		/************************************************/
		SendDlgItemMessage (hDlg, SAB_ABOUT, WM_SETTEXT, 0,
		                    ( LONG )sad->lpszCaption);
		SendDlgItemMessage (hDlg, SAB_TEXT, WM_SETTEXT, 0,
		                    ( LONG )sad->lpszText);

		/************************************************/
		/*	Twin specific 2 lines			*/
		/************************************************/
		GetModuleFileName(0,abouttext,256);

		/* add any shell about specific string */
		SendDlgItemMessage(hDlg, SAB_USER, WM_SETTEXT, 0,
			(LONG) abouttext);

		GetWindowsDirectory(dirname,256);
		sprintf(abouttext,"Windows: %s",dirname);
		SendDlgItemMessage(hDlg, SAB_WINDOW, WM_SETTEXT, 0,
			(LONG) abouttext);

		/************************************************/
		/*	Host specific 3 lines			*/
		/************************************************/
		/* host system information */
		sprintf ( abouttext, "Host: %s %s",
			TWIN_PLATFORM,
			getenv("DISPLAY"));
		SendDlgItemMessage(hDlg, SAB_HOST, WM_SETTEXT, 0,
			(LONG)abouttext);

		sprintf(abouttext,"Willows TWIN %d.%d.%d %s",
			VER_MAJOR, VER_MINOR, VER_PATCH, VER_RELEASE);
		SendDlgItemMessage(hDlg, SAB_TERM, WM_SETTEXT, 0,
			(LONG)abouttext);

		version = GetVersion();
		sprintf(abouttext,"Version: %s %d.%d build %d) %s",
			"Win32",
			LOBYTE(LOWORD(version)), HIBYTE(LOWORD(version)),
			HIWORD(version),
			TWINDBG);
		SendDlgItemMessage(hDlg, SAB_VERSION, WM_SETTEXT, 0,
			(LONG)abouttext);
	
		/************************************************/
		/*	Icon passed in?				*/
		/************************************************/
		if (sad->hIcon) 
	    		SendDlgItemMessage(hDlg, SAB_ICON, STM_SETICON,
				(WPARAM)sad->hIcon, 0L);

		return TRUE;

	case WM_COMMAND:
		switch ( GET_WM_COMMAND_ID(wParam, lParam) )
		{
			case IDOK:
				EndDialog(hDlg, IDOK);
				break;
			default:
				return ( FALSE );
		}
		return ( TRUE );
	}
	return FALSE;
}
