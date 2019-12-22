/*    
	OleIF.c	1.8
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
#include "ole.h"

#include "kerndef.h"
#include "Endian.h"
#include "Log.h"
#include "DPMI.h"
#include "BinTypes.h"
#include "make_thunk.h"

#if defined(TWIN_BIN16)
void
IT_OLELOADFROMSTREAM(ENV *envp,LONGPROC f)
{
    DWORD retcode;
    LPBYTE lp1,lp2,lp3;
    LPSTR lpString1,lpString2;

    lp1 = (LPBYTE)GetAddress(GETWORD(SP+26),GETWORD(SP+24));
    lpString1 = (LPSTR)GetAddress(GETWORD(SP+22),GETWORD(SP+20));
    lp2 = (LPBYTE)GetAddress(GETWORD(SP+18),GETWORD(SP+16));
    lpString2 = (LPSTR)GetAddress(GETWORD(SP+10),GETWORD(SP+8));
    lp3 = (LPBYTE)GetAddress(GETWORD(SP+6),GETWORD(SP+4));

    retcode = OleLoadFromStream((LPOLESTREAM)lp1,lpString1,
		(LPOLECLIENT)lp2,GETDWORD(SP+12),
		lpString2,(LPOLEOBJECT *)lp3);

    envp->reg.sp += 5*LP_86 + LONG_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

void
IT_OLEENUMOBJ(ENV *envp,LONGPROC f)
{
    DWORD retcode;
    LPBYTE lp1;

    lp1 = (LPBYTE)GetAddress(GETWORD(SP+6),GETWORD(SP+4));
    retcode = OleEnumObjects((LHCLIENTDOC)GETDWORD(SP+8),
			(LPOLEOBJECT *)lp1);
    envp->reg.sp += LP_86 + LONG_86 + RET_86;
    envp->reg.ax = LOWORD(retcode);
    envp->reg.dx = HIWORD(retcode);
}

void
IT_REGCLIENTDOC(ENV *envp,LONGPROC f)
{
	LHCLIENTDOC lhHandle;
	LPSTR lpString1,lpString2;
	LPBYTE lpStr;
	DWORD retcode;

	lpString1 = (LPSTR)GetAddress(GETWORD(SP+18),GETWORD(SP+16));
	lpString2 = (LPSTR)GetAddress(GETWORD(SP+14),GETWORD(SP+12));
	lpStr = (LPBYTE)GetAddress(GETWORD(SP+6),GETWORD(SP+4));
	retcode = OleRegisterClientDoc(lpString1,lpString2,0L,&lhHandle);
	PUTDWORD(lpStr,lhHandle);
	envp->reg.sp += 3*LP_86 + DWORD_86 + RET_86;
	envp->reg.ax = LOWORD(retcode);
	envp->reg.dx = HIWORD(retcode);
}

void
IT_UNBLOCKSERVER (ENV *envp,LONGPROC f)	/* OleUnblockServer */
{
	DWORD retcode;
	LPBYTE lpData;
	BOOL flag;

	lpData = (LPBYTE)GetAddress(GETWORD(SP+6),GETWORD(SP+4));
	flag = (BOOL)GETWORD(lpData);
	retcode = (f)((LHSERVER)GETDWORD(SP+8),&flag);
	PUTWORD(lpData,(WORD)flag);
	envp->reg.sp += LONG_86 + LP_86 + RET_86;
	envp->reg.ax = LOWORD(retcode);
	envp->reg.dx = HIWORD(retcode);
}

#endif
