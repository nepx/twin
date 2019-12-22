/*    
	LoadDLL.c	2.62
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
#include "Classes.h"
#include "dos.h"
#include "LoadDLL.h"
#include "Driver.h"
#include "invoke_binary.h"
#include "KrnTask.h"
#include "ldt.h"
#include "compat.h"

#ifdef TWIN_BIN16
static void InitClassBinProcs();
static BOOL InitClassBinEnumProc(HCLASS32,LPWNDCLASS,LPARAM);

static void PatchExportedPrologs(LPUSERDLL);
static void ProcessIteratedData(SEGTAB *);
static void ProcessRelocs(LPMODULEINFO,LPBYTE,LPBYTE,WORD,UINT,UINT);
static void PatchSegmentPrologs(MODULEINFO *, int);
static int ReadResidTable(LPUSERDLL, ENTRYTAB *);
static int ReadNonResidTable(LPUSERDLL, ENTRYTAB *);

static ENTRYTAB *ProcessEntryTable(USERDLL *);
static void LEXE_perror(int errnum);
static void CreatePSP(LPSTR lpCmdLine, HTASK hTask);
#endif

int FillUserDLL(LPUSERDLL);

extern HANDLE TWIN_LoadFile(WORD,LPSTR,LPARAM);
extern LPSTR strpbrkr(LPCSTR,LPCSTR);
extern TYPEINFO *ReadResourceTable(HINSTANCE,USERDLL *);
extern MODULEINFO *GetModuleFromSelector(WORD);

void FileErrorMB(WORD , LPSTR );

extern ENV *envp_global;
extern WORD return_thunk_selector;

#ifdef TWIN_BIN16
extern LRESULT	hsw_common_nat_to_bin(HWND,UINT,WPARAM,LPARAM);
extern LRESULT	hsw_common_bin_to_nat(HWND,UINT,WPARAM,LPARAM);
extern LRESULT	hsw_listbox_nat_to_bin(HWND,UINT,WPARAM,LPARAM);
extern LRESULT	hsw_listbox_bin_to_nat(HWND,UINT,WPARAM,LPARAM);
extern LRESULT	hsw_combobox_nat_to_bin(HWND,UINT,WPARAM,LPARAM);
extern LRESULT	hsw_combobox_bin_to_nat(HWND,UINT,WPARAM,LPARAM);
extern LRESULT	hsw_edit_nat_to_bin(HWND,UINT,WPARAM,LPARAM);
extern LRESULT	hsw_edit_bin_to_nat(HWND,UINT,WPARAM,LPARAM);
extern LRESULT	hsw_mdiclient_nat_to_bin(HWND,UINT,WPARAM,LPARAM);
extern LRESULT	hsw_mdiclient_bin_to_nat(HWND,UINT,WPARAM,LPARAM);
#endif

extern LRESULT  EditMemoryBin(HWND,UINT,WPARAM,LPARAM);
extern FARPROC  OEM_GetProcAddress(MODULEINFO *,UINT);

extern void FreeTask(HANDLE);
extern BOOL TWIN_LocalInit(UINT,UINT,UINT);

extern void invoke_binary(void);
UINT GetPSPSelector();

BOOL InitBinary(LPSTR,UINT);

HANDLE TWIN_DebugGetSelectorHandle(WORD);
BOOL LoadDuplicateSegment(UINT , UINT , MODULEINFO *);

extern FARPROC lpfnOEMGetPAddr;
extern EDITMEMORYPROC lpfnEditMemBin;

extern WNDPROC lpfnDefaultBinToNat;
extern WNDPROC lpfnDefaultNatToBin;

#define STACKSIZE 0x1000

#ifdef TWIN_BIN16
static int
ReadResidTable(USERDLL *lpDLL, ENTRYTAB *api)
{
    int i,j,count = 0;
    int len;
    LPBYTE str;
    LPSTR lpString;
    char buf[BUFFER];
    BOOL bByteIndex;

    if ((!lpDLL->lpHeaderImage) || (!lpDLL->wResTblOffset)) {
	return count;
    }
    str = (LPBYTE)lpDLL->lpHeaderImage + lpDLL->wResTblOffset;

   /***********************************************************
    *
    * It seems that if there are less than 0xff entries, the ordinal
    * is perceived as a byte value;  example -- NWLOCALE.DLL 
    *
    **********************************************************/
    bByteIndex = (lpDLL->wNumEntries > 0xff) ? FALSE : TRUE;

    while ((len = (int)(*str++))) {
	memcpy(buf,(LPSTR)str,len);
	str += len;
	i = (int)GETWORD(str);
	if (bByteIndex) 
	    i &= 0xff;
	lpString = WinMalloc(len+1);
	for(j=0;j<len;j++)
	    lpString[j] = toupper(buf[j]);
	lpString[len] = '\0';   
	if (!count++) 	 /* first entry -- module name */
	    lpDLL->lpDLLName = lpString;
	else 
	    api[i].name = lpString;
	str += 2;
    }
    return count;
}

static int
ReadNonResidTable(USERDLL *lpDLL, ENTRYTAB *api)
{
    int i,count = 0,nBytesRead = 0;
    int len;
    LPBYTE str;
    LPSTR lpString;
    WORD wSize;

    wSize = lpDLL->wNResTblSize;
    if (wSize) {
	str = (LPBYTE)lpDLL->lpNonResTable;
	while ((len = (int)(*str++)) && (nBytesRead < (int)wSize)) {
	    lpString = WinMalloc(len+1);
	    memcpy(lpString,(LPSTR)str,len);
	    lpString[len] = '\0';
	    if (count++) {
	        i = (int)GETWORD(str + len);
	        api[i].name = lpString;
	    }
	    else 
		lpDLL->lpDLLDescription = lpString; 
	    str += len + 2;
	    nBytesRead += len + 3;
	}
    }
    return count;
}

static ENTRYTAB *
ProcessEntryTable(USERDLL *lpDLL)
{
    ENTRYTAB *lpEntryTable;
    LPBYTE lpEntryData,ptr;
    WORD wNumEntries = 1;
    BYTE bBundleCount,bBundleFlag;
    static char lpEmptyString[] = "";
    int i,j;

    lpEntryData = lpDLL->lpHeaderImage + lpDLL->wEntryTblOffset;
    ptr = lpEntryData;
    while (ptr < lpEntryData + lpDLL->wEntryTblSize) {
	bBundleCount = *ptr++;
	if (!bBundleCount)
	    break;
	bBundleFlag = *ptr++;
	wNumEntries += bBundleCount;
	if (bBundleFlag)
	    ptr += bBundleCount * ((bBundleFlag == 0xff)?6:3);
    }
    if (!(lpEntryTable = (ENTRYTAB *)WinMalloc
			(sizeof(ENTRYTAB)*(wNumEntries+1))))
	return lpEntryTable;
    memset((LPSTR)lpEntryTable,'\0', sizeof(ENTRYTAB)*(wNumEntries+1));
    for (i=0; i<(int)wNumEntries; i++)
	lpEntryTable[i].name = lpEmptyString;
    ptr = lpEntryData;
    i = 1;
    while ((bBundleCount = *ptr++)) {
	bBundleFlag = *ptr++;
	for (j=0; j<(int)bBundleCount; j++,i++)
	    if (bBundleFlag == 0xff) {
		lpEntryTable[i].sel = (WORD)(*(ptr+3)<<3);
		lpEntryTable[i].off = GETWORD(ptr+4);
		ptr += 6; 
	    }
	    else if (bBundleFlag) {
		lpEntryTable[i].off = GETWORD(ptr+1);
		lpEntryTable[i].sel = (WORD)bBundleFlag<<3;
		ptr += 3;
	    }
    }
    lpDLL->wNumEntries = wNumEntries;
    return lpEntryTable;
}

static void
PatchExportedPrologs(LPUSERDLL lpUserDLL)
{
    LPBYTE	lpEntryT;
    LPBYTE	lpSegData;
    WORD wBundleCount;
    WORD wBundleEntry;
    WORD wBundleSegment;
    WORD wEntrySegment;
    WORD wEntryOffset;
    WORD wEntryFlags;
    WORD i;

    lpEntryT = lpUserDLL->lpEntryTable;
    while (1) {
	wBundleCount = (WORD)(*lpEntryT++);
	if (!wBundleCount) return;
	wBundleSegment = (WORD)(*(lpEntryT++));
	if (wBundleSegment) {
	    wBundleEntry = (wBundleSegment == 0xff)?MOVABLESEG_ENTRYSZ:
				FIXEDSEG_ENTRYSZ;
	    for (i = 0; i < wBundleCount; i++) {
		wEntryFlags = (WORD)(*(lpEntryT));
		if ((wEntryFlags & ENTRYF_EXPORTED) &&
		    (((lpUserDLL->wProgramFlags & HEADER_SINGLEDATA) && 
		     (wEntryFlags & ENTRYF_SHAREDDATA)) ||
		    (lpUserDLL->wProgramFlags & HEADER_MULTIPLEDATA))) {
		    if (wBundleSegment == 0xff) {
			wEntrySegment = (WORD)(*(lpEntryT+3));
			wEntryOffset = GETWORD(lpEntryT+4);
		    }
		    else {
			wEntrySegment = wBundleSegment;
			wEntryOffset = GETWORD(lpEntryT+1);
		    }
		    lpSegData = GetPhysicalAddress((UINT)
			((wEntrySegment-1+lpUserDLL->wSelectorBase)<<3));
		    if (lpSegData && lpSegData != (LPBYTE)-1) {
			/* segment is valid and pre-loaded */
			lpSegData += wEntryOffset;
		        if ((
				/* MOV AX,DS */
			     (*lpSegData == 0x8c && *(lpSegData+1) == 0xd8) ||
				/* PUSH DS; POP AX */
			     (*lpSegData == 0x1e && *(lpSegData+1) == 0x58) ||
				/* NOP; NOP */
			     (*lpSegData == 0x90 && *(lpSegData+1) == 0x90)
			     ) && *(lpSegData+2) == 0x90) {
			    if (wEntryFlags & ENTRYF_SHAREDDATA)
			    {
				*lpSegData = 0xb8;
				PUTWORD(lpSegData+1,
					((lpUserDLL->wAutoData-1+
					  lpUserDLL->wSelectorBase) << 3) | 
					0x7);
			    }
			    else
			    {
				lpSegData[0] = 0x90; /* NOP */
				lpSegData[1] = 0x90; /* NOP */
				lpSegData[2] = 0x90; /* NOP */
			    }
			}
			else if ( /* ENTER xxxx,yy; PUSH DS; MOV DS,AX */
			    *lpSegData == 0xc8 && *(lpSegData+4) == 0x1e &&
			    *(lpSegData+5) == 0x8e && *(lpSegData+6) == 0xd8) {
			    *(lpSegData+5) = 0x16; /* PUSH SS */
			    *(lpSegData+6) = 0x1f; /* POP DS */
			}
		    }
		}
		lpEntryT += wBundleEntry;
	    }
	}
    }
}

