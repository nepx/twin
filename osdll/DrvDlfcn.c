/********************************************************************

        @(#)DrvDLFcn 1.0
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

#include "windows.h"

#ifdef HAVE_LIBDLD

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#else
#include "twindlfcn.h"
#endif

#include <dl.h>

void *
dlopen(const char * dlname, int dlflag)
{
  int shlflag = 0;

  if (dlflag & RTLD_LAZY) shlflag |= BIND_DEFERRED;
  if (dlflag & RTLD_NOW)  shlflag |= BIND_IMMEDIATE;
  shlflag |= DYNAMIC_PATH;

  return ((void *) (shl_t) shl_load((const char*) dlname, (int) shlflag, 0L));
}

void *
dlsym(void * dlhandle, const char * sname)
{
  void *adress;
  void* shlhandle;

  shlhandle = dlhandle;
  if (dlhandle == RTLD_NEXT)
    shlhandle = NULL;

  if (shl_findsym((shl_t*) &shlhandle, (const char*) sname, TYPE_UNDEFINED, &adress))
    return NULL;
  else
    return (void*) adress;
}

int
dlclose(void * dlhandle)
{
  void* shlhandle;
  shlhandle = dlhandle;

  return (shl_unload((shl_t) shlhandle));
}

char *
DrvGetShlibExt(void)
{
  /*
   *  Return a pointer to a string with the extension used on this
   *  platform for shared libraries.
   */
  return(".sl");
}

#else

char *
DrvGetShlibExt(void)
{
  /*
   *  Return a pointer to a string with the extension used on this
   *  platform for shared libraries.
   */
  return(".so");
}

#endif /* HAVE_LIBDL */
