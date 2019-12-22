/*************************************************************************
	@(#)compat.h	2.13
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

**************************************************************************/
 
#define WD_DEBUGGER     0   /* load the the debugger 		*/
#define WD_NODITHERPURE 1   /* how to handle dithering      	*/
#define WD_COLORMAP	2   /* allocate private colormap    	*/
#define WD_HIRESFONTS 	3   /* use hi-res fonts, not low 	*/
#define WD_VGAMODE	4   /* default to 640x480x16colors  	*/
#define WD_NOTHREADS	5   /* disallow threads 		*/
#define WD_FATALERR     6   /* break into debugger 		*/
#define WD_HANDLERR	7   /* break on handleobj errors 	*/
#define WD_APISTUBS     8   /* break, NO msgbox for STUB 	*/
#define WD_NOPAGING	9   /* disable demand page loading  	*/
#define WD_NOCHANGECASE 10  /* don't change case of filenames 	*/
#define WD_MAPCURDIR	11  /* xdos map current directory   	*/
#define WD_COLORMASK    12  /* use rgb masks for mono convert   */

#define WD_FAILERR    13  /* break in logstr() on FAIL error    */
#define WD_WARNERR    14  /* break in logstr() on WARN error    */
#define WD_FIXERR     15  /* break in logstr() on FIXME error   */
#define WD_STUBERR    16  /* break in logstr() on STUB error    */
#define WD_SYNCMODE   17  /* set display to syncronouse mode    */

#define WD_EMULEVEL   18  /* intp32 verbosity control           */

#define WD_LASTVALUE  18

int	GetConfigOption(int);
