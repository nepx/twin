/*=======================================================
				prnsetup.c
  =======================================================*/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <windowsx.h>
#include <print.h>
#include "resource.h"
#include "prnsetup.h"

HINSTANCE LoadDriver(LPCSTR);
int GetDevDrvPort(char **pDev, char **pDrv, char **pPort);

HINSTANCE	_hInst;		/* Global hInstance */


#pragma argsused
int PASCAL
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	char		szAppName[] = "Invisible";
	MSG         msg;
	WNDCLASS    wndclass;

	_hInst = hInstance;
	if ( !hPrevInst ) {
		memset(&wndclass, 0, sizeof(wndclass));
		wndclass.lpfnWndProc   = InvisibleWndProc;
		wndclass.hInstance     = _hInst;
		wndclass.lpszClassName = szAppName;
		RegisterClass(&wndclass);
	}

	CreateWindow(szAppName, "Invisible", WS_POPUP,
				CW_USEDEFAULT, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT,
				0, 0, _hInst, NULL);

	while ( GetMessage(&msg, 0, 0, 0) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}
	return msg.wParam;
}


/*-----------------------------------------------------------------
						Window Procedure
  -----------------------------------------------------------------*/
LRESULT FAR PASCAL _export
InvisibleWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DLGPROC	lpfnPrintersProc;

	switch ( message ) {
		case WM_CREATE:
			lpfnPrintersProc = (DLGPROC)MakeProcInstance(
					(FARPROC)PrintersDlgProc, _hInst);
			DialogBox(_hInst, "Printers", hWnd, lpfnPrintersProc);
			FreeProcInstance((FARPROC)lpfnPrintersProc);
			SendMessage(hWnd, WM_CLOSE, 0, 0L);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


/*-----------------------------------------------------------------------
			PrintersDlgProc()
  -----------------------------------------------------------------------*/
#pragma argsused
BOOL CALLBACK _export
PrintersDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int	ind;		/* Current selection in IDL_PRN_LIST  */
	static int	indDef;		/* Index in IDL_PRN_LIST, showing the */
					/* default printer                    */
	static int	iExit = CANCEL; /* Flag controled Cancel/Close button */
	static HWND	hStc, hLst;	/* hWnds of static control & listbox  */
	/*************
	int		(FAR PASCAL *lpfnExtDevMode)
			(HWND,HINSTANCE,LPDEVMODE,LPSTR,LPSTR,LPDEVMODE,LPSTR,WORD);
	**************/
	PROC		lpfnExtDevMode; 
	DLGPROC		lpfnDlgProc;
	LPSTR		lp, lp1;
	HGLOBAL		hPort;
	char		*pDev, *pPort;
	int			rc, n, i;
	HINSTANCE	hMod;
	ADDSTRUCT	add;
	char		buf[BUFFER_SIZE], str[BUFFER_SIZE], s[BUFFER_SIZE],
				tbuf[BUFFER_SIZE];

	switch ( message ) {
		case WM_INITDIALOG:
			hStc = GetDlgItem(hDlg, IDS_DEFAULT_PRN);
			hLst = GetDlgItem(hDlg, IDL_PRN_LIST);
			/* Get the information about all printers installed */
			/* in the system.                                   */
			if ( GetProfileString("devices", NULL, "", buf, sizeof(buf)) ) {
				lp = (LPSTR)buf;
				while ( *lp ) {
					GetProfileString("devices", lp, "", str, sizeof(str));
					lp1 = strtok(str, ", ");
					lp1 = strtok(NULL, ", ");
					wsprintf(s, "%s on %s", lp, lp1);
					SendMessage(hLst, LB_ADDSTRING, 0, (LPARAM)(LPSTR)s);
					lp += lstrlen(lp) + 1;
				}
			}
			/* Get the information about default printer from */
			/* win.ini                                        */
			if ( GetDevDrvPort(&pDev, 0, &pPort) ) {
				wsprintf(buf, "%s on %s", (LPSTR)pDev, (LPSTR)pPort);
				ind = (int)SendMessage(hLst, LB_FINDSTRINGEXACT, -1,
						(LPARAM)(LPSTR)buf);
				if ( ind == LB_ERR ) {
					sprintf(str, "Default printer %s is not listed in [devices] in WIN.INI.\nDefault printer is removed.",
							buf);
					MessageBox(hDlg, str, "Warning", MB_ICONEXCLAMATION);
					WriteProfileString("windows", "device", NULL);
					strcpy(buf, "No Default Printer");
					EnableWindow(GetDlgItem(hDlg, IDB_CONNECT), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDB_SETUP), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDB_REMOVE), FALSE);
				}
				else
					SendMessage(hLst, LB_SETCURSEL, ind, 0L);

				SendMessage(hStc, WM_SETTEXT, 0, (LPARAM)(LPSTR)buf);
			}
			else
				SendMessage(hStc, WM_SETTEXT, 0, 
						(LPARAM)(LPSTR)"No Default Printer");

			return TRUE;

		case WM_COMMAND:
			switch ( wParam ) {
				case IDB_EXIT:
					if ( iExit == CLOSE ) {
						if ( indDef == -1 ) {
							rc = MessageBox(hDlg, "No Default printer specified.\n Do you want to exit Printer Setup?",
								"Warning", MB_ICONEXCLAMATION | MB_YESNO);
							if ( rc == IDNO )
								return TRUE;
						}
						SendMessage(hStc, WM_GETTEXT, sizeof(buf),
								(LPARAM)(LPSTR)buf);
						rc = (int)SendMessage(hLst, LB_FINDSTRINGEXACT, -1,
								(LPARAM)(LPSTR)buf);
						if ( indDef != -1 && rc == LB_ERR ) {
							rc = MessageBox(hDlg, "Default Printer is not in the list of installed printers.\n Do you want to exit Printer Setup?",
								"Warning", MB_ICONEXCLAMATION | MB_YESNO);
							if ( rc == IDNO )
								return TRUE;
						}

						n = (int)SendMessage(hLst, LB_GETCOUNT, 0, 0L);
						hPort = GlobalAlloc(GHND, 1024);
						lp = GlobalLock(hPort);
						GetProfileString("ports", NULL, "", lp, 1024);
						for ( i = 0; i < n; i++ ) {
							SendMessage(hLst, LB_GETTEXT, i, (LPARAM)(LPSTR)s);
							GetDevPort(&add, s);
							lp1 = lp;
							rc = 0;
							while ( *lp1 ) {
								if ( !lstrcmp(lp1, add.p2) ) {
									rc = 1;
									break;
								}
								lp1 += lstrlen(lp1) + 1;
							}
							if ( !rc ) {
								sprintf(buf, "Port %s used by %s is not installed.\nDo you want to exit Printer Setup?",
											add.p2, add.p1);
								rc = MessageBox(hDlg, buf, "Warning",
											MB_ICONEXCLAMATION | MB_YESNO);
								if ( rc == IDNO )
									return TRUE;
							}
						}

						/* Delete [devices] */
						WriteProfileString("devices", NULL, NULL);
						/* Delete device entry in [windows] */
						WriteProfileString("windows", "device", NULL);

						if ( indDef == -1 )
							WriteProfileString("windows", "device", NULL);
						else {
							SendMessage(hStc,WM_GETTEXT,sizeof(s),(LPARAM)(LPSTR)s);
							GetDevPort(&add, s);
							strcpy(str, add.p2);
							/* Driver name should be taken from CONTROL.INF */
							wsprintf(buf, "%s,%s,%s", (LPSTR)add.p1,
								(LPSTR)"PSCRIPT", (LPSTR)add.p2);

							/* Write new device entry in [windows] */
							WriteProfileString("windows", "device", (LPSTR)buf);
						}

						rc = (int)SendMessage(hLst, LB_GETCOUNT, 0, 0L);
						if ( rc )
							--rc;
						for ( ; rc >= 0; rc-- ) {
							SendMessage(hLst,LB_GETTEXT,rc,(LPARAM)(LPSTR)buf);
							GetDevPort(&add, buf);
							strcpy(str, add.p2);
							/* Driver name should be taken from CONTROL.INF */
							wsprintf(s, "%s,%s", (LPSTR)"PSCRIPT", (LPSTR)str);
							WriteProfileString("devices",(LPSTR)add.p1,(LPSTR)s);
						}
						EndDialog(hDlg, 1);
					}
					else
						EndDialog(hDlg, 0);

					break;
				case IDB_CONNECT:
					ind = (int)SendMessage(hLst, LB_GETCURSEL, 0, 0);
					SendMessage(hLst, LB_GETTEXT, ind, (LPARAM)(LPSTR)buf);
					GetDevPort(&add, buf);
					lpfnDlgProc = (DLGPROC)MakeProcInstance(
									(FARPROC)ConnectDlgProc, _hInst);
					rc = DialogBoxParam(_hInst, "Connect", hDlg, lpfnDlgProc,
									(LPARAM)&add);
					FreeProcInstance((FARPROC)lpfnDlgProc);
					if ( rc ) {
						wsprintf(buf, "%s on %s", (LPSTR)add.p1, (LPSTR)add.p2);
						SendMessage(hLst, LB_DELETESTRING, ind, 0L);
						SendMessage(hLst, LB_INSERTSTRING, ind,
									(LPARAM)(LPSTR)buf);
						SendMessage(hLst, LB_SETCURSEL, ind, 0L);
						if ( ind == indDef )
							UpdateDefaultPrinter(hStc, hLst, ind);
						ChangeExitStatus(hDlg, &iExit);
					}
					break;
				case IDB_SETUP:
					SendMessage(hLst, LB_GETTEXT, ind, (LPARAM)(LPSTR)buf);
					GetDevPort(&add, buf);
					if ( !GetProfileString("devices", add.p1, "", str,
									sizeof(str)) ) {
						sprintf(str, "Cannot get driver name for %s printer",
								add.p1);
						MessageBox(hDlg, str, "Error", MB_ICONEXCLAMATION);
						break;
					}
					strcpy(tbuf, add.p1);		/* tbuf = device name */
					strcpy(s, add.p2);			/* s = port name      */
					lp = strtok(str, ", ");		/* str = driver name  */
					strcpy(buf, str);
					strcat(buf, ".DRV");
					if ((hMod = LoadLibrary((LPSTR)buf)) <= HINSTANCE_ERROR) {
						sprintf(str, "Cannot load %s driver", (LPSTR)buf);
						MessageBox(hDlg, str, "Error", MB_ICONEXCLAMATION);
						break;
					}
					lpfnExtDevMode = GetProcAddress(hMod, "EXTDEVICEMODE");
					if ( !lpfnExtDevMode ) {
						FreeLibrary(hMod);
						sprintf(str, "Cannot run setup for %s driver",
									(LPSTR)buf);
						MessageBox(hDlg, str, "Error", MB_ICONEXCLAMATION);
						break;
					}
					rc = lpfnExtDevMode(hDlg, hMod, 0, (LPSTR)tbuf,	(LPSTR)s, 0,
								NULL, DM_OUT_DEFAULT | DM_IN_PROMPT);
					FreeLibrary(hMod);
					if ( rc == IDOK )
						ChangeExitStatus(hDlg, &iExit);
					break;
				case IDB_REMOVE:
					ind = (int)SendMessage(hLst, LB_GETCURSEL, 0, 0);
					SendMessage(hLst, LB_GETTEXT, ind, (LPARAM)(LPSTR)str);
					LoadString(_hInst, IDS_DEL_PRN_QUESTION, s, sizeof(s));
					sprintf(buf, s, str);
					rc = MessageBox(hDlg, buf, "Printer Setup", MB_ICONEXCLAMATION |
								MB_YESNO | MB_DEFBUTTON2);
					if ( rc == IDYES ) {
						SendMessage(hLst, LB_DELETESTRING, ind, 0L);
						SendMessage(hStc, WM_GETTEXT, sizeof(buf), (LPARAM)(LPSTR)buf);
						if ( !strcmp(str, buf) ) {
							SendMessage(hStc, WM_SETTEXT, 0,
										(LPARAM)(LPSTR)"No Default Printer");
							indDef = -1;
						}
						ChangeExitStatus(hDlg, &iExit);
					}
					break;
				case IDB_ADD:
					buf[0] = 0;
					add.p1 = buf;
					add.p2 = 0;
					add.flags = ADD_PRINTER;
					lpfnDlgProc = (DLGPROC)MakeProcInstance((FARPROC)AddDlgProc, _hInst);
					rc = DialogBoxParam(_hInst, "AddDialog2", hDlg, lpfnDlgProc,
							(LPARAM)&add);
					FreeProcInstance((FARPROC)lpfnDlgProc);
					if ( rc ) {
						add.flags = NEW_PRINTER;
						add.p2 = str;
						str[0] = '\0';
						lpfnDlgProc = (DLGPROC)MakeProcInstance((FARPROC)ConnectDlgProc, _hInst);
						rc = DialogBoxParam(_hInst, "Connect", hDlg, lpfnDlgProc,
									(LPARAM)&add);
						FreeProcInstance((FARPROC)lpfnDlgProc);
						if ( rc ) {
							wsprintf(s, "%s on %s", (LPSTR)buf, (LPSTR)str);
							SendMessage(hLst, LB_INSERTSTRING, ind, (LPARAM)(LPSTR)s);
							indDef++;
							ChangeExitStatus(hDlg, &iExit);
							/* Add the printer to the [devices], otherwise
							 * we'll fail if user clicks on Setup just after
							 * adding a Printer.  Because it won't find the
							 * driver. FIXME: "PSCRIPT" should be taken from
							 * elsewhere ([drivers] sec?)
							 */
							wsprintf(s,"%s,%s","PSCRIPT",add.p2);
							WriteProfileString("devices",add.p1,s);
						}
					}
					break;
				case IDB_SET_AS_DEF:
					ind = (int)SendMessage(hLst, LB_GETCURSEL, 0, 0);
					UpdateDefaultPrinter(hStc, GetDlgItem(hDlg, IDL_PRN_LIST), ind);
					indDef = ind;
					ChangeExitStatus(hDlg, &iExit);
					break;
				case IDL_PRN_LIST:
					switch ( HIWORD(lParam) ) {
						case LBN_DBLCLK:
							UpdateDefaultPrinter(hStc, hLst, ind);
							indDef = ind;
							ChangeExitStatus(hDlg, &iExit);
							break;
						case LBN_SELCHANGE:
							ind = (int)SendMessage(hLst, LB_GETCURSEL, 0, 0L);
							break;
						default:
							break;
					}
					EnableWindow(GetDlgItem(hDlg, IDB_CONNECT), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDB_SETUP), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDB_REMOVE), TRUE);
					break;
				default:
					return FALSE;
			} /* End of switch(wParam) */
			return TRUE;

		default:
			break;
	} /* End of switch(message) */
	return FALSE;
}


