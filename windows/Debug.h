

#define TWIN_DBGINIT	0	/* initialize debugger entry point */
#define TWIN_DBGLOAD	1	/* load the twin debugger */
#define TWIN_DBGCALL	2	/* call the debugger */
#define TWIN_DBGADDR	3	/* retrieve symbolic name of address */
#define TWIN_DBGADDS	4	/* add symbol to symbol table */

typedef struct {
	char *symbol;
	void *address;
	int   flag;
} DBG_INFO;

extern void TWIN_DebuggerProc(int,int, void *);

