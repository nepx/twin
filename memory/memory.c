
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "windows.h"

#include "WinDefs.h"
#include "kerndef.h"
#include "Resources.h"
#include "Module.h"
#include "Kernel.h"
#include "Driver.h"
#include "Log.h"
#include "WinMalloc.h"
#include "Strings32.h"
#include "virtual.h"

extern VMINFO TWINVM;

static char *savedstrings[512];

LPSTR
WinSaveString(LPCSTR lpstr)
{
	int i;
	char *lp;

	if(lpstr == (LPCSTR) 0xffffffff) {
		for(i=0;i<512;i++) {
		   if(savedstrings[i]) {
		     WinFree(savedstrings[i]);
		   }
		}
		return 0;
	}
	for(i=0;i<512;i++) {
		if (savedstrings[i] == 0) {
		   break;
		}

		if (strcmp(savedstrings[i],lpstr) == 0) {
		   return savedstrings[i];
		}
	}
	if(i == 512) {
		lp = WinStrdup(lpstr);
		return lp;
	}
	savedstrings[i] = WinMalloc(strlen(lpstr) + 1);
	strcpy(savedstrings[i],lpstr);
	return savedstrings[i];
}

LPSTR
WinStrdup(LPCSTR szStringIn)
{
	char * szStringOut;
	short int Length;
	static int dwtotal;
	static int dwcount;

	if(szStringIn == (LPCSTR) 0xffffffff) {
		logstr(LF_MEM,"total allocated by WinStrdup: %dBytes %d calls\n",
			dwtotal,dwcount);
		return 0;
	}

	/* get length of string */
	Length = strlen( szStringIn );
	Length++;
	dwtotal += Length;
	dwcount++;

	/* alloc space to hold dup */
	szStringOut = (char *) WINMALLOC( Length,1,0 );

	/* copy string */
	strcpy( szStringOut, szStringIn );

	/* return pointer to dup string */
	return ( szStringOut );
}

LPVOID WinCalloc( unsigned int units, unsigned int size )
{
	LPVOID *ptr;
	ptr = WinMalloc(units * size);
	if (*ptr)
		memset(ptr, 0, size);
	return ptr;
}

typedef struct {
	LPVOID 	(*malloc)(int);
	void    (*free)(LPVOID);
	LPVOID  (*realloc)(LPVOID,unsigned int);
} MEMALLOC;

static	MEMALLOC basic = {
	(LPVOID) malloc,
	free,
	realloc
};

MEMALLOC *TWIN_Memory = &basic;

LPVOID
WinMalloc(unsigned int size)
{
    DWORD dwSize = (size+3) & ~3;
    LPVOID lpCore;

    dwSize += 32;
    lpCore = TWIN_Memory->malloc(dwSize);
    if (TWINVM.vm_map)
	logstr(LF_API,"WinMalloc(%ld) %p-%p\n",
		size,lpCore,(int)lpCore + dwSize);


    return lpCore;
}

void 
WinFree(LPVOID ptr)
{
    LPVOID lpCore = ptr;
    TWIN_Memory->free(lpCore);
    if (TWINVM.vm_map)
	logstr(LF_API,"WinFree(%p)\n",ptr);
}

LPVOID
WinRealloc(LPVOID ptr, unsigned int size)
{
   LPVOID lpCore;
   DWORD dwSize = (size+3) & ~3;

   if (!ptr) {
      lpCore = TWIN_Memory->malloc(size);
      return lpCore;
   }

    lpCore = TWIN_Memory->realloc(ptr,dwSize);

    if (TWINVM.vm_map)
	logstr(LF_API,"WinRealloc(%p,%ld) %d %p\n",
		ptr,size,dwSize,lpCore);
    return lpCore;
}

/*****************************************************************************/

#ifdef WINMALLOC_CHECK

typedef struct _malloc_tag {
	struct _malloc_tag *next;
	char	*data;
	int     size;
	int	call;
	char 	*lparam;
	int	 wparam;
	int	 flag;
	int 	 handle;
} MALLOCINFO;

MALLOCINFO	*lpMallocInfo;

static int	totalalloc;
static int	totalfree;
static int	totalacalls;
static int	totalfcalls;

