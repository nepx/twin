/*    
	FileSys.c	2.33
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
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "windows.h"
#include "windowsx.h"

#include "kerndef.h"
#include "Log.h"
#include "Resources.h"
#include "Module.h"
#include "mfs_config.h"
#include "mfs_fileio.h"
#include "compat.h"
#include "WinConfig.h"
#include "FileSys.h"

extern char *strpbrkr(char *,char *);
extern char *WinSaveString(LPSTR);

/*
 * we will support a system drive array, which shows the mapping
 * between drive numbers (letters) and unix directories and/or
 * devices, ala a: b:
 * 
 * we may have default mapping so that h: is the home directory,
 * other mappings are possible, eg. m: is /usr/spool/mail.
 *
 * for now, just return DRIVE_FIXED, later we will replace with
 * DRIVE_REMOVEABLE	for floppy drives using the DOS file package
 * DRIVE_REMOTE		for NFS file systems...
 */

#define DRIVE_C	'C'

UINT WINAPI
#ifdef TWIN32
GetDriveType16(int nDrive)
#else
GetDriveType(int nDrive)
#endif
{
	UINT	drvtype;

	/*   API accepts 0 as A:, etc, while MFS thinks 0 is current drive
	**   and A: starts from 1
	*/
	drvtype = (UINT)MFS_DRIVETYPE(nDrive+1);

	APISTR((LF_API,"GetDriveType(drive:%x) returns %x\n",nDrive,drvtype));
	return drvtype;
}

/*
 * If the environment variable WINDIR is set, then set WindowsDirectory
 * to that. Read system.ini to get WindowsSystemDirectory, defaulting to
 * WindowsDirectory appended with "system".
 */
 
char *TWIN_WinSysDir;
char *TWIN_WinDir;
char *TWIN_WinPath;
char *TWIN_TempPath;

void
InitFileSystem()
{
    int  len; 
    char windir[_MAX_PATH];

    /* get the windows directory */
    GetTwinString(WCP_WINDOWS,windir,_MAX_PATH);

    /* Remove the terminating slash if it exists */
    len = strlen(windir);
    if (windir[len-1] == '/')
        windir[len-1] = '\0';

    /* allocate a buffer to hold the directory string */
    TWIN_WinDir = WinSaveString(windir);

    /* get the system directory by appending /system */
    strcat(windir, "/system");
    TWIN_WinSysDir = WinSaveString(windir);

    GetTwinString(WCP_OPENPATH,windir,_MAX_PATH);

    TWIN_WinPath = WinSaveString(windir);

    GetTwinString(WCP_TEMP, windir, _MAX_PATH);
    TWIN_TempPath = WinSaveString(windir);
}
	
UINT WINAPI
GetSystemDirectory(LPSTR lpBuffer, UINT nSize)
{
	int	nLength,nWSDLength;

	if (lpBuffer == NULL)
	    return 0;

	if (TWIN_WinSysDir == NULL) {
	    lpBuffer[0] = '\0';
	    return 0;
	}

	nWSDLength = strlen(TWIN_WinSysDir)+1;
	nLength = min((int)nSize,(int)nWSDLength);
	strncpy(lpBuffer,TWIN_WinSysDir,nLength);

	APISTR((LF_API,"GetSystemDirectory() returns %s\n",lpBuffer));
	return nWSDLength;
}

UINT  WINAPI
GetWindowsDirectory(LPSTR lpBuffer, UINT nSize)
{
	int nLength,nWDLength;

	if (lpBuffer == NULL)
	    return 0;

	if (TWIN_WinDir == NULL) {
	    lpBuffer[0] = '\0';
	    return 0;
	}

	nWDLength = strlen(TWIN_WinDir);
	nLength = min((int)nSize, (int)nWDLength+1);
	strncpy(lpBuffer, TWIN_WinDir, nLength);

	APISTR((LF_API,"GetWindowsDirectory() returns %s\n",lpBuffer));
	return (nSize <= nWDLength) ? nWDLength+1 : nLength-1;
}

BYTE WINAPI
GetTempDrive(char cDriveLetter)
{
	return DRIVE_C;
}

DWORD
GetTempPath(DWORD cchBuffer, LPTSTR lpszTempFile)
{

  MFS_DOSPATH(lpszTempFile, TWIN_TempPath);

  return 1L;
}

