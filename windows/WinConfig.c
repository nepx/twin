/*    
	WinConfig.c	1.23
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

/*************************************************************************
 *
 * Manage configuration parameters through a single configuration file.
 * 
 * These environment variables control parameters:
 *
 * TWINRC		location of configuration file if set
 * 			default, current working directory, home, willows
 * HOME			location of users home directory if set
 * 			default, value retrieved from pwnam
 * LOGNAME		user name if set
 *			default, use value from pwnam
 * DISPLAY		display server if set
 * 			default, value retrieved from twinrc file
 * PATH			path used to find and open files,
 *			default if not in TWINRC is to use PATH
 *			
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Driver.h"
#include "WinConfig.h"
#include "platform.h"
#include "compat.h"

/**************************************************************************
 *
 * [boot]		startup configuration settings
 * [windows]		run-time configuration settings
 * [xdos]		mapping of DOS drives
 * [commondialogs]	how to drive common dialog fileopen dialog
 *
 * [fontalias]		what font mapping to use
 * [colors]		what colors to use
 *  
 **************************************************************************
 *
 *	parameter	what parameter is being addressed
 *	opcode	
 *		1	use environment as override if set
 *			use section, keyname, default to lookup string
 *		6	use environment as override if set
 *			use section, keyname, default to lookup string
 *			if null, return path to TWINRC
 *
 *	lpszsection	section to use
 *	lpszkeyname	keyname to use
 *	lpszdefault	default value
 *	lpszenviron	environment string
 *	lpszstring	allocated buffer
 *
 *************************************************************************/


static char *sections[] = {
	"boot",
	"windows",
	"XDOS",
	"CommonDialogs"
};

static TWIN_CONFIG  *wcpdata = NULL;


/* name of configuration file */
extern BOOL TWIN_InitDriver();

BOOL InitTwinConfig();

char *
GetTwinFilename(void)
{
	static char *twinrc;

	/* have we already set it */
	if(twinrc)
		return twinrc;

	TWIN_InitDriver();
	twinrc = (char *) DRVCALL_CONFIG(PCFGH_GETFILENAME, 0, 0 , NULL);

	return twinrc;
	
}


char *
GetTwinBuffer(TWIN_CONFIG *tp,char *wcpbuf)
{
	/* if we have a buffer, use it */
	if(wcpbuf)
		return wcpbuf;

	/* if we already allocated a buffer, return it */
	if(tp->lpszstring)
		return tp->lpszstring;

	/* allocate a permanent buffer */
	tp->lpszstring = (char *) WinMalloc(256);

	/* return it */
	return tp->lpszstring;	
}

char *
GetTwinString(int wcp, char *wcpbuf,int nsize )
{
	TWIN_CONFIG  *tp;
	char     *p = NULL;
	static char *twinrc;

	InitTwinConfig();

	if(twinrc == 0)
		twinrc = GetTwinFilename();
	
	if (wcpdata == NULL)
		wcpdata = (TWIN_CONFIG *)DRVCALL_CONFIG(PCFGH_GETDEFAULTS, 0, 0 , NULL);
	for(tp = wcpdata;tp->parameter > 0;tp++) {
		if(tp->parameter == wcp) {
		  switch(tp->opcode) {
			case 1:
				if(tp->lpszenviron && 
				   (p = (char *) getenv(tp->lpszenviron))) {
				       strcpy(GetTwinBuffer(tp,wcpbuf),p);

				       /* we got the environment variable */
				       return GetTwinBuffer(tp,wcpbuf);
				} 
				p = GetTwinBuffer(tp,wcpbuf);
				if(nsize == 0)
					nsize = 256;
				GetPrivateProfileString(
					sections[tp->section],
					tp->lpszkeyname,
					tp->lpszdefault,
					p,
					nsize, 
					twinrc);

				return GetTwinBuffer(tp,wcpbuf);

			 case 6:
				if(tp->lpszenviron && 
				   (p = (char *) getenv(tp->lpszenviron))) {
				       strcpy(GetTwinBuffer(tp,wcpbuf),p);

				       /* we got the environment variable */
				       return GetTwinBuffer(tp,wcpbuf);
				}
			
				p = GetTwinBuffer(tp,wcpbuf);
				if(nsize == 0)
					nsize = 256;
				/* get the value from the file */
				GetPrivateProfileString(
		  			sections[tp->section],
					tp->lpszkeyname,
					tp->lpszdefault,
					p,
	    				nsize, 
					twinrc);

				/* if we got something return it */
				if(strlen(p)) {
					return p;
				}
					
				/* use dirname(TwinFilename) */
				strcpy(p, GetTwinFilename());

				/* worst case same as TwinFilename directory */
				return p;
				
			default:
				break;
		  }
		  return wcpbuf;
		}
	}

	return 0;
}