/*-----------------------------------------------------------------------
			ConnectDlgProc()
  -----------------------------------------------------------------------*/
BOOL CALLBACK _export
ConnectDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static LPADDSTRUCT	lpdp;
	static HWND			hLst;
	static int			ind = -1;
	DLGPROC				lpfnDlgProc;
	ADDSTRUCT			pa;
	LPSTR				lp;
	int					rc = 0;
	char				buf[BUFFER_SIZE], str[BUFFER_SIZE], s[BUFFER_SIZE];

	switch ( message ) {
		case WM_INITDIALOG:
			if ( !lParam ) {
				EndDialog(hDlg, 0);
				return FALSE;
			}
			hLst = GetDlgItem(hDlg, IDL_PORTS);
			lpdp = (LPADDSTRUCT)lParam;
			/* Get the names of all ports from win.ini */
			GetProfileString("ports", NULL, "", buf, sizeof(buf));
			if ( (lp = (LPSTR)buf) != 0 )
				while ( *lp ) {
					SendMessage(hLst, LB_ADDSTRING, 0, (LPARAM)lp);
					lp += lstrlen(lp) + 1;
				}

			if ( *lpdp->p2 ) {
				ind = (int)SendMessage(hLst, LB_FINDSTRINGEXACT, -1,
							(LPARAM)(LPSTR)lpdp->p2);
				if ( ind == LB_ERR ) {
					sprintf(buf, "Port %s is not installed.\nAdded automatically.",
							lpdp->p2);
					MessageBox(hDlg, buf, "Warning", MB_ICONINFORMATION);
					SendMessage(hLst, LB_ADDSTRING, 0, (LPARAM)(LPSTR)lpdp->p2);
					WriteProfileString("ports", lpdp->p2, "");
					ind = (int)SendMessage(hLst, LB_GETCOUNT, 0, 0L) - 1;
				}
				SendMessage(hLst, LB_SETCURSEL, ind, 0L);
				GetProfileString("Ports", lpdp->p2, "", buf, sizeof(buf));
				if ( !buf[0] )
					strcpy(buf, "None");
			}
			else
				if ( lpdp->flags == NEW_PRINTER )
					buf[0] = '\0';
				else {
					MessageBox(hDlg, "Port is not specified", "Error",
							MB_ICONEXCLAMATION);
					EndDialog(hDlg, 0);
					return TRUE;
				}

			SendDlgItemMessage(hDlg, IDS_ALIAS_TXT, WM_SETTEXT, 0,
						(LPARAM)(LPSTR)buf);
 			SendDlgItemMessage(hDlg, IDS_PRINTER, WM_SETTEXT, 0,
						(LPARAM)(LPSTR)lpdp->p1);
			return TRUE;

		case WM_COMMAND:
			switch ( wParam ) {
				case IDOK:
					ind = (int)SendDlgItemMessage(hDlg, IDL_PORTS, LB_GETCURSEL, 0, 0L);
					if ( ind == -1 ) {
						MessageBeep(MB_ICONEXCLAMATION);
						return TRUE;
					}
					rc = 1;
					break;
				case IDCANCEL:
					rc = 0;
					break;
				case IDL_PORTS:
					ind = (int)SendDlgItemMessage(hDlg, IDL_PORTS, LB_GETCURSEL, 0, 0L);
					if ( HIWORD(lParam) == LBN_DBLCLK )
						break;		/* Try to leave dialog box */
					SendMessage(hLst, LB_GETTEXT, ind, (LPARAM)(LPSTR)buf);
					GetProfileString("Ports",buf,"None",buf,sizeof(buf));
					SendDlgItemMessage(hDlg, IDS_ALIAS_TXT, WM_SETTEXT, 0,
							(LPARAM)(LPSTR)buf);
					return TRUE;
				case IDB_ADD_PORT:
					pa.flags = ADD_PORT;
					pa.p1 = buf;
					pa.p2 = 0;
					buf[0] = '\0';
					lpfnDlgProc = (DLGPROC)MakeProcInstance(
							(FARPROC)AddDlgProc, _hInst);
					rc = DialogBoxParam(_hInst, "AddDialog1", hDlg, lpfnDlgProc,
							(LPARAM)&pa);
					FreeProcInstance((FARPROC)lpfnDlgProc);
					if ( rc ) {
						SendMessage(hLst, LB_INSERTSTRING, ind, (LPARAM)
								(LPSTR)buf);
						GetProfileString("Ports", buf, "None", str, sizeof(str));
						SendDlgItemMessage(hDlg, IDS_ALIAS_TXT, WM_SETTEXT, 0,
								(LPARAM)(LPSTR)str);
						WriteProfileString("ports", buf, "");
						EnableWindow(GetDlgItem(hDlg, IDCANCEL), FALSE);
					}
					return TRUE;
				case IDB_REMOVE_PORT:
					SendMessage(hLst, LB_GETTEXT, ind, (LPARAM)(LPSTR)str);
					LoadString(_hInst, IDS_DEL_PORT_QUESTION, s, sizeof(s));
					sprintf(buf, s, str);
					rc = MessageBox(hDlg, buf, "Printer Setup", MB_ICONEXCLAMATION |
								MB_YESNO | MB_DEFBUTTON2);
					if ( rc == IDYES ) {
						SendMessage(hLst, LB_DELETESTRING, ind, 0L);
						WriteProfileString("ports", str, NULL);
						EnableWindow(GetDlgItem(hDlg, IDCANCEL), FALSE);
						SendDlgItemMessage(hDlg, IDS_ALIAS_TXT, WM_SETTEXT, 0,
								(LPARAM)(LPSTR)"");
						ind = -1;	/* No selection in ports listbox */
					}
					return TRUE;
				case IDB_ALIAS_BUTTON:
					if ( (ind = (int)SendMessage(hLst, LB_GETCURSEL, 0, 0)) == -1 )
						return FALSE;
					SendMessage(hLst, LB_GETTEXT, ind, (LPARAM)(LPSTR)buf);
					SendDlgItemMessage(hDlg, IDS_ALIAS_TXT, WM_GETTEXT, sizeof(str),
									(LPARAM)(LPSTR)str);
					pa.flags = ADD_ALIAS;
					pa.p1 = str;
					pa.p2 = buf;
					lpfnDlgProc = (DLGPROC)MakeProcInstance(
							(FARPROC)AddDlgProc, _hInst);
					rc = DialogBoxParam(_hInst, "AddDialog1", hDlg, lpfnDlgProc,
							(LPARAM)&pa);
					FreeProcInstance((FARPROC)lpfnDlgProc);
					if ( rc ) {
						if ( !str[0] ) {
							rc = FALSE;
							strcpy(str, "None");
						}
						#ifdef PC_BUILD
						if ( strcmpi(str, "None") ) {
						#else
						if ( strcasecmp(str, "None") ) {
						#endif
							WriteProfileString("Ports", pa.p2, pa.p1);
							EnableWindow(GetDlgItem(hDlg, IDCANCEL), FALSE);
						}
						SendDlgItemMessage(hDlg, IDS_ALIAS_TXT, WM_SETTEXT, 0,
											(LPARAM)(LPSTR)str);
					}
					return TRUE;
				default:
					return FALSE;
			}
			if ( rc ) {
				ind = (int)SendMessage(hLst, LB_GETCURSEL, 0, 0);
				SendMessage(hLst, LB_GETTEXT, ind, (LPARAM)(LPSTR)lpdp->p2);
				EndDialog(hDlg, 1);
			}
			else
				EndDialog(hDlg, 0);
			return TRUE;
	}
	return FALSE;
}


