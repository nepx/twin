
#ifndef _MTLOCK_H_
#define _MTLOCK_H_

#define _MTLOCK_TABLE	0
#define _MTLOCK_WIN16   1
#define _MTLOCK_VIDEO	2
#define _MTLOCK_QUEUE	3
#define _MTLOCK_THREAD  4
#define _MTLOCK_WINDOW  5
#define _MTLOCK_PROCESS 6

#define _NMTLOCKS _MTLOCK_PROCESS+1

void TWIN_EnterSection(int ,char *,int );
void TWIN_LeaveSection(int );

#if 0
#define LOCK_SECTION(id)	TWIN_EnterSection(id,__FILE__,__LINE__);
#define UNLOCK_SECTION(id)	TWIN_LeaveSection(id);
#else
#define LOCK_SECTION(id)
#define UNLOCK_SECTION(id)
#endif

#endif
