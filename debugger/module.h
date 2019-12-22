/*************************************************************************
 * This source is derived from the Linux wine project. see www.winehq.com.
 * Bug fixes and patches to this file should be made in the original and then 
 * reflected into Twine.  Please see WINE in this distribution for more 
 * information.  This file is from wine-981129/debugger/module.c
 ************************************************************************/

/* Macros for easier access to context registers */

#define EAX_reg(context)     ((context)->Eax)
#define EBX_reg(context)     ((context)->Ebx)
#define ECX_reg(context)     ((context)->Ecx)
#define EDX_reg(context)     ((context)->Edx)
#define ESI_reg(context)     ((context)->Esi)
#define EDI_reg(context)     ((context)->Edi)
#define EBP_reg(context)     ((context)->Ebp)

#define CS_reg(context)      ((context)->SegCs)
#define DS_reg(context)      ((context)->SegDs)
#define ES_reg(context)      ((context)->SegEs)
#define FS_reg(context)      ((context)->SegFs)
#define GS_reg(context)      ((context)->SegGs)
#define SS_reg(context)      ((context)->SegSs)

#define EFL_reg(context)     ((context)->EFlags)
#define EIP_reg(context)     ((context)->Eip)
#define ESP_reg(context)     ((context)->Esp)

#define AX_reg(context)      (*(WORD*)&EAX_reg(context))
#define BX_reg(context)      (*(WORD*)&EBX_reg(context))
#define CX_reg(context)      (*(WORD*)&ECX_reg(context))
#define DX_reg(context)      (*(WORD*)&EDX_reg(context))
#define SI_reg(context)      (*(WORD*)&ESI_reg(context))
#define DI_reg(context)      (*(WORD*)&EDI_reg(context))
#define BP_reg(context)      (*(WORD*)&EBP_reg(context))

#define AL_reg(context)      (*(BYTE*)&EAX_reg(context))
#define AH_reg(context)      (*((BYTE*)&EAX_reg(context)+1))
#define BL_reg(context)      (*(BYTE*)&EBX_reg(context))
#define BH_reg(context)      (*((BYTE*)&EBX_reg(context)+1))
#define CL_reg(context)      (*(BYTE*)&ECX_reg(context))
#define CH_reg(context)      (*((BYTE*)&ECX_reg(context)+1))
#define DL_reg(context)      (*(BYTE*)&EDX_reg(context))
#define DH_reg(context)      (*((BYTE*)&EDX_reg(context)+1))
                            
#define IP_reg(context)      (*(WORD*)&EIP_reg(context))
#define SP_reg(context)      (*(WORD*)&ESP_reg(context))
                            
#define FL_reg(context)      (*(WORD*)&EFL_reg(context))

#define SET_CFLAG(context)   (EFL_reg(context) |= 0x0001)
#define RESET_CFLAG(context) (EFL_reg(context) &= 0xfffffffe)
