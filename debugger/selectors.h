/*************************************************************************
 * This source is derived from the Linux wine project. see www.winehq.com.
 * Bug fixes and patches to this file should be made in the original and then 
 * reflected into Twine.  Please see WINE in this distribution for more 
 * information.  This file is from wine-981129/debugger/selectors.h
 ************************************************************************/

#ifdef X386
# define __GET_SEG(seg,res) __asm__( "movw %%" seg ",%w0" : "=r" (res) )
# define __SET_SEG(seg,val) __asm__( "movw %w0,%%" seg : : "r" (val) )
#else  /* __i386__ */
# define __GET_SEG(seg,res) ((res) = 0)
# define __SET_SEG(seg,val) /* nothing */
#endif  /* __i386__ */

#define GET_CS(cs) __GET_SEG("cs",cs)
#define GET_DS(ds) __GET_SEG("ds",ds)
#define GET_ES(es) __GET_SEG("es",es)
#define GET_FS(fs) __GET_SEG("fs",fs)
#define GET_GS(gs) __GET_SEG("gs",gs)
#define GET_SS(ss) __GET_SEG("ss",ss)

#define SET_CS(cs) __SET_SEG("cs",cs)
#define SET_DS(ds) __SET_SEG("ds",ds)
#define SET_ES(es) __SET_SEG("es",es)
#define SET_FS(fs) __SET_SEG("fs",fs)
#define SET_GS(gs) __SET_SEG("gs",gs)
#define SET_SS(ss) __SET_SEG("ss",ss)

