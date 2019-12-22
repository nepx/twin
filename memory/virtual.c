
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "Log.h"
#include "virtual.h"
#include "Strings32.h"

#if 0
typedef struct _vm_map{
	struct _vm_map *next;		/* next element */

	void 	       *base;		/* where it starts */
	void 	       *limit;		/* next unavailable */
	void	       *range;		/* range this block is in */

	/* these are the WIN32 flags */
	DWORD   	type;		/* type from VirtualAlloc */	
	DWORD 		protect;	/* protect from VirtualAlloc */

	/* these are our internal flags */
	int		flag;		/* our internal flags */
} VM_MAP;

#define PAGEGRAN 	(0x1000*64)

typedef struct {
	VM_MAP *vm_list;
	int     vm_cnt;
	VM_MAP *vm_free;
	VM_MAP *vm_map;
	int     vm_dirty;
} VMINFO;

#endif

VMINFO TWINVM;

static VM_MAP * TWIN_getvm();
static int mmap_fd();

void VirtualInit(int);

void VIRTUAL_Dump(void);
void VIRTUAL_DumpInfo(int,LPSTR);

extern void debuggerbreak(void);

/*
 * External Interface to internal VM structures and functions
 * 
 * Initialize VM subsystem
 * return VM linked list...
 */

/* These are always the same on an i386, and it will be faster this way */
#define page_mask  0xfff
#define granularity_mask 0xffff

int
TWIN_vmtype(LPVOID base,DWORD type,char *typestr)
{
	int  separator = 0;
	int  vflag = 0;

	strcpy(typestr,"");		
	if(type == 0xffffffff) {
		strcat(typestr,"SYSTEM");
		return 0;
	}
	if(type&MEM_COMMIT) {
		strcat(typestr,"COMMIT");
		separator++;
		if(base == 0) {
			vflag = MEM_FREE;
		} else {
			vflag = MEM_FREE|MEM_RESERVE;
		}
	}
	if(type&MEM_RESERVE) {
		if(separator)
			strcat(typestr,"|");
		strcat(typestr,"RESERVE");
		separator++;
		vflag = MEM_FREE;
	}
	if(type&MEM_DECOMMIT) {
		if(separator)
			strcat(typestr,"|");
		strcat(typestr,"DECOMMIT");
		separator++;
		vflag = MEM_COMMIT;
	}
	if(type&MEM_RELEASE) {
		if(separator)
			strcat(typestr,"|");
		strcat(typestr,"RELEASE");
		separator++;
		vflag = MEM_RESERVE;
	}
	if(type&MEM_FREE) {
		if(separator)
			strcat(typestr,"|");
		strcat(typestr,"FREE");
	}
		
	return vflag;
}
void
VIRTUAL_DumpInfo(int bConsole,LPSTR lpHeader)
{
	int lf_dump;
	VM_MAP *vm;
	char typestr[80];

	if(bConsole)
		lf_dump = LF_CONSOLE;
	else	lf_dump = LF_LOG;

	logstr(lf_dump,"%s\n",lpHeader);

	if(TWINVM.vm_map == 0)
		VirtualInit(1);
	logstr(lf_dump,"Virtual Memory Info\n");
	logstr(lf_dump,"Free Cache %8.8x (%d)\n",TWINVM.vm_list,TWINVM.vm_cnt);
	logstr(lf_dump,"Free List\n");
	for(vm = TWINVM.vm_free;vm;vm = vm->next) {
		logstr(lf_dump,"    %8.8x\n",vm);
	}
	logstr(lf_dump,"Allocated List  %p\n",TWINVM.vm_map);
	logstr(lf_dump,        "addr      next      base      limit      range     type      protect  flags\n");

	for(vm=TWINVM.vm_map;vm;vm = vm->next) {
		TWIN_vmtype(0,vm->type,typestr);
		
		logstr(lf_dump,"%8.8x  %8.8x  %-8.8x  %8.8x   %8.8x  %-.8x  %-.4x     %.8x %s\n",
			vm,vm->next,vm->base,vm->limit,vm->range,vm->type,vm->protect,vm->flag,
			typestr);
	}
}

void
VIRTUAL_Dump()
{
	VIRTUAL_DumpInfo(1,"Virtual Memory Information\n");
}