/*
 * build a temporary filename
 * if unique == 0
 *	use system time to create unique name,
 * else
 *	use unique to generate specific name
 * return the specific unique key used.
 */

int WINAPI
#ifdef TWIN32
GetTempFileName16(BYTE cDriveLetter, LPCSTR lpPrefix,
		UINT unique, LPSTR lpTempFile)
#else
GetTempFileName(BYTE cDriveLetter, LPCSTR lpPrefix,
		UINT unique, LPSTR lpTempFile)
#endif
{
    char szNativePath[256];
    LPSTR lpszPath = szNativePath;
    HFILE hFile;
    UINT uUnique = unique;
    OFSTRUCT of;
    int Len;
	
    cDriveLetter = cDriveLetter & TF_FORCEDRIVE ?
		cDriveLetter & ~TF_FORCEDRIVE:DRIVE_C;

    MFS_DOSPATH(szNativePath, TWIN_TempPath);

    if ( *(lpszPath+1) == ':' ) {
	cDriveLetter = *lpszPath;
	lpszPath += 2;
    } 

    if ( *lpszPath == '\\' || *lpszPath == '/' )
	lpszPath++;

    Len = strlen(lpszPath);
    if ( lpszPath[Len-1] == '\\' || lpszPath[Len-1] == '/' )
	lpszPath[Len-1] = '\0';

    if (uUnique == 0)
	uUnique = GetCurrentTime();

    sprintf(lpTempFile,"%c:\\%s\\%c%.3s%4.4x%s",
	cDriveLetter,lpszPath,'~',lpPrefix,uUnique,".TMP");

    APISTR((LF_API,
	"GetTempFileName(drive:%x,prefix:%s,unique=%x,tempfile=%s)\n",
	cDriveLetter,lpPrefix,uUnique,lpTempFile));

    if (unique)
	return uUnique;

    while ((hFile = OpenFile(lpTempFile,&of,OF_READWRITE|OF_EXIST)) !=
			HFILE_ERROR) {
	sprintf(lpTempFile,"%c:\\%s\\%c%.3s%4.4x%s",
	    cDriveLetter,lpszPath,'~',lpPrefix,++uUnique,".TMP");
	APISTR((LF_API,"GetTempFileName: trying %s...\n",lpTempFile));
    }

    if ((hFile = OpenFile(lpTempFile,&of,OF_CREATE)) == HFILE_ERROR)
	return 0;
    else {
	_lclose(hFile);
	return uUnique;
    }
}

/*
 * we should interface to system here, and get
 * available amount upto the system limit.
 */
UINT WINAPI
SetHandleCount(UINT nCount)
{
	return nCount;
}


UINT WINAPI
SetErrorMode(UINT fuErrorMode)
{
	static UINT fuCurrentMode = 0;
	UINT fuTemp;

	fuTemp = fuCurrentMode;
	fuCurrentMode = fuErrorMode;
	APISTR((LF_API,"SetErrorMode(mode:%x) returns %x\n",
		fuErrorMode,fuTemp));
	return fuTemp;
}

/************************************************************************/

#define MIN_RES_FD 0x100
#define MAKE_RES_FD(d) ((d) + MIN_RES_FD)
#define GET_INDEX(d) ((d) - MIN_RES_FD)
#define IS_RES_FD(d) (((d) >= MIN_RES_FD) ? 1 : 0)

#define NFILEHANDLES	128

struct {
	HANDLE hInst;
	HANDLE rsrc_handle;
	long file_ptr;
} rsrc_file_data[NFILEHANDLES];

int WINAPI
AccessResource(HINSTANCE hInst, HRSRC hRsrc) 
{ 
#ifdef LATER
	int fd;

	for(fd=0;fd < NFILEHANDLES && rsrc_file_data[fd].rsrc_handle != 0;fd++);

	if(fd == NFILEHANDLES)
		return -1;

	rsrc_file_data[fd].hInst = hInst;
	rsrc_file_data[fd].rsrc_handle = hRsrc;
	rsrc_file_data[fd].file_ptr = 0;
	fd = MAKE_RES_FD(fd);
	return fd;
#endif
	/* mem_xxx routines are disconnected */
	/* read AccessResource is in binary layer... */
	return -1;
}

