/*    
	utils.c	2.20
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
#include "utils.h"
#include "Log.h"
#include "compat.h"
#include "ctype.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "debugdefs.h"

/*
 *	semantics:
 *	debug = [class]{+|-}channel[=stream]
 *	1	class is one of builtin 8
 *		       console trace warn stub fixme fail call ret
 *
 *      2	channels are builtin
 *			API BIN DOS GDI INT KERNEL LIB MEM MSG SYS USER X11
 *		or any user defined
 *			ie. WARN(profile,".twinrc does not exist\n");
 *		or the special class all
 *	
 *	3	stream is one of
 *		0	no output, same as -channel
 *		1	stdout
 *		2	stderr
 *		3	logfile as defined by WIN_OPENLOG
 *
 *	[tools]
 *	debug = warn+api,trace-lib,-X11,+sys=2
 *
 *	debug class is left to right
 *
 */

typedef struct {
	int     init;
	int	fd;
	char   *file;
} TWINDEBUG;

static TWINDEBUG	TwinDebug;

static int
opnlog()
{
        char	*fn;

	if (TwinDebug.fd)
		return(TwinDebug.fd); 

	if (TwinDebug.file)
		fn = TwinDebug.file;
	else
		fn = (char *)getenv("WIN_OPENLOG");

	if ( fn ) {
		char *p;

		if( *fn == '-') 
			return (TwinDebug.fd = 1);

		/* get rid of old style mask... */
		if ((p = strchr(fn,' ')))
			*p = 0;	
		return (TwinDebug.fd = creat(fn, 0666));
	}

	return 0;
}

void
DumpMemory(char *s,char *p,int n)
{
	int i,j;
	char x;
	char   *chrptr;
	char	buffer[132];
	char	temp[80];

	if(n==0)
		return;

	logstr(LF_CONSOLE,"%s:\n",s?s:"DumpMemory");
	if(p == 0) {
		logstr(LF_CONSOLE,"NULL pointer\n");
		return;
	}
	for(i=0;i<n;i++) {
		sprintf(buffer,"%x: ",(unsigned int) p);
		chrptr = p;

		for(j=0;j<16;j++) {
			x=*p++;
			x &= 0xff;
			sprintf(temp,"%2.2x ",x&0xff);
			strcat(buffer,temp);
		}		
		strcat(buffer,"   ");
		p = chrptr;
		chrptr = buffer + strlen(buffer);
		for(j=0;j<16;j++) {
			if(isprint(*p))
				*chrptr++ = *p;
			else	*chrptr++ = '.';
			p++;
		}	
		*chrptr = 0;

		i += 16;
		logstr(LF_CONSOLE,"%s\n",buffer);
	}
}

extern void debuggerbreak();

static int
logbreak(int reason)
{
	int dobreak = 0;

	switch(reason) {
	case dbcl_FIXME:
		dobreak = WD_FIXERR;
		break;
	case dbcl_WARN:
		dobreak = WD_WARNERR;
		break;
	case dbcl_STUB:
		dobreak = WD_STUBERR;
		break;
	case dbcl_FAIL:
	default:
		dobreak = WD_FAILERR;
		break;
		
	}
	if(dobreak && GetConfigOption(dobreak))
		debuggerbreak();
	return 0;
}


/*VARARGS */
void
logstr(unsigned long flg, ...)
{
	va_list args;
	static int apidepth;
	int  fd = 0;
	char *fmt;
	char *header = 0;
	char title[32];
	short class,channel;
	char format[4096];

	va_start(args, flg);

	/* point to fmt and args... */
	fmt = va_arg(args, char *);
	channel = flg>>8;
	class   = flg & 0xff;

	fd = debug_msg_enabled[channel][class];

	switch(fd) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3: 
		fd = TwinDebug.fd;
	}

	
	/* we may not be logging on this class/channel */
	if(fd) {

		switch(class) {
			case dbcl_CONSOLE:
				break;

			case dbcl_FIXME:
			case dbcl_WARN:
			case dbcl_STUB:
				logbreak(class);
				/* fall through */	

			case dbcl_TRACE:
				/* print out a header */
				sprintf(title,"%s %s         ",
					(char *)debug_ch_name[channel],
					(char *)debug_cl_name[class]);
				sprintf(format,"%-14.14s : ",title);
				header = title;
				break;

			case dbcl_FAIL:
				/* check if we should break */
				logbreak(class);	
				/* fall through */

			case dbcl_CALL:
			case dbcl_RET:
				if(class == dbcl_RET || class == dbcl_FAIL)
					apidepth--;
				/* print out a header */
				sprintf(title,"%-3.3s %-5.5s %2d",
					(char *)debug_ch_name[channel],
					(char *)debug_cl_name[class],
					apidepth);
				if(class == dbcl_CALL)
					apidepth++;
				sprintf(format,"%-14.14s : ",title);
				header = format;
				break;
		}

		if(header) {
			write(fd, format, strlen(format));
		}
		
		/* print out remainder of message locally */
		vsprintf(format,fmt,args);
		write(fd, format, strlen(format));

		/* if not to log file, put it there anyway */
		if((fd != TwinDebug.fd) && TwinDebug.fd)
			write(TwinDebug.fd,format,strlen(format));
	}

	va_end(args);
}