void
VirtualInit(int bInit)
{
	void   *start;
	size_t size;
	int 	prot,flags;
	int     fd;
	int     cflag;
	VM_MAP *vm,*vp;

	void   *base;
	int	i;

	if (bInit == 0) {
		TWINVM.vm_map = 0;
		return;			
	}
	if (TWINVM.vm_map)
	   return;

	prot  = PROT_READ|PROT_WRITE|PROT_EXEC;
	flags = MAP_PRIVATE;

	size = PAGEGRAN;

	fd = mmap_fd();

	/* build the first block */	
	vm = TWIN_getvm();
	vm->base    = 0;
	vm->limit   = (void *) 0xffffffff;
	vm->range   = (void *) 0xffffffff;
	vm->protect = 0;
	vm->type    = 0;	
	vm->flag    = 0xffffffff;
	vm->next    = 0;

	TWINVM.vm_map = vm;

	start = 0;
	cflag = 0xffffffff;

	for(;;) {
		/* now set the real values of vm */
		base = mmap(start,size,prot,flags,fd,(off_t) start);

		/* usable */
		i = (start == base)?MEM_FREE:0xffffffff;
				
		if(base > 0)
			munmap(base,size);	

		if(base == (void *) 0xffffffff) {
			vp = TWIN_getvm();
			vp->base  = start;
			vp->limit = vm->limit;
			vp->range = vm->range;
			vp->protect = 0;
			vp->type    = i;	
			vp->flag    = i;	
			vp->next    = 0;

			vm->limit = start;
			vm->range = start;
			vm->next  = vp;
			vm = vp;
			break;
		}

		if( i != cflag) {
			vp = TWIN_getvm();
			vp->base  = start;
			vp->limit = vm->limit;
			vp->range = vm->range;
			vp->protect = 0;
			vp->type    = i;	
			vp->flag    = i;	
			vp->next    = 0;

			vm->limit = start;
			vm->range = start;
			vm->next  = vp;
			vm = vp;
			cflag = i;
		}
		
		start += size; 
	}	
	//VIRTUAL_DumpInfo(0,"At end of initialization");
}

static VM_MAP *
TWIN_getvm()
{
	VM_MAP *vm;

	/* do we have a free element */
	/* if so, pull from list     */
	if(TWINVM.vm_free) {
		vm = TWINVM.vm_free;
		vm->next = 0;
		vm->flag = MEM_FREE;
		TWINVM.vm_free = TWINVM.vm_free->next;
		return vm;
	}
	
	/* do we have any in our cache? */
	/* if not, allocate new cache   */
	if(TWINVM.vm_cnt == 0) {
		TWINVM.vm_list = (VM_MAP *) WinMalloc(sizeof(VM_MAP)*256);
		TWINVM.vm_cnt  = 256;
	}

	/* pull the first from the cache */
	vm = &TWINVM.vm_list[256 - TWINVM.vm_cnt];
	TWINVM.vm_cnt--;
	vm->next = 0;
	vm->flag = MEM_FREE;
	return vm;
}

static int
mmap_fd()
{
	static int fd;

	if(fd)
	  return fd;

	fd = open("/dev/zero",O_RDONLY);
	if(fd < 0) {
	     fd = 0;
	}
	return fd;
}


