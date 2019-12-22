
typedef struct _heap_arena {
	DWORD dwSize;
	struct _heap_arena *prev;
	struct _heap_arena *next;
	WORD signature;
} HEAP_ARENA; 

typedef struct {
	DWORD dwSize;
	HEAP_ARENA arena;
} FREE_LIST_HEADER;

typedef struct {
	DWORD dwSize;
	DWORD nextBlock;
	FREE_LIST_HEADER freeListArray[4];
	void *nextHeap;
	HANDLE hCriticalSection;
	CRITICAL_SECTION criticalSection;	
	DWORD  dummy[2];
	short  flags;
	short  signature;
} *HEAP_HEADER;



