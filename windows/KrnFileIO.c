/*    
	KrnFileIO.c	1.12
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
#include "windowsx.h"

#include "Log.h"
#include "mfs_config.h"
#include "FileSys.h"

char *strpbrkr(char *,char *);
extern char *TWIN_WinPath;

DWORD WINAPI
FileCdr(FARPROC lpfnCdrProc)
{
    APISTR((LF_APISTUB,"FileCdr(FARPROC=%x)\n",lpfnCdrProc));
    return (DWORD)TRUE;
}

/*
 * open the file given by lpszFileName
 * search in all the right places...
 * put in the 'resulting' name in buf
 * 
 * Search order is...
 * 1. The current directory and/or a fully qualified name.
 * 2. The Windows directory.
 * 3. The Windows system directory.
 * 4. The directory containing the executable file for hTask.
 * 5. The directories listed in the PATH env variable.
 * 
 * bailout w/ failure...
 */

static int
appendslash(char *path) 
{
	char *slash;
	if((slash = strrchr(path,'/'))) {
		slash++;
		if(*slash)
			return 1;
		return 0;
	}
	return 1;
}

HFILE
OpenEngine(LPCSTR lpszFile,LPOFSTRUCT lpofs,UINT wFunction)
{
    char szFullPath[_MAX_PATH];
    int mode, nLen;
    LPSTR lpTemp,lpszFileName = (LPSTR)lpszFile;
    HFILE hFile;
    char *PATH,*path,*next;

    /* set the open mode from the lowest two bits, this is for _lopen  */
    mode  = wFunction & 0x3;

    /* PHASE 1: use current directory and/or fully qualified name  */
    /* note:    this will also resolve any leading <DRIVE>: prefix */
    /*          the <DRIVE>: prefix can resolve to either a mapped */
    /*  	directory or a physical device (floppy)	 	   */

#ifdef LATER
	handle case of OF_REOPEN
	does this open the file given by the lpofs struct?
        if it fails, do we restart the whole thing?
#endif

    MFS_ALTPATH(szFullPath, lpszFileName);

    hFile = _lopen(szFullPath, mode);

    /* if it worked, then we are done... */
    if (hFile != HFILE_ERROR) {
	nLen = min((int)strlen(szFullPath),_MAX_PATH-1);
	strncpy(lpofs->szPathName,szFullPath,nLen);
	lpofs->szPathName[nLen] = '\0';
	return hFile;
    }

    /* if it was fully qualified, and we are NOT to search then quit */
    /* LATER do we want basname here?  we may have foo/bar           */
    /*       in which case do we want just the bar or foo/bar?       */
    if ((*lpszFileName == '/') || (*(lpszFileName+1) == ':') ||
		(*lpszFileName == '\\')) {
	if ((wFunction & OF_SEARCH) == 0)
	    return hFile;
    }

    /* note: if lpTemp then we do have strlen, so check if its the last */
    if ((lpTemp = (LPSTR)strpbrkr(szFullPath,"/:\\")))
	lpszFileName = lpTemp+1;

    if (strlen(lpszFileName) == 0)
	return hFile;

    /* PHASE 2: use windows directory */
    GetWindowsDirectory(lpofs->szPathName, 255);

    if (appendslash(lpofs->szPathName))
    	strcat(lpofs->szPathName,"/");

    strcat(lpofs->szPathName,lpszFileName);

    hFile = _lopen(lpofs->szPathName,mode);
    if (hFile != HFILE_ERROR) {
	return hFile;
    }

    /* PHASE 3: use system directory */
    GetSystemDirectory(lpofs->szPathName, 255);

    if (appendslash(lpofs->szPathName))
    	strcat(lpofs->szPathName,"/");

    strcat(lpofs->szPathName,lpszFileName);

    hFile = _lopen(lpofs->szPathName,mode);

    if (hFile != HFILE_ERROR) {
	return hFile;
    }

    PATH = (char *) WinStrdup(TWIN_WinPath);
    /* walk the path... */
    
    for(next = PATH;next && *next;next = path) {

	/* get the next element and terminate it */
	path = strchr(next,':');
	if(path) {
		*path = 0;		/* end the current element */
		path++;			/* point to the nextone    */
	}

	/* build the current name... */
	strcpy(lpofs->szPathName,next);

        if(appendslash(lpofs->szPathName))
    		strcat(lpofs->szPathName,"/");

	strcat(lpofs->szPathName,lpszFileName);

	hFile = _lopen(lpofs->szPathName,mode);
	if (hFile != HFILE_ERROR) {
		WinFree(PATH);
		return hFile;
	}
    }

    if(PATH)
    	WinFree(PATH);
    
    return hFile;
}