LPVOID 
VirtualAlloc( LPVOID addr, DWORD size, DWORD type, DWORD protect )
{
	VM_MAP *vm,*vp = 0;
	void *lbase    = 0;
	void *base = addr;
	int   vflag;
	DWORD isize = 0;
	char  typestr[80];
	
	vflag = TWIN_vmtype(base,type,typestr);

	logstr(LF_APICALL,"VirtualAlloc(LPVOID=%x,DWORD=%x,DWORD=%x (%s),DWORD=%x)\n",
		base,size,type,typestr,protect);


    	if (addr) {
        	if (type & MEM_RESERVE) /* Round down to 64k boundary */
            		base = (void *)(((UINT)addr + granularity_mask) & ~granularity_mask);
        	else
            		base = (void *)((UINT)(addr) & ~page_mask);

        	isize = (((UINT)addr + size + page_mask) & ~page_mask) - (UINT)base;
		size = isize;
    	} else {
        	base = 0;
        	size = (size + page_mask) & ~page_mask;
    	}

	/* have we figured out what we have available... */
	/* if not, initialize the vm subsystem		 */
	if(TWINVM.vm_map == 0)
		VirtualInit(1);

	/* look in our maps for the desired memory...		*/
	/* it is sorted by address, each pointing to next...	*/

	for(vm=TWINVM.vm_map;vm;vm = vm->next) {

		if((vm->flag == 0xffffffff))
			continue;

		/* precondition */
		if((vm->flag & vflag) == 0)
			continue;

		/* candidate block, too small    */
		if(vm->base + size > vm->range) {
			continue;
		}	

		/* is the desired block in the range of this map? */
		if((base) && (vm->base <= base) && (base < vm->range)) { 

			vp = vm;
			if(vm->base < base) {
				vp = TWIN_getvm();

				/* link this one in */
				*vp = *vm;
				vm->next = vp;
				vm->limit = base;
				vp->base  = base;
				vm->range = base;
			}
			
			break;
		}

		if(base)
			continue;

		/* is this range too small */
		if((int)(vm->range - vm->base) < size)
			continue;

		/* have we found one yet */
		/* this is to find the smallest/largest? block that fits */
		if(vp == 0)
			vp = vm;

		/* this is to find the last that fits */
		if(type & MEM_TOP_DOWN)
			vp = vm;
	}

	/* we have a SUBMAP, now setup its attributes */
	/* base here should be 0, we have to break it up */
	if(vp) {
		/* this one is too big, break it up */
		if(vp->base + size < vp->limit) {
			vm = TWIN_getvm();
			/* link this one in */
			*vm = *vp;

			vp->next = vm;

			vp->limit    = vp->base + size;
			vm->base     = vp->base + size;
		}

		vp->protect = protect;
		vp->type    = type;

		if(type & MEM_COMMIT) {
			int  prot;
			int  flags;
			int  fd;

			if((vp->flag & MEM_RESERVE) == 0)
				vp->range = vp->base + size;

			vp->flag = MEM_COMMIT;

			prot  = PROT_READ|PROT_WRITE|PROT_EXEC;
			flags = MAP_PRIVATE|MAP_FIXED;
			fd    = mmap_fd();
			lbase = mmap(vp->base,size,prot,flags,fd,(off_t) base);

			if(lbase == 0) {
				VIRTUAL_DumpInfo(1,"mmap failed");
				debuggerbreak();
			}

			if((DWORD) lbase == 0xffffffff) {
				logstr(LF_CONSOLE,"%p = mmap(%p,%x,%x,%x,%x,%p)\n",
					lbase,vp->base,size,prot,flags,fd,base);
				FatalAppExit(0,"Virtual Memory allocation error.");
			}
		} else {
			vp->flag = MEM_RESERVE;
			vp->range = vp->base + size;
					
			lbase = vp->base;
		}
	} else {
		/* seterrormode */
		/* cannot allocate desired memory */
		lbase = 0;
	}

	//VIRTUAL_DumpInfo(0,"At end of allocation");
	logstr(LF_APIRET,"VirtualAlloc returns LPVOID %p\n",lbase);
	
	return lbase;
}

/* (WIN32) Virtual Memory ************************************************** */

BOOL	WINAPI
VirtualProtect(LPVOID lpAddress, DWORD dwSize,
	DWORD dwAccessFlags, PDWORD pdwOldAccessFlags)
{
	APISTR((LF_API, "VirtualProtect: (API) addr %p size %ld flags %lx\n",
		lpAddress, dwSize, dwAccessFlags));
	return (TRUE);
}

BOOL	WINAPI
VirtualProtectEx(HANDLE hProcess, LPVOID lpAddress, DWORD dwSize,
	DWORD dwAccessFlags, PDWORD pdwOldAccessFlags)
{
	APISTR((LF_API, "VirtualProtectEx: (API) addr %p size %ld flags %lx\n",
		lpAddress, dwSize, dwAccessFlags));
	return (TRUE);
}

#if 0
	struct _vm_map *next;		/* next element */
	void 	       *base;		/* where it starts */
	void 	       *limit;		/* next unavailable */
	DWORD   	type;		/* type from VirtualAlloc */	
	DWORD 		protect;	/* protect from VirtualAlloc */
	int		flag;		/* our internal flags */
#endif