static void
PatchSegmentPrologs(MODULEINFO *modinfo, int nSegNum)
{
    LPBYTE	lpEntryT;
    LPBYTE	lpSegData;
    WORD wBundleCount;
    WORD wBundleEntry;
    WORD wBundleSegment;
    WORD wEntrySegment;
    WORD wEntryOffset;
    WORD wEntryFlags;
    WORD i;

    lpEntryT = (LPBYTE)modinfo->lpDLL->lpEntryTable;
    while (1) {
	wBundleCount = (WORD)(*lpEntryT++);
	if (!wBundleCount) return;
	wBundleSegment = (WORD)(*(lpEntryT++));
	if (wBundleSegment) {
	    wBundleEntry = (wBundleSegment == 0xff)?MOVABLESEG_ENTRYSZ:
				FIXEDSEG_ENTRYSZ;
	    for (i = 0; i < wBundleCount; i++) {
		wEntryFlags = (WORD)(*(lpEntryT));
		if ((wEntryFlags & ENTRYF_EXPORTED) &&
		    (((modinfo->wProgramFlags & HEADER_SINGLEDATA) && 
		     (wEntryFlags & ENTRYF_SHAREDDATA)) ||
		    (modinfo->wProgramFlags & HEADER_MULTIPLEDATA))) {
		    if (wBundleSegment == 0xff) {
			wEntrySegment = (WORD)(*(lpEntryT+3));
			wEntryOffset = GETWORD(lpEntryT+4);
		    }
		    else {
			wEntrySegment = wBundleSegment;
			wEntryOffset = GETWORD(lpEntryT+1);
		    }
		    if (nSegNum == (int)wEntrySegment) {
			lpSegData = GetPhysicalAddress((UINT)
				((wEntrySegment-1+modinfo->wSelBase)<<3))
				+ wEntryOffset;
			if (((*lpSegData == 0x8c && *(lpSegData+1) == 0xd8) ||
			    (*lpSegData == 0x1e && *(lpSegData+1) == 0x58)) &&
				*(lpSegData+2) == 0x90) {
			    if (wEntryFlags & ENTRYF_SHAREDDATA)
			    {
				*lpSegData = 0xb8;
				PUTWORD(lpSegData+1,modinfo->wDGROUP);
			    }
			    else
			    {
				lpSegData[0] = 0x90; /* NOP */
				lpSegData[1] = 0x90; /* NOP */
				lpSegData[2] = 0x90; /* NOP */
			    }
			}
			else if ( /* ENTER xxxx,yy; PUSH DS; MOV DS,AX */
			    *lpSegData == 0xc8 && *(lpSegData+4) == 0x1e &&
			    *(lpSegData+5) == 0x8e && *(lpSegData+6) == 0xd8) {
			    *(lpSegData+5) = 0x16; /* PUSH SS */
			    *(lpSegData+6) = 0x1f; /* POP DS */
			}
		    }
		}
		lpEntryT += wBundleEntry;
	    }
	}
    }
}

#define RELOC_SEL(m,s) (((s) == uiSegNum) ? (uiSel|0x7) : DLL_SEL((m),(s)))

static void
ProcessRelocs(LPMODULEINFO lpmi,LPBYTE lpSegData,
		LPBYTE lpRelocData,WORD wRelocCount,
	        UINT uiSegNum, UINT uiSel)
{
    WORD wCount;
    LPBYTE lpRelocPtr;
    LPUSERDLL lpUserDLL = lpmi->lpDLL;
    BYTE bFlag = 0, bSource = 0;
    WORD wSourceOffset,wModIndex,wDLLIndex,wProcNum;
    WORD wChainOffset;
    WORD wSegNum,wOffset;
    WORD wNameOffset,wNameLength;
    WORD wAddValue;
    HANDLE hModule;
    LPMODULEINFO modinfo;
    register int i;
    char buf[BUFFER];
    ENTRYTAB *lpEntry;
    DWORD dwWinFlags;

    for (wCount = 0,lpRelocPtr = lpRelocData;
	 wCount < wRelocCount;
	 wCount++,lpRelocPtr += RELOCITEM_SZ) {
	wSourceOffset = GETWORD(lpRelocPtr + RELOCATION_OFFSET);
	bSource = *(lpRelocPtr + RELOCATION_SRC) & RELOCSRC_MASK;
	switch(bSource) {
	    case RELOCSRC_LOBYTE:
	    case RELOCSRC_SEGMENT:
	    case RELOCSRC_FARADDR:
	    case RELOCSRC_OFFSET:
		break;
	    default:
		LEXE_perror(LXERR_INVRELOCSRC);
	}
	bFlag = *(lpRelocPtr + RELOCATION_FLAG);
	switch(bFlag & RELOCF_TARGETMASK) {
	    case RELOCF_INTERNALREF:
		wSegNum = GETWORD(lpRelocPtr + RELOCIREF_SEGNUM);
		wOffset = GETWORD(lpRelocPtr + RELOCIREF_INDEX);
		if (wSegNum == 0xff) {
		    wSegNum = lpmi->lpEntryTable[wOffset].sel>>3;
		    wOffset = lpmi->lpEntryTable[wOffset].off;
		}
		if (bFlag & RELOCF_ADDITIVE) {
		    wAddValue = GETWORD(lpSegData+wSourceOffset);
		    switch(bSource) {
			case RELOCSRC_LOBYTE:
			    *(lpSegData+wSourceOffset) += LOBYTE(wAddValue);
			    break;
			case RELOCSRC_SEGMENT:
			    PUTWORD(lpSegData+wSourceOffset,
				     RELOC_SEL(lpmi,wSegNum)+wAddValue);
			    break;
			case RELOCSRC_OFFSET:
			    PUTWORD(lpSegData+wSourceOffset,wOffset+wAddValue);
			    break;
			case RELOCSRC_FARADDR:
			    PUTWORD(lpSegData+wSourceOffset,wOffset+wAddValue);
			    PUTWORD(lpSegData+wSourceOffset+2,
				    RELOC_SEL(lpmi,wSegNum));
			    break;
		    }
		}
		else
		    do {
			wChainOffset = wSourceOffset;
			wSourceOffset = GETWORD(lpSegData+wSourceOffset);
			switch(bSource) {
			    case RELOCSRC_LOBYTE:
				*(lpSegData+wChainOffset) = LOBYTE(wOffset);
				break;
			    case RELOCSRC_FARADDR:
				PUTWORD(lpSegData+wChainOffset+2,
					 RELOC_SEL(lpmi,wSegNum));
				PUTWORD(lpSegData+wChainOffset,wOffset);
				break;
			    case RELOCSRC_SEGMENT:
				PUTWORD(lpSegData+wChainOffset,
					 RELOC_SEL(lpmi,wSegNum));
				break;
			    case RELOCSRC_OFFSET:
				PUTWORD(lpSegData+wChainOffset,wOffset);
				break;
			}
		    } while (wSourceOffset != 0xffff);
		break;
	    case RELOCF_IMPORTORDINAL:
		wProcNum = GETWORD(lpRelocPtr + RELOCIORD_PROCNUM);
		wModIndex = GETWORD(lpRelocPtr + RELOCIORD_INDEX)-1;
		hModule = lpmi->lpModIndex[wModIndex];
		modinfo = GETMODULEINFO(hModule);
		if (modinfo->lpDLL) {
		    wDLLIndex = (modinfo->lpEntryTable[wProcNum].sel +
				((modinfo->wSelBase - 1)<< 3))| 0x07;
		    wProcNum = modinfo->lpEntryTable[wProcNum].off;
		}
		else {
		    wDLLIndex = (modinfo->wSelBase << 3) | 0x07;
		    wProcNum = wProcNum << 3;
		}

		if (bFlag & RELOCF_ADDITIVE) {
		    switch(bSource) {
			case RELOCSRC_FARADDR:
			    PUTWORD(lpSegData+wSourceOffset,wProcNum);
			    PUTWORD(lpSegData+wSourceOffset+2,wDLLIndex);
			    break;
			case RELOCSRC_SEGMENT:
			    PUTWORD(lpSegData+wSourceOffset,wDLLIndex);
			    break;
			case RELOCSRC_LOBYTE:
			    if (strcmp(modinfo->lpModuleName,"KERNEL")) {
		    		LEXE_perror(LXERR_INVRELOCCOMB);
				break;
			    }
			    switch(wProcNum>>3) {
				case __AHSHIFT:
				    *(lpSegData+wSourceOffset) += GetAHSHIFT();
				    break;
				case __AHINCR:
				    *(lpSegData+wSourceOffset) += GetAHINCR();
				    break;
				case __WINFLAGS:
				    dwWinFlags = GetWinFlags();
				    *(lpSegData+wSourceOffset) +=
						LOBYTE(dwWinFlags);
				    break;
				default:
				    break;
			    }
			    break;
			case RELOCSRC_OFFSET:
			    wAddValue = GETWORD(lpSegData+wSourceOffset);
			    if (strcmp(modinfo->lpModuleName,"KERNEL")) {
				PUTWORD(lpSegData+wSourceOffset,
					wAddValue + wProcNum);
			    }
			    else
			        switch(wProcNum>>3) {
				    case __WINFLAGS:
					PUTWORD(lpSegData+wSourceOffset,
				    		wAddValue +
						LOWORD(GetWinFlags()));
					break;
				    case __AHSHIFT:
					PUTWORD(lpSegData+wSourceOffset,
						wAddValue + GetAHSHIFT());
					break;
				    case __AHINCR:
					PUTWORD(lpSegData+wSourceOffset,
						wAddValue + GetAHINCR());
					break;
				    case __ROMBIOS:
					PUTWORD(lpSegData+wSourceOffset,
						0);
					break;
				    case __C000H:
					PUTWORD(lpSegData+wSourceOffset,
						0);
					break;
				    default:
					PUTWORD(lpSegData+wSourceOffset,
						wAddValue + wProcNum);
					break;
				}
				break;
		    }
		}
		else
		    do {
			wChainOffset = wSourceOffset;
			wSourceOffset = GETWORD(lpSegData+wSourceOffset);
			switch(bSource) {
			    case RELOCSRC_FARADDR:
			        PUTWORD(lpSegData+wChainOffset,wProcNum);
			        PUTWORD(lpSegData+wChainOffset+2,wDLLIndex);
				break;
			    case RELOCSRC_SEGMENT:
				PUTWORD(lpSegData+wChainOffset,wDLLIndex);
				break;
			    case RELOCSRC_LOBYTE:
			        if (strcmp(modinfo->lpModuleName,"KERNEL")) {
		    		    LEXE_perror(LXERR_INVRELOCCOMB);
				    break;
			        }
			        switch(wProcNum>>3) {
				    case __WINFLAGS:
					dwWinFlags = GetWinFlags();
					*(lpSegData+wSourceOffset) =
						LOBYTE(dwWinFlags);
					break;
				    case __AHSHIFT:
				        *(lpSegData+wChainOffset) = GetAHSHIFT();
				        break;
				    case __AHINCR:
				        *(lpSegData+wChainOffset) = GetAHINCR();
				        break;
				    default:
				        break;
			        }
			        break;
			    case RELOCSRC_OFFSET:
				if (strcmp(modinfo->lpModuleName,"KERNEL"))
				{
				    PUTWORD(lpSegData+wChainOffset,wProcNum);
				}
				else
				  switch(wProcNum >> 3) {
				    case __WINFLAGS:
					PUTWORD(lpSegData+wChainOffset,
				    		LOWORD(GetWinFlags()));
					break;
				    case __AHSHIFT:
					PUTWORD(lpSegData+wChainOffset,GetAHSHIFT());
					break;
				    case __AHINCR:
					PUTWORD(lpSegData+wChainOffset,GetAHINCR());
					break;
				    case __ROMBIOS:
					PUTWORD(lpSegData+wChainOffset,
						0);
					break;
				    case __C000H:
					PUTWORD(lpSegData+wChainOffset,
						0);
					break;
				    default:
					PUTWORD(lpSegData+wChainOffset,
						wProcNum);
					break;
				  }
				break;
			}
		    } while (wSourceOffset != 0xffff);
		RELEASEMODULEINFO(modinfo);
		break;
	    case RELOCF_IMPORTNAME:
		wProcNum = (WORD)-1;
		wNameOffset = GETWORD(lpRelocPtr + RELOCINAME_OFFSET);
		wNameLength = (WORD)(*(lpUserDLL->lpImportTable + wNameOffset));
		memcpy(buf,(LPSTR)lpUserDLL->lpImportTable+wNameOffset+1,wNameLength);
		buf[wNameLength] = '\0';
		wModIndex = GETWORD(lpRelocPtr + RELOCIORD_INDEX)-1;
		hModule = lpUserDLL->lpModIndex[wModIndex];
		modinfo = GETMODULEINFO(hModule);

		if (!modinfo)
		    LEXE_perror(LXERR_MODULENOTFOUND);

		for(i=0,lpEntry = modinfo->lpEntryTable;
			lpEntry && lpEntry->name;i++,lpEntry++)
		    if (!strcasecmp(lpEntry->name,buf)) {
			wProcNum = (WORD)i;
			break;
		    }
		if (wProcNum == (WORD)-1) {
		    if (strcasecmp(buf,__C000H_BY_NAME))
			LEXE_perror(LXERR_ENTRYNOTFOUND);

		    wProcNum = wDLLIndex = 0;
		}
		else {
		    wProcNum = lpEntry->off;
		    wDLLIndex = (lpEntry->sel + 
				((modinfo->wSelBase - 1)<<3))| 0x7;
		}
		if (bFlag & RELOCF_ADDITIVE) {
		    switch(bSource) {
			case RELOCSRC_FARADDR:
			    PUTWORD(lpSegData+wSourceOffset,wProcNum);
			    PUTWORD(lpSegData+wSourceOffset+2,wDLLIndex);
			    break;
			case RELOCSRC_SEGMENT:
			    PUTWORD(lpSegData+wSourceOffset,wDLLIndex);
			    break;
			case RELOCSRC_OFFSET:
			    PUTWORD(lpSegData+wSourceOffset,wProcNum);
			    break;
		    }
		}
		else
		    do {
			wChainOffset = wSourceOffset;
			wSourceOffset = GETWORD(lpSegData+wSourceOffset);
			switch(bSource) {
			    case RELOCSRC_FARADDR:
			        PUTWORD(lpSegData+wChainOffset,wProcNum);
			        PUTWORD(lpSegData+wChainOffset+2,wDLLIndex);
				break;
			    case RELOCSRC_SEGMENT:
				PUTWORD(lpSegData+wChainOffset,wDLLIndex);
				break;
			    case RELOCSRC_OFFSET:
				PUTWORD(lpSegData+wChainOffset,wProcNum);
				break;
			}
		    } while (wSourceOffset != 0xffff);
		RELEASEMODULEINFO(modinfo);
		break;
	    case RELOCF_OSFIXUP:
    /* We do not touch anything here, since we assume 80x87 available. */
		break;
	    default:
		LEXE_perror(LXERR_INVRELOCTRG);
	}
    }
}
#endif