#ifdef LATER
static int
mem_read(int fd, char *buffer, int nread)
{
	HRSRC hRsrc;
	HINSTANCE hInst;
	HANDLE h;
	int index;
	char *p;

	index = GET_INDEX(fd);
	hRsrc = rsrc_file_data[index].rsrc_handle;
	hInst = rsrc_file_data[index].hInst;
	h = LoadResource(hInst, hRsrc);
	p = (LPSTR)LockResource(h) + rsrc_file_data[index].file_ptr;
	memcpy(buffer, p, nread);
	rsrc_file_data[index].file_ptr += nread;
	UnlockResource(h);
	return nread;
}

static long
mem_seek(int fd, long offset, int origin)
{
	int index;

	index = GET_INDEX(fd);
	switch (origin) {
		case SEEK_SET:		/* from beginning */
			rsrc_file_data[index].file_ptr = offset;
			break;
		case SEEK_CUR:		/* from current position */
			rsrc_file_data[index].file_ptr += offset;
			break;
		case SEEK_END:		/* from end */
		default:
			break;
	}
	return rsrc_file_data[index].file_ptr;
}

static int
mem_close(int fd)
{
	rsrc_file_data[GET_INDEX(fd)].rsrc_handle = 0;
	return 0;
}
#endif

/************************************************************************/

#define OF_OPENMASK	(OF_READ|OF_READWRITE|OF_WRITE|OF_CREATE)
#define OF_FILEMASK	(OF_DELETE|OF_PARSE)
#define OF_MASK		(OF_OPENMASK|OF_FILEMASK)

HFILE
_lopen(LPCSTR lpfname,int wFunction)
{
	int fd;
	int nFunction;
   	
	/* Don't assume a 1:1 relationship between OF_* modes and O_* modes */
	/* Here we translate the read/write permission bits (which had better */
	/* be the low 2 bits.  If not, we're in trouble.  Other bits are  */
	/* passed through unchanged */
	
	nFunction = wFunction & 3;
   	
	switch (wFunction & 3) {
		case OF_READ:
			nFunction |= O_RDONLY;
   			break;
   		case OF_READWRITE:
   			nFunction |= O_RDWR;
   			break;
   		case OF_WRITE:
   			nFunction |= O_WRONLY;
   			break;
   		default:
   			ERRSTR((LF_ERROR, "_lopen: bad file open mode %x\n", wFunction));
   			return HFILE_ERROR;
   	}

	fd = MFS_OPEN((char*)lpfname, nFunction);
	if (fd < 0)
		 return HFILE_ERROR;
	return (HFILE)fd;
}

HFILE
_lclose(HFILE fd)
{
	if (fd == HFILE_ERROR)
		return HFILE_ERROR;
	return (HFILE)MFS_CLOSE(fd);
}

HFILE
_lcreat(LPCSTR lpfname,int fileattr)
{
    int fd;
    int openmask = O_CREAT|O_TRUNC;

    openmask |= O_RDWR;			/* READONLY case not supported */
    fd = MFS_CREATE((char*)lpfname, openmask);
    if (fd < 0)
	return HFILE_ERROR;
    return (HFILE)fd;
}

LONG
_llseek(HFILE fd, LONG offset, int origin)
{
    long position;
    if (fd == HFILE_ERROR)
	return -1L;
    position = -1L;
    MFS_SEEK(fd, offset, origin, &position);
    return position;
}

long
_hread(HFILE fd, void _huge* hpvbuffer, long count)
{
    if (fd == HFILE_ERROR)
	return -1L;
    return MFS_READ(fd, hpvbuffer, count);
}

long
_hwrite(HFILE fd, const void _huge* hpvbuffer, long count)
{
    return MFS_WRITE(fd, (char*)hpvbuffer, count);
}

UINT 
_lread(HFILE fd,LPVOID buffer,UINT count)
{
    UINT    rc;
    APISTR((LF_APICALL,"_lread(HFILE=%d,LPVOID=%p,UINT=%d\n",
	fd,buffer,count));
    rc =  _hread(fd,(void _huge*) buffer,(long) count);
    APISTR((LF_APIRET,"_lread returns UINT %d\n",rc));
    return rc;
}

UINT 
_lwrite(HFILE fd,const VOID *buffer,UINT count)
{
    return MFS_WRITE(fd, (char*)buffer, count);
}

int
_ldelete(LPCSTR lpfname)	
{
    int fd;

    fd = MFS_DELETE((char*)lpfname);
    if (fd < 0)
	return -1;
    return fd;
}

/***************************************************************************/