/* 
 *	OpenFile
 *	OpenFile is the primary interface to the filesystem.
 *	There are three types of operations that can be performed.
 *		1) open/create a file	OF_READ,OF_READWRITE,OF_WRITE,OF_CREATE
 *		2) delete a file	OF_DELETE
 *		3) parse a filename	OF_PARSE
 *
 *	For case 1 and 2, you can pass a flag OF_REOPEN to use the
 *	data in an OFSTRUCT to reopen the file instead of using 
 *	the specified filename.
 *
 *	For case 1, you can pass a flag, OF_EXIST which will close the
 *	file after it has been opened/created.
 *
 *	For case 1 and 2, except for OF_CREATE, you can use the OF_PROMPT
 *	flag, with the optional OF_CANCEL flag to prompt the user for
 *	a filename.
 *	
 *	For OF_READ only, you can use the OF_VERIFY flag to see if the
 *	file data/time stamp has changed.
 *
 *	issues:
 *		must special case devices, LPTx: COMx:
 */

HFILE WINAPI
OpenFile(LPCSTR lpszFileName,OFSTRUCT *lpofs,UINT wFunction)
{
    HFILE       fd;
    const char *lpstr = lpszFileName;

    /*
     *  The "lpstr" variable is needed as a workaround to a bug
     *  in the SunOS "acc" compiler, which treats the LPCSTR typedef
     *  as type "const char * const" rather than the requested
     *  "const char *".
     */

    APISTR((LF_APICALL,"OpenFile(LPCSTR=%s,OFSTRUCT=%p,UINT=%x)\n",
	lpszFileName,lpofs,wFunction));

    if ((wFunction & OF_REOPEN)) {
	lpstr = lpofs->szPathName;
    }
    if (!lpstr) {
           APISTR((LF_APIRET,"OpenFile: returns HFILE HFILE_ERROR\n"));
           return HFILE_ERROR;
    }

    if(wFunction & OF_PARSE) {
	memset((LPSTR)lpofs,0,sizeof(OFSTRUCT));
	MFS_ALTPATH(lpofs->szPathName, lpstr);
        APISTR((LF_APIRET,"OpenFile: returns HFILE 0\n"));
	return 0;
    }

    lpofs->fFixedDisk = 1;

    if(wFunction & OF_DELETE) {
	if(_ldelete(lpstr) == 0) {
          APISTR((LF_APIRET,"OpenFile: returns HFILE 0\n"));
	  return 0;
	} else {
          APISTR((LF_APIRET,"OpenFile: returns HFILE HFILE_ERROR\n"));
	  return HFILE_ERROR;
	}
    }

    if(wFunction & OF_CREATE) {
	fd = _lcreat(lpstr, 0);
        strcpy(lpofs->szPathName,lpstr);
    } else
	fd = OpenEngine(lpstr,lpofs,wFunction);		

#ifdef LATER
    if(fd < 0) {
	if((wFunction & (OF_PROMPT|OF_CREATE)) == OF_PROMPT) {
	}
    }
    if((wFunction & (OF_READ|OF_VERIFY)) == (OF_READ|OF_VERIFY)) {
    }
#endif

    if(fd < 0) {
    	APISTR((LF_APIRET,"OpenFile: returns HFILE %x\n",(UINT)fd));
	return fd;
    }

    if(wFunction & OF_EXIST)
	_lclose(fd);

    APISTR((LF_APIRET,"OpenFile: returns HFILE %x\n",(UINT)fd));
    return fd;
}
