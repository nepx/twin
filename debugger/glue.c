/*
 * glue.c
 *
 * System dependent code.
 */

#include "windows.h"
#include "twindll.h"
#include "dlfcn.h"
#include "Debug.h"
#include "debugger.h"

void  DEBUG_Main( int signal );
int   DEBUG_ProcessElfObject(const char * filename, void *load_offset);

LIBENTRY_DECL(debug_init);

/*  
 * Initialization Entry Point.
 * called when debugger shared library is loaded.
 * calls TWIN_DebuggerProc to initialize callback.
 */

void twin_debug_proc(int ,int ,void *);

void
debug_init()
{
	fprintf(stderr,"LoadLibrary: DEBUGGER\n");	
	TWIN_DebuggerProc(0,0,(void *) twin_debug_proc);
}

/*
 * Interface to load symbols from a shared library
 * pass in the shared library name.  This will be
 * hooked into from the native library loadlibrary
 */
void DEBUG_LoadLibrary(char *name)
{
	Dl_info info;
	void *hso;
	void *addr;
	int rc;

	/*
	 * The following is a way to find out where a shared
	 * library is located.  It takes two steps, first 
	 * get a KNOWN address in the library, then using
	 * that address, lookup the ld.so information that
	 * describes where it is in memory
	 */
	hso = dlopen(name,RTLD_LAZY);
	addr = dlsym(hso,"_DYNAMIC");
	dlclose(hso);

	rc = dladdr(addr,&info);

	if(rc) {
		DEBUG_ProcessElfObject(info.dli_fname, info.dli_fbase);
	}
}

int 
DEBUG_RegisterELFDebugInfo(int load_addr,u_long size,char *filename)
{
	return 0;
}

void debuggerbreak();

void
twin_debug_proc(int opcode,int wparam,void *lparam)
{
	FILE *init_file;
    	char fname[256];

	if(opcode == TWIN_DBGLOAD) {
    
    		init_file = fopen(".windbginit", "r");
    		if (!init_file) {
			sprintf(fname, "%s/.windbginit", getenv("HOME"));
			init_file = fopen(fname, "r");
    		}


    		if (init_file)
			debuggerbreak();
		return;
	}

	if(opcode == TWIN_DBGADDS) {
		DBG_INFO *p = (DBG_INFO *) lparam;
		char *buffer = p->symbol;
		int   flags  = p->flag;
		DBG_ADDR addr;

    		addr.seg = 0;
    		addr.type = NULL;
		addr.off = (DWORD) p->address;

		DEBUG_AddSymbol(buffer,&addr,NULL,flags);

		return;
	}

	if(opcode == TWIN_DBGADDR) {
		char *p = 0;
		Dl_info info;

#if	0
		struct name_hash *nh;

            	DBG_ADDR address = { NULL, 0, wparam };

		p = DEBUG_FindNearestSymbol( &address, TRUE, &nh, 0, 0);

#else
		{
			char *errstr;
			info.dli_fname = 0;
			info.dli_fbase = 0;
			info.dli_sname = 0;
			info.dli_saddr = 0;

			dladdr((void *) wparam,&info);
			errstr = dlerror();

			p = (char *) info.dli_sname;
		}
#endif

		if(p) {
			sprintf((char *)lparam,"%s",p);
		} else {
			sprintf((char *)lparam,"[%x]",wparam);
		}

		return;
	}

 	/* call wine debugger */
	DEBUG_SetSigContext( lparam );
	DEBUG_Main( wparam );
	DEBUG_GetSigContext( lparam );
}
