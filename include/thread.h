
#ifndef _thread_h_
#define _thread_h_
/*
 * this is our definition
 */
typedef struct _process_db {
        K32OBJ         	header;         /*  00 Kernel object header */
	HTASK    	hTask;
    	HANDLE	       *handletable;
} PDB;

/* Thread exception block */
typedef struct _TEB
{
    void        *except;         /* 00 Head of exception handling chain */
    void        *stack_top;      /* 04 Top of thread stack */
    void        *stack_low;      /* 08 Stack low-water mark */
    HTASK        htask16;        /* 0c Win16 task handle */
    WORD         stack_sel;      /* 0e 16-bit stack selector */
    DWORD        selman_list;    /* 10 Selector manager list */
    DWORD        user_ptr;       /* 14 User pointer */
    struct _TEB *self;           /* 18 Pointer to this structure */
    WORD         flags;          /* 1c Flags */
    WORD         mutex_count;    /* 1e Win16 mutex count */
    DWORD        debug_context;  /* 20 Debug context */
    DWORD       *ppriority;      /* 24 Pointer to current priority */
    HANDLE       queue;          /* 28 Message queue */
    LPVOID      *tls_ptr;        /* 2c Pointer to TLS array */
    PDB         *process;        /* 30 owning process (used by NT3.51 applets)*/
} TEB;

/* Thread exception flags */
#define TEBF_WIN32  0x0001
#define TEBF_TRAP   0x0002

/* Event waiting structure */
typedef struct
{
    DWORD         count;     /* Count of valid objects */
    BOOL          wait_all;  /* Wait for all objects flag */
    K32OBJ       *objs[MAXIMUM_WAIT_OBJECTS];  /* Object pointers */
    int           server[MAXIMUM_WAIT_OBJECTS];  /* Server handles */
} WAIT_STRUCT;

/* Thread database */
typedef struct _THDB
{
    K32OBJ         header;         /*  00 Kernel object header */
    void	  *process;        /*  08 Process owning this thread */
    HANDLE         event;          /*  0c Thread event */
    TEB            teb;            /*  10 Thread exception block */
    DWORD          flags;          /*  44 Flags */
    DWORD          dwExit;         /*  48 Termination status */
    WORD           teb_sel;        /*  4c Selector to TEB */
    WORD           emu_sel;        /*  4e 80387 emulator selector */
    int            thread_errno;   /*  50 Per-thread errno (was: unknown) */
    WAIT_STRUCT   *wait_list;      /*  54 Event waiting list */
    int            thread_h_errno; /*  50 Per-thread h_errno (was: unknown) */
    void          *ring0_thread;   /*  5c Pointer to ring 0 thread */
    void          *ptdbx;          /*  60 Pointer to TDBX structure */
    void          *stack_base;     /*  64 Base of the stack */
    void          *exit_stack;     /*  68 Stack pointer on thread exit */
    void          *emu_data;       /*  6c Related to 80387 emulation */
    DWORD          last_error;     /*  70 Last error code */
    void          *debugger_CB;    /*  74 Debugger context block */
    DWORD          debug_thread;   /*  78 Thread debugging this one (?) */
    void          *pcontext;       /*  7c Thread register context */
    DWORD          cur_stack;      /*  80 Current stack (was: unknown) */
    DWORD          unknown3[2];    /*  84 Unknown */
    WORD           current_ss;     /*  8c Another 16-bit stack selector */
    WORD           pad2;           /*  8e */
    void          *ss_table;       /*  90 Pointer to info about 16-bit stack */
    WORD           thunk_ss;       /*  94 Yet another 16-bit stack selector */
    WORD           pad3;           /*  96 */
    LPVOID         tls_array[64];  /*  98 Thread local storage */
    DWORD          delta_priority; /* 198 Priority delta */
    DWORD          unknown4[7];    /* 19c Unknown */
    void          *create_data;    /* 1b8 Pointer to creation structure */
    DWORD          suspend_count;  /* 1bc SuspendThread() counter */
    void          *entry_point;    /* 1c0 Thread entry point (was: unknown) */
    void          *entry_arg;      /* 1c4 Entry point arg (was: unknown) */
    int            unix_pid;       /* 1c8 Unix thread pid (was: unknown) */
    DWORD          unknown5[3];    /* 1cc Unknown */
    DWORD          sys_count[4];   /* 1d8 Syslevel mutex entry counters */
    CRITICAL_SECTION *sys_mutex[4];/* 1e8 Syslevel mutex pointers */
    DWORD          unknown6[2];    /* 1f8 Unknown */
} THDB;

/* Thread queue entry */
typedef struct _THREAD_ENTRY
{
    THDB                 *thread;
    struct _THREAD_ENTRY *next;
} THREAD_ENTRY;

/* Current Process pseudo-handle - Returned by GetCurrentProcess*/
#define CURRENT_PROCESS_PSEUDOHANDLE ((HANDLE32)0x7fffffff)

#define CURRENT_THREAD_PSEUDOHANDLE 	0xfffffffe
#define THREAD_OBFUSCATOR	    	0xdeadbeef
#define THREAD_ID_TO_THDB(id)    ((THDB *)((id) ^ THREAD_OBFUSCATOR))
#define THDB_TO_THREAD_ID(thb)   ((DWORD)(thb) ^ THREAD_OBFUSCATOR)

#endif