/* fixme+all=2 fail+all=2 warn+all=2 stub+all=3  */
/* debug+all=2 trace+all=3 call+all=3 ret+all=3  */
/* fixme+api=3 fail+api=3 warn+api=3 debug+api=3 */

void
DEBUG_TraceFlag(char *lpstr)
{
	int  cl,ch,fd;
	char *s,*p;
	char *class = "";

	fd = 3;
	s = strchr(lpstr,'-');
	if(s==0)
	    s = strchr(lpstr,'+');
	p = strchr(lpstr,'=');
	if(s && *s == '-') {
		fd = 0;
		*s++ = 0;
		class = lpstr;
		lpstr = s;
	}
	if(s && *s == '+') {
		fd = 3;
		*s++ = 0;
		class = lpstr;
		lpstr = s;
	}

	if(p) {
		*p++ = 0;
		/* TODO if a filename, open and pass its fd? */
		fd = atoi(p);
	}

	if(class && strlen(class) == 0) class = 0;
	if(lpstr && strcasecmp(lpstr,"all") == 0) lpstr = 0;

	for(ch=0;ch<DEBUG_CHANNEL_COUNT;ch++) {
	   if(lpstr == 0 || strcasecmp(lpstr,debug_ch_name[ch]) ==0) {
		for(cl=0;cl<8;cl++) {
			if(class == 0 || strcasecmp(class,debug_cl_name[cl]) == 0) {
				debug_msg_enabled[ch][cl] = fd;

			}
		}
		/* if we had a channel, we are done */
		if(lpstr)
			break;
            }
	}
}

typedef struct {
        char *base;
        char *current;
        char *token;
} TOKEN;

char * TWIN_NextString(TOKEN *);
char * TWIN_FirstString(TOKEN *,char *,char *);

extern char *GetTwinFilename();

char *
TWIN_NextString(TOKEN *tp)
{
	char *p = tp->current;
	char *n;

	if(p == 0)
		return 0;

	/* skip leading spaces/tabs */
	while(*p == ' ' || *p == '\t') p++;

	/* find end of this string */
	n = strchr(p,' ');
	if(n == 0)
	 	n = strchr(p,'\t');
	if(n) {
		*n++ = 0;	
	}
	tp->current = n;

	if(strlen(p))
		return p;
	return 0;
}

char *
TWIN_FirstString(TOKEN *p,char *string,char *ch)
{
	p->base = string;	
	p->current = string;
	p->token = ch;

	return TWIN_NextString(p);
}

void
LoadPreLoads()
{
    TOKEN tk;
    char names[256];	
    char *p;
    int  len;

    len = GetPrivateProfileString((LPSTR)"boot",
                "preload",
                "",
                names,
                256,
		GetTwinFilename());

    if(len == 0)
	return;

    if(strlen(names)) {
	for(p = TWIN_FirstString(&tk,names," \t");p;p = TWIN_NextString(&tk)){
			LoadLibrary(p);
	}
    } 
}

void 
InitDebugSystem()
{
    	TOKEN tk;
	char trace[256];
    	char *p;

	if(TwinDebug.init == 0) {

		TwinDebug.init++;
		TwinDebug.fd = opnlog();

    		GetPrivateProfileString((LPSTR)"boot",
			"trace",
			"",
			trace,
			256,
			GetTwinFilename());

    		if(strlen(trace)) {
			p = TWIN_FirstString(&tk,trace," \t,");
			for(;p;p = TWIN_NextString(&tk)) {
				DEBUG_TraceFlag(p);
			}
    		} 

	}	
}
