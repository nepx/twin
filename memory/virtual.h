
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
