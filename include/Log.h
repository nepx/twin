#ifndef Log__h
#define Log__h	

#include "windows.h"
#include "debug.h"
#include "MsgCode.h"

void logstr(unsigned long flg, ...);

#ifdef NO_TRACE_MSGS
#define ERRSTR(x)	logstr x
#define APISTR(x)	
#define LOGSTR(x)
#define LOGMSG(h,m,w,l)
#define LOGMSGCALL(f,t,h,m,w,l)
#define LOGMSGRET(f,t,h,m,w,l)
#else
#define ERRSTR(x)	logstr x
#define APISTR(x)	logstr x
#define LOGSTR(x)	logstr x
#define LOGMSG(h,m,w,l) LogTwinMsgCall(LF_MSG,0,h,m,w,l,__FUNCTION__);
#define LOGMSGCALL(f,t,h,m,w,l) LogTwinMsgCall(f,t,h,m,w,l,__FUNCTION__);
#define LOGMSGRET(f,t,h,m,w,l) LogTwinMsgRet(f,t,h,m,w,l,__FUNCTION__);
#endif

#define LOGFLAG(cl,ch)	(dbch_##ch << 8 | dbcl_##cl)

/*
 *	CONSOLE		to stdout, as well as to log file
 *	TRACE		off by default
 *	WARN		unexpected operation
 *	FAIL		cannot recover
 *
 *	STUB		to do, not implemented
 *      FIXME		to do, partial implementation
 *	CALL
 *	RET
 */

#define LF_CONSOLE	LOGFLAG(CONSOLE,LIB)
#define LF_WARNING	LOGFLAG(WARN,LIB)
#define LF_ERROR	LOGFLAG(FAIL,LIB)
#define LF_FIXME	LOGFLAG(FIXME,LIB)

#define LF_APICALL	LOGFLAG(CALL,API)
#define LF_APIRET	LOGFLAG(RET,API)
#define LF_APIFAIL	LOGFLAG(FAIL,API)
#define LF_APISTUB	LOGFLAG(FIXME,API)

#define LF_BINCALL	LOGFLAG(CALL,BIN)
#define LF_BINRET	LOGFLAG(RET,BIN)

#define LF_INTCALL	LOGFLAG(CALL,INT)
#define LF_INTRET	LOGFLAG(RET,INT)
#define LF_INT86	LOGFLAG(TRACE,INT)

#define LF_API		LOGFLAG(TRACE,API)
#define LF_MSG		LOGFLAG(TRACE,MSG)
#define LF_LOGPE	LOGFLAG(TRACE,BIN)
#define LF_LOG		LOGFLAG(TRACE,LIB)
#define LF_MEM		LOGFLAG(TRACE,MEM)
#define LF_GDI		LOGFLAG(TRACE,GDI)
#define LF_USER		LOGFLAG(TRACE,USER)
#define LF_SYS		LOGFLAG(TRACE,KERNEL)

void logstr(unsigned long flg, ...);
void LogTwinMsgCall(DWORD,MSGCODE *,HWND ,int ,WPARAM ,LPARAM ,char *);
DWORD LogTwinMsgRet(DWORD,DWORD,HWND ,int ,WPARAM ,LPARAM ,char *);

#endif
