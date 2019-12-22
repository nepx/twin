
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "windows.h"
#include "kerndef.h"
#include "Kernel.h"
#include "Endian.h"
#include "Log.h"
#include "BinTypes.h"
#include "Resources.h"
#include "Module.h"
#include "LoadEXE.h"
#include "DPMI.h"
//#include "Edit.h"
#include "Classes.h"
#include "dos.h"
#include "compat.h"
#include "LoadDLL.h"
#include "Driver.h"
#include "invoke_binary.h"
#include "KrnTask.h"

extern MODULEINFO *GetModuleFromSelector(WORD);

int LoadModuleFromNEFile(MODULEINFO *, LPUSERDLL,  LPSTR , WORD );
int LoadModuleFromPEFile(MODULEINFO *, LPUSERDLL,  LPSTR , WORD );

void FileErrorMB(WORD , LPSTR );

int FillUserDLL(LPUSERDLL);

int
LoadModuleFromFile(MODULEINFO *modinfo, LPSTR lpszFileName, 
		   LPSTR lpCommandLine, WORD wFlags)
{
    char LocalFileName[256];
    char szDrvName[80] = "";
    char szDirName[80] = "";
    char szBaseName[80] = "";
    char szExtName[80] = "";
    HFILE hFile;
    LPUSERDLL lpUserDLL;
    OFSTRUCT ofs;
    int ret;

    _splitpath(lpszFileName,szDrvName,szDirName,szBaseName,szExtName);

    strcpy(LocalFileName,lpszFileName);

    if(strlen(szExtName) == 0) {
        strcpy(LocalFileName,szBaseName);

	if (wFlags & ILL_APPL)
	    strcat(LocalFileName,".exe");
	else
	    strcat(LocalFileName,".dll");
    }

    lpszFileName = LocalFileName;

    /* this will search all appropriate places... */
    if((hFile = OpenFile(lpszFileName,&ofs,OF_READ|OF_SEARCH)) == HFILE_ERROR) {
	/* use errormode here */
	if (!(wFlags & ILL_NOERROR)) {
	    if (wFlags & ILL_INTERMED)
		FileErrorMB(ILL_INTERMED,lpszFileName);
	    else
		FileErrorMB(ILL_FINAL,lpszFileName);
	} else
		FileErrorMB(ILL_INTERMED,lpszFileName);
	return (int)NULL;
    }

    modinfo->lpFileName = WinStrdup(ofs.szPathName);

    lpUserDLL = (LPUSERDLL)WinMalloc(sizeof(USERDLL));
    memset((LPSTR)lpUserDLL,'\0',sizeof(USERDLL));

    lpUserDLL->hfExeFile = hFile;

    /* 
     * FillUserDLL returns < 0 on file read error
     * returns == 0 if NOT win16 application
     * returns >  0 if     win16 application
     */

    if ((ret = FillUserDLL(lpUserDLL)) < 0) {
	    WinFree(lpUserDLL);
	    return ret;
    }

    if(ret)
    	    return LoadModuleFromNEFile(modinfo,lpUserDLL,lpCommandLine,wFlags);

    return LoadModuleFromPEFile(modinfo,lpUserDLL,lpCommandLine,wFlags);
}

#define SRC_SEG_COUNT 1	/* native modules, unlike binary ones, */
			/* always have only one segment	       */

extern BOOL TWIN_LocalInit(UINT,UINT,UINT);