BOOL 
VirtualFree(LPVOID addr,DWORD dwSize,DWORD mflag)
{
	VM_MAP *vm,*vp;
	int     limit;
	int	type;

	logstr(LF_APICALL,"VirtualFree(%p,%x,%x)\n",
		addr,dwSize,mflag);
	limit = (int)addr + dwSize;

	//VIRTUAL_DumpInfo(0,"At start of free");
	for(vm=TWINVM.vm_map;vm && dwSize;vm = vm->next) {

		if(vm->flag == 0xffffffff)
			continue;

		/* is the desired block in the range of this map? */
		if((vm->base <= addr) && (addr <= vm->limit)) { 

			// if necessary add block in front of addr
			if(vm->base < addr) {
				VM_MAP *vp;
				vp = TWIN_getvm();

				*vp = *vm;

				vp->next = vm->next;
				vp->base = addr;

				vm->next = vp;
				vm->limit = addr;
				vm = vp;
			}
			// if necessary add block after this one
			if(vm->base + dwSize < vm->limit) {
				VM_MAP *vp;
				vp = TWIN_getvm();

				*vp = *vm;	
				vp->next = vm->next;
				vp->base = vm->base + dwSize;
		
				vm->next = vp;
				vm->limit = vm->base + dwSize;
			}
			
			type = MEM_FREE;
			if(mflag & MEM_DECOMMIT) {
				if(vm->type == MEM_COMMIT) {
					munmap(vm->base,vm->limit - vm->base);
				}
				vp = vm->next;
				if(vm->range == vp->range) {
					if(vp->type & (MEM_FREE|MEM_RESERVE))
						type =  vp->type;
				}
				vm->type = type;
				vm->flag = type;

			} else if(mflag & MEM_RELEASE) {
				vm->type = type;
				vm->flag = type;

			}
			dwSize -= vm->limit - vm->base;
			addr    = vm->limit;
			continue;
		}
	}

	for(vm=TWINVM.vm_map;vm && dwSize;vm = vm->next) {

		vp = vm->next;
		if(vm->flag == 0xffffffff)
			continue;
		if(vp->flag == 0xffffffff)
			continue;
		if(vm->range == vp->range) {
			if(vm->flag == vp->flag) {

				vm->limit = vp->limit;
				vm->next  = vp->next;

				vp->next = TWINVM.vm_free;
				TWINVM.vm_free = vp->next;
			}
		}
	}
	//VIRTUAL_DumpInfo(0,"After VirtualFree");
	logstr(LF_APIRET,"VirtualFree returns BOOL TRUE\n");
	return TRUE;
}

/***********************************************************************
 *           GlobalMemoryStatus   (KERNEL32.327)
 * RETURNS
 *	None
 */

typedef struct _MEMORYSTATUS { // mst 
    DWORD dwLength;        // sizeof(MEMORYSTATUS) 
    DWORD dwMemoryLoad;    // percent of memory in use 
    DWORD dwTotalPhys;     // bytes of physical memory 
    DWORD dwAvailPhys;     // free physical memory bytes 
    DWORD dwTotalPageFile; // bytes of paging file 
    DWORD dwAvailPageFile; // free bytes of paging file 
    DWORD dwTotalVirtual;  // user bytes of address space 
    DWORD dwAvailVirtual;  // free user bytes 
} MEMORYSTATUS, *LPMEMORYSTATUS; 
 

void WINAPI GlobalMemoryStatus(
            LPMEMORYSTATUS lpmem
) {
#ifdef linux
    FILE *f = fopen( "/proc/meminfo", "r" );
    if (f)
    {
        char buffer[256];
        int total, used, free;

        lpmem->dwTotalPhys = lpmem->dwAvailPhys = 0;
        lpmem->dwTotalPageFile = lpmem->dwAvailPageFile = 0;
        while (fgets( buffer, sizeof(buffer), f ))
        {
	    /* old style /proc/meminfo ... */
            if (sscanf( buffer, "Mem: %d %d %d", &total, &used, &free ))
            {
                lpmem->dwTotalPhys += total;
                lpmem->dwAvailPhys += free;
            }
            if (sscanf( buffer, "Swap: %d %d %d", &total, &used, &free ))
            {
                lpmem->dwTotalPageFile += total;
                lpmem->dwAvailPageFile += free;
            }

	    /* new style /proc/meminfo ... */
	    if (sscanf(buffer, "MemTotal: %d", &total))
	    	lpmem->dwTotalPhys = total*1024;
	    if (sscanf(buffer, "MemFree: %d", &free))
	    	lpmem->dwAvailPhys = free*1024;
	    if (sscanf(buffer, "SwapTotal: %d", &total))
	        lpmem->dwTotalPageFile = total*1024;
	    if (sscanf(buffer, "SwapFree: %d", &free))
	        lpmem->dwAvailPageFile = free*1024;
        }
        fclose( f );

        if (lpmem->dwTotalPhys)
        {
            lpmem->dwTotalVirtual = lpmem->dwTotalPhys+lpmem->dwTotalPageFile;
            lpmem->dwAvailVirtual = lpmem->dwAvailPhys+lpmem->dwAvailPageFile;
            lpmem->dwMemoryLoad = (lpmem->dwTotalVirtual-lpmem->dwAvailVirtual)
                                      / (lpmem->dwTotalVirtual / 100);
            return;
        }
    }
#endif
    /* FIXME: should do something for other systems */
    lpmem->dwMemoryLoad    = 0;
    lpmem->dwTotalPhys     = 16*1024*1024;
    lpmem->dwAvailPhys     = 16*1024*1024;
    lpmem->dwTotalPageFile = 16*1024*1024;
    lpmem->dwAvailPageFile = 16*1024*1024;
    lpmem->dwTotalVirtual  = 32*1024*1024;
    lpmem->dwAvailVirtual  = 32*1024*1024;
}

