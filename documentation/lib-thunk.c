
Intro
-----

This document will try to describe what a native thunk is, and how it is
used, and by doing so, what is necessary to build one for an arbitrary
platform.

For the purposes of this discussion, a native thunk is a piece of code that
completes the transition from native code to binary code.  Native code means
the code that is compiled natively for a given platform, while binary code
is the application specific code, for now, it will mean 16bit protected mode
code.  If we can extend this to include 32bit linear application code has 
not been determined, and is beyond the scope of this document.

Background
----------

Windows has several functions that are passed to the windows libraries, for
it to 'callback' at a later time.  Instances of these are, window procedures
defined by RegisterClass(), and enumeration functions.  For better or worse,
we early on defined that the library would NOT know anything about binary 
code, and that the binary code needed to interface to the library, just as
any other application would.  A major first issue was that of addresses. How
can a 16bit protected mode address, ie. CS:IP be passed to the library, so
that it can be called back?  This mechanism would have to work on both risc
and intel transparently.  It would also have to maintain the semantics of
the call, for a call to a window procedure the library ONLY knows to pass
HWND, MSG, WPARAM, LPARAM to the the window procedure defined in its class
structure, (from RegisterClass).  Our solution is a function, 

	NATIVE_ADDRESS make_native_thunk(BINARY_ADDRESS, HANDLER);

This function returns a native address that 'knows' that it should call
the binary code, (through the interpreter or directly) with CS:IP set to
BINARY_ADDRESS.  We added the concept of HANDLER to allow us to translate
arguments and results before and after the call to invoke_binary(). We use
the single function, invoke_binary() regardless of calling the interpreter
or executing it directly.  The HANDLER's main purpose is to push arguments
onto the binary stack, after appropriate endian conversions.

Before dissecting a major HANDLER, lets start with a very small one, that
used by EnumWindows().  This callback expects to be passed an HWND and
an LPARAM. To do this, the handler below is used, and is prototyped just
like the actual handler.  It adjusts the stack, and pushes its arguments.
It then calls invoke_binary().  Where/how does invoke_binary "know" where
to go?  The answer is that make_native_thunk saves both the BINARY address
and the HANDLER.  It also 'knows' the prototype of the HANDLER?

BOOL CALLBACK
hsw_wndenumproc(HWND hwnd, LPARAM lParam)
{
    envp_global->reg.sp -= HANDLE_86 + LONG_86;
    PUTWORD((LPBYTE)envp_global->reg.sp+4,hwnd);
    PUTDWORD((LPBYTE)envp_global->reg.sp,lParam);
    invoke_binary();
    return (BOOL)envp_global->reg.ax;
}

Thunk_Template[]
----------------

I will give here the x86 thunk, and try to show that they all are essentially
the same.

	char thunk_template[] = {
		0x90, 0x53,                     /* PUSH EBX */
		0x90, 0xbb,			/* MOV  EBX, 0 */ 
		0x00, 0x00, 0x00, 0x00,		/* data -- envp_global */
		0x8b, 0x1b, 0x90, 0x90,         /* MOV  EBX, [EBX]  */
		0x90, 0xc7, 0x43, TRANS_ADDR,	/* MOV  [EBX+TRANS_ADDR], 0L */
		0x90, 0x90, 0x90, 0x90, 	/* data -- binaddr    */
		0x90, 0x90,			/* alignment */
		0x5b,				/* POP  EBX */
		0xe9,				/* JMP  */
		0x90, 0x90, 0x90, 0x90, 	/* data -- conv()offset     */
		0x54, 0x57, 0x53, 0x48		/* THUNK_MAGIC */
	};

	#define TARG_OFF        4
	#define CONV_OFF        6
	#define CUR_ENV_OFF	1
	#define THUNK_MAGIC_OFFSET 7
	#define THUNK_SIZE	8

When we create a native thunk, we allocate memory for a thunk_template, and
copy the template into the new memory. We then fill in 3 pieces of data,
	1) the address of the pointer to envp_global
	2) the BINADDR passed to us
	3) the HANDLER (conv()) passed to us.

We then return the address of this thunk, (and on some platforms, make sure
its executable, and that the code/data caches are syncronized). 

When we execute the thunk, as in (*lpfn)(hwnd,lparam), lets watch what happens.

		PUSH EBX 

		MOV  EBX, (&envp_global *) 
			  ----------------
		MOV  EBX, [EBX]  

		MOV  [EBX+TRANS_ADDR], BINADDR
				       -------
		POP  EBX 

		JMP  HANDLER
		     -------

The underlined values were inserted into the template. 

What this does is,
1) save EBX, 2) load it with the address of a pointer to envp_global, 
3) load EBX with the pointer to envp_global, 4) set CS:IP in envp_global
with BINADDR, 5) restore EBX, and 6) JUMP, not call the handler.  

At this point, the stack is identical to when the thunk was called, and on
return from the HANDLER, we return to the original caller. 

The handler works as described above.

The global envp_global points to the x86 environment that will be active
on the next invoke_binary, at the call to make_native_thunk, we don't 
know the address of the actual envp structure will be valid when the 
thunk is called.  The thunk needs to copy just the CS:IP into the currently
active envp, and then call the HANDLER.

You can easily see the sparc thunk works similarly, and if you know the
other architectures, you can see they do essentially the same thing.

----------------------------------------------------------------------------
This ends the first pass to the definition and design of a native thunk.

Let the questions begin...
