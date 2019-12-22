/*
 * ldt.h
 *
 * Definitions of structures used with the modify_ldt system call.
 */
#ifndef _LINUX_LDT_H
#define _LINUX_LDT_H

/* Maximum number of LDT entries supported. */
#define LDT_ENTRIES	8192
/* The size of each LDT entry. */
#define LDT_ENTRY_SIZE	8

struct modify_ldt_ldt_s {
	unsigned int  entry_number;
	unsigned long base_addr;
	unsigned int  limit;
	unsigned int  seg_32bit:1;
	unsigned int  contents:2;
	unsigned int  read_exec_only:1;
	unsigned int  limit_in_pages:1;
	unsigned int  seg_not_present:1;
	unsigned int  useable:1;
};

#define MODIFY_LDT_CONTENTS_DATA	0
#define MODIFY_LDT_CONTENTS_STACK	1
#define MODIFY_LDT_CONTENTS_CODE	2

#define __AHSHIFT  3  /* don't change! */
#define __AHINCR   (1 << __AHSHIFT)

#define SELECTOR_TO_ENTRY(sel)  (((int)(sel) & 0xffff) >> __AHSHIFT)
#define ENTRY_TO_SELECTOR(i)    ((i) ? (((int)(i) << __AHSHIFT) | 7) : 0)

#ifdef WINE
extern unsigned char ldt_flags_copy[LDT_SIZE];
#define IS_LDT_ENTRY_FREE(i)    (!(ldt_flags_copy[(i)] & LDT_FLAGS_ALLOCATED))
#define IS_SELECTOR_FREE(sel)   (IS_LDT_ENTRY_FREE(SELECTOR_TO_ENTRY(sel)))
#define GET_SEL_BASE(sel)       (ldt_copy[SELECTOR_TO_ENTRY(sel)].base)
#define GET_SEL_LIMIT(sel)      (ldt_copy[SELECTOR_TO_ENTRY(sel)].limit)
#define GET_SEL_FLAGS(sel)   (ldt_flags_copy[SELECTOR_TO_ENTRY(sel)])
#else
/* TWIN */
#define IS_LDT_ENTRY_FREE(i)    (GetPhysicalAddress(ENTRY_TO_SELECTOR(i)) == 0)
#define IS_SELECTOR_FREE(sel)   (GetPhysicalAddress(sel) == 0)
#define GET_SEL_BASE(sel)       (GetPhysicalAddress(sel))
#define GET_SEL_LIMIT(sel)      (GetSelectorLimit(sel))
#define GET_SEL_FLAGS(sel)   GetSelectorFlags(sel)
#endif

/* Convert a segmented ptr (16:16) to a linear (32) pointer */

#define PTR_SEG_OFF_TO_LIN(seg,off) \
   ((void*)(GET_SEL_BASE(seg) + (unsigned int)(off)))
#define PTR_SEG_TO_LIN(ptr) \
   PTR_SEG_OFF_TO_LIN(SELECTOROF(ptr),OFFSETOF(ptr))
#define PTR_SEG_OFF_TO_SEGPTR(seg,off) \
   ((SEGPTR)MAKELONG(off,seg))
#define PTR_SEG_OFF_TO_HUGEPTR(seg,off) \
   PTR_SEG_OFF_TO_SEGPTR( (seg) + (HIWORD(off) << __AHSHIFT), LOWORD(off) )


#define LDT_FLAGS_TYPE      0x03  /* Mask for segment type */
#define LDT_FLAGS_READONLY  0x04  /* Segment is read-only (data) */
#define LDT_FLAGS_EXECONLY  0x04  /* Segment is execute-only (code) */
#define LDT_FLAGS_32BIT     0x08  /* Segment is 32-bit (code or stack) */
#define LDT_FLAGS_BIG       0x10  /* Segment is big (limit is in pages) */
#define LDT_FLAGS_ALLOCATED 0x80  /* Segment is allocated (no longer free) */

#define FIRST_LDT_ENTRY_TO_ALLOC  17

/* Determine if sel is a system selector (i.e. not managed by Wine) */
#define IS_SELECTOR_SYSTEM(sel) \
   (!((sel) & 4) || (SELECTOR_TO_ENTRY(sel) < FIRST_LDT_ENTRY_TO_ALLOC))
#define IS_SELECTOR_32BIT(sel) \
   (IS_SELECTOR_SYSTEM(sel) || (GET_SEL_FLAGS(sel) & LDT_FLAGS_32BIT))

#endif
