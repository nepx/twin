/*    
	mfs_findfile.c	1.22
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

*	MFS find file implementation.

 */

#include "platform.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>

#include "xdos.h"
#include "Log.h"
#include "BinTypes.h"
#include "xdosproc.h"
#include "dosdefn.h"
#include "Endian.h"

#include "mfs_config.h"
#include "mfs_core.h"
#include "mfs_dir.h"

#ifndef TM_TIME
#define TM_TIME(ft)	((((ft)->tm_hour<<11) & 0xF800) | \
			(((ft)->tm_min<<5) & 0x07E0) | \
			(((ft)->tm_sec>>1) & 0x001F))
#endif

#ifndef TM_DATE
#define TM_DATE(ft)	(((((ft)->tm_year - 80)<<9) & 0xFE00) | \
			((((ft)->tm_mon+1)<<5) & 0x01E0) | \
			(((ft)->tm_mday) & 0x001F))
#endif

extern CONFIG xdos;
void CopyDTAData(LPBYTE ,char *,WORD ,WORD ,DWORD ,BYTE );
unsigned long error_code(unsigned int);

void
get_mask(char filename[], char *buffer)
{
    int i, j;

    if (strcmp(buffer, "*") == 0 || strcmp(buffer, "*.*") == 0)
	strcpy(filename, "*.*");

    /* Copy base name into filename, skip all spaces. */
    j = 0;
    for (i = 0; i < 8; ++i, ++j, ++buffer)
	{
	if (*buffer == ' ')
	    break;
	filename[j] = *buffer;
	}
    while (i < 8 && *buffer == ' ')
	{
	++i;
	++buffer;
	}

    /* Quit if no extension. */
    if (*buffer == ' ') /* Successive bytes can't have non-spaces. */
	{
	filename[j] = '\0';
	return;
	}

    /* Copy extension into filename, skip all spaces. */
    filename[j++] = '.';
    for (i = 0; i < 3; ++i, ++j, ++buffer)
	{
	if (*buffer == ' ')
	    break;
	filename[j] = *buffer;
	}

    filename[j] = '\0';
}

void
set_mask(char buffer[], char *filename)
{
    char *p, *q;
    int i;

    p = strrchr(filename, '\\');
    if (p && p != filename)
	++p;
    else
	p = filename;

    /* Simplify wildcard if possible. */
    if (strcmp(p, "???????????") == 0 || strcmp(p, "????????.???") == 0 ||
	strcmp(p, ".") == 0 || strcmp(p, "..") == 0)
	p = "*";

    if ((q = strchr(p, '.')) == 0)
	{
	/* No extension.  Copy the base name and pad the rest with spaces. */
	for (i = 0; i < 8 && *p; ++i, ++p)
	    buffer[i] = *p;
	for (; i < 11; ++i)
	    buffer[i] = ' ';
	}
    else
	{
	/* Put base name into first 8 bytes, pad with spaces if shorter. */
	for (i = 0; i < 8; ++i, ++p)
	    {
	    if (*p == '.')
		break;
	    buffer[i] = *p;
	    }
	for (; i < 8; ++i)
	    buffer[i] = ' ';

	/* Put extension in last 3 bytes, pad with spaces if shorter. */
	if (*p == '.')
	    ++p;
	for (; i < 11; ++i, ++p)
	    {
	    if (*p == 0)
		break;
	    buffer[i] = *p;
	    }
	for (; i < 11; ++i)
	    buffer[i] = ' ';
	}
}

