/********************************************************************

	@(#)DrvConfig.c	1.16   Unix configuration subsystem implementation.
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


********************************************************************/


/* Includes */
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <Driver.h>

#include <pwd.h>

#include "WinConfig.h"

/* Local prototypes */
static char *DrvGetTwinPrefsFName(void);

static DWORD DrvConfigDoNothing(LPARAM,LPARAM,LPVOID);
static DWORD DrvConfigGetDefaults(LPARAM,LPARAM,LPVOID);
static DWORD DrvConfigGetFileName(LPARAM,LPARAM,LPVOID);

DWORD DrvConfigTab(void);

/* Driver configuration defaults */
/* Twin RC file default table format: */
/* Parameter, Opcode, Section, Keyname, Default, Env, lParam, string */
TWIN_CONFIG  wcpdata[] = {
/* [boot] */
	{WCP_DISPLAY, 6,WCS_BOOT,"display",	":0.0",		"DISPLAY", 0},
	{WCP_WINDOWS, 6,WCS_BOOT,"windows",	0,		"WINDOWS", 0},
	{WCP_TEMP,    6,WCS_BOOT,"temp",	"/tmp",		"TMP",     0},
	{WCP_OPENPATH,1,WCS_BOOT,"openpath",	"",		"OPENPATH",0},

/* [windows] */
	{WCP_DBLCLICK, 1,WCS_WINDOWS,"DoubleClickRate", "250",	0,	   0},
	{WCP_FONTFACE, 1,WCS_WINDOWS,"FontFace", 	"Helv",	0,	   0},
	{WCP_FONTSIZE, 1,WCS_WINDOWS,"FontSize", 	"18", 	0,	   0},
	{WCP_FONTBOLD, 1,WCS_WINDOWS,"FontWeight",   	"700", 	0,	   0},

#ifdef TWIN32
	{WCP_MENUSIZE, 1,WCS_WINDOWS,"MenuSize", 	"14", 	0,	   0},
	{WCP_MENUBOLD, 1,WCS_WINDOWS,"MenuWeight",   	"400", 	0,	   0},
	{WCP_DLGSIZE, 1,WCS_WINDOWS,"MenuSize", 	"12", 	0,	   0},
	{WCP_DLGBOLD, 1,WCS_WINDOWS,"MenuWeight",   	"700", 	0,	   0},
#else
	{WCP_MENUSIZE, 1,WCS_WINDOWS,"MenuSize", 	"18", 	0,	   0},
	{WCP_MENUBOLD, 1,WCS_WINDOWS,"MenuWeight",   	"700", 	0,	   0},
	{WCP_DLGSIZE, 1,WCS_WINDOWS,"MenuSize", 	"12", 	0,	   0},
	{WCP_DLGBOLD, 1,WCS_WINDOWS,"MenuWeight",   	"700", 	0,	   0},
#endif

	{WCP_ICONFONTFACE, 1,WCS_WINDOWS,"IconFontFace","Helv",	0,	   0},
	{WCP_ICONFONTSIZE, 1,WCS_WINDOWS,"IconFontSize","14", 	0,	   0},

	{0,0,0,0,0,0,0},
};

static DWORD
DrvConfigGetDefaults(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
	return (DWORD)wcpdata;
}

static DWORD
DrvConfigGetFileName(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
	return (DWORD)DrvGetTwinPrefsFName();
}

static DWORD
DrvConfigDoNothing(LPARAM dwParam1, LPARAM dwParam2, LPVOID lpStruct)
{
	return 1L;
}


static TWINDRVSUBPROC DrvConfigEntryTab[] = {
	DrvConfigDoNothing, 
	DrvConfigDoNothing, 
	DrvConfigDoNothing,
	DrvConfigGetFileName,
	DrvConfigGetDefaults
};

DWORD
DrvConfigTab(void)
{
	return (DWORD)DrvConfigEntryTab;
}

/********************************************************************
*   DrvGetTwinPrefsFName
*
*	Get the full path name of the "Twin Preferences" file.
********************************************************************/
static char *DrvGetTwinPrefsFName(void)
{
        int length;
	static char twinrc[256];
	char *p;
   	struct   stat sbuf; 	 
	struct   passwd *pw;

	/* OPTION 1: if TWINRC set, use its value */
	if((p = (char *) getenv("TWINRC"))) {
		strcat(twinrc,p);
		return twinrc;
	}

	/* OPTION 2: check to see if a twinrc in current directory */
	getcwd(twinrc,256);

        /* Add slash/backslash? */
        length = strlen( twinrc );
	p = twinrc + (length - 1 );
        if( *p != '/' && *p != '\\' )
        {
          strcat(twinrc,TWIN_SLASHSTRING);
        }
	strcat(twinrc,"twinrc");

  	/* should be mfs_stat */
	/* also check stat value for a file vs. other type. */
	if(stat(twinrc,&sbuf) == 0) {
		if(S_ISREG(sbuf.st_mode)) {
			return twinrc;
		}
	}

	/* OPTION 3: get user home directory, use .twinrc */
	p = (char *)getenv("HOME");
	if (p == 0) {
		if((pw = getpwuid(getpid()))) {
			p = pw->pw_dir;	
		}
	}
	if (p) {
		sprintf(twinrc,"%s/%s",p,TWINRCFILE);
	}
	else 
        {
          /* If none, use current working directory */
	  getcwd(twinrc,256);

          /* Add slash/backslash? */
          length = strlen( twinrc );
	  p = twinrc + (length - 1 );
          if( *p != '/' && *p != '\\' )
          {
            strcat(twinrc,TWIN_SLASHSTRING);
          }
	  strcat(twinrc,"twinrc");
	}

	return twinrc;
}
