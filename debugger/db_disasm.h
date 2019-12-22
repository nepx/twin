/*************************************************************************
 * This source is derived from the Linux wine project. see www.winehq.com.
 * Bug fixes and patches to this file should be made in the original and then 
 * reflected into Twine.  Please see WINE in this distribution for more 
 * information.  This file is from wine-981129/debugger/db_disasm.h
 ************************************************************************/

#define FALSE 0
#define TRUE 1

#ifdef linux
typedef unsigned char boolean_t;
#endif

typedef unsigned long db_addr_t;

extern db_addr_t db_disasm(db_addr_t loc, boolean_t altfmt, boolean_t flag16,
			   boolean_t flagnoprint);
