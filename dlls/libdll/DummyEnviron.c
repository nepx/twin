/*    DummyEnviron.c	1.3	1/29/96 dummy declaration of environ for linux
 */

/*
 *  Dummy declaration of _environ needed on Linux platform, due to bug
 *  in the way the crt*.o files interoperate with -shared -Bsymbolic flags.
 */

char **__environ;

