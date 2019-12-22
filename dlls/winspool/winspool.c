/*    
	Ver.c	2.8
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
#include "windows.h"


void
NewTrap(char *name)
{
	int	rc;
	char	buf[128];

	sprintf(buf, "Unimplemented API: %s\n\nPress OK to continue execution,\npress Cancel to quit application", name);
	rc = MessageBox((HWND)NULL, buf, "Trap", MB_ICONEXCLAMATION | MB_OKCANCEL);
	if ( rc == IDCANCEL )
		FatalExit(0);
}


void
Version32End()
{
	return;
}

