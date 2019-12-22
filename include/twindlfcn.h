#ifndef DLFCN_H
#define DLFCN_H

/*
 * Various defines and so forth for the dynamic linker
 */

/* For dlopen () */
#define RTLD_LAZY	1
#define RTLD_NOW	2
#define RTLD_GLOBAL	0x100

/* For dlsym */
#define RTLD_NEXT	((void *)-1)

/* The usual prototypes.  We use void * instead of the actual
 * datatype - the user does not manipulate the handles at all.
 */
 
extern void * dlopen (const char * , int );
extern void * dlsym (void *, const char *);
extern int    dlclose(void *);

#endif