unsigned long 
GetTwinInt(int wcp)
{
	char wcpbuffer[256];
	char *wcptr;

	wcptr = GetTwinString(wcp,wcpbuffer,256);
	
	return strtol(wcptr,0,0);
}

typedef	struct {
	int	value;
	int     init;
	char   *string;
} CONFIGOPTION;

static CONFIGOPTION configoption[WD_LASTVALUE+1] = {
	{ 0, 0,"debugger"},	/* load the the debugger 		*/
	{ 0, 0,"noditherpure"},	/* how to handle dithering      	*/
	{ 0, 0,"colormap"},	/* allocate private colormap    	*/
	{ 0, 0,"hiresfonts"},	/* use hi-res fonts, not low 		*/
	{ 0, 0,"vgamode"}, 	/* default to 640x480x16colors  	*/
	{ 0, 0,"nothreads"},	/* don't allow threads			*/
	{ 0, 0,"fatalerr"},	/* break into debugger 			*/
	{ 0, 0,"handlerr"},	/* break on handleobj errors 		*/
	{ 0, 0,"noapistubs"},	/* NO msgbox for STUB 			*/
	{ 0, 0,"nopaging"},  	/* disable demand page loading  	*/
	{ 0, 0,"nochangecase"},	/* don't change case of filenames 	*/
	{ 0, 0,"mapcurdir"},	/* xdos map current directory   	*/
	{ 0, 0,"colormask"},	/* use rgb colormask to convert mono    */

	{ 0, 0,"failerr"},	/* break on log FAIL errors 		*/
	{ 0, 0,"warnerr"},	/* break on log WARN errors 		*/
	{ 0, 0,"fixmeerr"},	/* break on log FIXME errors 		*/
	{ 0, 0,"stuberr"},	/* break on log STUB  errors 		*/
	{ 0, 0,"syncmode"},	/* set display to syncronouse mode	*/
	
	{ 0, 0,"emulevel"},	/* intp32 verbosity control             */
};	

int
GetConfigOption(int uid)
{
	char           *twinrc = GetTwinFilename();
        CONFIGOPTION   *cp     = 0;
	int    		rc     = 0;

	if(uid < 0 || uid >WD_LASTVALUE)
		return 0;

	cp = &configoption[uid]; 

#ifdef DEBUG
	if(uid == WD_DEBUGGER)
		cp->value = 1;
#endif
	
	if(cp) {
		if(cp->init)
			return cp->value;

		rc = GetPrivateProfileInt("boot",
			cp->string,	/* keyname */
			cp->value,	/* default value */
			twinrc);	

		cp->value = rc;
		cp->init  = 1;
	}

	return rc;
}

/********************************************************************
*	GetEnv
*
*	Our version of getenv that reads the [Environment] section of the twinrc file to obtain variables.
********************************************************************/
char *GetEnv(const char *env)
{
	char *twinrc = GetTwinFilename();
	static char buf[256];
	int count;

	/* Try and retrieve the environment variable */
	count = GetPrivateProfileString(sections[WCS_ENV], env, NULL, buf, 256,
		twinrc);

	if (count == 0)
		/* The default was used, meaning nothing was found */
		return(NULL);
	else
		/* Return the buffer containing the value found in the twinrc
		 * [Environment] section.
		 */
		return(buf);
}

BOOL InitTwinConfig()
{
    static BOOL binit = FALSE;

    if (!binit) {
     FILE*           fp;
     TWIN_CONFIG  *tp;
     char *twinrc;
     char buf[MAX_PATH];

     binit = TRUE;

     twinrc = GetTwinFilename();

     if (wcpdata == NULL)
          wcpdata = (TWIN_CONFIG *)DRVCALL_CONFIG(PCFGH_GETDEFAULTS, 0, 0 ,
NULL);

     /* Try to open .twinrc */
     fp = fopen(twinrc, "r");

     if (!fp) { /* Failed : create default with only boot section */
         fp = fopen(twinrc, "w");
         fprintf(fp,"%s\n","[boot]");

         for(tp = wcpdata;tp->parameter > 0;tp++) {
          if ( tp->parameter && tp->section == WCS_BOOT &&
               tp->lpszkeyname && *(tp->lpszkeyname) ) {
              strcpy(buf,tp->lpszkeyname);
              strcat(buf,"=");
              strcat(buf,tp->lpszdefault ? tp->lpszdefault : "");

              fprintf(fp,"%s\n",buf);
          }
         }
     }

     fclose(fp);
    } /* if (!binit) */

   return TRUE;
}