typedef struct _MEMORY_BASIC_INFORMATION { // mbi 
    LPVOID BaseAddress;            // base address of region 
    LPVOID AllocationBase;         // allocation base address 
    DWORD AllocationProtect;      // initial access protection 
    DWORD RegionSize;             // size, in bytes, of region 
    DWORD State;                  // committed, reserved, free 
    DWORD Protect;                // current access protection 
    DWORD Type;                   // type of pages 
} MEMORY_BASIC_INFORMATION; 
typedef MEMORY_BASIC_INFORMATION *PMEMORY_BASIC_INFORMATION; 
 

DWORD VirtualQuery(  LPCVOID lpAddress,// address of region
  PMEMORY_BASIC_INFORMATION lpBuffer,  // address of information buffer
  DWORD dwLength)                      // size of buffer
{
	VM_MAP *vm;

	logstr(LF_APICALL,"VirtualQuery(%p,%p,%lx)\n",
		lpAddress,lpBuffer,dwLength);

        lpAddress = (void *)((UINT)(lpAddress) & ~page_mask);
	for(vm=TWINVM.vm_map;vm;vm = vm->next) {

		/* is the desired block in the range of this map? */
		if((vm->base <= lpAddress) && (lpAddress <= vm->limit)) { 
			lpBuffer->BaseAddress = (LPVOID) lpAddress;
			lpBuffer->AllocationBase = vm->base;
			lpBuffer->RegionSize = vm->limit - vm->base;
			lpBuffer->State      = vm->type;
			lpBuffer->Protect    = vm->protect;
			lpBuffer->Type       = MEM_PRIVATE;
		}
		break;
	}

	logstr(LF_APIRET,"VirtualQuery returns DWORD %lx\n",dwLength);
	
	return dwLength;
}

BOOL VirtualLock(  LPVOID lpAddress,  // address of first byte of range to lock
  DWORD dwSize)       // number of bytes in range to lock
{
	logstr(LF_APICALL,"VirtualLock(%p,%lx)\n",
		lpAddress,dwSize);
	logstr(LF_APIRET,"VirtualLock returns DWORD %lx\n",dwSize);
	
	return dwSize;
}


BOOL VirtualUnlock(  LPVOID lpAddress,  // address of first byte of range
  DWORD dwSize)       // number of bytes in range
{
	logstr(LF_APICALL,"VirtualUnlock(%p,%lx)\n",
		lpAddress,dwSize);
	logstr(LF_APIRET,"VirtualUnlock returns DWORD %lx\n",dwSize);
	
	return dwSize;
}

BOOL WINAPI
IsBadReadPtr(const void *lpsz, UINT cchMax)
{
    APISTR((LF_APISTUB,"IsBadReadPtr(void * %p,UINT=%d)\n", lpsz, cchMax));
    return FALSE;
}

BOOL WINAPI
IsBadWritePtr(void *lp, UINT cb)
{
    APISTR((LF_APISTUB,"IsBadWritePtr(void * %p,UINT=%d)\n", lp, cb));
    return FALSE;
}

BOOL WINAPI
IsBadCodePtr(FARPROC fp)
{
    VM_MAP *vm;
    void *lpsz = (void *) fp;

    APISTR((LF_APICALL,"IsBadCodePtr(void * %p)\n", lpsz));

    for(vm=TWINVM.vm_map;vm;vm = vm->next) {
	if(lpsz > vm->limit)
		continue;
	if(lpsz < vm->base)
		break;

	if((vm->protect & (PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE))) {
    		APISTR((LF_APIRET,"IsBadReadPtr returns BOOL %d\n",FALSE));
		return FALSE;
	}
	break;

    }

    APISTR((LF_APIRET,"IsCodeReadPtr returns BOOL %d\n",TRUE));
    return TRUE;
}

#ifdef TWIN_BIN32

BOOL WINAPI
IsBadStringPtrW(const void *lp, UINT cb)
{
    int len;
    len = lstrlenW(lp);
    if(len < cb)
	cb = len;
    return IsBadReadPtr(lp,cb);
}

BOOL WINAPI
IsBadStringPtrA(const void *lp, UINT cb)
{
    int len;
    len = lstrlenW(lp);
    if(len < cb)
	cb = len;
    return IsBadReadPtr(lp,cb);
}

#endif