/*-----------------------------------------------------------------------
			AddDlgProc()

	This dialog procedure manages AddPrinter, AddPort, and AddAlias
	dialog boxes. WM_INITDIALOG expects pointer to ADDSTRUCT structure
	with Flag determining what this dialog adds, and pointers to
	caption bar and initial string in edit control.
  -----------------------------------------------------------------------*/
BOOL CALLBACK _export
AddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static LPADDSTRUCT	lpAdd;
	static HWND			hEdt;	/* Edit control window handle */
	DLGPROC				lpfnDlgProc;
	char				buf[BUFFER_SIZE];
	int					rc;

	switch ( message ) {
		case WM_INITDIALOG:
			if ( !lParam ) {
				MessageBox(GetParent(hDlg), "Cannot open dialog box", "Error",
							MB_OK);
				EndDialog(hDlg, 0);
				return FALSE;
			}
			lpAdd = (LPADDSTRUCT)lParam;
			hEdt  = GetDlgItem(hDlg, IDE_EDIT);

			if ( lpAdd->p2 )
				SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM)(LPSTR)lpAdd->p2);
			else {
				/* Load dialog caption string */
				LoadString(_hInst, LOWORD(lpAdd->flags) + STR_CAPTION, buf,
							sizeof(buf));
				SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM)(LPSTR)buf);
			}
			/* Load the name of static control accompanying edit control */
			LoadString(_hInst, LOWORD(lpAdd->flags) + STR_EDIT, buf, sizeof(buf));
			SendDlgItemMessage(hDlg, IDS_EDIT_NAME, WM_SETTEXT, 0, (LPARAM)(LPSTR)buf);
			/* Set text of edit control */
			if ( lpAdd->p1 )
				SendMessage(hEdt, WM_SETTEXT, 0, (LPARAM)(LPSTR)lpAdd->p1);
			return TRUE;

		case WM_COMMAND:
			switch ( wParam ) {
				case IDOK:
					SendMessage(hEdt, WM_GETTEXT, 80, (LPARAM)(LPSTR)lpAdd->p1);
					EndDialog(hDlg, 1);
					break;
				case IDCANCEL:
					EndDialog(hDlg, 0);
					break;
				case IDB_BROWSE:
				        strcpy(buf,"");
				        lpfnDlgProc = (DLGPROC)MakeProcInstance((FARPROC)BrowseDlgProc, _hInst);
					rc = DialogBoxParam(_hInst, "BrowseDialog", hDlg, lpfnDlgProc,
							(LPARAM)(LPSTR)buf);
					FreeProcInstance((FARPROC)lpfnDlgProc);
					if ( rc ) {
					    SendMessage(hEdt, WM_SETTEXT, 0, 
							(LPARAM)(LPSTR)buf);
					    
					}
					break;
				default:
					return FALSE;
			}
			return TRUE;

		default:
			break;
	}
	return FALSE;
}