LPVOID
WinMallocCheck(unsigned int size, char *lparam,int wparam,int flag, int handle)
{
	LPVOID   lpMemory;
	MALLOCINFO *p;

        lpMemory =  TWIN_Memory->malloc(size);

        logstr(LF_MEM,"%s:%d: WinMallocCheck data=%p size=%d %x %x\n",
		lparam,wparam,
                lpMemory,size,
		flag,handle);

	p = (MALLOCINFO *) TWIN_Memory->malloc(sizeof(MALLOCINFO));
	memset(p,0,sizeof(MALLOCINFO));

	p->next = lpMallocInfo;
	p->data = lpMemory;
	p->size = size;

	p->lparam = lparam;
	p->wparam = wparam;
	p->flag   = flag;
	p->handle = handle; 
	p->call   = totalacalls;
	
	lpMallocInfo = p;
	totalacalls++;

	totalalloc += size;

        return lpMemory;
}

void
WinFreeCheck(LPVOID ptr,char *lparam, int wparam)
{
	MALLOCINFO *p,*l;

	for(p=lpMallocInfo; p; p = p->next) {
		if(p->data == ptr) {
			if(lpMallocInfo == p) {
				lpMallocInfo = p->next;
			} else {
				for(l=lpMallocInfo; l; l = l->next) {
					if(l->next == p) {
						l->next = p->next;
					}
				}
			}
        		logstr(LF_MEM,"%s:%d: WinFreeCheck: data=%x from=%s:%d %x %x\n",
			    lparam,wparam,
                	    ptr,
			    p->lparam,p->wparam,
			    p->flag,p->handle);

			totalfree += p->size;
			totalfcalls++;

			TWIN_Memory->free(p);
			break;
		}
	}
	if(p == 0) {
		logstr(LF_MEM,"*** ERROR *** WinFreeCheck: freeing %x %s:%d\n",
			ptr, lparam,wparam);
	}
	TWIN_Memory->free(ptr);
}

LPVOID
WinReallocCheck(LPVOID ptr, unsigned int size, char *lparam, int wparam,int flag,int handle)
{
	LPVOID	lp;
	MALLOCINFO *p;

	for(p=lpMallocInfo; p; p = p->next) {
		if(p->data == ptr) {
			totalalloc -= p->size;
			break;
		}
	}

		lp = TWIN_Memory->realloc(ptr,size);

        logstr(LF_MEM,"%s:%d: WinReallocCheck: old data=%x new data=%x size=%x %x %x\n",
		lparam,wparam,
                ptr,lp,size,
		flag,handle);

	if(ptr == 0)
		return 0;

	if(lp) {
		p->size  = size;
	} else {
		p->size  = 0;
	}

	p->data  = lp;
	p->lparam = lparam;
	p->wparam = wparam;

	totalalloc += size;	
	return lp;
}

long
WinMallocInfo(int opcode, char *ptr,int flag, int handle)
{
	MALLOCINFO *p;
	int	total,n;

	if(lpMallocInfo == 0)
		return 0;

	switch(opcode) {
	case WMI_DUMP:
		logstr(LF_MEM,"WinMallocInfo Malloc Chain\n");

		total = 0;
		n     = 0;
		for(p=lpMallocInfo; p; p = p->next) {
			if (p->flag == 7)
			   continue;
			logstr(LF_MEM,"%d: %s:%d: %p #%d size=%d type=%x handle=%x\n",
				n++,
				p->lparam?p->lparam:"(void)",p->wparam,
				p->data,
				p->call,p->size,p->flag,p->handle);
			total += p->size;
			
		}

		logstr(LF_MEM,"WinMallocInfo Statistics\n");
		logstr(LF_MEM,"   calls alloc = %d size=%d\n",
			totalacalls,totalalloc);
		logstr(LF_MEM,"   calls free = %d size=%d\n",
			totalfcalls,totalfree);
		logstr(LF_MEM,"   remaining = %d delta=%d\n",
			total, totalalloc-totalfree);
		break;

	case WMI_TAG:
		for(p=lpMallocInfo; p; p = p->next) {
			if(p->data == ptr) {
				p->flag   = flag;	
				p->handle = handle;
				break;
			}
		}
		return 0;

	case WMI_CHECK:
		for(p=lpMallocInfo; p; p = p->next) {
			if(p->handle == handle) {
				logstr(LF_MEM,"WinMallocInfo: %s:%d %p %d %d\n",
					p->lparam, p->wparam,p->data,
					p->flag, p->handle);
				return 1;
			}
		}
		return 0;
		
	default:
		break;
	}
	return 0;
}


void TWIN_FreeAllMemory()	/* free all remaining memory as we exit windows .. */
{
	MALLOCINFO *p, *q;
	for(p=lpMallocInfo; p; p = q) {
		q	=	p->next;
		TWIN_Memory->free(p);
	}
}

#endif
