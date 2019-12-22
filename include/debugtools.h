
#ifndef __WINE_DEBUGTOOLS_H
#define __WINE_DEBUGTOOLS_H

#define DEBUG_RUNTIME

#define DEBUG_CLASS_COUNT 8

extern short debug_msg_enabled[][DEBUG_CLASS_COUNT];

#include "Log.h"

extern void debuggerbreak();

/*****************************************************************/

#define dbg_ch_index(ch) (dbch_##ch)
#define dbg_cl_index(cl) (dbcl_##cl)
#define dbg_fmt(cl,ch)	 ((dbg_ch_index(ch) << 8) | dbg_cl_index(cl))

#define DEBUGGING(cl, ch) \
  (dbg_ch_index(ch) >=0 && dbg_cl_index(cl) >= 0 && \
   debug_msg_enabled[dbg_ch_index(ch)][dbg_cl_index(cl)])

#define DPRINTF(dbg,format, args...) logstr(dbg, format, ## args)

#define DPRINTF_(cl, ch, format, args...) \
  do {if(!DEBUGGING(cl, ch)) ; \
  else logstr( dbg_fmt(cl,ch),format, ## args); } while (0)

/*****************************************************************/
/* use configure to allow user to compile out debugging messages */
/*****************************************************************/

#ifndef NO_TRACE_MSGS
#define TRACE(fmt, args...) DPRINTF_(TRACE, LIB, fmt, ## args)
#define MSG(fmt, args...) DPRINTF_(WARN,LIB,fmt,##args)
#else
#define TRACE(fmt, args...) do { } while (0)
#define MSG(fmt, args...) do {} while (0)
#endif /* NO_TRACE_MSGS */

#ifndef NO_DEBUG_MSGS
#define WARN(fmt, args...)  DPRINTF_(WARN,  LIB, fmt, ## args)
#define FIXME(fmt, args...) DPRINTF_(FIXME, LIB, fmt, ## args)
#define ERR(fmt,args...)    DPRINTF_(FIXME, LIB, fmt, ## args)
#else
#define WARN( fmt, args...) do { } while (0)
#define FIXME( fmt, args...) do { } while (0)
#endif /* NO_DEBUG_MSGS */

#define FAIL( fmt, args...)   DPRINTF_(FAIL, LIB, fmt, ## args)

/*****************************************************************/
/* if the debug message is compiled out, make these return false */
/*****************************************************************/

#ifndef NO_TRACE_MSGS
#define TRACE_ON(ch)  DEBUGGING(TRACE, ch)
#else
#define TRACE_ON(ch) 0
#endif /* NO_TRACE_MSGS */

#ifndef NO_DEBUG_MSGS
#define WARN_ON(ch)   DEBUGGING(WARN, ch)
#define FIXME_ON(ch)  DEBUGGING(FIXME, ch)
#else
#define WARN_ON(ch) 0
#define FIXME_ON(ch) 0
#endif /* NO_DEBUG_MSGS */

#define FAIL_ON(ch)    DEBUGGING(FAIL, ch)

#endif  /* __WINE_DEBUGTOOLS_H */