/*********************************************************************/
DWORD 
mfs_findfirst(DWORD p1, DWORD p2, DWORD p3, DWORD p4) /* %%% */
{
   LPSTR  filename; /* p1 */
   int    attrib;   /* p2 */
   LPBYTE dta;      /* p3 */
   int    nDrive;
   char   szDir[_MAX_PATH];

   filename = (LPSTR)p1;
   attrib = (int)p2;
   dta = (LPBYTE)p3;

   /*
   **    Special case for _A_VOLID...
   */
   if (attrib == _A_VOLID)
      {
      if (toupper(filename[0]) < 'A' || toupper(filename[0]) > 'Z')
         nDrive = MFS_GETDRIVE();
      else if (filename[1] != ':')
              nDrive = MFS_GETDRIVE();
      else nDrive = toupper(filename[0]) - 'A' + 1;

      if (xdos.drivemap[nDrive] != NULL)
	{
	CopyDTAData(dta, xdos.drivemap[nDrive]->name, 0, 0, 0, 0);
	return MFS_SUCCESS;
	}
      return MFS_FAILURE;
      }
   else
      {
      /*   Retrive directory spec, then chdir to that directory and
      **   call findnext to find first file...
      */
      char *p;
      nDrive = MFS_ALTNAME(szDir, filename);
      p = strrchr(szDir, '/');
      if (p && p != szDir)
	*p = '\0';
      }
   MFS_CHDIR(szDir);
   *dta = (BYTE)nDrive;
   set_mask(dta+0x1, filename);
   *(dta+0xc) = (BYTE) attrib;
   PUTWORD(dta+0xd, 0); /* count */
   PUTWORD(dta+0xf, 0); /* cluster */
   return(mfs_findfile((DWORD)szDir, 0, (DWORD)dta, 0));
}

/*********************************************************************/
DWORD 
mfs_findnext(DWORD p1, DWORD p2, DWORD p3, DWORD p4) /* %%% */
{
	return(mfs_findfile((DWORD)NULL, (DWORD)0, p1, (DWORD)0));
}

char *
mfs_slash(int id)
{
	if(id == 0)
		return "/";
	return "\\";
}

/*********************************************************************/
DWORD 
mfs_findfile(DWORD p1, DWORD p2, DWORD p3, DWORD p4)
{
	LPSTR   pathname;  /* p1 */
	int	attr;      /* p2 */
	LPBYTE  dta;	   /* p3 */
	char	result[256];
	BYTE	bAttr = 0x0;
	char    filename[15];
	char   *fname;
	struct stat sbuf; 	 
	struct  tm *filetime;
	int	rc;
	static MFSDIRENT *dirp;

	pathname = (LPSTR)p1;
	attr = (int)p2;
	dta = (LPBYTE)p3;

	if (pathname) {
	   if (dirp) 
	      mfs_closedir((DWORD)dirp, 0L, 0L, 0L);
	   dirp = (MFSDIRENT *)mfs_opendir((DWORD)pathname, 0L, 0L, 0L);
	}

	if (dirp == 0)
	   return MFS_FAILURE;

	get_mask(filename, (LPSTR)(dta+0x1));
	attr = (int) *(dta+0xc);

	while (TRUE) {
	   fname = (LPSTR)mfs_readdir((DWORD)dirp, 0L, 0L, 0L);

	   if (fname == NULL) {
	      mfs_closedir((DWORD)dirp, 0L, 0L, 0L);
	      dirp = 0;
	      error_code(NO_MORE_FILES);
	      return MFS_FAILURE;
	   }
	    
	   /* check for match with filespec */
	   if (mfs_regexp((DWORD)fname, (DWORD)filename, 0L, 0L)) {

	      /* we need to build the right name...
	       * fname is just a filename, use directory saved in MFSDIRENT
	       */
	      strcpy(result,dirp->mdir);
	      strcat(result,mfs_slash(0));
	      strcat(result,fname);

	      /* matched, now check file attributes */
	      rc = mfs_stat((DWORD)result, (DWORD)&sbuf, 0L, 0L);
	      if (rc == 0) {	
		
		 if (sbuf.st_mode & S_IFDIR) {
		    if ((attr & 0x10) == 0)
		       continue;
		    bAttr = 0x10;
		 } else {
		      if ((attr & 0x8010) == 0x8010)
			 continue;
		      bAttr = 0;
		 }
 	      } else {
		continue;
	      }

	      filetime = localtime(&sbuf.st_mtime);
	      strupr(fname);
	      CopyDTAData(dta,fname, TM_TIME(filetime), TM_DATE(filetime),
			(DWORD)sbuf.st_size,bAttr);
	      break;
	   } /* if name matched */ 
	} /* while */
	return MFS_SUCCESS;
}