void
LoadModuleFromDscr(MODULEINFO *modinfo, LPMODULETAB mod_tab, WORD wFlags)
{
    MODULEDSCR *mod_dscr;
    int nNumEntries;
    HGLOBAL hGlobal;
    LPMEMORYINFO lpMemInfo;
    LPBYTE lpJumpTable;
    LPBYTE lpb;
    WORD wSel;
    HINSTANCE hInst = 0;
    SEGTAB *lpSegTable;
    WORD i;
    LONGPROC init_func;
    LPSTR lpCmdLine = modinfo->lpFileName;

    if (!modinfo->hInst) {
	
	hInst = GlobalAlloc(GMEM_MOVEABLE,0x110);
	modinfo->hInst = hInst;
	modinfo->nRefCnt++;

	if (!CreateDataInstance(hInst,(HMODULE)modinfo->ObjHead.hObj,
			GetCurrentTask())) {
	    FatalAppExit(0,"Cannot create data instance\n");
	}

	lpb = (LPBYTE) GlobalLock(hInst);
	wSel = AssignSelector(lpb, 0, TRANSFER_DATA, 0x110);
	SetSelectorHandle(wSel, hInst);
	if (TWIN_LocalInit(wSel, 0, 0x100))
	    modinfo->wDGROUP = wSel;

    }
    mod_dscr = mod_tab->dscr;
    if (!modinfo->ResourceTable && mod_dscr->resource)
	modinfo->ResourceTable = (TYPEINFO *)(mod_dscr->resource);
    if (!modinfo->lpEntryTable && mod_dscr->entry_table)
	modinfo->lpEntryTable = mod_dscr->entry_table;
    if (!modinfo->map_table && mod_dscr->map_table) {
	modinfo->map_table = mod_dscr->map_table;
    }

    if (1) {
	modinfo->wSegCount = SRC_SEG_COUNT;
	modinfo->wSelBase = AssignSelRange(SRC_SEG_COUNT);
	modinfo->lpEntryTable = mod_dscr->entry_table;
	if ((lpSegTable = mod_dscr->seg_table)) {
	    if (mod_tab->flags & MODULE_SYSTEM) {  /* system DLL */
		for(nNumEntries = 0;modinfo->lpEntryTable[nNumEntries].name;
				nNumEntries++);
		hGlobal = GlobalAlloc(GHND,nNumEntries*8);
		lpJumpTable = (LPBYTE)GlobalLock(hGlobal);
		AssignSelector(lpJumpTable,(modinfo->wSelBase)<<3,
				TRANSFER_CALLBACK,nNumEntries*8);
		SetModuleIndex((UINT)(modinfo->wSelBase)<<3,modinfo->bModIndex);
		SetSelectorHandle((UINT)(modinfo->wSelBase)<<3,hGlobal);

    /* We store the pointer to the CONV-TARG table at the beginning */
    /* of the new segment */

		*((long *)lpJumpTable) = (long)lpSegTable[0].image;
	    }
	    else	/* translated code module -- should this be removed? */
		for (i=0; i< modinfo->wSegCount; i++) {
		    AssignSelector((LPBYTE)lpSegTable[i].image,
			(modinfo->wSelBase+i)<<3,
			(BYTE)lpSegTable[i].transfer,
			(DWORD)lpSegTable[i].size);
		    SetModuleIndex((UINT)(modinfo->wSelBase+i)<<3,
			modinfo->bModIndex);
		    hGlobal = GlobalAlloc(GHND,0);
		    if ((lpMemInfo = GETHANDLEINFO(hGlobal))) {
			lpMemInfo->lpCore = lpSegTable[i].image;
			RELEASEHANDLEINFO(lpMemInfo);
		    }
		    SetSelectorHandle((UINT)(modinfo->wSelBase+i)<<3,hGlobal);
		}
	}
    }

    if (mod_dscr->entry_table) 
	    init_func = (LONGPROC)mod_dscr->entry_table[0].fun;
    else
	    init_func = 0;

    if (wFlags & ILL_APPL) {
	    HTASK hTask;
	    TASKINFO *lpTaskInfo;
            HINSTANCE hPrevInst=0;
	    WORD nCmdShow = SW_SHOWNORMAL;

	    // force the current task to now be hInstance
	    // and everything that goes with it...
    	    hTask = GetCurrentTask();
 	    lpTaskInfo = (TASKINFO *)GETTASKINFO(hTask);
	    lpTaskInfo->hInst = modinfo->hInst;

	    /* Init routine is WinMain */
	    if (init_func) {
		init_func(modinfo->hInst, hPrevInst, lpCmdLine, nCmdShow);
	    }
    } else {
	    /* Init routine is LibMain */
	    if (init_func)
		init_func(modinfo->hInst, 0, 0, lpCmdLine);
    }
}

int
LoadModuleFromPEFile(MODULEINFO *modinfo,LPUSERDLL lpUserDLL,LPSTR lpCmdLine,WORD wFlags)
{
	int	ret = 0;
#ifdef TWIN_STATICPE
	/* if binding libpe32.so at compile time 
	 * use this option instead of the above
   	 */
	ret = ExecPE(modinfo,lpUserDLL,lpCmdLine,wFlags);
#else
	static HINSTANCE hInst;
	static FARPROC	 opfn;

	if(hInst == 0)
		hInst = LoadLibrary("pe32");
	if(hInst == 0) {
		char 	buffer[256];
		strncpy(buffer,"pe32",200);
		strcat(buffer,"File not found.");

    		MessageBox((HWND)NULL,buffer,"LoadPE error",MB_OK);
		return 0;
	}
	if(opfn == 0)
		opfn = GetProcAddress(hInst,"EXECPE");

	if(opfn) {
		ret = opfn (modinfo,lpUserDLL,lpCmdLine,wFlags);
  	} 
#endif

	return ret; 
}

void
FileErrorMB(WORD wFlags, LPSTR lpszFileName)
{
    char buf[BUFFER];

    if (wFlags & ILL_INTERMED) {
	wsprintf(buf,"Cannot find %s\n",lpszFileName);
	MessageBox((HWND)NULL,buf,"File Error",MB_OK);
    }
    else {	/* wFlags & ILL_FINAL */
	wsprintf(buf, "%s %s %s %s %s %s",
		"Cannot find file",
		lpszFileName,
		"(or one of its components).",
		"Check to ensure the path and filename are ",
		"correct and that all required libraries are ",
		"available.");
	MessageBox((HWND)NULL,buf,"Application Execution Error",
			MB_OK|MB_ICONEXCLAMATION);
    }
}

LPSTR
GetProcName(WORD wSel, WORD wOrd)
{
    MODULEINFO *modinfo;

    if (!(modinfo = GetModuleFromSelector(wSel)))
	return NULL;

    if (modinfo->lpEntryTable)
	return modinfo->lpEntryTable[wOrd].name;
    else
	return NULL;
}

void
LogProcName(WORD wSel, WORD wOff, WORD wAction)
{

    	static LPSTR       lpProcName;

	LPSTR	    lpModName;
    	char buffer[80];

	if (lpProcName == 0) {
	    sprintf(buffer,"module(%x:%x)",wSel,wOff);
	    lpProcName = (LPSTR) buffer;
	}

	if (wAction == 1) {
		lpProcName = (LPSTR) GetProcName(wSel,wOff>>3);
		lpModName = (LPSTR) GetProcName(wSel,0);
		LOGSTR((LF_BINCALL,"%s:%s\n",lpModName,lpProcName));
	} else {
		LOGSTR((LF_BINRET,"AX=%x DX=%x\n",
			wSel,wOff));
	}
}