void
UpdateDefaultPrinter(HWND hStc, HWND hLst, int ind)
{
	char	buf[128];

	SendMessage(hLst, LB_GETTEXT, ind, (LPARAM)(LPSTR)buf);
	SendMessage(hStc, WM_SETTEXT, 0, (LPARAM)(LPSTR)buf);
}


void
GetDevPort(ADDSTRUCT *pdp, char *s)
{
	pdp->p1  = s;
	pdp->p2 = strstr(s, " on ");
	if (pdp->p2) {
	    *pdp->p2 = '\0';
	    pdp->p2 += 4;
	} else {
	    pdp->p2 = pdp->p1;
	    *pdp->p2 = '\0';
	}	    
}


void
ChangeExitStatus(HWND hDlg, int *iExitStatus)
{
	*iExitStatus = CLOSE;
	SendDlgItemMessage(hDlg, IDB_EXIT, WM_SETTEXT, 0, (LPARAM)(LPSTR)"Close");
}


BOOL CALLBACK _export
BrowseDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND			hEdt;
    static HWND			hLBox;
    static LPSTR		lpName;
    HINSTANCE hDriver;
    FARPROC   lpfnQueryNames;
    HGLOBAL hDeviceNames;
    LPSTR lpDeviceNames;
    int nPrinters, i;
    char buf[256];

    switch ( message ) {
    case WM_INITDIALOG:
	hEdt  = GetDlgItem(hDlg, IDS_PRINTERS_TXT);
	SendMessage(hEdt, WM_SETTEXT, 0, lParam);
	lpName = (LPSTR) lParam;
	
	hLBox  = GetDlgItem(hDlg, IDL_PRINTERS);
	if (!(hDriver = LoadDriver("PSCRIPT"))) { // Should read PSCRIPT from .winrc
	    sprintf(buf, "Failed to load %s driver\nAbort.","PSCRIPT");
	    MessageBox(hDlg, buf, "Error", MB_ICONINFORMATION);
	    EndDialog(hDlg,0);
	    return FALSE;
	}
	if (!(lpfnQueryNames = GetProcAddress(hDriver,"QueryDeviceNames"))) {
#ifdef DEBUG
	    sprintf(buf, "Failed to find QueryDeviceNames in %s driver\nAbort.","PSCRIPT");
	    MessageBox(hDlg, buf, "Error", MB_ICONINFORMATION);
#endif
	    EndDialog(hDlg,0);
	    return FALSE;
	}
	
	nPrinters = (*lpfnQueryNames)((HANDLE)0, (LPSTR)NULL);
	/* 64 is taken from the PSCRIPT driver : may be more in others ? */
	hDeviceNames  = (HGLOBAL) GlobalAlloc(GHND, nPrinters * 64);
	lpDeviceNames = GlobalLock(hDeviceNames);

	nPrinters = (*lpfnQueryNames)((HANDLE)0, (LPSTR)lpDeviceNames);
	
	for (i=0; i<nPrinters; i++, lpDeviceNames+=64) {
	    SendMessage(hLBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)lpDeviceNames);
	}	

	GlobalUnlock(hDeviceNames);
	GlobalFree(hDeviceNames);	
	return TRUE;

    case WM_COMMAND:
	switch (GET_WM_COMMAND_ID(wParam, lParam)) {
	case IDOK:
	    SendMessage(hEdt, WM_GETTEXT, 80, (LPARAM)(LPSTR)lpName);
	    EndDialog(hDlg, 1);
	    break;
	case IDCANCEL:
	    EndDialog(hDlg, 0);
	    break;
	case IDL_PRINTERS:
	    switch(GET_WM_COMMAND_CMD(wParam, lParam)) {
	    case LBN_SELCHANGE:
		i = (int) SendMessage(hLBox, LB_GETCURSEL, 0, 0);
		SendMessage(hLBox, LB_GETTEXT, i, (LPARAM)(LPSTR)lpName);
		SendMessage(hEdt, WM_SETTEXT, 0, (LPARAM)lpName);
		break;
	    default:
		break;
	    }
	default:
	    return FALSE;
	}
	return TRUE;
	
    default:
	break;
    }
    return FALSE;    
}

