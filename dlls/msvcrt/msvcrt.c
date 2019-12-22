

#include <stdio.h>
#include <ctype.h>
#include "windows.h"
#include "Log.h"

int TWIN__isascii(int ch)
{
	return isascii(ch);
}

int __app_type;

int
__set_app_type(int app_type)
{
	return __app_type = app_type;
}	

int __fmode;

/*
 * return pointer to commandline
 */
extern char *_acmdln;

extern char *GetCommandLineA();

char **
__p__acmdln()
{
	return &_acmdln;
}

int *
__p__fmode()
{
	return &__fmode;
}

int __commode;

int * 
__p__commode()
{
	return &__commode;
}

unsigned int
_controlfp(unsigned int precision,unsigned int control)
{
	return 0;
}

typedef void ( *PFV)(void);

void
_initterm(PFV *pfbegin,PFV *pfend)
{
       /*
         * walk the table of function pointers from the top down, until
         * bottom is encountered.  Do not skip the first entry.
         */
        for ( ;pfbegin < pfend ; pfbegin++)
        {
            /*
             * if current table entry is non-NULL (and not -1), call
             * thru it from end of table to begin.
             */
            if ( *pfbegin != NULL && *pfbegin != (PFV) -1 ) {
		//printf("calling entry %p\n",pfbegin);
                (**pfbegin)();
		//printf("returned from entry %p\n",pfbegin);
	    }
        }
}


BOOL
__dllonexit(int flag,PFV *pfbegin,PFV *pfend)
{
	//printf("__dllonexit %x %p %p\n",flag,pfbegin,pfend);
	return TRUE;
}

void
_setargv()
{
	//printf("non-wildcard version __setargv\n");
}

void
__setargv()
{
	//printf("wildcard version __setargv\n");
}

extern int __argc;
extern char **__argv;
extern char **__environ;
extern char *__acmdln;


typedef	struct {
	int	newmode;
} _startupinfo;

void __getmainargs (
        int *pargc,
        char ***pargv,
        char ***penvp,
        int dowildcard,
        _startupinfo * startinfo
        )
{
	logstr(LF_APICALL,
		"__getmainargs(int*=%p,char***%p,char***%p,int=%d,_startupinfo *%p)\n",
		pargc,pargv,penvp,dowildcard,startinfo?startinfo:0);

        if ( dowildcard )
            __setargv();    /* do wildcard expansion after parsing args */
        else
            _setargv();     /* NO wildcard expansion; just parse args */

        *pargc = __argc;
        *pargv = __argv;
        *penvp = __environ;
	logstr(LF_APIRET,"__getmainargs returns void\n");
}

int msvcrt32End()
{
	return 0;
}

void TWINexit(int rc)
{
	ExitProcess(rc);
}