void
IT_GETPADDR(ENV *envp,LONGPROC f)
{
    DWORD retcode = 0L;
    LPSTR lpszProc;
    HINSTANCE hInst;
    HANDLE hModule;
    LPMODULEINFO lpModuleInfo;
    ENTRYTAB *lpEntry;
    register int i;
    WORD sel = 0;

    lpszProc = (LPSTR)GetAddress(GETWORD(SP+6),GETWORD(SP+4));
    hInst = (HANDLE) GetSelectorHandle(GETWORD(SP+8));
    if (!(lpModuleInfo = GETMODULEINFO(hInst)))	/* might be hModule already */
    {
	hModule = GetModuleFromInstance(hInst);
	lpModuleInfo = GETMODULEINFO(hModule);
    }
    if (lpModuleInfo) {
	    if (!HIWORD(lpszProc)) {
		lpEntry = &lpModuleInfo->lpEntryTable[LOWORD((DWORD)lpszProc)];
		if (lpEntry->sel) 
		    sel = ((lpModuleInfo->wSelBase<<3) + 
				lpEntry->sel - 0x8) | 0x7;
		retcode = MAKELONG(lpEntry->off,sel);
	    }
	    else {
		for(i=0,lpEntry = lpModuleInfo->lpEntryTable;
			(lpEntry->name && !retcode);i++,lpEntry++)
		if (!strcasecmp(lpszProc,lpEntry->name)) {
		    if (lpEntry->sel) 
			sel = ((lpModuleInfo->wSelBase<<3) +
					 lpEntry->sel - 0x8)|0x7;
		    retcode = MAKELONG(lpEntry->off,sel);
		    break;
	    }
	}
	RELEASEMODULEINFO(lpModuleInfo);
    }
    envp->reg.sp += HANDLE_86 + LP_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

typedef struct tagBINTONAT
{
	WNDPROC lpfnNatToBin;
	WNDPROC lpfnBinToNat;
} BINTONAT;

#ifdef TWIN_BIN16
static BINTONAT SystemClassBinToNat[] = {
	{ hsw_common_nat_to_bin, hsw_common_bin_to_nat},	/* ROOTWClass */
	{ hsw_common_nat_to_bin, hsw_common_bin_to_nat},	/* BUTTON */
	{ hsw_combobox_nat_to_bin, hsw_combobox_bin_to_nat}, 	/* COMBOBOX */
	{ hsw_edit_nat_to_bin,hsw_edit_bin_to_nat},		/* EDIT */
	{ hsw_listbox_nat_to_bin,hsw_listbox_bin_to_nat},	/* LISTBOX */
	{ hsw_listbox_nat_to_bin,hsw_listbox_bin_to_nat},	/* COMBOLBOX */
	{ hsw_mdiclient_nat_to_bin, hsw_mdiclient_bin_to_nat},	/* MDICLIENT */
	{ hsw_common_nat_to_bin, hsw_common_bin_to_nat},	/* SCROLLBAR */
	{ hsw_common_nat_to_bin, hsw_common_bin_to_nat},	/* SYSSCROLL */
	{ hsw_common_nat_to_bin, hsw_common_bin_to_nat},	/* STATIC */
	{ hsw_common_nat_to_bin, hsw_common_bin_to_nat},	/* HSW_FRAMECLASS */
	{ hsw_common_nat_to_bin, hsw_common_bin_to_nat},	/* DIALOGCLASS */
	{ hsw_common_nat_to_bin, hsw_common_bin_to_nat},	/* TRACKPOPUP */
	{ hsw_common_nat_to_bin, hsw_common_bin_to_nat},	/* ICONTITLE */
	{ hsw_listbox_nat_to_bin,hsw_listbox_bin_to_nat},	/* MENULBOX */
	{ NULL,NULL }
};
#endif

#ifdef TWIN_BIN16
static void
InitClassBinProcs()
{
    int i = 0;

    while (SystemGlobalClasses[i].WndClassEx.lpszClassName) {
	SystemGlobalClasses[i].lpfnNatToBin =
			SystemClassBinToNat[i].lpfnNatToBin;
	SystemGlobalClasses[i].lpfnBinToNat =
			SystemClassBinToNat[i].lpfnBinToNat;
	i++;
    }

    lpfnDefaultNatToBin = (WNDPROC)hsw_common_nat_to_bin;
    lpfnDefaultBinToNat = (WNDPROC)hsw_common_bin_to_nat;
    EnumClasses(ECF_ALLCLASSES,InitClassBinEnumProc,0L);
}

static BOOL
InitClassBinEnumProc(HCLASS32 hClass32, LPWNDCLASS lpWndClass, LPARAM lParam)
{
    LONG lpfnBinProc;
    int i;

    if (lpWndClass->style & CS_SYSTEMGLOBAL) {
	for (i=0; i<=LOOKUP_MENULBOX; i++) 
	    if (atmGlobalLookup[i] == 
			(ATOM)LOWORD((DWORD)lpWndClass->lpszClassName))
		break;
	if (i<=LOOKUP_MENULBOX) {
	    SetClassHandleLong(hClass32,GCL_BINTONAT,
				(LONG)SystemClassBinToNat[i].lpfnBinToNat);
	    SetClassHandleLong(hClass32,GCL_NATTOBIN,
				(LONG)SystemClassBinToNat[i].lpfnNatToBin);
	}
    }

    if (!(lpfnBinProc = GetClassHandleLong(hClass32,GCL_BINTONAT)))
	SetClassHandleLong(hClass32,GCL_BINTONAT,(LONG)lpfnDefaultBinToNat);
    if (!(lpfnBinProc = GetClassHandleLong(hClass32,GCL_NATTOBIN)))
	SetClassHandleLong(hClass32,GCL_NATTOBIN,(LONG)lpfnDefaultNatToBin);

    return TRUE;
}
#endif

void
IT_INITTASK (ENV *envp,LONGPROC f)
{
	LPINITT lpInitStruct;
	HANDLE hTask;
	TASKINFO *lpTaskInfo;
	LPBYTE lpStruct;

	lpInitStruct = (LPINITT)(f)();
/* fill in NULL segment data (ref. Schulman & Pietrek) */
	lpStruct = (LPBYTE)GetPhysicalAddress(envp->reg.ds);

/* init local heap */
	if (lpInitStruct->wHeapSize)
	    (void) TWIN_LocalInit((UINT)envp->reg.ds,
		(UINT)lpInitStruct->wStackBottom,
		(UINT)lpInitStruct->wStackBottom +
			(UINT)lpInitStruct->wHeapSize);

	PUTWORD((LPBYTE)(lpStruct+wMustBeZero),0);
	PUTDWORD((LPBYTE)(lpStruct+dwOldSSSP),0);
	PUTWORD((LPBYTE)(lpStruct+pAtomTable),0);
	PUTWORD((LPBYTE)(lpStruct+pStackTop),
				lpInitStruct->wStackTop);
	PUTWORD((LPBYTE)(lpStruct+pStackMin),
				lpInitStruct->wStackBottom);
	PUTWORD((LPBYTE)(lpStruct+pStackBottom),
				lpInitStruct->wStackBottom);

	envp->reg.cx = (REGISTER)(lpInitStruct->wStackBottom -
			lpInitStruct->wStackTop);
	envp->reg.dx = (REGISTER)(lpInitStruct->nCmdShow);

	hTask = GetCurrentTask();
	if ((lpTaskInfo = GETTASKINFO(hTask))) {
	    lpStruct = GetPhysicalAddress(lpTaskInfo->wTDBSelector);
	    envp->reg.es = GETWORD(lpStruct+0x60); /* PSP selector */
	    PUTWORD(lpStruct+0x1c, lpTaskInfo->wDGROUP);
	    PUTWORD(lpStruct+0x1e,(UINT)GetModuleFromInstance(lpTaskInfo->hInst));
	    RELEASETASKINFO(lpTaskInfo);
	}
	envp->reg.bx = 0x80;
	envp->reg.ax = 1;
	envp->reg.sp += RET_86;
}

UINT
GetTDBSelector(HTASK hTask)
{
    LPTASKINFO lpTaskInfo;

    if ((lpTaskInfo = GETTASKINFO(hTask))) {
        UINT wResult = lpTaskInfo->wTDBSelector;
	RELEASETASKINFO(lpTaskInfo);
	return wResult;
    }
    else
	return 0;
}


void
_86_GetCurrentPDB (ENV *envp,LONGPROC f)
{
    envp->reg.ax = (REGISTER)GetPSPSelector();
    envp->reg.sp += RET_86;
}

void
IT_GETMODULEHANDLE (ENV *envp,LONGPROC f)
{
    DWORD retcode;
    LPSTR lpString;

    if (GETWORD(SP+6))
	lpString = (LPSTR)GetAddress(GETWORD(SP+6),GETWORD(SP+4));
    else		  /* we are widening what this returns... */
	lpString = (LPSTR)(unsigned long)GetSelectorHandle(GETWORD(SP+4));
    retcode = (f)(lpString);
    retcode = (DWORD)GetDataSelectorFromInstance((HINSTANCE)retcode); /*xxxxx*/
    envp->reg.sp += LP_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

void
IT_LOADMODULE (ENV *envp,LONGPROC f)
{
    LPBYTE lpStruct;
    PARAMBLOCK pblock;
    UINT CmdShow[2];
    LPBYTE lpCmdShow;
    DWORD retcode;
    LPSTR lpszModuleName;
    char FileName[_MAX_PATH];

    lpStruct = (LPBYTE)GetAddress(GETWORD(SP+6),GETWORD(SP+4));
    pblock.wSegEnv = GETWORD(lpStruct);
    pblock.lpszCmdLine = (LPSTR)GetAddress
			(GETWORD(lpStruct+4),GETWORD(lpStruct+2));
    lpCmdShow = (LPBYTE)GetAddress(GETWORD(lpStruct+8),GETWORD(lpStruct+6));
    CmdShow[0] = (UINT)GETWORD(lpCmdShow);
    CmdShow[1] = (UINT)GETWORD(lpCmdShow+2);
    pblock.lpShow = CmdShow;
    lpszModuleName = (LPSTR)GetAddress(GETWORD(SP+10),GETWORD(SP+8));
    if (strchr(lpszModuleName,'\\')) {
	xdoscall(XDOS_GETALTNAME,0,(void *) FileName,(void *) lpszModuleName);
	lpszModuleName = &FileName[0];
    }
    retcode = (DWORD)(f)(lpszModuleName,(LPVOID)&pblock);
    if (retcode > 32)
	retcode = GetDataSelectorFromInstance((HINSTANCE)retcode);
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
    envp->reg.sp += 2*LP_86 + RET_86;
}

int
LoadModuleFromNEFile(MODULEINFO *modinfo, LPUSERDLL lpUserDLL,
		   LPSTR lpCmdLine, WORD wFlags)
{
#ifdef TWIN_BIN16
    char buf[BUFFER];
    WORD i;
    int ret;
    WORD wOffset,wLength,wSegFlags;
    HINSTANCE hInst = 0;
    HTASK hTask;
    SEGTAB *lpSegTable;
    LPSEGENTRY lpSegEntry;
    LPBYTE lpRelocData;
    HGLOBAL hGlobal = 0;
    DWORD dwStartOffset;
    WORD wRelocCount,wRelocBytes;
    WORD wTemp;
    ENV *envp;
    REGISTER dll_stack = 0;
    LPBYTE lpStack;
    DWORD dwStackSize;
    LPTASKINFO lpTaskInfo;
    BOOL bError = FALSE;
    int bNoPaging;
    HFILE hFile;

    /* do we not support paging win16 binaries */
    bNoPaging = GetConfigOption(WD_NOPAGING);

    hFile = lpUserDLL->hfExeFile;

    if (!(wFlags & (ILL_NOFORK|ILL_RESOURCE)) &&
	!(lpUserDLL->wProgramFlags & HEADER_LIBRARY)) {
		hTask = CreateTask();
		CreatePSP(lpCmdLine, hTask);

    } else
		hTask = GetCurrentTask();

    if (wFlags & ILL_RESOURCE || lpUserDLL->wSegCount == 0) {
	hInst = GlobalAlloc(GMEM_MOVEABLE,0);
	modinfo->hInst = hInst;
	CreateDataInstance(hInst,(HMODULE)modinfo->ObjHead.hObj,
                        hTask);
	modinfo->wDGROUP = ASSIGNSEL(-1, 0);
	SetSelectorHandle(modinfo->wDGROUP, hInst);
	modinfo->ResourceTable = ReadResourceTable(hInst,lpUserDLL);
	_lclose(hFile);
	return (int)modinfo->ObjHead.hObj;
    }

    /********************************************************************/
    /*									*/
    /********************************************************************/

    modinfo->lpEntryTable = ProcessEntryTable(lpUserDLL);
    ReadResidTable(lpUserDLL,modinfo->lpEntryTable);

    if (lpUserDLL->lpNonResTable) {
	ReadNonResidTable(lpUserDLL,modinfo->lpEntryTable);
	WinFree((LPSTR)lpUserDLL->lpNonResTable);
    }
    modinfo->lpModuleName = lpUserDLL->lpDLLName;

    modinfo->atmModuleName = AddAtom(modinfo->lpModuleName);
    modinfo->lpModuleDescr = lpUserDLL->lpDLLDescription;
    lpUserDLL->wSelectorBase = AssignSelRange((int)lpUserDLL->wSegCount);
    modinfo->wSelBase = lpUserDLL->wSelectorBase;

    /********************************************************************/
    /*									*/
    /********************************************************************/

    if (lpUserDLL->wAutoData)
	modinfo->wDGROUP = DLL_SEL(modinfo,lpUserDLL->wAutoData);
    else
    {
	hInst = GlobalAlloc(GMEM_MOVEABLE,0);
	modinfo->hInst = hInst;
	CreateDataInstance(hInst,(HMODULE)modinfo->ObjHead.hObj,hTask);
	modinfo->wDGROUP = ASSIGNSEL(-1, 0);
	SetSelectorHandle(modinfo->wDGROUP, hInst);
    }
    
    modinfo->lpDLL = lpUserDLL;
    modinfo->wSegCount = lpUserDLL->wSegCount;
    modinfo->wProgramFlags = lpUserDLL->wProgramFlags;
    modinfo->wSectorShift = lpUserDLL->wSectorShift;

    /********************************************************************/
    /*									*/
    /********************************************************************/

    if (lpUserDLL->wModCount) {
	    modinfo->lpModIndex = (LPHANDLE)WinMalloc
			(lpUserDLL->wModCount*sizeof(HANDLE));
	    memset((LPSTR)modinfo->lpModIndex,'\0',
			lpUserDLL->wModCount*sizeof(HANDLE));
    }
    else
	    modinfo->lpModIndex = (LPHANDLE)NULL;
    for (i = 0; i < lpUserDLL->wModCount; i++) {
	    wOffset = GETWORD((LPBYTE)(lpUserDLL->lpModuleTable+i));
	    wLength = (WORD)(*(lpUserDLL->lpImportTable+wOffset));
	    lstrcpyn(buf,(LPSTR)(lpUserDLL->lpImportTable+wOffset+1),wLength+1);

	    modinfo->lpModIndex[i] = TWIN_LoadFile(
				ILL_BINARY|ILL_INTERMED|(wFlags & ILL_NOERROR),
				"",(LPARAM)buf);
	    if (modinfo->lpModIndex[i] == 0)
		bError = TRUE;
    }
    if (bError) {
	if (!(wFlags & ILL_NOERROR))
	    FileErrorMB(ILL_FINAL,modinfo->lpFileName);
	WinFree((LPSTR)modinfo->lpModIndex);
	WinFree((LPSTR)lpUserDLL);
	FreeTask(hTask);
	return 0;
    }

    logstr(LF_CONSOLE,"LoadModule16: %s\n",modinfo->lpFileName);

    /********************************************************************/
    /*									*/
    /********************************************************************/

    lpUserDLL->lpModIndex = modinfo->lpModIndex;

    if (lpUserDLL->wSegCount) {
	lpSegTable = (SEGTAB *)WinMalloc
			(sizeof(SEGTAB)*lpUserDLL->wSegCount);
	modinfo->lpSegTable = lpSegTable;
	lpSegEntry = (LPSEGENTRY)(lpUserDLL->lpSegmentTable);
	for (i = 0; i < lpUserDLL->wSegCount; i++,lpSegEntry++) {
	    lpSegTable[i].reserved =
                                GETWORD((LPBYTE)(&lpSegEntry->sec_offset));
	    lpSegTable[i].size = GETWORD((LPBYTE)(&lpSegEntry->length));
	    lpSegTable[i].flags = GETWORD((LPBYTE)(&lpSegEntry->flag));
	    lpSegTable[i].alloc = GETWORD((LPBYTE)(&lpSegEntry->alloc_sz));

    /********************************************************************/
    /* If the allocation size in the table is 0 and offset is specified,*/
    /*	then the minimum alloc size is 64K				*/
    /********************************************************************/

	    if (!lpSegTable[i].alloc && lpSegTable[i].reserved)
		lpSegTable[i].alloc = 0xffff;

	    if (i == (lpUserDLL->wAutoData-1)) {
		lpSegTable[i].alloc += lpUserDLL->wInitStack +
						lpUserDLL->wInitHeap + 1;
		lpSegTable[i].alloc &= 0xfffffffe;
		lpUserDLL->wStackBottom = lpSegTable[i].alloc - 
						lpUserDLL->wInitHeap;
		/* force preloading of autodata segment */
		lpSegTable[i].flags |= SEG_PRELOAD;
	    }
            
	    wSegFlags = lpSegTable[i].flags;
	    if (wSegFlags & SEG_PRELOAD || bNoPaging ||
		i == (lpUserDLL->wAutoData-1)) {
		hGlobal = GlobalAlloc(GHND,lpSegTable[i].alloc);
		lpSegTable[i].image = (LPSTR)GlobalLock(hGlobal);
	    }
	    else {
		hGlobal = GlobalAlloc(GHND,0L);
		lpSegTable[i].image = NULL;
	    }

    /********************************************************************/
    /* hInst of a module is a handle of its DGROUP 			*/
    /********************************************************************/

	    if (i == (lpUserDLL->wAutoData-1)) 
		hInst = hGlobal;
	    if (lpSegTable[i].flags & SEG_DATA) 
		lpSegTable[i].transfer = (WORD)TRANSFER_DATA;
	    else 
	        lpSegTable[i].transfer = (WORD)TRANSFER_CODE16;
	    AssignSelector((LPBYTE)lpSegTable[i].image,
					(lpUserDLL->wSelectorBase+i)<<3,
					(BYTE)lpSegTable[i].transfer,
					(DWORD)lpSegTable[i].alloc);
	    SetSelectorHandle((UINT)((lpUserDLL->wSelectorBase+i)<<3),hGlobal);
	    SetModuleIndex((UINT)((lpUserDLL->wSelectorBase+i)<<3),
					modinfo->bModIndex);
	    if (wSegFlags & SEG_PRELOAD || bNoPaging || i == (lpUserDLL->wAutoData-1)) {
		dwStartOffset = (DWORD)(lpSegTable[i].reserved <<
			 lpUserDLL->wSectorShift);
		if (!lpSegTable[i].size && dwStartOffset)
	            lpSegTable[i].size = 0xffff;
		_llseek(lpUserDLL->hfExeFile,dwStartOffset,0);
		if ((ret = _lread(lpUserDLL->hfExeFile,lpSegTable[i].image,
			lpSegTable[i].size)) < (UINT)lpSegTable[i].size) {
		    LEXE_perror(LXERR_FILEREAD);
		    _lclose(lpUserDLL->hfExeFile);
		    FreeTask(hTask);
		    return(-LXERR_FILEREAD);
		}

		if (wSegFlags & SEG_ITERATED)
		    ProcessIteratedData(&lpSegTable[i]);

		if (wSegFlags & SEG_RELOCINFO) {
		    if ((ret = _lread(lpUserDLL->hfExeFile,&wTemp,2)) < 2) {
			LEXE_perror(LXERR_FILEREAD);
			_lclose(lpUserDLL->hfExeFile);
			FreeTask(hTask);
			return(-LXERR_FILEREAD);
		    }
		    wRelocCount = GETWORD((LPBYTE)&wTemp);
		    wRelocBytes = wRelocCount*RELOCITEM_SZ;
		    lpRelocData = (LPBYTE)WinMalloc(wRelocBytes);

		    if ((ret = _lread(lpUserDLL->hfExeFile,lpRelocData,
				wRelocBytes)) < (int)wRelocBytes) {
			LEXE_perror(LXERR_FILEREAD);
			_lclose(lpUserDLL->hfExeFile);
			FreeTask(hTask);
			return(-LXERR_FILEREAD);
		    }
		    ProcessRelocs(modinfo,(LPBYTE)lpSegTable[i].image,
			lpRelocData,wRelocCount,0xffff,0xffff);
		    WinFree((LPSTR)lpRelocData);
		}
	    }
	}
	PatchExportedPrologs(lpUserDLL);
    }
    else
	modinfo->lpSegTable = NULL;

	if (!hInst)
	    hInst = GlobalAlloc(GMEM_MOVEABLE,0);
	CreateDataInstance(hInst,(HMODULE)modinfo->ObjHead.hObj,
                        hTask);
	modinfo->hInst = hInst;
	modinfo->ResourceTable = ReadResourceTable(hInst,lpUserDLL);

	_lclose(lpUserDLL->hfExeFile);

	if (lpUserDLL->wInitCS) 	 { /* call init routine*/

	    envp = (ENV *)WinMalloc(sizeof(ENV));
	    memset((LPSTR)envp,'\0',sizeof(ENV));
	    envp->reg.cs = DLL_SEL(modinfo,lpUserDLL->wInitCS);

	    if (lpUserDLL->wProgramFlags & HEADER_LIBRARY) {
		dwStackSize = (lpUserDLL->wInitStack)?
					lpUserDLL->wInitStack:STACKSIZE;
	        hGlobal = GlobalAlloc(GHND,dwStackSize);
		lpStack = (LPBYTE)GlobalLock(hGlobal);
		dll_stack = AssignSelector(lpStack,0,TRANSFER_DATA,dwStackSize);
		SetSelectorHandle(dll_stack,hGlobal);
		envp->reg.cx = lpUserDLL->wInitHeap;
		envp->reg.es = 0;
		envp->reg.ss = dll_stack;
		envp->reg.ds = (lpUserDLL->wAutoData)?
			DLL_SEL(modinfo,lpUserDLL->wAutoData):dll_stack;

		envp->reg.bp = (REGISTER)lpStack;
		envp->reg.sp = (REGISTER)
				(lpStack + dwStackSize - sizeof(DWORD));
	    }
	    else {
		modinfo->fMakeInstance = TRUE;
		
		envp->reg.cx = 0;
		envp->reg.ds = DLL_SEL(modinfo,lpUserDLL->wAutoData);
		envp->reg.es = envp->reg.ds;
		envp->reg.ss = (lpUserDLL->wInitSS && 
				lpUserDLL->wInitSS != lpUserDLL->wAutoData) ?
			DLL_SEL(modinfo,lpUserDLL->wInitSS) : envp->reg.ds;

		envp->reg.bp = (REGISTER)GetPhysicalAddress(envp->reg.ss);
		envp->reg.sp = envp->reg.bp + ((lpUserDLL->wInitSP) ? 
			lpUserDLL->wInitSP : lpUserDLL->wStackBottom);
	    }

	    envp->reg.di = envp->reg.ds;
	    envp->reg.si = 0;
	    envp->reg.bx = lpUserDLL->wInitStack;
	    envp->trans_addr = (BINADDR)MAKELONG(lpUserDLL->wInitIP,
				(WORD)envp->reg.cs);

	    if (hTask == GetCurrentTask()) {

		envp->prev_env = envp_global;

		if (envp_global)
		    envp->level = envp_global->level + 1;

		envp_global = envp;

		if (wFlags & (ILL_APPL|ILL_EXEC)) {
		    lpTaskInfo = GETTASKINFO(hTask);
		    lpTaskInfo->hInst = hInst;
		    lpTaskInfo->uiHeapSize = lpUserDLL->wInitHeap;
		    lpTaskInfo->uiStackSize = lpUserDLL->wInitStack;
		    lpTaskInfo->uiAutodataSize = lpUserDLL->wStackBottom - 
			lpUserDLL->wInitStack;
		    lpTaskInfo->wDGROUP = envp->reg.ds;
		    RELEASETASKINFO(lpTaskInfo);
		} 

		invoke_binary();

		envp_global = (ENV *)envp->prev_env;

		WinFree((LPSTR)envp);

		if (dll_stack) {
		    GlobalUnlock(hGlobal);
		    GlobalFree(hGlobal);
		    FreeSelector(dll_stack);
		}
	    } else /* new task */ {

		InitializeTask(hTask, envp, hInst,
			       lpUserDLL->wInitHeap,
			       lpUserDLL->wInitStack,
			       lpUserDLL->wStackBottom-lpUserDLL->wInitStack);

		DirectedYield(hTask);

	    }
	}
#endif
	return (int)modinfo->ObjHead.hObj;
}

extern char *GetTwinFilename();

static WORD BuildEnvSegment(LPSTR lpCmdLine)
{
    char *lpFile;
    HGLOBAL hGlobal;
    char *lpKey;
    char *lpData;
    char *lpEnv;
    char *lpDosEnv;
    char  KeyColor[256];
    WORD  wSel;
    char buf[_MAX_PATH], Path[_MAX_PATH];

    hGlobal = GlobalAlloc(GHND,64*1024);
    lpEnv  = GlobalLock(hGlobal);
    lpDosEnv = lpEnv;

    /* clear out the buffer */
    memset(lpEnv,'\0',64*1024);
    lpData = WinMalloc(64*1024);
    memset(lpData,'\0',64*1024);

    lpFile = GetTwinFilename();

    GetPrivateProfileString((LPSTR)"Environment",NULL,NULL,
				lpData,64*1024,lpFile);

    /* 
     * add user defined environment strings
     */
    for(lpKey = lpData; *lpKey != 0; lpKey += strlen(lpKey) + 1) {

	GetPrivateProfileString((LPSTR)"Environment",
			        (LPSTR) lpKey,
				(LPSTR) 0,
				KeyColor,
				256,
				lpFile);
	strcat(lpEnv,lpKey);
	strcat(lpEnv,"=");
	strcat(lpEnv,KeyColor);

	lpEnv += strlen(lpEnv);
	lpEnv++;
    }

    /* 
     * always put the PATH into the environment
     */
    strcat((LPSTR)lpEnv,"PATH=C:\\;");
    GetWindowsDirectory(buf,_MAX_PATH);
    strcat(buf,"/");
    xdoscall(XDOS_GETDOSNAME,0,(void *) Path,(void *) buf);
    strcat((LPSTR)lpEnv,Path);

    lpEnv += strlen(lpEnv);

    /* double null terminator */
    /* this ends the environment list */
    *lpEnv++ = 0;;

    /* put in a WORD 1??? */
    *lpEnv++ = 1;;
    *lpEnv++ = 0;;

    /* add null commandline */
    *lpEnv++ = 0;;

    wSel = ASSIGNSEL(lpDosEnv, 64*1024);
   
    SetSelectorHandle(wSel,hGlobal);

    WinFree(lpData);
    return wSel;
}


#ifdef X386
_EXTERN_ WORD native_cs;
_EXTERN_ WORD native_ds;
#endif


BOOL
LoadDuplicateSegment(UINT uiSel, UINT uiSegNum, MODULEINFO *modinfo)
{
#ifdef TWIN_BIN16
    LPBYTE lpRelocData;
    HFILE hFile;
    HGLOBAL hGlobal;
    SEGTAB *lpSegInfo;
    DWORD dwStartOffset;
    WORD wRelocCount,wRelocBytes;
    WORD wTemp,wFlags;
    int ret;

#ifdef SEVERE
    LOGSTR((LF_LOG,"LoadSegment: selector %x",uiSel));
    LOGSTR((LF_LOG,"... module %s seg %x\n",modinfo->lpModuleName,uiSegNum));
#endif

    if ((WORD)uiSegNum > modinfo->wSegCount || !uiSegNum) {
	return FALSE;
    }

    lpSegInfo = &modinfo->lpSegTable[uiSegNum-1];
    if (!lpSegInfo) {
	return FALSE;
    }
    hFile = _lopen(modinfo->lpFileName,READ);
    if (hFile == HFILE_ERROR) {
	return FALSE;
    }

    hGlobal = GetSelectorHandle(uiSel);
    if (hGlobal)
	hGlobal = GlobalReAlloc(hGlobal,lpSegInfo->alloc,GHND);
    else
	hGlobal = GlobalAlloc(GHND,lpSegInfo->alloc);

    lpSegInfo->image = (LPSTR)GlobalLock(hGlobal);

    dwStartOffset = (DWORD)(lpSegInfo->reserved << modinfo->wSectorShift);
    _llseek(hFile,dwStartOffset,0);
    if ((ret = _lread(hFile,lpSegInfo->image,lpSegInfo->size)) <
			 (UINT)lpSegInfo->size) {
	LEXE_perror(LXERR_FILEREAD);
	_lclose(hFile);
	return FALSE;
    }

    if (lpSegInfo->flags & SEG_ITERATED)
	ProcessIteratedData(lpSegInfo);

    /* Special case for Quattro Pro -- we patch the procedure reading    */
    /* directly from BIOS timer (40:006c) by making it return right away */

    if (uiSegNum == 0x33) {
	if (!lstrcmpi(modinfo->lpModuleName,"QPW")) {
	    LPBYTE ptr;
	    ptr = (LPBYTE)lpSegInfo->image + 0x1c5b;
	    *ptr++ = 0x5d;
	    *ptr++ = 0x4d;
	    *ptr++ = 0xca;
	    *ptr++ = 0x02;
	    *ptr++ = 0x00;
	}
    }

    /* Special case for Delphi -- we patch the procedure reading   */
    /* directly from BIOS timer (40:006c) by making it return zero */

    if (uiSegNum == 0x4) {
	if (!lstrcmpi(modinfo->lpModuleName,"APPBUILD")) {
	    LPBYTE ptr;
	    ptr = (LPBYTE)lpSegInfo->image + 0xf472;
	    *ptr++ = 0xb8;
	    *ptr++ = 0x0;
	    *ptr++ = 0x0;
	    *ptr++ = 0xc3;
	}
    }

    if (lpSegInfo->flags & SEG_RELOCINFO) {
	if ((ret = _lread(hFile,&wTemp,2)) < 2) {
	    LEXE_perror(LXERR_FILEREAD);
	    _lclose(hFile);
	    return FALSE;
	}
	wRelocCount = GETWORD((LPBYTE)&wTemp);
	wRelocBytes = wRelocCount*RELOCITEM_SZ;
	lpRelocData = (LPBYTE)WinMalloc(wRelocBytes);

	if ((ret = _lread(hFile,lpRelocData,wRelocBytes)) < (int)wRelocBytes) {
	    LEXE_perror(LXERR_FILEREAD);
	    _lclose(hFile);
	    return FALSE;
	}
	ProcessRelocs(modinfo,(LPBYTE)lpSegInfo->image,lpRelocData,wRelocCount,
		      uiSegNum, uiSel);
	WinFree((LPSTR)lpRelocData);
    }
    SetPhysicalAddress(uiSel,lpSegInfo->image);
    PatchSegmentPrologs(modinfo,uiSegNum);
    _lclose(hFile);

    SetSelectorHandle(uiSel,hGlobal);
    wFlags = GetSelectorFlags(uiSel);
    wFlags |= DF_PRESENT;
    SetSelectorFlags(uiSel,wFlags);
    DPMI_Notify(DN_MODIFY,uiSel);

    return TRUE;
#else
    return FALSE;
#endif
}

#ifdef TWIN_BIN16
static void
ProcessIteratedData(SEGTAB *lpSegInfo)
{
    LPSTR lpTemp,lpSrc,lpDest;
    int nCount,i;
    UINT wNum, wLen;

    if (!lpSegInfo->size)
	return;
    lpTemp = WinMalloc(lpSegInfo->size);
    memcpy(lpTemp,lpSegInfo->image,lpSegInfo->size);
    for (lpSrc = lpTemp, lpDest = lpSegInfo->image, nCount = 0;
	 	nCount < lpSegInfo->size - 4;) {
	wNum = GETWORD(lpSrc);
	wLen = GETWORD(lpSrc+2);
	lpSrc += 4;
	for (i = 0; i < (int)wNum; i++) {
	    memcpy(lpDest,lpSrc,wLen);
	    lpDest += wLen;
	}
	lpSrc += wLen;
	nCount += wLen + 4;
    }
    lpSegInfo->size = lpDest - lpSegInfo->image;
    WinFree(lpTemp);
}
#endif

SEGMENTINFO *
GetSegmentInfo(UINT uSel, SEGMENTINFO *sinfo)
{
    MODULEINFO *modinfo;
    static SEGMENTINFO seginfo;

    if (uSel == 0)
	return (SEGMENTINFO *)-1;
    if (!(modinfo = GetModuleFromSelector(uSel)))
	return (SEGMENTINFO *)-1;

    if (sinfo == NULL)
	sinfo = &seginfo;
    sinfo->lpModuleName = modinfo->lpModuleName;
    sinfo->lpFileName = modinfo->lpFileName;
    sinfo->nSegmentOrd = (uSel>>3) - modinfo->wSelBase + 1;
    sinfo->wSegType = GetSelectorFlags(uSel);
    return sinfo;
}

SEGMENTINFO * 
gdb_SegInfo(UINT uSel)
{
    static SEGMENTINFO seginfo;

    if (GetSegmentInfo(uSel,&seginfo) == (SEGMENTINFO *)-1) {
	logstr(LF_CONSOLE,"Invalid selector!\n");
	return FALSE;
    }

    return &seginfo;
}

WORD GetCurrentDataSelector()
{
    TASKINFO *taskinfo = GETTASKINFO(GetCurrentTask());
    WORD wResult;

    if (!taskinfo)
	return 0;

    wResult = taskinfo->wDGROUP;
    RELEASETASKINFO(taskinfo);
    return wResult;
}

WORD GetDataSelectorFromInstance(HINSTANCE hInst)
{
    HMODULE hModule;
    HTASK hTask;
    MODULEINFO *modinfo = NULL;
    TASKINFO *taskinfo = NULL;

    if (hInst == 0)
	return 0;

    if ((modinfo = CHECKMODULEINFO(hInst))) {
        WORD wResult = modinfo->wDGROUP;
	RELEASEMODULEINFO(modinfo);
	return wResult;
      }
    else
    {
	hModule = GetModuleFromInstance(hInst);
	modinfo = GETMODULEINFO(hModule);
    }

    /*
     * If we couldn't get a valid modinfo or if this is a native lib/app,
     * return NULL because there is no selector for this instance.
     */
    if (!modinfo /* || !modinfo->lpDLL */)
	return 0;

    /*
     * If this module is not flagged fMakeInstance, then there is only one
     * copy of it in memory.  In this case return the value stored in the
     * module table.
     */
    else if (!modinfo->fMakeInstance) {
        WORD wResult = modinfo->wDGROUP;
	RELEASEMODULEINFO(modinfo);
	return wResult;
    }

    /*
     * There may be more than one DGROUP for this module.  So, we need to
     * look into the task structure to get the DGROUP for this instance.
     */
    else
    {
        WORD wResult;
	hTask = GetTaskFromInstance(hInst);
	taskinfo = GETTASKINFO(hTask);

	wResult = taskinfo->wDGROUP;
	RELEASETASKINFO(taskinfo);
	RELEASEMODULEINFO(modinfo);
	return wResult;
    }
}
/* TaskMain is the first function executed by a created task.  It takes    */
/* care of setting up and invoking the binary application. When the binary */
/* task exits, then this task is deleted.                                  */
static void TaskMain()
{
    invoke_binary();
    DeleteTask(GetCurrentTask());
}

void
InitializeTask(HTASK hTask, ENV *envp, HINSTANCE hInst,
		    UINT heap_size, UINT stack_size, UINT bss_size)
{
    LPTASKINFO lpTask;
    LPTASKINFO lpCurrentTask;
    HTASK      hCurrentTask;
    
    lpTask = GETTASKINFO(hTask);
    hCurrentTask = GetCurrentTask();
    lpCurrentTask = GETTASKINFO(hCurrentTask);
    
    if (!lpTask || !lpCurrentTask) {
      if (lpTask)
	RELEASETASKINFO(lpTask);
      if (lpCurrentTask)
	RELEASETASKINFO(lpCurrentTask);
      return;
    }

    lpTask->hInst = hInst;
    lpTask->wStateFlags |= TASK_READY;
    lpTask->GlobalEnvp = envp;
    lpTask->uiHeapSize = heap_size;
    lpTask->uiStackSize = stack_size;
    lpTask->uiAutodataSize = bss_size;
    lpTask->wDGROUP = envp->reg.ds;

    lpTask->ThreadData = DRVCALL_SYSTEM(PSSH_CREATETHREAD, (DWORD) TaskMain, 
					0, 0);
    RELEASETASKINFO(lpTask);
    RELEASETASKINFO(lpCurrentTask);
}

#ifdef TWIN_BIN16
static char *wh_errlist[] = {
        "No MZ signature in file",
        "No NE signature in file",
        "Not a windows exe",
        "Error during file read",
        "Not enough memory",
        "Invalid relocation source type",
        "Invalid relocation target",
	"Cannot open .EXE file",
	"Invalid relocation combination",
	"DLL name not found",
	"Entry not found",
        ""
};
	
static void
LEXE_perror(int errnum)
{
    MessageBox((HWND)NULL,wh_errlist[errnum - 1],"LoadEXE error",MB_OK);
}
#endif

void
TWIN_CreateENV(LPTASKINFO lpTask)
{
    HGLOBAL hGlobal;
    LPBYTE lpStack;

    lpTask->GlobalEnvp = (ENV *)WinMalloc(sizeof(ENV));
    memset((LPSTR)lpTask->GlobalEnvp,'\0',sizeof(ENV));

    hGlobal = GlobalAlloc(GHND,STACKSIZE);
    lpStack = (LPBYTE)GlobalLock(hGlobal);
    lpTask->GlobalEnvp->reg.ss = AssignSelector(lpStack, 0,
						TRANSFER_DATA, STACKSIZE);
    SetSelectorHandle(lpTask->GlobalEnvp->reg.ss, hGlobal);
    lpTask->GlobalEnvp->reg.bp = lpTask->GlobalEnvp->reg.sp = (REGISTER)
			(lpStack + STACKSIZE - sizeof(DWORD));
}

UINT
GetPSPSelector()
{
    TASKINFO *lpTaskInfo;
    HANDLE hTask;
    LPBYTE lpStruct;

    hTask = GetCurrentTask();
    lpTaskInfo = GETTASKINFO(hTask);
    if (lpTaskInfo) {
	lpStruct = GetPhysicalAddress(lpTaskInfo->wTDBSelector);
	RELEASETASKINFO(lpTaskInfo);
	return (UINT)GETWORD(lpStruct+0x60);
    }
    else
	return 0;
}

int
LoadNewInstance(MODULEINFO *modinfo, LPSTR lpCmdLine, WORD flags)
{
#ifdef TWIN_BIN16
    HTASK hTask;
    LPUSERDLL lpUserDLL;
    SEGTAB *lpSegInfo;
    HGLOBAL hDSdup;
    BYTE *lpbDSdup;
    WORD wDSdup;
    WORD wDStemplate;
    DWORD dwDStemplateSize;
    ENV *envp;
    HINSTANCE hPrevInstance;
    extern HINSTANCE FindPreviousInstance(HMODULE,HTASK);
    
    lpUserDLL = modinfo->lpDLL;
    lpSegInfo = &modinfo->lpSegTable[lpUserDLL->wAutoData - 1];

    if (lpUserDLL->wInitSS && lpUserDLL->wInitSS != lpUserDLL->wAutoData)
	return 0;

    hTask = CreateTask();
    CreatePSP(lpCmdLine, hTask);
    
    envp = (ENV *)WinMalloc(sizeof(ENV));
    memset((LPSTR)envp,'\0', sizeof(ENV));
    envp->reg.cs = DLL_SEL(modinfo, lpUserDLL->wInitCS);

    hPrevInstance = FindPreviousInstance((HMODULE)modinfo->ObjHead.hObj,
					 hTask);

    if (hPrevInstance)
    {
	/*
	 * We need to duplicate the auto-data seg.  This will let
	 * us have more than one instance of this app.
	 */
	wDStemplate = DLL_SEL(modinfo, lpUserDLL->wAutoData);
	dwDStemplateSize = GetSelectorLimit(wDStemplate);
	
	hDSdup = GlobalAlloc(GMEM_MOVEABLE, dwDStemplateSize);
	lpbDSdup = (BYTE*) GlobalLock(hDSdup);
	wDSdup = AssignSelector(lpbDSdup, 0, 
				GetSelectorType(wDStemplate), 
				(DWORD) lpSegInfo->alloc);
	SetSelectorHandle(wDSdup, hDSdup);
	CreateDataInstance(hDSdup, (HMODULE)modinfo->ObjHead.hObj,
			   hTask);
    }
    else
    {
	/*
	 * If there is no previous instance, then this module ran and then
	 * exited.  Now we are trying run this app again.  So, let's just
	 * reload the auto-data seg in the place that it used to be.
	 */
	wDSdup = DLL_SEL(modinfo, lpUserDLL->wAutoData);
	hDSdup = GetSelectorHandle(wDSdup);
	lpbDSdup = (BYTE*) GlobalLock(hDSdup);

	AssignSelector(lpbDSdup, wDSdup, GetSelectorType(wDSdup),
		       (DWORD) lpSegInfo->alloc);
	CreateDataInstance(hDSdup, (HMODULE)modinfo->ObjHead.hObj, hTask);
    }

    if (!LoadDuplicateSegment(wDSdup, lpUserDLL->wAutoData, modinfo))
	return 0;
    
    envp->reg.cx = 0;
    envp->reg.ds = wDSdup;
    envp->reg.es = envp->reg.ds;
    envp->reg.ss = envp->reg.ds;
    envp->reg.bp = (REGISTER)GetPhysicalAddress(envp->reg.ss);
    envp->reg.sp = envp->reg.bp + ((lpUserDLL->wInitSP) ? 
				   lpUserDLL->wInitSP : 
				   lpUserDLL->wStackBottom);

    envp->reg.di = wDSdup;
    envp->reg.si = GetDataSelectorFromInstance(hPrevInstance);
    envp->reg.bx = lpUserDLL->wInitStack;
    envp->trans_addr = (BINADDR)MAKELONG(lpUserDLL->wInitIP,
					 (WORD)envp->reg.cs);

    InitializeTask(hTask, envp, hDSdup,
		   lpUserDLL->wInitHeap,
		   lpUserDLL->wInitStack,
		   lpUserDLL->wStackBottom-lpUserDLL->wInitStack);
    DirectedYield(hTask);

#endif
    return (int)modinfo->ObjHead.hObj;
}

void 
TWIN_InitializeBinaryCode(int nInt)
{
#ifdef TWIN_BIN16
    HGLOBAL hThunkSeg;
    LPBYTE lpRetThunk;
    extern void TWIN_InitializeConvertMsg(void);


    InitClassBinProcs();
    if (!(hThunkSeg = GlobalAlloc(GHND,0x20)))
	FatalAppExit(0,"Failed to allocate return thunk segment");

    lpRetThunk = (LPBYTE)GlobalLock(hThunkSeg);
    return_thunk_selector =
		AssignSelector(lpRetThunk,0,TRANSFER_RETURN,0x20);
    SetSelectorHandle(return_thunk_selector,hThunkSeg);

    lpfnDefaultBinToNat = (WNDPROC)hsw_common_bin_to_nat;
    lpfnDefaultNatToBin = (WNDPROC)hsw_common_nat_to_bin;
    lpfnOEMGetPAddr = (FARPROC)OEM_GetProcAddress;
    lpfnEditMemBin = EditMemoryBin;

    TWIN_InitializeConvertMsg();
#endif
}

BOOL
LoadSegment(UINT uiSel)
{
    UINT uiSegNum;
    MODULEINFO *modinfo;

#ifdef X386
    if (uiSel == native_cs || uiSel == native_ds) {
	ERRSTR((LF_ERROR," ... failed with native code or data segment\n"));
	return FALSE;
    }
#endif

    if (GetPhysicalAddress(uiSel) != (LPBYTE)-1) {
	ERRSTR((LF_ERROR," ... invalid segment, no address defined\n"));
	return FALSE;	/* segment is already loaded or invalid */
    }

    if (!(modinfo = GetModuleFromSelector(uiSel))) {
	ERRSTR((LF_ERROR," ... failed to find module\n"));
	return FALSE;
    }

    uiSegNum = (uiSel >>3) - modinfo->wSelBase + 1;

    return LoadDuplicateSegment(uiSel, uiSegNum, modinfo);
}

static void 
CreatePSP(LPSTR lpCmdLine, HTASK hTask)
{
    HGLOBAL hGlobal;
    UINT uPSPSelector;
    TASKINFO *lpTaskInfo;
    LPBYTE lpStruct;
    WORD wSel;

    if (!lpCmdLine)
	lpCmdLine = "";

    if (hTask == 0)
	return;

    lpTaskInfo = (TASKINFO *)GETTASKINFO(hTask);
    lpTaskInfo->lpCmdLine = lpCmdLine;

		/* allocate TDB and PSP segment */
    hGlobal = GlobalAlloc(GHND,0x200);
    lpStruct = (LPBYTE)GlobalLock(hGlobal);
		/* assign TDB and PSP selectors */
    lpTaskInfo->wTDBSelector = AssignSelector(lpStruct,0,TRANSFER_DATA,0x200);
    uPSPSelector = AssignSelector(lpStruct+0x100,0,TRANSFER_DATA,0x100);
    SetSelectorHandle(lpTaskInfo->wTDBSelector,hTask);
		/* fill in TDB fields */
    PUTWORD(lpStruct+0x60,uPSPSelector);
    lstrcpy((LPSTR)(lpStruct+0x180),lpCmdLine);
    PUTWORD(lpStruct+0x62,0x80); /* far ptr to command line in PSP */
    PUTWORD(lpStruct+0x64,uPSPSelector);
    *(lpStruct+0xfa) = 'T';	/* TDB signature */
    *(lpStruct+0xfb) = 'D';

	/* allocate env segment and put selector to PSP+0x2c */
	/* PSP is offset 0x100 from TDB */
    wSel = BuildEnvSegment(lpCmdLine);

    PUTWORD(lpStruct+0x12c,wSel);

    RELEASETASKINFO(lpTaskInfo);
}
BOOL
InitBinary(LPSTR lpCmdLine, UINT uFlags)
{
    HGLOBAL hGlobal;
    LPBYTE lpStack;
    HTASK  hTask;
    static BOOL bInitBinary = FALSE;

    if (bInitBinary)
	return TRUE;

    if ((hTask = GetCurrentTask())) {
    	    CreatePSP(lpCmdLine, hTask);

	    envp_global = (ENV *)WinMalloc(sizeof(ENV));
	    memset((LPSTR)envp_global,'\0',sizeof(ENV));

	    hGlobal = GlobalAlloc(GHND,STACKSIZE);
	    lpStack = (LPBYTE)GlobalLock(hGlobal);
	    envp_global->reg.ss = AssignSelector(lpStack,0,TRANSFER_DATA,STACKSIZE);
	    SetSelectorHandle(envp_global->reg.ss,hGlobal);
	    envp_global->reg.bp = envp_global->reg.sp = (REGISTER)
				(lpStack + STACKSIZE - sizeof(DWORD));

	    bInitBinary = TRUE;
    }
    return TRUE;
}

int
FillUserDLL(LPUSERDLL lpDLL)
{
#ifdef TWIN_BIN16
    UINT	ret;
    WORD	count;
    DWORD	dwcount;
    LPSTR	lpbyte;

/* alloc memory and read first 0x40 bytes of the MZ header */
    lpDLL->lpHeaderImage = (LPBYTE)WinMalloc(EXEHDR_SZ);

    memset(lpDLL->lpHeaderImage,0,EXEHDR_SZ);

    if ((ret = _lread(lpDLL->hfExeFile,lpDLL->lpHeaderImage,
		EXEHDR_SZ)) != EXEHDR_SZ)
	LEXE_exit(LXERR_FILEREAD);

    count = GETWORD(lpDLL->lpHeaderImage+MZ_SIGNATURE);

    if (GETWORD(lpDLL->lpHeaderImage+MZ_SIGNATURE) != 0x5a4d)
	LEXE_exit(LXERR_NOMZHDR);

/* read in first 0x40 bytes of the NE header */
    dwcount = GETDWORD(lpDLL->lpHeaderImage+0x3c);
    lpDLL->dwNEOffset = dwcount;

/* seek to start of new exe header */
    _llseek(lpDLL->hfExeFile,lpDLL->dwNEOffset,0);

    memset(lpDLL->lpHeaderImage,0,NEWEXEHDR_SZ);

/* read in the new exe header */
    if ((ret = _lread(lpDLL->hfExeFile,
	lpDLL->lpHeaderImage,NEWEXEHDR_SZ)) < NEWEXEHDR_SZ)
	LEXE_exit(LXERR_FILEREAD);

    lpbyte = (LPSTR) (lpDLL->lpHeaderImage+NE_SIGNATURE);
    count = GETWORD(lpDLL->lpHeaderImage+NE_SIGNATURE);

    if (GETWORD(lpDLL->lpHeaderImage+NE_SIGNATURE) != 0x454e) {
	return 0;
    }

/* determine the size of the NE header, realloc memory and read it in 
    (everything up to an including non-resident table) */

    lpDLL->dwResPart = GETDWORD(lpDLL->lpHeaderImage+NONRESIDENT_TBL);
    lpDLL->wNResTblSize = GETWORD(lpDLL->lpHeaderImage + NONRESIDENTTBL_SZ);
    lpDLL->lpHeaderImage = (LPBYTE)WinRealloc((LPSTR)lpDLL->lpHeaderImage,
		lpDLL->dwResPart);

    _llseek(lpDLL->hfExeFile,lpDLL->dwNEOffset,0);

    if ((ret = _lread(lpDLL->hfExeFile,lpDLL->lpHeaderImage,
	lpDLL->dwResPart - lpDLL->dwNEOffset)) !=
		(lpDLL->dwResPart - lpDLL->dwNEOffset))
	LEXE_exit(LXERR_FILEREAD);

    if (lpDLL->wNResTblSize) {
	lpDLL->lpNonResTable = (LPBYTE)WinMalloc(lpDLL->wNResTblSize);
	_llseek(lpDLL->hfExeFile,lpDLL->dwResPart,0);
	if ((ret = _lread(lpDLL->hfExeFile,lpDLL->lpNonResTable,
		lpDLL->wNResTblSize)) != lpDLL->wNResTblSize)
	    LEXE_exit(LXERR_FILEREAD);
    }
/* extract values that we need for future use */
    lpDLL->wProgramFlags = GETWORD(lpDLL->lpHeaderImage + HEADER_FLAG);
    lpDLL->bAdditFlags = GETWORD(lpDLL->lpHeaderImage + MORE_FLAGS);
    lpDLL->wFLOffset = GETWORD(lpDLL->lpHeaderImage + FAST_LOAD_AREA);
    lpDLL->wFLLength = GETWORD(lpDLL->lpHeaderImage + FAST_LOAD_SZ);
    lpDLL->wAutoData = GETWORD(lpDLL->lpHeaderImage + AUTO_DATASEG);
    lpDLL->wInitHeap = GETWORD(lpDLL->lpHeaderImage + INIT_HEAP);

#if 0
    /* this will guarantee some kind of initial heap, so that
     * LocalAlloc() will have something to expand if its not
     * big enough. (This is for schdplus.exe and friends)
     * (the word is that right now it crashes winword.exe)
     */
    if (lpDLL->wInitHeap == 0)
	lpDLL->wInitHeap = 0x110;
#endif
    lpDLL->wInitStack = GETWORD(lpDLL->lpHeaderImage + INIT_STACK);
	
    lpDLL->wInitCS = GETWORD(lpDLL->lpHeaderImage + INIT_CS);
    lpDLL->wInitIP = GETWORD(lpDLL->lpHeaderImage + INIT_IP);
    lpDLL->wInitSS = GETWORD(lpDLL->lpHeaderImage + INIT_SS);
    lpDLL->wInitSP = GETWORD(lpDLL->lpHeaderImage + INIT_SP);
    lpDLL->wSegCount = GETWORD(lpDLL->lpHeaderImage + SEGMENTTBL_SZ);
    lpDLL->wSegOffset = GETWORD(lpDLL->lpHeaderImage + SEGMENT_TBL);
    lpDLL->wModCount = GETWORD(lpDLL->lpHeaderImage + MODULEREFTBL_SZ);
    lpDLL->wModOffset = GETWORD(lpDLL->lpHeaderImage + MODULEREF_TBL);
    lpDLL->wEntryTblSize = GETWORD(lpDLL->lpHeaderImage + ENTRYTBL_SZ);
    lpDLL->wEntryTblOffset = GETWORD(lpDLL->lpHeaderImage + ENTRY_TBL);
    lpDLL->wResourceTblOffset = GETWORD(lpDLL->lpHeaderImage + RESOURCE_TBL);
    lpDLL->dwNResTblOffset = GETDWORD(lpDLL->lpHeaderImage + NONRESIDENT_TBL);
    lpDLL->wResTblOffset = GETWORD(lpDLL->lpHeaderImage + RESIDENT_TBL);
    lpDLL->wImpTblOffset = GETWORD(lpDLL->lpHeaderImage + IMPORTED_TBL);
    lpDLL->wMovCount = GETWORD(lpDLL->lpHeaderImage + MOVABLE_ENTRYPT);
    lpDLL->wSectorShift = GETWORD(lpDLL->lpHeaderImage + SHIFT_CNT);
    lpDLL->wTargetOS = GETWORD(lpDLL->lpHeaderImage + TARGET_OS);
    lpDLL->wWinVer = GETWORD(lpDLL->lpHeaderImage + WIN_VER);

    lpDLL->lpImportTable =
		lpDLL->lpHeaderImage+lpDLL->wImpTblOffset;
    lpDLL->lpSegmentTable =
		lpDLL->lpHeaderImage+lpDLL->wSegOffset;
    lpDLL->lpModuleTable =
		(LPWORD)(lpDLL->lpHeaderImage+lpDLL->wModOffset);
    lpDLL->lpEntryTable =
		lpDLL->lpHeaderImage+lpDLL->wEntryTblOffset;
    return 1;
#else
    return 0;
#endif
}

void TWIN_SetPSPSelector(HTASK hTask, WORD selector)
{
    TASKINFO *lpTaskInfo;
    LPBYTE lpTDB;
    
    lpTaskInfo = (TASKINFO *) GETTASKINFO(hTask);
    lpTDB = GetSelectorAddress(lpTaskInfo->wTDBSelector);
    
    PUTWORD(lpTDB + 0x60, selector);
    RELEASETASKINFO(lpTaskInfo);
}
