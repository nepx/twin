/* NOTE:
 * This file was modified for DOSEMU by the DOSEMU-team.
 * The original is 'Copyright 1997 Willows Software, Inc.' and generously
 * was put under the GNU Library General Public License.
 * ( for more information see http://www.willows.com/ )
 *
 * We make use of section 3 of the GNU Library General Public License
 * ('...opt to apply the terms of the ordinary GNU General Public License...'),
 * because the resulting product is an integrated part of DOSEMU and
 * can not be considered to be a 'library' in the terms of Library License.
 * The (below) original copyright notice from Willows therefore was edited
 * conforming to section 3 of the GNU Library General Public License.
 *
 * Nov. 1 1997, The DOSEMU team.
 */


/*    
	interp_main.c	1.31
    	Copyright 1997 Willows Software, Inc. 

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; see the file COPYING.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.


For more information about the Willows Twin Libraries.

	http://www.willows.com	

To send email to the maintainer of the Willows Twin Libraries.

	mailto:twin@willows.com 

changes for use with dosemu-0.67 1997/10/20 vignani@tin.it
changes for use with dosemu-0.99 1998/12/13 vignani@tin.it
 */

#include "emu-globv.h"

#ifndef _MSC_VER
#include <sys/time.h>
#endif
#include "hsw_interp.h"
#include "mod_rm.h"

/* ======================== global cpuemu variables =================== */
BOOL data32=0, code32=0;
BYTE *ea_ = NULL;
DWORD eav_ = 0;
#ifndef DOSEMU
Interp_VAR	interp_variables;
extern void	INT_handler(int, void *);	/* sorry - can't use ENV */
#endif
/* ==================================================================== */

extern long instr_count;
extern int emu_under_X;
#ifdef EMU_STAT
#include "timers.h"
extern long InsFreq[];
#ifdef EMU_PROFILE
extern long long InsTimes[];
static int LastOp = 0x100;
static long long pfT0 = 0;
static long long pfT1 = 0;
#endif
#endif	/* EMU_STAT */

char unknown_msg[] = "unknown opcode %02x at %04x:%04x\n";
char illegal_msg[] = "illegal opcode %02x at %04x:%04x\n";
char unsupp_msg[]  = "unsupported opcode %02x at %04x:%04x\n";

BYTE parity[256] = 
    {PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    PARITY_FLAG, 0, 0, PARITY_FLAG, 0, PARITY_FLAG, PARITY_FLAG, 0,
    0, PARITY_FLAG, PARITY_FLAG, 0, PARITY_FLAG, 0, 0, PARITY_FLAG};

FUNCT_PTR hsw_fp87_mem0[8] = {hsw_fp87_00m, hsw_fp87_01m, hsw_fp87_02m,
			      hsw_fp87_03m, hsw_fp87_04m, hsw_fp87_05m,
			      hsw_fp87_06m, hsw_fp87_07m};
FUNCT_PTR hsw_fp87_mem1[8] = {hsw_fp87_10m, hsw_fp87_11m, hsw_fp87_12m,
			      hsw_fp87_13m, hsw_fp87_14m, hsw_fp87_15m,
			      hsw_fp87_16m, hsw_fp87_17m};
FUNCT_PTR hsw_fp87_mem2[8] = {hsw_fp87_20m, hsw_fp87_21m, hsw_fp87_22m,
			      hsw_fp87_23m, hsw_fp87_24m, hsw_fp87_25m,
			      hsw_fp87_26m, hsw_fp87_27m};
FUNCT_PTR hsw_fp87_mem3[8] = {hsw_fp87_30m, hsw_fp87_31m, hsw_fp87_32m,
			      hsw_fp87_33m, hsw_fp87_34m, hsw_fp87_35m,
			      hsw_fp87_36m, hsw_fp87_37m};
FUNCT_PTR hsw_fp87_mem4[8] = {hsw_fp87_40m, hsw_fp87_41m, hsw_fp87_42m,
			      hsw_fp87_43m, hsw_fp87_44m, hsw_fp87_45m,
			      hsw_fp87_46m, hsw_fp87_47m};
FUNCT_PTR hsw_fp87_mem5[8] = {hsw_fp87_50m, hsw_fp87_51m, hsw_fp87_52m,
			      hsw_fp87_53m, hsw_fp87_54m, hsw_fp87_55m,
			      hsw_fp87_56m, hsw_fp87_57m};
FUNCT_PTR hsw_fp87_mem6[8] = {hsw_fp87_60m, hsw_fp87_61m, hsw_fp87_62m,
			      hsw_fp87_63m, hsw_fp87_64m, hsw_fp87_65m,
			      hsw_fp87_66m, hsw_fp87_67m};
FUNCT_PTR hsw_fp87_mem7[8] = {hsw_fp87_70m, hsw_fp87_71m, hsw_fp87_72m,
			      hsw_fp87_73m, hsw_fp87_74m, hsw_fp87_75m,
			      hsw_fp87_76m, hsw_fp87_77m};
FUNCT_PTR hsw_fp87_reg0[8] = {hsw_fp87_00r, hsw_fp87_01r, hsw_fp87_02r,
			      hsw_fp87_03r, hsw_fp87_04r, hsw_fp87_05r,
			      hsw_fp87_06r, hsw_fp87_07r};
FUNCT_PTR hsw_fp87_reg1[8] = {hsw_fp87_10r, hsw_fp87_11r, hsw_fp87_12r,
			      hsw_fp87_13r, hsw_fp87_14r, hsw_fp87_15r,
			      hsw_fp87_16r, hsw_fp87_17r};
FUNCT_PTR hsw_fp87_reg2[8] = {hsw_fp87_20r, hsw_fp87_21r, hsw_fp87_22r,
			      hsw_fp87_23r, hsw_fp87_24r, hsw_fp87_25r,
			      hsw_fp87_26r, hsw_fp87_27r};
FUNCT_PTR hsw_fp87_reg3[8] = {hsw_fp87_30r, hsw_fp87_31r, hsw_fp87_32r,
			      hsw_fp87_33r, hsw_fp87_34r, hsw_fp87_35r,
			      hsw_fp87_36r, hsw_fp87_37r};
FUNCT_PTR hsw_fp87_reg4[8] = {hsw_fp87_40r, hsw_fp87_41r, hsw_fp87_42r,
			      hsw_fp87_43r, hsw_fp87_44r, hsw_fp87_45r,
			      hsw_fp87_46r, hsw_fp87_47r};
FUNCT_PTR hsw_fp87_reg5[8] = {hsw_fp87_50r, hsw_fp87_51r, hsw_fp87_52r,
			      hsw_fp87_53r, hsw_fp87_54r, hsw_fp87_55r,
			      hsw_fp87_56r, hsw_fp87_57r};
FUNCT_PTR hsw_fp87_reg6[8] = {hsw_fp87_60r, hsw_fp87_61r, hsw_fp87_62r,
			      hsw_fp87_63r, hsw_fp87_64r, hsw_fp87_65r,
			      hsw_fp87_66r, hsw_fp87_67r};
FUNCT_PTR hsw_fp87_reg7[8] = {hsw_fp87_70r, hsw_fp87_71r, hsw_fp87_72r,
			      hsw_fp87_73r, hsw_fp87_74r, hsw_fp87_75r,
			      hsw_fp87_76r, hsw_fp87_77r};

#define IncEMUtime(i)	EMUtime+=((i)*CYCperINS)

void
invoke_data (ENVPARAMS)
{
    static char buf[0x100];

    sprintf(buf,"ERROR: data selector in CS\n cs:ip %x/%x\ncalled from %x:%x",
		HIWORD(env->trans_addr),LOWORD(env->trans_addr),
		HIWORD(env->return_addr),LOWORD(env->return_addr));
    FatalAppExit(0,buf);
}

BYTE *
  hsw_interp_16_16 (ENVPARAMS, BYTE *P0, register BYTE *__PC,
  int *err, int cycmax)
{
    BYTE *mem_ref;
    WORD atmp;
#if defined(__GNUC__) && defined(__i386__)
    register BYTE *PC asm("%esi");
#else
    register BYTE *PC;
#endif
    *err = 0;
    PC = __PC;

#ifndef	NO_TRACE_MSGS
    if (ea_ && (d_emu>6)) e_printf(">> previous EA: %p=%02lx\n",ea_,FETCH_QUAD(ea_));
#endif

next_switch:
#if defined(EMU_STAT) && defined(EMU_PROFILE)
    if (d_emu==0) pfT1 = GETTSC();
#endif
    if (CEmuStat & CeS_MODE_PM32) {
      if ((cycmax--) <= 0) return(PC);	/* no error, we're going back to 32/32 */
	}
    else {
#ifdef DOSEMU
      if (CEmuStat & (CeS_TRAP|CeS_DRTRAP|CeS_SIGPEND|CeS_LOCK)) {
	if (CEmuStat & CeS_LOCK)
	    CEmuStat &= ~CeS_LOCK;
	else {
	    if (CEmuStat & CeS_TRAP) {
	      if (*err) {
		if (*err==EXCP01_SSTP) CEmuStat &= ~CeS_TRAP; return (PC);
	      }
	      *err = EXCP01_SSTP;
	    }
	    else if (CEmuStat & CeS_DRTRAP) {
	      if (e_debug_check(PC)) { *err = EXCP01_SSTP; return(PC); }
	    }
	    else if ((iENV->flags & INTERRUPT_FLAG) && (CEmuStat & CeS_SIGPEND)) {
	      /* force exit after signal */
	      CEmuStat = (CEmuStat & ~CeS_SIGPEND) | CeS_SIGACT;
	      *err=EXCP_SIGNAL; return (PC);
	}
	}
	}
#endif
      instr_count++;
      IncEMUtime(1);
fast_switch:
      OVERR_DS = LONG_DS;
      OVERR_SS = LONG_SS;
      P0 = PC;
#ifndef NO_TRACE_MSGS
      if (d_emu) {
	e_trace(ENVARGS, P0, PC, 0);
    }
#endif
#ifdef EMU_STAT
      InsFreq[*((WORD *)PC)] += 1;
#ifdef EMU_PROFILE
      if (d_emu==0) {
	if (LastOp < 0x100) {
	    InsTimes[LastOp] += (pfT1 - pfT0);
	}
	LastOp = *PC; pfT0 = GETTSC();
    }
#endif
#endif
    }

override: ;    /* single semicolon needed to attach label to */
    switch (*PC) {
/*00*/	case ADDbfrm: {
	    DWORD res, src1, src2;
#ifndef NO_DEBUG_MSGS
	    /* 00 00 00 00 00 is probably invalid code... */
	    if (FETCH_QUAD(PC+1)==0) { *err=-98; return P0; }
#endif
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *HREG1; mem_ref = MEM_REF; src1 = *mem_ref;
	    *mem_ref = res = src1 + src2;
	    SETBFLAGS(0);
	    } goto next_switch; 
/*01*/	case ADDwfrm: {
	    DWORD src1, src2, res;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = *XREG1; mem_ref = MEM_REF; 
	    src1 = mFETCH_WORD(mem_ref);
		res = src1 + src2;
	    mPUT_WORD(mem_ref, res);
	    SETWFLAGS(0);
	    } goto next_switch; 
/*02*/	case ADDbtrm: {
	    DWORD res, src1, src2;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *MEM_REF; mem_ref = HREG1; src1 = *mem_ref;
	    *mem_ref = res = src1 + src2;
	    SETBFLAGS(0);
	    } goto next_switch; 
/*03*/	case ADDwtrm: {
	    DWORD src1, src2, res;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = *XREG1; mem_ref = MEM_REF; 
	    src1 = mFETCH_WORD(mem_ref);
	    *XREG1 = res = src1 + src2;
	    SETWFLAGS(0);
	    } goto next_switch; 
/*04*/	case ADDbia: {
	    DWORD src1, src2, res;
	    src1 = AL; src2 = *(PC+1);
	    res = src1 + src2;
	    AL = (BYTE)res;
	    SETBFLAGS(0);
	    } PC += 2; goto next_switch;
/*05*/	case ADDwia: {
	    DWORD src1, src2, res;
	    src1 = AX; src2 = FETCH_WORD((PC+1));
	    res = src1 + src2;
	    AX = (WORD)res;
	    SETWFLAGS(0);
	    } PC += 3; goto next_switch;
/*06*/	case PUSHes: {
	    PUSHWORD(SHORT_ES_16); 
	    } PC += 1; goto next_switch;
/*07*/	case POPes: {	/* restartable */
	    register WORD wtemp = TOS_WORD;
	    if ((*err = SET_SEGREG(LONG_ES,BIG_ES,MK_ES,wtemp))) {
		/* error code = sel(15..3),GDT/LDT(2),IDT(1),EXT(0) */
		iVAR->error_addr=wtemp&0xfffc; return P0; }
	    POPWORD(wtemp);
	    SHORT_ES_32 = wtemp; }
	    PC += 1; goto next_switch;
/*08*/	case ORbfrm: {
	    DWORD res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    mem_ref = MEM_REF;
	    res = *mem_ref | *HREG1;
	    *mem_ref = (BYTE)res;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } goto next_switch; 
/*09*/	case ORwfrm: {
	    DWORD res;
	    PC += hsw_modrm_16_word(MODARGS);
	    mem_ref = MEM_REF; 
	    res = mFETCH_WORD(mem_ref) | *XREG1;
	    mPUT_WORD(mem_ref, res);
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } goto next_switch; 
/*0a*/	case ORbtrm: {
	    DWORD res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    res = *HREG1 | *MEM_REF;
	    *HREG1 = (BYTE)res;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } goto next_switch; 
/*0b*/	case ORwtrm: {
	    DWORD res;
	    PC += hsw_modrm_16_word(MODARGS);
	    res = mFETCH_WORD(MEM_REF) | *XREG1;
	    *XREG1 = (WORD)res;
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } goto next_switch; 
/*0c*/	case ORbi: {
	    DWORD res;
	    res = AL | PC[1];
	    AL = (BYTE)res;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } PC += 2; goto next_switch;
/*0d*/	case ORwi: {
	    DWORD res;
	    res = AX | FETCH_WORD((PC+1));
	    AX = (WORD)res;
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } PC += 3; goto next_switch;
/*0e*/	case PUSHcs: {
	    PUSHWORD(SHORT_CS_16);
	    } PC += 1; goto next_switch;
/*0f*/	case TwoByteESC: {
	    register DWORD temp;
	    BYTE op;
	    switch ((op=*(PC+1))) {
		case 0x00: /* GRP6 - Extended Opcode 20 */
		    switch ((*(PC+2)>>3)&7) {
			case 0: /* SLDT */ {
			    /* Store Local Descriptor Table Register */
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    mem_ref = MEM_REF;
			    temp = 0 /* should be LDT selector */;
			    mPUT_WORD(mem_ref,temp);
			    } goto next_switch;
			case 1: /* STR */ {
			    /* Store Task Register */
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    mem_ref = MEM_REF;
			    temp = 0 /* should be Task Register */;
			    mPUT_WORD(mem_ref,temp);
			    } goto next_switch;
			case 2: /* LLDT */ /* Privileged */
			    /* Load Local Descriptor Table Register */
			    if (CPL != 0) goto not_permitted;
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    goto next_switch;
			case 3: /* LTR */ {
			    /* Load Task Register */
			    if (VM86F) goto illegal_op;  /* PM only */
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    mem_ref = MEM_REF;
			    temp = mFETCH_WORD(mem_ref);
/**/			    goto not_implemented;
			    }
			case 4: /* VERR */ {
			    int temp;
			    if (VM86F) goto illegal_op;  /* PM only */
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    temp = mFETCH_WORD(MEM_REF);
			    if (hsw_verr(temp)) SET_ZF;
				else CLEAR_ZF;
			    } goto next_switch;
			case 5: /* VERW */ {
			    int temp;
			    if (VM86F) goto illegal_op;  /* PM only */
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    temp = mFETCH_WORD(MEM_REF);
			    if (hsw_verw(temp)) SET_ZF;
				else CLEAR_ZF;
			    } goto next_switch;
			case 6: /* Illegal */
			case 7: /* Illegal */
			    goto illegal_op;
		    }
		case 0x01: /* GRP7 - Extended Opcode 21 */
		    switch ((*(PC+2)>>3)&7) {
			case 0: /* SGDT */ {
			    /* Store Global Descriptor Table Register */
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    mem_ref = MEM_REF;
			    temp = 0; /* should be LIMIT field */;
			    PUT_WORD(mem_ref,temp);
			    temp = 0; /* should be BASE field (4 bytes) */
			    PUT_QUAD(mem_ref,temp);
			    } goto next_switch;
			case 1: /* SIDT */ {
			    /* Store Interrupt Descriptor Table Register */
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    mem_ref = MEM_REF;
			    temp = 0; /* should be LIMIT field */;
			    PUT_WORD(mem_ref,temp);
			    temp = 0; /* should be BASE field (4 bytes) */
			    PUT_QUAD(mem_ref,temp);
			    } goto next_switch;
			case 2: /* LGDT */ /* Privileged */
			    /* Load Global Descriptor Table Register */
			    if (CPL != 0) goto not_permitted;
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
/**/			    goto not_implemented;
			case 3: /* LIDT */ /* Privileged */
			    /* Load Interrupt Descriptor Table Register */
			    if (CPL != 0) goto not_permitted;
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    goto next_switch;
			case 4: { /* SMSW, 80286 compatibility */
			    /* Store Machine Status Word */
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    mem_ref = MEM_REF;
			    temp = CRs[0]&0x3f;
			    mPUT_WORD(mem_ref,temp);
			    } goto next_switch;
			case 5: /* Illegal */
			    goto illegal_op;
			case 6: { /* LMSW, 80286 compatibility, Privileged */
			    /* Load Machine Status Word */
			    if (CPL != 0) goto not_permitted;
			    PC += 1; PC += hsw_modrm_16_word(MODARGS);
			    temp = mFETCH_WORD(MEM_REF);
			    CRs[0] = (CRs[0]&0xffffffd0) | (temp & 0x2f);
			    /* check RM/PM switch! */
			    } goto next_switch;
			case 7: /* Illegal */
			    goto illegal_op;
		    }
		case 0x02: /* LAR */ {
		    /* Load Access Rights Byte */
			WORD _sel;
		    if (VM86F) goto illegal_op;  /* PM only */
		    PC += 1; PC += hsw_modrm_16_word(MODARGS);
		    mem_ref = MEM_REF;
		    _sel = mFETCH_WORD(mem_ref);
		    if (IS_GDT(_sel)||((_sel>>3)>LDT_ENTRIES))
			CLEAR_ZF;
		else {
			temp = (GetSelectorFlags(_sel) & 0xff) << 8;
			if (temp) SetFlagAccessed(_sel);
			    SET_ZF;
			    *XREG1 = (WORD)temp;
		}
		    } goto next_switch;
		case 0x03: /* LSL */ {
		    /* Load Segment Limit */
			WORD _sel;
		    if (VM86F) goto illegal_op;  /* PM only */
		    PC += 1; PC += hsw_modrm_16_word(MODARGS);
		    mem_ref = MEM_REF;
		    _sel = mFETCH_WORD(mem_ref);
		if (IS_GDT(_sel)||((_sel>>3)>LDT_ENTRIES))
			CLEAR_ZF;
		else {
		temp= (WORD)GetSelectorByteLimit(_sel);
			*XREG1 = (WORD)temp;
			SET_ZF;
		}
		    /* what do I do here??? */
		    } goto next_switch;
/* case 0x04:	LOADALL */
/* case 0x05:	LOADALL(286) - SYSCALL(K6) */
		case 0x06: /* CLTS */ /* Privileged */
		    /* Clear Task State Register */
		    if (CPL != 0) goto not_permitted;
		    PC += 2; goto next_switch;
/* case 0x07:	LOADALL(386) - SYSRET(K6) etc. */
		case 0x08: /* INVD */
		    /* INValiDate cache */
		    PC += 2; goto next_switch;
		case 0x09: /* WBINVD */
		    /* Write-Back and INValiDate cache */
		    PC += 2; goto next_switch;
		case 0x0a: goto illegal_op;
		case 0x0b: goto illegal_op;	/* UD2 */
/* case 0x0d:	Code Extension 25(AMD-3D) */
/* case 0x0e:	FEMMS(K6-3D) */
		case 0x0f:	/* AMD-3D */
		    PC += 2; goto next_switch;
/* case 0x10-0x1f:	various V20/MMX instr. */
		case 0x20: /* MOVcdrd */ /* Privileged */
		case 0x22: { /* MOVrdcd */ /* Privileged */
		    DWORD *srg; int reg;
		    if (VM86F) goto not_permitted;
		    reg = (PC[2]>>3)&7;
		    if ((reg==1)||(reg>4)) goto illegal_op;
		    if ((PC[2]&0xc0)!=0xc0) goto illegal_op;
		    switch (PC[2]&7) {
			case 0: srg = &(EAX); break;
			case 1: srg = &(ECX); break;
			case 2: srg = &(EDX); break;
			case 3: srg = &(EBX); break;
			case 6: srg = &(ESI); break;
			case 7: srg = &(EDI); break;
			default: goto illegal_op;
		    }
		    if (PC[1]&2) {	/* write to CRs */
			if (reg==0) {
			    if ((CRs[0] ^ *srg) & 1) {
				/*dbug_printf("RM/PM switch not allowed\n");*/
				FatalAppExit(0, "PROT");
			    }
			    CRs[0] = (*srg&0xe005002f)|0x10;
			}
			else CRs[reg] = *srg;
		    } else {
			*srg = CRs[reg];
		    }
		    } PC += 3; goto next_switch;
		case 0x21: /* MOVddrd */ /* Privileged */
		case 0x23: { /* MOVrddd */ /* Privileged */
		    DWORD *srg; int reg;
		    if (VM86F) goto not_permitted;
		    reg = (PC[2]>>3)&7;
		    if ((PC[2]&0xc0)!=0xc0) goto illegal_op;
		    switch (PC[2]&7) {
			case 0: srg = &(EAX); break;
			case 1: srg = &(ECX); break;
			case 2: srg = &(EDX); break;
			case 3: srg = &(EBX); break;
			case 6: srg = &(ESI); break;
			case 7: srg = &(EDI); break;
			default: goto illegal_op;
		    }
		    if (PC[1]&2) {	/* write to DRs */
			DRs[reg] = *srg;
		    } else {
			*srg = DRs[reg];
		    }
		    } PC += 3; goto next_switch;
		case 0x24: /* MOVtdrd */ /* Privileged */
		case 0x26: { /* MOVrdtd */ /* Privileged */
		    DWORD *srg; int reg;
		    if (VM86F) goto not_permitted;
		    reg = (PC[2]>>3)&7;
		    if (reg < 6) goto illegal_op;
		    reg -= 6;
		    if ((PC[2]&0xc0)!=0xc0) goto illegal_op;
		    switch (PC[2]&7) {
			case 0: srg = &(EAX); break;
			case 1: srg = &(ECX); break;
			case 2: srg = &(EDX); break;
			case 3: srg = &(EBX); break;
			case 6: srg = &(ESI); break;
			case 7: srg = &(EDI); break;
			default: goto illegal_op;
		    }
		    if (PC[1]&2) {	/* write to TRs */
			TRs[reg] = *srg;
		    } else {
			*srg = TRs[reg];
		    }
		    } PC += 3; goto next_switch;
/* case 0x27-0x2f:	various V20/MMX instr. */
		case 0x30:	/* WRMSR */
                    goto not_implemented;
		case 0x31: /* RDTSC */
		    {
			EAX = (DWORD)EMUtime;
			EDX = (DWORD)(EMUtime>>32);
			PC += 2; goto next_switch;
		    }
		case 0x32:	/* RDMSR */
                    goto not_implemented;
/* case 0x33:	RDPMC(P6) */
/* case 0x34:	SYSENTER(PII) */
/* case 0x35:	SYSEXIT(PII) */
/* case 0x40-0x4f:	CMOV(P6) */
/* case 0x50-0x5f:	various Cyrix/MMX */
#ifdef __GNUC__
		case 0x60 ... 0x6b:	/* MMX */
		case 0x6e: case 0x6f:
		case 0x74 ... 0x77:
#else
		case 0x60:	/* MMX */
		case 0x61: case 0x62: case 0x63: case 0x64:
		case 0x65: case 0x66: case 0x67: case 0x68:
		case 0x69: case 0x6a: case 0x6b:
		case 0x6e: case 0x6f:
		case 0x74: case 0x75: case 0x76: case 0x77:
#endif
/* case 0x78-0x7e:	Cyrix */
		case 0x7e: case 0x7f:
		    PC += 2; goto next_switch;
		case 0x80: /* JOimmdisp */
#ifndef NO_TRACE_MSGS
		    iENV->flags &= ~OVERFLOW_FLAG;
#endif
		    if (IS_OF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
#ifndef NO_TRACE_MSGS
			iENV->flags |= OVERFLOW_FLAG;
#endif
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x81: /* JNOimmdisp */
#ifndef NO_TRACE_MSGS
		    iENV->flags |= OVERFLOW_FLAG;
#endif
		    if (!IS_OF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
#ifndef NO_TRACE_MSGS
			iENV->flags &= ~OVERFLOW_FLAG;
#endif
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x82: /* JBimmdisp */
		    if (IS_CF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x83: /* JNBimmdisp */
		    if (!IS_CF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x84: /* JZimmdisp */
		    if (IS_ZF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x85: /* JNZimmdisp */
		    if (!IS_ZF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x86: /* JBEimmdisp */
		    if (IS_CF_SET || IS_ZF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x87: /* JNBEimmdisp */
		    if (!(IS_CF_SET || IS_ZF_SET)) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x88: /* JSimmdisp */
		    if (IS_SF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x89: /* JNSimmdisp */
		    if (!IS_SF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x8a: /* JPimmdisp */
#ifndef NO_TRACE_MSGS
		    iENV->flags &= ~PARITY_FLAG;
#endif
		    if (IS_PF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
#ifndef NO_TRACE_MSGS
			iENV->flags |= PARITY_FLAG;
#endif
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x8b: /* JNPimmdisp */
#ifndef NO_TRACE_MSGS
		    iENV->flags |= PARITY_FLAG;
#endif
		    if (!IS_PF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
#ifndef NO_TRACE_MSGS
			iENV->flags &= ~PARITY_FLAG;
#endif
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x8c: /* JLimmdisp */
		    if (IS_SF_SET ^ IS_OF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x8d: /* JNLimmdisp */
		    if (!(IS_SF_SET ^ IS_OF_SET)) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x8e: /* JLEimmdisp */
		    if ((IS_SF_SET ^ IS_OF_SET) || IS_ZF_SET) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x8f: /* JNLEimmdisp */
		    if (!(IS_SF_SET ^ IS_OF_SET) && !(IS_ZF_SET)) {
			PC += (4 + (signed short)FETCH_WORD(PC+2));
			goto next_switch;
		    } PC += 4; goto next_switch;
		case 0x90: /* SETObrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (IS_OF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x91: /* SETNObrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (!IS_OF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x92: /* SETBbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (IS_CF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x93: /* SETNBbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (!IS_CF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x94: /* SETZbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (IS_ZF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x95: /* SETNZbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (!IS_ZF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x96: /* SETBEbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (IS_CF_SET || IS_ZF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x97: /* SETNBEbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (!(IS_CF_SET || IS_ZF_SET)) ? 1 : 0;
		    } goto next_switch;
		case 0x98: /* SETSbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (IS_SF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x99: /* SETNSbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (!(IS_SF_SET)) ? 1 : 0;
		    } goto next_switch;
		case 0x9a: /* SETPbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (IS_PF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x9b: /* SETNPbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (!IS_PF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x9c: /* SETLbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (IS_SF_SET ^ IS_OF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x9d: /* SETNLbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (!(IS_SF_SET ^ IS_OF_SET)) ? 1 : 0;
		    } goto next_switch;
		case 0x9e: /* SETLEbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = ((IS_SF_SET ^ IS_OF_SET) || IS_ZF_SET) ? 1 : 0;
		    } goto next_switch;
		case 0x9f: /* SETNLEbrm */ {
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    *MEM_REF = (!(IS_SF_SET ^ IS_OF_SET) && !(IS_ZF_SET)) ? 1 : 0;
		    } goto next_switch;
		case 0xa0: /* PUSHfs */ {
		    PUSHWORD(SHORT_FS_16);
		    } PC += 2; goto next_switch;
		case 0xa1: /* POPfs */ {
		    register WORD wtemp = TOS_WORD;
		    if ((*err = SET_SEGREG(LONG_FS,BIG_FS,MK_FS,wtemp))) {
			iVAR->error_addr=wtemp&0xfffc; return P0; }
		    POPWORD(wtemp);
		    SHORT_FS_32 = wtemp;
		    } PC += 2; goto next_switch;
		case 0xa2: /* CPUID */ {
		      if (EAX==0) {
			EAX = 1;
			EBX = 0x756e6547;
			ECX = 0x6c65746e;
			EDX = 0x49656e69;
		      }
		      else if (EAX==1) {
			EAX = 0x052c;
			EBX = ECX = 0;
			EDX = 0x1bf;
		      }
		    } PC += 2; goto next_switch;
                case 0xa3: /* BT */ {
		    DWORD ind1;
		    long ind2;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    mem_ref = MEM_REF; ind2 = *XREG1;
		    ind2 = ((ind2 << 16) >> 16);
                    if (IS_MODE_REG) {
			ind2 = (ind2 & 0xf);
			temp = FETCH_XREG(mem_ref);
			CARRY = ((int)temp >> ind2)&1;
                    } else {
			if(ind2 >= 0) {
			    ind1 = (ind2 >> 4) << 1;
                            mem_ref += ind1;
                            temp = FETCH_WORD(mem_ref);
			    ind2 = ind2 & 0xf;
			    CARRY = (WORD)((temp >> ind2) & 0x1);
                        } else {
			    ind2 = -ind2 - 1;
			    ind1 = ((ind2 >> 4) + 1) << 1;
                            mem_ref -= ind1;
                            temp = FETCH_WORD(mem_ref);
			    ind2 = ind2 & 0xf;
			    CARRY = (((temp << ind2) & 0x8000)? 1:0);
                        }
                    }          
		    } goto next_switch;
		case 0xa4: /* SHLDimm */
		    /* Double Precision Shift Left by IMMED */
		case 0xa5: /* SHLDcl */ {
		    /* Double Precision Shift Left by CL */
		    unsigned int count;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    if (op==0xa4)
			{ count = *PC & 0x1f; PC++; }
		    else
			count = CL & 0x1f;
		    if (count) {
			register DWORD temp; WORD carry;
			mem_ref = MEM_REF;
			/* CF <- [dest(MEM_REF)][ src(REG1) ]
			 * if C==0 no flag change
			 * if C==1 change only OF
			 * if C>1  OF undef,change CF
			 * C modulo 32 */
			/* combine into 32-bit int, low word=src high=dest */
			temp = mFETCH_WORD(mem_ref);
			if (count==1) SRC1_16=SRC2_16=temp;
			temp = (temp<<16) | *XREG1;
			carry = (temp >> (32-count)) & 1;
			temp = (temp << count) >> 16;
			mPUT_WORD(mem_ref, temp);
			RES_16 = temp;
			CARRY=carry;
		    }
		    } goto next_switch;
		case 0xa6:{ /* CMPXCHGb */
		    DWORD src1, src2;
		    int res;
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
            	    src2 = *HREG1; mem_ref = MEM_REF;
		    src1 = *mem_ref;
            	    res = src1 - AL;
		    if (res) AL = (BYTE)src1; else *mem_ref = (BYTE)src2;
		    src2 = AL; SETBFLAGS(1);
/**/		goto not_implemented;
                    }
		case 0xa7:{ /* CMPXCHGw */
		    int res;
		    DWORD src1, src2;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    src2 = *XREG1; mem_ref = MEM_REF; 
		    src1 = mFETCH_WORD(mem_ref);
		    res = src1 - AX;
		    if(res) AX = (WORD)src1;
		    else {
			    mPUT_WORD(mem_ref,src2);
			 }
		    src2 = AX; SETWFLAGS(1);
/**/		goto not_implemented;
		    }
		case 0xa8: /* PUSHgs */ {
		    PUSHWORD(SHORT_GS_16);
		    } PC += 2; goto next_switch;
		case 0xa9: /* POPgs */ {
		    register WORD wtemp = TOS_WORD;
		    if ((*err = SET_SEGREG(LONG_GS,BIG_GS,MK_GS,wtemp))) {
			iVAR->error_addr=wtemp&0xfffc; return P0; }
		    POPWORD(wtemp);
		    SHORT_GS_32 = wtemp;
		    } PC += 2; goto next_switch;
		case 0xaa:
		    goto illegal_op;
                case 0xab: /* BTS */ {
		    DWORD ind1;
		    long ind2; 
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    mem_ref = MEM_REF; ind2 = *XREG1;
		    ind2 = ((ind2 << 16) >> 16);
                    if (IS_MODE_REG) {
			ind2 = (ind2 & 0xf);
                        temp = *(WORD *)mem_ref;
			CARRY = ((int)temp >> ind2)&1;
			temp |= (0x1 << ind2);
			*(WORD *)mem_ref = (WORD)temp;
                    } else {  
			if(ind2 >= 0) {
			    ind1 = ((ind2 >> 4) << 1);
                            mem_ref += ind1;
                            temp = FETCH_WORD(mem_ref);
			    ind2 = ind2 & 0xf;
			    CARRY = (WORD)((temp >> ind2) & 0x1);
			    temp |= (0x1 << ind2);
                            PUT_WORD(mem_ref,temp);
                        } else {
			    ind2 = -ind2 - 1;
			    ind1 = ((ind2 >> 4) + 1) << 1;
                            mem_ref -= ind1;
                            temp = FETCH_WORD(mem_ref);
			    ind2 = ind2 & 0xf;
			    CARRY = (((temp << ind2) & 0x8000)? 1:0);
			    temp |= (0x8000 >> ind2);
                            PUT_WORD(mem_ref,temp);
                        }     
                    }
		    } goto next_switch;
	    case 0xac: /* SHRDimm */
		    /* Double Precision Shift Right by IMMED */
		case 0xad: /* SHRDcl */ {
		    /* Double Precision Shift Right by CL */
		    unsigned int count;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    if (op==0xac)
			{ count = *PC & 0x1f; PC++; }
		    else
			count = CL & 0x1f;
		    if (count) {
			register DWORD temp; WORD carry;
			mem_ref = MEM_REF;
			/* [ src(REG1) ][dest(MEM_REF)] -> CF
			 * if C==0 no flag change
			 * if C==1 change only OF
			 * if C>1  OF undef,change CF
			 * C modulo 32 */
			/* combine into 32-bit int, low word=dest high=src */
			temp = mFETCH_WORD(mem_ref);
			if (count==1) SRC1_16=SRC2_16=temp;
			temp |= (*XREG1 << 16);
			carry = (temp >> (count-1)) & 1;
			temp = (temp >> count);
			mPUT_WORD(mem_ref, temp);
			RES_16 = temp;
			CARRY=carry;
		    }
		    } goto next_switch;
/* case 0xae:	Code Extension 24(MMX) */
		case 0xaf: /* IMULregrm */ {
			int res, src1, src2;
			PC++; PC += hsw_modrm_16_word(MODARGS);
			src1 = (signed short)*XREG1;
			src2 = (signed short)mFETCH_WORD(MEM_REF);
                        res = src1 * src2;
			*XREG1 = (WORD)res;
			res >>= 15;	/* not 16! */
			if ((res==0)||(res==0xffffffff)) {  /* CF=OF=0 */
			    CARRY=0; SRC1_16 = SRC2_16 = RES_16; /* 000,111 */
			}
			else {
			    CARRY=1; SRC1_16 = SRC2_16 = ~RES_16;  /* 001,110 */
		 	}
                        } goto next_switch;
/* case 0xb0-0xb1:	CMPXCHG */
		case 0xb2: /* LSS */ {
		    register WORD wtemp;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    if (IS_MODE_REG) {
			/* Illegal */
			goto illegal_op;
		    }
		    mem_ref = MEM_REF;
		    wtemp = FETCH_WORD(mem_ref+2);
		    if ((*err = SET_SEGREG(LONG_SS,BIG_SS,MK_SS,wtemp))) {
			iVAR->error_addr=wtemp&0xfffc; return P0; }
		    *XREG1 = FETCH_WORD(mem_ref);
		    SHORT_SS_32 = wtemp;
		    } goto next_switch;
                case 0xb3: /* BTR */ {
		    DWORD ind1;
		    long ind2;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    mem_ref = MEM_REF; ind2 = *XREG1;
		    ind2 = ((ind2 << 16) >> 16);
                    if (IS_MODE_REG) {
			ind2 = (ind2 & 0xf);
                        temp = *(WORD *)mem_ref;
			CARRY = ((int)temp >> ind2)&1;
			temp &= ~(0x1 << ind2);
			*(WORD *)mem_ref = (WORD)temp;
                    } else {
			if(ind2 >= 0) {
			    ind1 = ((ind2 >> 4) << 1);
                            mem_ref += ind1;
                            temp = FETCH_WORD(mem_ref);
			    ind2 = ind2 & 0xf;
			    CARRY = (WORD)((temp >> ind2) & 0x1);
			    temp &= ~(0x1 << ind2);
                            PUT_WORD(mem_ref,temp);
                        } else {
			    ind2 = -ind2 - 1;
			    ind1 = ((ind2 >> 4) + 1) << 1;
                            mem_ref -= ind1;
                            temp = FETCH_WORD(mem_ref);
			    ind2 = ind2 & 0xf;
			    CARRY = (((temp << ind2) & 0x8000)? 1:0);
			    temp &= ~(0x8000 >> ind2);
                            PUT_WORD(mem_ref,temp);
                        }
                    }
		    } goto next_switch;
		case 0xb4: /* LFS */ {
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    if (IS_MODE_REG) { 	/* Illegal */
		goto illegal_op;
		    }
		    mem_ref = MEM_REF;
		    temp = FETCH_WORD(mem_ref+2);
		    if ((*err = SET_SEGREG(LONG_FS,BIG_FS,MK_FS,temp))) {
			iVAR->error_addr=temp&0xfffc; return P0; }
		    *XREG1 = FETCH_WORD(mem_ref);
		    SHORT_FS_32 = temp;
		    } goto next_switch;
		case 0xb5: /* LGS */ {
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    if (IS_MODE_REG) {	/* Illegal */
		goto illegal_op;
		    }
		    mem_ref = MEM_REF;
		    temp = FETCH_WORD(mem_ref+2);
		    if ((*err = SET_SEGREG(LONG_GS,BIG_GS,MK_GS,temp))) {
			iVAR->error_addr=temp&0xfffc; return P0; }
		    *XREG1 = FETCH_WORD(mem_ref);
		    SHORT_GS_32 = temp;
		    } goto next_switch;
		case 0xb6: /* MOVZXb */ {
		    register WORD wtemp;
		    int ref = (*(PC+2)>>3)&7;
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    wtemp = *(BYTE *)MEM_REF;
		    switch (ref) {
		      case 0: AX = wtemp; goto next_switch;
		      case 1: CX = wtemp; goto next_switch;
		      case 2: DX = wtemp; goto next_switch;
		      case 3: BX = wtemp; goto next_switch;
		      case 4: SP = wtemp; goto next_switch;
		      case 5: BP = wtemp; goto next_switch;
		      case 6: SI = wtemp; goto next_switch;
		      case 7: DI = wtemp; goto next_switch;
		    }
		    }
		case 0xb7: /* MOVZXw */ {
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    *XREG1 = mFETCH_WORD(MEM_REF);
		  } goto next_switch;
		case 0xb9: goto illegal_op;	/* UD2 */
		case 0xba: /* GRP8 - Code Extension 22 */
		    switch ((*(PC+2)>>3)&7) {
			case 0: /* Illegal */
			case 1: /* Illegal */
			case 2: /* Illegal */
			case 3: /* Illegal */
				goto illegal_op;
			case 4: /* BT */ {
			    int temp1;
			    PC++; PC += hsw_modrm_16_word(MODARGS);
                	    mem_ref = MEM_REF; temp = *PC;  PC++;
			    temp1 = mFETCH_WORD(mem_ref);
                            CARRY = ((int)temp1>>(int)( temp & 0xf))&1;
                    	    } goto next_switch;
			case 5: /* BTS */ {
			    int temp1;
			    PC++; PC += hsw_modrm_16_word(MODARGS);
                	    mem_ref = MEM_REF; temp = (*PC) & 0xf;  PC++;
			    temp1 = mFETCH_WORD(mem_ref);
                                CARRY = (temp1 >> temp)&1;
				temp1 |= (0x1 << temp);
			    mPUT_WORD(mem_ref,temp1);
                    	    } goto next_switch;
			case 6: /* BTR */ {
			    int temp1;
			    PC++; PC += hsw_modrm_16_word(MODARGS);
                	    mem_ref = MEM_REF; temp = (*PC) & 0xf;  PC++;
			    temp1 = mFETCH_WORD(mem_ref);
                                CARRY = (temp1 >> temp)&1;
				temp1 &= ~(0x1 << temp);
			    mPUT_WORD(mem_ref,temp1);
                    	    } goto next_switch;
			case 7: /* BTC */ {
			    int temp1;
			    PC++; PC += hsw_modrm_16_word(MODARGS);
                	    mem_ref = MEM_REF; temp = (*PC) & 0xf;  PC++;
			    temp1 = mFETCH_WORD(mem_ref);
                                CARRY = (temp1 >> temp)&1;
				temp1 ^= (0x1 << temp);
			    mPUT_WORD(mem_ref,temp1);
                    	    } goto next_switch;
			}
                case 0xbb: /* BTC */ {
		    DWORD ind1;
		    long ind2;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    mem_ref = MEM_REF; ind2 = *XREG1;
		    ind2 = ((ind2 << 16) >> 16);
                    if (IS_MODE_REG) {
			ind2 = (ind2 & 0xf);
                        temp = *(WORD *)mem_ref;
			CARRY = ((int)temp >> ind2)&1;
			temp ^= (0x1 << ind2);
			*(WORD *)mem_ref = (WORD)temp;
                    } else {
			if(ind2 >= 0) {
			    ind1 = ((ind2 >> 4) << 1);
                            mem_ref += ind1;
                            temp = FETCH_WORD(mem_ref);
			    ind2 = ind2 & 0xf;
			    CARRY = (WORD)((temp >> ind2) & 0x1);
			    temp ^= (0x1 << ind2);
                            PUT_WORD(mem_ref,temp);
                        } else {
			    ind2 = -ind2 - 1;
			    ind1 = ((ind2 >> 4) + 1) << 1;
                            mem_ref += ind1;
                            temp = FETCH_WORD(mem_ref);
			    ind2 = ind2 & 0xf;
			    CARRY = (((temp << ind2) & 0x8000)? 1:0);
			    temp ^= (0x8000 >> ind2);
                            PUT_WORD(mem_ref,temp);
                        }
                    }
		    } goto next_switch;
		case 0xbc: /* BSF */ {
		    int i;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    mem_ref = MEM_REF;
		    temp = mFETCH_WORD(mem_ref);
		    if(temp) {
		    for(i=0; i<16; i++)
			if((temp >> i) & 0x1) break ;
		    *XREG1 = i; RES_16 = 1;
		    } else RES_16 = 0;
		    } goto next_switch;
		case 0xbd: /* BSR */ {
		    int i;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
		    mem_ref = MEM_REF;
		    temp = mFETCH_WORD(mem_ref);
		    if(temp) {
		    for(i=15; i>=0; i--)
			if((temp & ( 0x1 << i))) break;
		    *XREG1 = i; RES_16 = 1;
		    } else RES_16 = 0;
		    } goto next_switch;
		case 0xbe: /* MOVSXb */ {
		    register signed long stemp;
		    int ref = (*(PC+2)>>3)&7;
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
		    stemp = *(signed char *)MEM_REF;
		    switch (ref) {
		      case 0: AX = (WORD)stemp; goto next_switch; /* check asm! */
		      case 1: CX = (WORD)stemp; goto next_switch;
		      case 2: DX = (WORD)stemp; goto next_switch;
		      case 3: BX = (WORD)stemp; goto next_switch;
		      case 4: SP = (WORD)stemp; goto next_switch;
		      case 5: BP = (WORD)stemp; goto next_switch;
		      case 6: SI = (WORD)stemp; goto next_switch;
		      case 7: DI = (WORD)stemp; goto next_switch;
		    }
		    }
		case 0xbf: { /* MOVSXw */
		    register signed long stemp;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
                    mem_ref = MEM_REF;
		    stemp = mFETCH_WORD(mem_ref);
		    stemp = ((stemp<<16)>>16);
		    *XREG1 = (WORD)stemp;
		    } goto next_switch;
                case 0xc0: { /* XADDb */
		    int res,src1,src2;
		    PC++; PC += hsw_modrm_16_byte(MODARGS);
                    src2 = *HREG1; mem_ref = MEM_REF; src1 = *mem_ref;
                    *HREG1 = src1;
                    *mem_ref = res = src1 + src2;
		    SETBFLAGS(0);
                    } goto next_switch;
                case 0xc1: { /* XADDw */
		    int res,src1,src2;
		    PC++; PC += hsw_modrm_16_word(MODARGS);
                    src2 = *XREG1; mem_ref = MEM_REF;
		    src1 = mFETCH_WORD(mem_ref);
                        res = src1 + src2;
		    mPUT_WORD(mem_ref, res);
                    *XREG1 = src1;
		    SETWFLAGS(0);
                    } goto next_switch;
/* case 0xc2-0xc6:	MMX */
/* case 0xc7:	Code Extension 23 - 01=CMPXCHG8B mem */
                case 0xc8: /* BSWAPeax */ {
		    temp = EAX;
                    EAX = (temp << 24) | (temp >> 24) |
                        ((temp << 8) & 0xff0000) | ((temp >> 8)& 0xff00);
		    } PC += 2; goto next_switch;
		case 0xc9: /* BSWAPecx */ {
		    temp = ECX;
                    ECX = (temp << 24) | (temp >> 24) |
                        ((temp << 8) & 0xff0000) | ((temp >> 8)& 0xff00);
		    } PC += 2; goto next_switch;
                case 0xca: /* BSWAPedx */ {
		    temp = EDX;
                    EDX = (temp << 24) | (temp >> 24) |
                        ((temp << 8) & 0xff0000) | ((temp >> 8)& 0xff00);
		    } PC += 2; goto next_switch;
		case 0xcb: /* BSWAPebx */ {
		    temp = EBX;
                    EBX = (temp << 24) | (temp >> 24) |
                        ((temp << 8) & 0xff0000) | ((temp >> 8)& 0xff00);
		    } PC += 2; goto next_switch;
                case 0xcc: /* BSWAPesp */ {
		    temp = ESP;
                    ESP = (temp << 24) | (temp >> 24) |
                        ((temp << 8) & 0xff0000) | ((temp >> 8)& 0xff00);
		    } PC += 2; goto next_switch;
                case 0xcd: /* BSWAPebp */ {
		    temp = EBP;
                    EBP = (temp << 24) | (temp >> 24) |
                        ((temp << 8) & 0xff0000) | ((temp >> 8)& 0xff00);
		    } PC += 2; goto next_switch;
                case 0xce: /* BSWAPesi */ {
		    temp = ESI;
                    ESI = (temp << 24) | (temp >> 24) |
                        ((temp << 8) & 0xff0000) | ((temp >> 8)& 0xff00);
		    } PC += 2; goto next_switch;
                case 0xcf: /* BSWAPedi */ {
		    temp = EDI;
                    EDI = (temp << 24) | (temp >> 24) |
                        ((temp << 8) & 0xff0000) | ((temp >> 8)& 0xff00);
		    } PC += 2; goto next_switch;
#ifdef __GNUC__
		case 0xd1 ... 0xd3:	/* MMX */
		case 0xd5: case 0xd7: case 0xda: case 0xde:
		case 0xdf:
		case 0xe0 ... 0xe5:
		case 0xf1 ... 0xf3:
		case 0xf5 ... 0xf7:
		case 0xfc ... 0xfe:
#else
		case 0xd1:	/* MMX */
		case 0xd2: case 0xd3:
		case 0xd5: case 0xd7: case 0xda: case 0xde:
		case 0xdf:
		case 0xe0: case 0xe1: case 0xe2: case 0xe3:
		case 0xe4: case 0xe5:
		case 0xf1: case 0xf2: case 0xf3:
		case 0xf5: case 0xf6: case 0xf7:
		case 0xfc: case 0xfd: case 0xfe:
#endif
			PC += 2; goto next_switch;
		case 0xff:	/* WinOS2 WANTS an int06 */
#ifdef DOSEMU
			*err=EXCP06_ILLOP; return P0;
#endif
		default: goto illegal_op;
		}
	    }

/*10*/	case ADCbfrm: {
	    int res, src1, src2;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *HREG1; mem_ref = MEM_REF; src1 = *mem_ref;
	    *mem_ref = res = src1 + src2 + (CARRY & 1);
	    SETBFLAGS(0);
	    } goto next_switch; 
/*11*/	case ADCwfrm: {
	    int res, src1, src2;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = *XREG1; mem_ref = MEM_REF; 
	    src1 = mFETCH_WORD(mem_ref);
	    res = src1 + src2 + (CARRY & 1);
	    mPUT_WORD(mem_ref, res);
	    SETWFLAGS(0);
	    } goto next_switch; 
/*12*/	case ADCbtrm: {
	    int res, src1, src2;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *MEM_REF; mem_ref = HREG1; src1 = *mem_ref;
	    *mem_ref = res = src1 + src2 + (CARRY & 1);
	    SETBFLAGS(0);
	    } goto next_switch; 
/*13*/	case ADCwtrm: {
	    int res, src1, src2;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = *XREG1; mem_ref = MEM_REF; 
	    src1 = mFETCH_WORD(mem_ref);
	    *XREG1 = res = src1 + src2 + (CARRY & 1);
	    SETWFLAGS(0);
	    } goto next_switch; 
/*14*/	case ADCbi: {
	    int res, src1, src2;
	    src1 = AL; 
	    src2 = *(PC+1);
	    res = src1 + src2 + (CARRY & 1);
	    AL = (BYTE)res;
	    SETBFLAGS(0);
	    } PC += 2; goto next_switch;
/*15*/	case ADCwi: {
	    int res, src1, src2;
	    src1 = AX; 
	    src2 = (FETCH_WORD((PC+1)));
	    res = src1 + src2 + (CARRY & 1);
	    AX = (WORD)res;
	    SETWFLAGS(0);
	    } PC += 3; goto next_switch;
/*16*/	case PUSHss: {
	    PUSHWORD(SHORT_SS_16);
	    } PC += 1; goto next_switch;
/*17*/	case POPss: {
	    register WORD wtemp;
	    POPWORD(wtemp);
	    if ((*err = SET_SEGREG(LONG_SS,BIG_SS,MK_SS,wtemp))) {
		PUSHWORD(wtemp); iVAR->error_addr=wtemp&0xfffc; return P0; }
	    CEmuStat |= CeS_LOCK;
	    SHORT_SS_32 = wtemp;
	    } PC += 1; goto next_switch;
/*18*/	case SBBbfrm: {
	    int res, src1, src2;
	    int carry = CARRY & 1;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *HREG1; mem_ref = MEM_REF; src1 = *mem_ref;
	    *mem_ref = res = src1 - src2 - carry;
	    SETBFLAGS(1);
	    } goto next_switch; 
/*19*/	case SBBwfrm: {
	    int res, src1, src2;
	    int carry = CARRY & 1;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = *XREG1; mem_ref = MEM_REF; 
	    src1 = mFETCH_WORD(mem_ref);
	    res = src1 - src2 - carry;
	    mPUT_WORD(mem_ref, res);
	    SETWFLAGS(1);
	    } goto next_switch; 
/*1a*/	case SBBbtrm: {
	    int res, src1, src2;
	    int carry = CARRY & 1;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *MEM_REF; mem_ref = HREG1; src1 = *mem_ref;
	    *mem_ref = res = src1 - src2 - carry;
	    SETBFLAGS(1);
	    } goto next_switch; 
/*1b*/	case SBBwtrm: {
	    int res, src1, src2;
	    int carry = CARRY & 1;
	    PC += hsw_modrm_16_word(MODARGS);
	    src1 = *XREG1; mem_ref = MEM_REF; 
	    src2 = mFETCH_WORD(mem_ref);
	    *XREG1 = res = src1 - src2 - carry;
	    SETWFLAGS(1);
	    } goto next_switch; 
/*1c*/	case SBBbi: {
	    int res, src1, src2;
	    int carry = CARRY & 1;
	    src1 = AL; 
	    src2 = *(PC+1);
	    AL = res = src1 - src2 - carry;
	    SETBFLAGS(1);
	    } PC += 2; goto next_switch;
/*1d*/	case SBBwi: {
	    int res, src1, src2;
	    int carry = CARRY & 1;
	    src1 = AX; 
	    src2 = (FETCH_WORD((PC+1)));
	    res = src1 - src2 - carry;
	    AX = (WORD)res;
	    SETWFLAGS(1);
	    } PC += 3; goto next_switch;
/*1e*/	case PUSHds: {
	    PUSHWORD(SHORT_DS_16);
	    } PC += 1; goto next_switch;
/*1f*/	case POPds: {
	    register WORD wtemp = TOS_WORD;
	    if ((*err = SET_SEGREG(LONG_DS,BIG_DS,MK_DS,wtemp))) {
		iVAR->error_addr=wtemp&0xfffc; return P0; }
	    POPWORD(wtemp);
	    SHORT_DS_32 = wtemp;
	    } PC += 1; goto next_switch;

/*20*/	case ANDbfrm: {
	    DWORD res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    mem_ref = MEM_REF;
	    res = *mem_ref & *HREG1;
	    *mem_ref = (BYTE)res;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } goto next_switch; 
/*21*/	case ANDwfrm: {
	    DWORD res;
	    PC += hsw_modrm_16_word(MODARGS);
	    mem_ref = MEM_REF; 
	    res = mFETCH_WORD(mem_ref) & *XREG1;
	    mPUT_WORD(mem_ref, res);
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } goto next_switch; 
/*22*/	case ANDbtrm: {
	    DWORD res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    res = *HREG1 & *MEM_REF;
	    *HREG1 = (BYTE)res;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } goto next_switch; 
/*23*/	case ANDwtrm: {
	    DWORD res;
	    PC += hsw_modrm_16_word(MODARGS);
	    res = mFETCH_WORD(MEM_REF) & *XREG1;
	    *XREG1 = (WORD)res;
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } goto next_switch; 
/*24*/	case ANDbi: {
	    DWORD res;
	    res = AL & PC[1];
	    AL = (BYTE)res;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } PC += 2; goto next_switch;
/*25*/	case ANDwi: {
	    DWORD res;
	    res = AX & FETCH_WORD((PC+1));
	    AX = (WORD)res;
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } PC += 3; goto next_switch;
/*26*/	case SEGes:
	    if (!VM86F && (SHORT_ES_16 < 4 || LONG_ES==INVALID_OVR)) {
		error("General Protection Fault: zero ES\n");
		goto bad_override;
	    }
	    OVERR_DS = OVERR_SS = LONG_ES;
	    PC+=1; goto override;
/*27*/	case DAA:
#ifndef NO_TRACE_MSGS
	    iENV->flags &= ~AUX_CARRY_FLAG;
#endif
	    if (((AL & 0x0f) > 9 ) || (IS_AF_SET)) {
		AL += 6;
#ifndef NO_TRACE_MSGS
		iENV->flags |= AUX_CARRY_FLAG;
#endif
		SET_AF;
	    } else CLEAR_AF;
	    if ((AL > 0x9f) || (IS_CF_SET)) {
		AL += 0x60;
		SET_CF;
	    } /*else CLEAR_CF;*/
	    RES_8 = AL;
	    BYTE_FLAG = BYTE_OP;
	    PC += 1; goto next_switch;
/*28*/	case SUBbfrm: {		/* r/m8 -= r8 */
	    DWORD src1, src2; int res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *HREG1; mem_ref = MEM_REF; src1 = *mem_ref;
	    *mem_ref = res = src1 - src2;
	    SETBFLAGS(1);
	    } goto next_switch; 
/*29*/	case SUBwfrm: {		/* r/m16 -= r16 */
	    int res, src1, src2;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = *XREG1; mem_ref = MEM_REF; 
	    src1 = mFETCH_WORD(mem_ref);
		res = src1 - src2;
	    mPUT_WORD(mem_ref, res);
	    SETWFLAGS(1);
	    } goto next_switch; 
/*2a*/	case SUBbtrm: {		/* r8 -= r/m8 */
	    DWORD src1, src2; int res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *MEM_REF; mem_ref = HREG1; src1 = *mem_ref;
	    *mem_ref = res = src1 - src2;
	    SETBFLAGS(1);
	    } goto next_switch; 
/*2b*/	case SUBwtrm: {		/* r16 -= r/m16 */
	    int res, src1, src2;
	    PC += hsw_modrm_16_word(MODARGS);
	    src1 = *XREG1; mem_ref = MEM_REF; 
	    src2 = mFETCH_WORD(mem_ref);
	    *XREG1 = res = src1 - src2;
	    SETWFLAGS(1);
	    } goto next_switch; 
/*2c*/	case SUBbi: {		/* sub al,imm8 */
	    DWORD src1, src2; int res;
	    src1 = AL; src2 = *(PC+1);
	    AL = res = src1 - src2;
	    SETBFLAGS(1);
	    } PC += 2; goto next_switch;
/*2d*/	case SUBwi: {		/* sub ax,imm16 */
	    DWORD res, src2; WORD src1;
	    src1 = AX; 
	    src2 = FETCH_WORD((PC+1));
	    res = src1 - src2;
	    AX = (WORD)res;
	    SETWFLAGS(1);
	    } PC += 3; goto next_switch;
/*2e*/	case SEGcs:		/* CS is already checked */
	    OVERR_DS = OVERR_SS = LONG_CS;
	    PC+=1; goto override;
/*2f*/	case DAS: {
	    BYTE carry = 0;
	    BYTE altmp = AL;
#ifndef NO_TRACE_MSGS
	    iENV->flags &= ~AUX_CARRY_FLAG;
#endif
	    if (((altmp & 0x0f) > 9) || (IS_AF_SET)) {
		AL -= 6;
		carry = CARRYB || (AL < 6);
		SET_AF;
#ifndef NO_TRACE_MSGS
		iENV->flags |= AUX_CARRY_FLAG;
#endif
	    } else CLEAR_AF;
	    if ((altmp > 0x99) || (IS_CF_SET)) {
		AL -= 0x60;
		carry = 1;
	    }
            RES_8 = AL;
	    CARRYB = carry;
            BYTE_FLAG = BYTE_OP;
	    } PC += 1; goto next_switch;

/*30*/	case XORbfrm: {
	    DWORD res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    mem_ref = MEM_REF;
	    res = *mem_ref ^ *HREG1;
	    *mem_ref = (BYTE)res;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } goto next_switch; 
/*31*/	case XORwfrm: {
	    DWORD res;
	    PC += hsw_modrm_16_word(MODARGS);
	    mem_ref = MEM_REF; 
	    res = mFETCH_WORD(mem_ref) ^ *XREG1;
	    mPUT_WORD(mem_ref, res);
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } goto next_switch; 
/*32*/	case XORbtrm: {
	    DWORD res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    res = *MEM_REF ^ *HREG1;
	    *HREG1 = (BYTE)res;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } goto next_switch; 
/*33*/	case XORwtrm: {
	    DWORD res;
	    PC += hsw_modrm_16_word(MODARGS);
	    res = mFETCH_WORD(MEM_REF) ^ *XREG1;
	    *XREG1 = (WORD)res;
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } goto next_switch; 
/*34*/	case XORbi: {
	    DWORD res;
	    res = AL ^ PC[1];
	    AL = (BYTE)res;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } PC += 2; goto next_switch;
/*35*/	case XORwi: {
	    DWORD res;
	    res = AX ^ FETCH_WORD((PC+1));
	    AX = (WORD)res;
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } PC += 3; goto next_switch;
/*36*/	case SEGss:		/* SS is already checked */
	    OVERR_DS = OVERR_SS = LONG_SS;
	    PC+=1; goto override;
/*37*/	case AAA: {
	    BYTE icarry = (AL > 0xf9);
#ifndef NO_TRACE_MSGS
	    iENV->flags &= ~AUX_CARRY_FLAG;
#endif
	    if (((AL & 0x0f) > 9 ) || (IS_AF_SET)) {
		AL = (AL + 6) & 0x0f;
		AH = (AH + 1 + icarry);
		SET_CF;
#ifndef NO_TRACE_MSGS
		iENV->flags |= AUX_CARRY_FLAG;
#endif
		SET_AF;
	    } else {
		CLEAR_CF;
		CLEAR_AF;
		AL &= 0x0f;
	    }
	    } PC += 1; goto next_switch;
/*38*/	case CMPbfrm: {
	    DWORD src1, src2; int res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *HREG1; src1 = *MEM_REF;
	    res = src1 - src2;
	    SETBFLAGS(1);
	    } goto next_switch; 
/*39*/	case CMPwfrm: {
	    int res, src1, src2;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = *XREG1; mem_ref = MEM_REF; 
	    src1 = mFETCH_WORD(mem_ref);
	    res = src1 - src2;
	    SETWFLAGS(1);
	    } goto next_switch; 
/*3a*/	case CMPbtrm: {
	    DWORD src1, src2; int res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *MEM_REF; src1 = *HREG1;
	    res = src1 - src2;
	    SETBFLAGS(1);
	    } goto next_switch; 
/*3b*/	case CMPwtrm: {
	    int res, src1, src2;
	    PC += hsw_modrm_16_word(MODARGS);
	    src1 = *XREG1; mem_ref = MEM_REF; 
	    src2 = mFETCH_WORD(mem_ref);
	    res = src1 - src2;
	    SETWFLAGS(1);
	    } goto next_switch; 
/*3c*/	case CMPbi: {
	    int src1, src2, res;
	    src1 = AL; src2 = *(PC+1);
	    res = src1 - src2;
	    SETBFLAGS(1);
	    } PC += 2; goto next_switch;
/*3d*/	case CMPwi: {
	    DWORD res, src2; WORD src1;
	    src1 = AX; 
	    src2 = FETCH_WORD((PC+1));
	    res = src1 - src2;
	    SETWFLAGS(1);
	    } PC += 3; goto next_switch;
/*3e*/	case SEGds:
	    if (!VM86F && (SHORT_DS_16 < 4 || LONG_DS==INVALID_OVR)) {
		error("General Protection Fault: zero DS\n");
		goto bad_override;
	    }
	    OVERR_DS = OVERR_SS = LONG_DS;
	    PC+=1; goto override;
/*3f*/	case AAS: {
	    BYTE icarry = (AL < 6);
#ifndef NO_TRACE_MSGS
	    iENV->flags &= ~AUX_CARRY_FLAG;
#endif
	    if (((AL & 0x0f) > 9 ) || (IS_AF_SET)) {
		AL = (AL - 6) & 0x0f;
		AH = (AH - 1 - icarry);
		SET_CF;
#ifndef NO_TRACE_MSGS
		iENV->flags |= AUX_CARRY_FLAG;
#endif
		SET_AF;
	    } else {
		CLEAR_CF;
		CLEAR_AF;
		AL &= 0x0f;
	    }
	    } PC += 1; goto next_switch;

/*40*/	case INCax: {
	    register int temp = EAX;
	    SRC1_16 = temp; SRC2_16 = 1;
	    RES_16 = AX = temp + 1;
	    } PC += 1; goto next_switch;
/*41*/	case INCcx: {
	    register int temp = ECX;
	    SRC1_16 = temp; SRC2_16 = 1;
	    RES_16 = CX = temp + 1;
	    } PC += 1; goto next_switch;
/*42*/	case INCdx: {
	    register int temp = EDX;
	    SRC1_16 = temp; SRC2_16 = 1;
	    RES_16 = DX = temp + 1;
	    } PC += 1; goto next_switch;
/*43*/	case INCbx: {
	    register int temp = EBX;
	    SRC1_16 = temp; SRC2_16 = 1;
	    RES_16 = BX = temp + 1;
	    } PC += 1; goto next_switch;
/*44*/	case INCsp: {
	    register int temp = ESP;
	    SRC1_16 = temp; SRC2_16 = 1;
	    RES_16 = SP = temp + 1;
	    } PC += 1; goto next_switch;
/*45*/	case INCbp: {
	    register int temp = EBP;
	    SRC1_16 = temp; SRC2_16 = 1;
	    RES_16 = BP = temp + 1;
	    } PC += 1; goto next_switch;
/*46*/	case INCsi: {
	    register int temp = ESI;
	    SRC1_16 = temp; SRC2_16 = 1;
	    RES_16 = SI = temp + 1;
	    } PC += 1; goto next_switch;
/*47*/	case INCdi: {
	    register int temp = EDI;
	    SRC1_16 = temp; SRC2_16 = 1;
	    RES_16 = DI = temp + 1;
	    } PC += 1; goto next_switch;
/*48*/	case DECax: {
	    register int temp = EAX;
	    SRC1_16 = temp; SRC2_16 = -1;
	    RES_16 = AX = temp - 1;
	    } PC += 1; goto next_switch;
/*49*/	case DECcx: {
	    register int temp = ECX;
	    SRC1_16 = temp; SRC2_16 = -1;
	    RES_16 = CX = temp - 1;
	    } PC += 1; goto next_switch;
/*4a*/	case DECdx: {
	    register int temp = EDX;
	    SRC1_16 = temp; SRC2_16 = -1;
	    RES_16 = DX = temp - 1;
	    } PC += 1; goto next_switch;
/*4b*/	case DECbx: {
	    register int temp = EBX;
	    SRC1_16 = temp; SRC2_16 = -1;
	    RES_16 = BX = temp - 1;
	    } PC += 1; goto next_switch;
/*4c*/	case DECsp: {
	    register int temp = ESP;
	    SRC1_16 = temp; SRC2_16 = -1;
	    RES_16 = SP = temp - 1;
	    } PC += 1; goto next_switch;
/*4d*/	case DECbp: {
	    register int temp = EBP;
	    SRC1_16 = temp; SRC2_16 = -1;
	    RES_16 = BP = temp - 1;
	    } PC += 1; goto next_switch;
/*4e*/	case DECsi: {
	    register int temp = ESI;
	    SRC1_16 = temp; SRC2_16 = -1;
	    RES_16 = SI = temp - 1;
	    } PC += 1; goto next_switch;
/*4f*/	case DECdi: {
	    register int temp = EDI;
	    SRC1_16 = temp; SRC2_16 = -1;
	    RES_16 = DI = temp - 1;
	    } PC += 1; goto next_switch;

/*50*/	case PUSHax: {
	    PUSHWORD(AX);
	    } PC += 1; goto next_switch;
/*51*/	case PUSHcx: {
	    PUSHWORD(CX);
	    } PC += 1; goto next_switch;
/*52*/	case PUSHdx: {
	    PUSHWORD(DX);
	    } PC += 1; goto next_switch;
/*53*/	case PUSHbx: {
	    PUSHWORD(BX);
	    } PC += 1; goto next_switch;
/*54*/	case PUSHsp: {
	    register WORD wtemp = SP;
	    PUSHWORD(wtemp);
	    } PC += 1; goto next_switch;
/*55*/	case PUSHbp: {
	    PUSHWORD(BP);
	    } PC += 1; goto next_switch;
/*56*/	case PUSHsi: {
	    PUSHWORD(SI);
	    } PC += 1; goto next_switch;
/*57*/	case PUSHdi: {
	    PUSHWORD(DI);
	    } PC += 1; goto next_switch;
/*58*/	case POPax: POPWORD(AX); PC += 1; goto next_switch;
/*59*/	case POPcx: POPWORD(CX); PC += 1; goto next_switch;
/*5a*/	case POPdx: POPWORD(DX); PC += 1; goto next_switch;
/*5b*/	case POPbx: POPWORD(BX); PC += 1; goto next_switch;
/*5c*/	case POPsp: {
	    register WORD wtemp;
	    POPWORD(wtemp);
	    SP = wtemp;
	    } PC += 1; goto next_switch;
/*5d*/	case POPbp: POPWORD(BP); PC += 1; goto next_switch;
/*5e*/	case POPsi: POPWORD(SI); PC += 1; goto next_switch;
/*5f*/	case POPdi: POPWORD(DI); PC += 1; goto next_switch;

/*60*/	case PUSHA: {
	    register BYTE *sp = LONG_SS+(BIG_SS? ESP:SP);
	    sp -= 16;
	    PUT_WORD(sp, DI);
	    PUT_WORD(sp+2, SI);
	    PUT_WORD(sp+4, BP);
	    PUT_WORD(sp+6, SP);
	    PUT_WORD(sp+8, BX);
	    PUT_WORD(sp+10, DX);
	    PUT_WORD(sp+12, CX);
	    PUT_WORD(sp+14, AX);
	    if (BIG_SS) ESP=sp-LONG_SS; else SP=sp-LONG_SS;
	    } PC += 1; goto next_switch;
/*61*/	case POPA: {
	    register BYTE *sp = LONG_SS+(BIG_SS? ESP:SP);
	    DI = FETCH_WORD(sp);
	    SI = FETCH_WORD(sp+2);
	    BP = FETCH_WORD(sp+4);
	    BX = FETCH_WORD(sp+8);	/* discard SP */
	    DX = FETCH_WORD(sp+10);
	    CX = FETCH_WORD(sp+12);
	    AX = FETCH_WORD(sp+14);
	    sp += 16;
	    if (BIG_SS) ESP=sp-LONG_SS; else SP=sp-LONG_SS;
	    } PC += 1; goto next_switch;
/*62*/	case BOUND: 
	    goto not_implemented;
/*63*/	case ARPL:
	    if (VM86F) goto illegal_op;  /* PM only */
	    goto not_implemented;
/*64*/	case SEGfs:
	    if (!VM86F && (SHORT_FS_16 < 4 || LONG_FS==INVALID_OVR)) {
		error("General Protection Fault: zero FS\n");
		goto bad_override;
	    }
	    OVERR_DS = OVERR_SS = LONG_FS;
	    PC+=1; goto override;
/*65*/	case SEGgs:
	    if (!VM86F && (SHORT_GS_16 < 4 || LONG_GS==INVALID_OVR)) {
		error("General Protection Fault: zero GS\n");
		goto bad_override;
	    }
	    OVERR_DS = OVERR_SS = LONG_GS;
	    PC+=1; goto override;
/*66*/	case OPERoverride:	/* 0x66: 32 bit operand, 16 bit addressing */
	    if (data32) goto bad_override;
	    if (d_emu>4) e_printf0("ENTER interp_32d_16a\n");
	    data32 = 1;
	    if (PC[1]==0x67) {
		code32 = 1;
		PC = hsw_interp_32_32 (ENVARGS, P0, PC+2, err, 1);
		code32 = 0;
	    }
	    else 
		PC = hsw_interp_32_16 (ENVARGS, P0, PC+1, err);
	    data32 = 0;
	    if (*err) return (*err==EXCP_GOBACK? PC:P0);
	    goto next_switch;
/*67*/	case ADDRoverride:	/* 0x67: 16 bit operand, 32 bit addressing */
	    if (code32) goto bad_override;
	    if (d_emu>4) e_printf0("ENTER interp_16d_32a\n");
	    code32 = 1;
	    if (PC[1]==0x66) {
		data32 = 1;
		PC = hsw_interp_32_32 (ENVARGS, P0, PC+2, err, 1);
		data32 = 0;
	    }
	    else
		PC = hsw_interp_16_32 (ENVARGS, P0, PC+1, err);
	    code32 = 0;
	    if (*err) return (*err==EXCP_GOBACK? PC:P0);
	    goto next_switch;
/*68*/	case PUSHwi: {
	    register WORD wtemp = FETCH_WORD(PC+1);
	    PUSHWORD(wtemp);
	    } PC += 3; goto next_switch;
/*69*/	case IMULwrm: {
	    int res, src1, src2;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = (signed short)FETCH_WORD(PC); PC += 2; /* imm16 */
	    src1 = (signed short)mFETCH_WORD(MEM_REF);    /* r/m16 */
	    res = src1 * src2;
	    *XREG1 = (WORD)res;
	    if ((res >= -32768) && (res <= 32767)) {  /* CF=OF=0 */
		CARRY=0; SRC1_16 = SRC2_16 = RES_16; /* 000,111 */
	    }
	    else {
		CARRY=1; SRC1_16 = SRC2_16 = ~RES_16;  /* 001,110 */
	    }
	    } goto next_switch; 
/*6a*/	case PUSHbi: {
	    register short temp = (signed char)*(PC+1);
	    PUSHWORD(temp);
	    } PC +=2; goto next_switch;
/*6b*/	case IMULbrm: {
	    int res, src1, src2;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = *(signed char *)(PC); PC += 1;	  /* imm8 */
	    src1 = (signed short)mFETCH_WORD(MEM_REF);	  /* r/m16 */
	    res = src1 * src2;
	    *XREG1 = (WORD)res;
	    if ((res >= -32768) && (res <= 32767)) {  /* CF=OF=0 */
		CARRY=0; SRC1_16 = SRC2_16 = RES_16; /* 000,111 */
	    }
	    else {
		CARRY=1; SRC1_16 = SRC2_16 = ~RES_16;  /* 001,110 */
	    }
	    } goto next_switch; 

/*70*/	case JO:
#ifndef NO_TRACE_MSGS
	    iENV->flags &= ~OVERFLOW_FLAG;
#endif
	    if (IS_OF_SET) {
#ifndef NO_TRACE_MSGS
	    iENV->flags |= OVERFLOW_FLAG;
#endif
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*71*/	case JNO:
#ifndef NO_TRACE_MSGS
	    iENV->flags |= OVERFLOW_FLAG;
#endif
	    if (!IS_OF_SET) {
#ifndef NO_TRACE_MSGS
	    iENV->flags &= ~OVERFLOW_FLAG;
#endif
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*72*/	case JB_JNAE: if (IS_CF_SET) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*73*/	case JNB_JAE: if (!IS_CF_SET) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*74*/	case JE_JZ: if (IS_ZF_SET) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*75*/	case JNE_JNZ: if (!IS_ZF_SET) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*76*/	case JBE_JNA: if (IS_CF_SET || IS_ZF_SET) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*77*/	case JNBE_JA: if (!(IS_CF_SET || IS_ZF_SET)) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*78*/	case JS: if (IS_SF_SET) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*79*/	case JNS: if (!(IS_SF_SET)) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*7a*/	case JP_JPE:
#ifndef NO_TRACE_MSGS
	    iENV->flags &= ~PARITY_FLAG;
#endif
	    if (IS_PF_SET) {
#ifndef NO_TRACE_MSGS
	    iENV->flags |= PARITY_FLAG;
#endif
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*7b*/	case JNP_JPO:
#ifndef NO_TRACE_MSGS
	    iENV->flags |= PARITY_FLAG;
#endif
	    if (!IS_PF_SET) {
#ifndef NO_TRACE_MSGS
	    iENV->flags &= ~PARITY_FLAG;
#endif
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*7c*/	case JL_JNGE: if (IS_SF_SET ^ IS_OF_SET) {	/* SF!=OF */
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*7d*/	case JNL_JGE: if (!(IS_SF_SET ^ IS_OF_SET)) {	/* SF==OF */
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*7e*/	case JLE_JNG: if ((IS_SF_SET ^ IS_OF_SET) || IS_ZF_SET) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;
/*7f*/	case JNLE_JG: if (!(IS_SF_SET ^ IS_OF_SET) && !(IS_ZF_SET)) {
	    JUMP(PC+1); goto fast_switch; 
	    } PC+=2; goto fast_switch;

/*82*/	case IMMEDbrm2:    /* out of order */
/*80*/	case IMMEDbrm: {
	    int src1, src2, res;
	    res = (*(PC+1)>>3)& 0x7;
	    PC += hsw_modrm_16_byte(MODARGS);
	    src2 = *PC; PC += 1;
	    mem_ref = MEM_REF; src1 = *mem_ref;
	    switch (res) {
		case 0: /* ADD */
		    *mem_ref = res = src1 + src2;
		    SETBFLAGS(0);
		    goto next_switch;
		case 1: /* OR */
		    *mem_ref = res = src1 | src2;
		    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
		    goto next_switch;
		case 2: /* ADC */
		    *mem_ref = res = src1 + src2 + (CARRY & 1);
		    SETBFLAGS(0);
		    goto next_switch;
		case 3: { /* SBB */
		    int carry = CARRY & 1;
		    *mem_ref = res = src1 - src2 - carry;
		    SETBFLAGS(1);
		    } goto next_switch;
		case 4: /* AND */
		    *mem_ref = res = src1 & src2;
		    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
		    goto next_switch;
		case 5: /* SUB */
		    *mem_ref = res = src1 - src2;
		    SETBFLAGS(1);
		    goto next_switch;
		case 6: /* XOR */
		    *mem_ref = res = src1 ^ src2;
		    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
		    goto next_switch;
		case 7: /* CMP */
		    res = src1 - src2;
		    SETBFLAGS(1);
		    goto next_switch;
		default: goto not_implemented;
	    }}
/*81*/	case IMMEDwrm: {
	    int src1, src2, res;
	    res = (*(PC+1)>>3)& 0x7;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = FETCH_WORD(PC); PC += 2;
	    mem_ref = MEM_REF; 
	    src1 = mFETCH_WORD(mem_ref);
	      switch (res) {
		case 0: /* ADD */
		    res = src1 + src2;
		    mPUT_WORD(mem_ref, res);
		    SETWFLAGS(0);
		    goto next_switch;
		case 1: /* OR */
		    res = src1 | src2;
		    mPUT_WORD(mem_ref, res);
		    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
		    goto next_switch;
		case 2: /* ADC */
		    res = src1 + src2 + (CARRY & 1);
		    mPUT_WORD(mem_ref, res);
		    SETWFLAGS(0);
		    goto next_switch;
		case 3: { /* SBB */
		    int carry = CARRY & 1;
		    res = src1 - src2 - carry;
		    mPUT_WORD(mem_ref, res);
		    SETWFLAGS(1);
		    } goto next_switch;
		case 4: /* AND */
		    res = src1 & src2;
		    mPUT_WORD(mem_ref, res);
		    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
		    goto next_switch;
		case 5: /* SUB */
		    res = src1 - src2;
		    mPUT_WORD(mem_ref, res);
		    SETWFLAGS(1);
		    goto next_switch;
		case 6: /* XOR */
		    res = src1 ^ src2;
		    mPUT_WORD(mem_ref, res);
		    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
		    goto next_switch;
		case 7: /* CMP */
		    res = src1 - src2;
		    SETWFLAGS(1);
		    goto next_switch;
		default: goto not_implemented;
	      }
	      }
/*83*/	case IMMEDisrm: {
	    int src1, src2, res;
	    res = (*(PC+1)>>3)& 0x7;
	    PC += hsw_modrm_16_word(MODARGS);
	    src2 = (signed char)*(PC); PC += 1;
	    src2 = src2 & 0xffff;
	    mem_ref = MEM_REF; 
	    src1 = mFETCH_WORD(mem_ref);
	      switch (res) {
		case 0: /* ADD */
		    res = src1 + src2;
		    mPUT_WORD(mem_ref, res);
		    SETWFLAGS(0);
		    goto next_switch;
		case 1: /* OR */
		    res = src1 | src2;
		    mPUT_WORD(mem_ref, res);
		    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
		    goto next_switch;
		case 2: /* ADC */
		    res = src1 + src2 + (CARRY & 1);
		    mPUT_WORD(mem_ref, res);
		    SETWFLAGS(0);
		    goto next_switch;
		case 3: { /* SBB */
		    int carry = CARRY & 1;
		    res = src1 - src2 - carry;
		    mPUT_WORD(mem_ref, res);
		    SETWFLAGS(1);
		    } goto next_switch;
		case 4: /* AND */
		    res = src1 & src2;
		    mPUT_WORD(mem_ref, res);
		    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
		    goto next_switch;
		case 5: /* SUB */
		    res = src1 - src2;
		    mPUT_WORD(mem_ref, res);
		    SETWFLAGS(1);
		    goto next_switch;
		case 6: /* XOR */
		    res = src1 ^ src2;
		    mPUT_WORD(mem_ref, res);
		    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
		    goto next_switch;
		case 7: /* CMP */
		    res = src1 - src2;
		    SETWFLAGS(1);
		    goto next_switch;
		default: goto not_implemented;
	      }
	    }
/*84*/	case TESTbrm: {
	    DWORD res;
	    PC += hsw_modrm_16_byte(MODARGS);
	    res = *MEM_REF & *HREG1;
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } goto next_switch; 
/*85*/	case TESTwrm: {
	    DWORD res;
	    PC += hsw_modrm_16_word(MODARGS);
	    res = mFETCH_WORD(MEM_REF) & *XREG1;
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } goto next_switch; 
/*86*/	case XCHGbrm: {
	    register BYTE temp;
	    PC += hsw_modrm_16_byte(MODARGS);
	    temp = *MEM_REF; *MEM_REF = *HREG1; *HREG1 = temp;
	    } goto next_switch; 
/*87*/	case XCHGwrm: {
	    register WORD wtemp;
	    PC += hsw_modrm_16_word(MODARGS);
	    mem_ref = MEM_REF;
	    wtemp = *XREG1;
	    *XREG1 = mFETCH_WORD(mem_ref);
	    mPUT_WORD(mem_ref, wtemp);
		goto next_switch; 
	    }

/*88*/	case MOVbfrm:
	    PC += hsw_modrm_16_byte(MODARGS);
	    mPUT_BYTE(MEM_REF, *HREG1); goto next_switch;
/*89*/	case MOVwfrm:
	    PC += hsw_modrm_16_word(MODARGS);
	    mPUT_WORD(MEM_REF, *XREG1); goto next_switch; 
/*8a*/	case MOVbtrm:
	    PC += hsw_modrm_16_byte(MODARGS);
	    *HREG1 = mFETCH_BYTE(MEM_REF); goto next_switch;
/*8b*/	case MOVwtrm:
	    PC += hsw_modrm_16_word(MODARGS);
	    *XREG1 = mFETCH_WORD(MEM_REF); goto next_switch;
/*8c*/	case MOVsrtrm: {
	    BYTE seg_reg = (*(PC + 1) >> 3) & 0x7;
	    PC += hsw_modrm_16_word(MODARGS);
		mem_ref = MEM_REF;
		switch (seg_reg) {
		    case 0: /* ES */
			mPUT_WORD(mem_ref, SHORT_ES_16);
			goto next_switch;
		    case 1: /* CS */
			mPUT_WORD(mem_ref, SHORT_CS_16);
			goto next_switch;
		    case 2: /* SS */
			mPUT_WORD(mem_ref, SHORT_SS_16);
			goto next_switch;
		    case 3: /* DS */
			mPUT_WORD(mem_ref, SHORT_DS_16);
			goto next_switch;
		    case 4: /* FS */
			mPUT_WORD(mem_ref, SHORT_FS_16);
			goto next_switch;
		    case 5: /* GS */
			mPUT_WORD(mem_ref, SHORT_GS_16);
			goto next_switch;
		    case 6: /* Illegal */
		    case 7: /* Illegal */
			goto illegal_op;
			/* trap this */
		    default: goto not_implemented;
		}
		}
/*8d*/	case LEA:
	    OVERR_DS = OVERR_SS = 0;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) {
		goto illegal_op;
	    } else {
		*XREG1 = (DWORD)MEM_REF;
	    }
	    goto next_switch;
/*8e*/	case MOVsrfrm: {
	    register WORD wtemp;
	    BYTE seg_reg = (*(PC + 1) >> 3) & 0x7;
	    PC += hsw_modrm_16_word(MODARGS);
	    wtemp = mFETCH_WORD(MEM_REF);
	    switch (seg_reg) {
		case 0: /* ES */
		    if ((*err = SET_SEGREG(LONG_ES,BIG_ES,MK_ES,wtemp))) {
			iVAR->error_addr=wtemp&0xfffc; return P0; }
		    SHORT_ES_16 = wtemp;
		    goto next_switch;
		case 2: /* SS */
		    if ((*err = SET_SEGREG(LONG_SS,BIG_SS,MK_SS,wtemp))) {
			iVAR->error_addr=wtemp&0xfffc; return P0; }
		    SHORT_SS_16 = wtemp;
		    goto next_switch;
		case 3: /* DS */
		    if ((*err = SET_SEGREG(LONG_DS,BIG_DS,MK_DS,wtemp))) {
			iVAR->error_addr=wtemp&0xfffc; return P0; }
		    SHORT_DS_16 = wtemp;
		    goto next_switch;
		case 4: /* FS */
		    if ((*err = SET_SEGREG(LONG_FS,BIG_FS,MK_FS,wtemp))) {
			iVAR->error_addr=wtemp&0xfffc; return P0; }
		    SHORT_FS_16 = wtemp;
		    goto next_switch;
		case 5: /* GS */
		    if ((*err = SET_SEGREG(LONG_GS,BIG_GS,MK_GS,wtemp))) {
			iVAR->error_addr=wtemp&0xfffc; return P0; }
		    SHORT_GS_16 = wtemp;
		    goto next_switch;
		case 1: /* CS */
		case 6: /* Illegal */
		case 7: /* Illegal */
		    goto illegal_op;
		    /* trap this */
		default: goto not_implemented;
	    }}
/*8f*/	case POPrm: {
	    register WORD wtemp;
	    POPWORD(wtemp);
	    PC += hsw_modrm_16_word(MODARGS);
	    mem_ref = MEM_REF;
	    PUT_WORD(mem_ref, wtemp);
	    } goto next_switch;

/*90*/	case NOP:
	    do PC++; while (*PC==0x90); goto next_switch;
/*91*/	case XCHGcx: {
	    register WORD wtemp = AX;
	    AX = CX;
	    CX = wtemp;
	    } PC += 1; goto next_switch;
/*92*/	case XCHGdx: {
	    register WORD wtemp = AX;
	    AX = DX;
	    DX = wtemp;
	    } PC += 1; goto next_switch;
/*93*/	case XCHGbx: {
	    register WORD wtemp = AX;
	    AX = BX;
	    BX = wtemp;
	    } PC += 1; goto next_switch;
/*94*/	case XCHGsp: {
	    register WORD wtemp = AX;
	    AX = SP;
	    SP = wtemp;
	    } PC += 1; goto next_switch;
/*95*/	case XCHGbp: {
	    register WORD wtemp = AX;
	    AX = BP;
	    BP = wtemp;
	    } PC += 1; goto next_switch;
/*96*/	case XCHGsi: {
	    register WORD wtemp = AX;
	    AX = SI;
	    SI = wtemp;
	    } PC += 1; goto next_switch;
/*97*/	case XCHGdi: {
	    register WORD wtemp = AX;
	    AX = DI;
	    DI = wtemp;
	    } PC += 1; goto next_switch;
/*98*/	case CBW:
	    AH = (AL & 0x80) ? 0xff : 0;
	    PC += 1; goto next_switch;
/*99*/	case CWD:
	    DX = (AX & 0x8000) ? 0xffff : 0;
	    PC += 1; goto next_switch;
/*9a*/	case CALLl: { /* restartable */
	    DWORD jcs, ocs = SHORT_CS_16;
	    DWORD jip, oip = PC - LONG_CS + 5;
	    WORD transfer_magic;
	    jip = FETCH_WORD(PC+1);
	    jcs = FETCH_WORD(PC+3);
	    transfer_magic = (WORD)GetSelectorType(jcs);
#ifdef DOSEMU
	    if ((*err = SET_SEGREG(LONG_CS,BIG_CS,MK_CS,jcs))==EXCP0B_NOSEG) {
		    iVAR->error_addr=jcs&0xfffc; return P0; }
	    SHORT_CS_16 = (WORD)jcs;
#endif
	    PUSHWORD(ocs);
	    PUSHWORD(oip);
#ifdef DOSEMU
	    PC = jip + LONG_CS;
#else
	    iENV->return_addr = (ocs << 16) | (WORD)oip;
#endif
	    if (VM86F || (transfer_magic == TRANSFER_CODE16)) {
#ifndef DOSEMU
		SHORT_CS_16 = (WORD)jcs;
		SET_SEGREG(LONG_CS,BIG_CS,MK_CS,jcs);
		PC = jip + LONG_CS;
#endif
		goto next_switch;
	    }
	    else if (transfer_magic == TRANSFER_CODE32) {
#ifdef DOSEMU
		*err = EXCP_GOBACK; return (PC);
#else
		goto not_implemented;
#endif
	    }
#ifndef DOSEMU
	    /* WARNING Bad design: putting the seg into the higher
	     * 16 bit WILL NOT WORK with 32-bit code!!! */
	    if ((transfer_magic == TRANSFER_CALLBACK) || 
	    		(transfer_magic == TRANSFER_BINARY))  {
		LONGPROC conv,targ;
		SEGIMAGE *lpSegImage = &((SEGIMAGE *)
	    		(*(long *)(SELECTOR_PADDRESS(jcs))))[jip>>3];
		targ = (LONGPROC)lpSegImage->targ;
		conv = (LONGPROC)lpSegImage->conv;
		EBP = (long)LONG_SS + EBP;
		ESP = (long)LONG_SS + ESP;
		trans_interp_flags(ENVARGS);
		if (transfer_magic == TRANSFER_CALLBACK) {
		    iENV->trans_addr = (BINADDR)MAKELONG(jip,jcs);
#ifndef NO_DEBUG_MSGS
		    LogProcName(jcs,jip,1);
#endif
		}
#ifndef NO_DEBUG_MSGS
		else    /* TRANSFER_BINARY */
		    e_printf("    CALLl: calling binary thunk %x:%x\n",
		    	(int)jcs, (int)jip);
#endif
		(conv)(env,targ);
#ifndef NO_DEBUG_MSGS
		LogProcName(AX,DX,0);
#endif
		SHORT_CS_16 = ocs = iENV->return_addr >> 16;
		oip = iENV->return_addr & 0xffff;
		SET_SEGREG(LONG_CS,BIG_CS,MK_CS,ocs);
		SET_SEGREG(LONG_DS,BIG_DS,MK_DS,SHORT_DS_16);
		SET_SEGREG(LONG_ES,BIG_ES,MK_ES,SHORT_ES_16);
		SET_SEGREG(LONG_SS,BIG_SS,MK_SS,SHORT_SS_16);
		EBP = EBP - (long)LONG_SS;
		ESP = ESP - (long)LONG_SS;
		trans_flags_to_interp(ENVARGS, iENV->flags);
		PC = LONG_CS + oip; goto next_switch;
	    }
	    else if (transfer_magic == TRANSFER_RETURN) {
		SHORT_CS_16 = jcs;
		iENV->return_addr = (jcs << 16) | jip;
#ifndef NO_DEBUG_MSGS
		e_printf("    CALLl: return %x:%x\n", (int)jcs, (int)jip);
#endif
		trans_interp_flags(ENVARGS);    
		EBP = EBP + (long)LONG_SS;
		ESP = ESP + (long)LONG_SS;
		return PC;
	    }
#endif
	    else
		invoke_data(ENVARGS);    /* TRANSFER_DATA or garbage */
        }
/*9b*/	case WAIT:
	    PC += 1; goto next_switch;
/*9c*/	case PUSHF: {
	    register WORD wtemp;
	    if (VM86F) goto not_permitted;
	    wtemp = (WORD)trans_interp_flags(ENVARGS);    
	    PUSHWORD(wtemp);
	    } PC += 1; goto next_switch;
/*9d*/	case POPF: {
	    DWORD temp=0;
	    if (VM86F) goto not_permitted;
	    POPWORD(temp);	/* change bits 0-15 */
	    /* skip reserved bits, in case of PUSH XX..POPF */
	    temp = (temp & 0x7fd5) | (iENV->flags & 0xffff0000) | 2;
	    BYTE_FLAG=0;
	    trans_flags_to_interp(ENVARGS, temp);
	    } PC += 1; goto next_switch;
/*9e*/	case SAHF: {
	    DWORD flags;
	    trans_interp_flags(ENVARGS);
	    flags = (iENV->flags & 0xffffff02) | (AH & 0xd5);
	    trans_flags_to_interp(ENVARGS, flags);
	    } PC += 1; goto next_switch;
/*9f*/	case LAHF:
	    AH = (BYTE)trans_interp_flags(ENVARGS);
	    PC += 1; goto next_switch;

/*a0*/	case MOVmal: {
	    mem_ref = OVERR_DS + FETCH_WORD((PC+1));
	    AL = *mem_ref;
	    } PC += 3; goto next_switch;
/*a1*/	case MOVmax: {
	    mem_ref = OVERR_DS + FETCH_WORD((PC+1));
	    AX = FETCH_WORD(mem_ref);
	    } PC += 3; goto next_switch;
/*a2*/	case MOValm: {
	    mem_ref = OVERR_DS + FETCH_WORD((PC+1));
	    *mem_ref = AL;
	    } PC += 3; goto next_switch;
/*a3*/	case MOVaxm: {
	    mem_ref = OVERR_DS + FETCH_WORD((PC+1));
	    PUT_WORD(mem_ref, AX);
	    } PC += 3; goto next_switch;
/*a4*/	case MOVSb: {
	    BYTE *src, *dest;
	    src = OVERR_DS + (SI);
	    dest = LONG_ES + (DI);
	    *dest = *src;
#ifndef	NO_TRACE_MSGS
	    if (d_emu>4) e_printf("------- [EA=%p->%p] %02lx\n",(ea_=src),dest,(eav_=*src));
#endif
	    (iENV->flags & DIRECTION_FLAG)?(SI--,DI--):(SI++,DI++);
	    } PC += 1; goto next_switch;
/*a5*/	case MOVSw: {
	    BYTE *src, *dest;
	    register int temp;
	    src = OVERR_DS + (SI);
	    dest = LONG_ES + (DI);
	    temp = FETCH_WORD(src);
#ifndef	NO_TRACE_MSGS
	    if (d_emu>4) e_printf("------- [EA=%p->%p] %04lx\n",(ea_=src),dest,(eav_=temp));
#endif
	    PUT_WORD(dest, temp);
	    (iENV->flags & DIRECTION_FLAG)?(SI-=2,DI-=2):(SI+=2,DI+=2);
	    } PC += 1; goto next_switch;
/*a6*/	case CMPSb: {
	    DWORD src1, src2; int res;
	    BYTE *src, *dest;
	    src = OVERR_DS + (SI);
	    dest = LONG_ES + (DI);
	    src1 = *src;
	    src2 = *dest;
#ifndef	NO_TRACE_MSGS
	    if (d_emu>4) e_printf("------- [EA=%p->%p] %02lx %02lx\n",(ea_=src),dest,(eav_=src1),src2);
#endif
	    res = src1 - src2;
	    (iENV->flags & DIRECTION_FLAG)?(SI--,DI--):(SI++,DI++);
	    SETBFLAGS(1);
	    } PC += 1; goto next_switch;
/*a7*/	case CMPSw: {
	    DWORD res, src1, src2;
	    BYTE *src, *dest;
	    src = OVERR_DS + (SI);
	    dest = LONG_ES + (DI);
	    src1 = FETCH_WORD(src);
	    src2 = FETCH_WORD(dest);
#ifndef	NO_TRACE_MSGS
	    if (d_emu>4) e_printf("------- [EA=%p->%p] %04lx %04lx\n",(ea_=src),dest,(eav_=src1),src2);
#endif
	    res = src1 - src2;
	    (iENV->flags & DIRECTION_FLAG)?(SI-=2,DI-=2):(SI+=2,DI+=2);
	    SETWFLAGS(1);
	    } PC += 1; goto next_switch;
/*a8*/	case TESTbi: {
	    DWORD res;
	    res = AL & PC[1];
	    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
	    } PC += 2; goto next_switch;
/*a9*/	case TESTwi: {
	    DWORD res;
	    res = AX & FETCH_WORD((PC+1));
	    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
	    } PC += 3; goto next_switch;
/*aa*/	case STOSb:
	    LONG_ES[DI] = AL;
	    (iENV->flags & DIRECTION_FLAG)?DI--:DI++;
	    PC += 1; goto next_switch;
/*ab*/	case STOSw:
	    PUT_WORD(LONG_ES+DI, AX);
	    (iENV->flags & DIRECTION_FLAG)?(DI-=2):(DI+=2);
	    PC += 1; goto next_switch;
/*ac*/	case LODSb: {
	    BYTE *seg;
	    seg = OVERR_DS + (SI);
	    AL = *seg;
	    (iENV->flags & DIRECTION_FLAG)?SI--:SI++;
	    } PC += 1; goto next_switch;
/*ad*/	case LODSw: {
	    BYTE *seg;
	    seg = OVERR_DS + (SI);
	    AX = FETCH_WORD(seg);
	    (iENV->flags & DIRECTION_FLAG)?(SI-=2):(SI+=2);
	    } PC += 1; goto next_switch;
/*ae*/	case SCASb: {
	    DWORD src1, src2; int res;
	    src1 = AL;
	    src2 = *((BYTE *)(LONG_ES+DI));
#ifndef	NO_TRACE_MSGS
	    if (d_emu>4) e_printf("------- [EA=%p] %02lx\n",(ea_=(LONG_ES+DI)),(eav_=src2));
#endif
	    res = src1 - src2;
	    (iENV->flags & DIRECTION_FLAG)?DI--:DI++;
	    SETBFLAGS(1);
	    } PC += 1; goto next_switch;
/*af*/	case SCASw: {
	    DWORD res, src1, src2;
	    src1 = AX;
	    mem_ref = LONG_ES+DI;
	    src2 = FETCH_WORD(mem_ref);
#ifndef	NO_TRACE_MSGS
	    if (d_emu>4) e_printf("------- [EA=%p] %04lx\n",(ea_=(LONG_ES+DI)),(eav_=src2));
#endif
	    res = src1 - src2;
	    (iENV->flags & DIRECTION_FLAG)?(DI-=2):(DI+=2);
	    SETWFLAGS(1);
	    } PC += 1; goto next_switch;

/*b0*/	case MOVial:
	    AL = *(PC+1);
	     PC += 2; goto next_switch;
/*b1*/	case MOVicl:
	    CL = *(PC+1);
	    PC += 2; goto next_switch;
/*b2*/	case MOVidl:
	    DL = *(PC+1);
	    PC += 2; goto next_switch;
/*b3*/	case MOVibl:
	    BL = *(PC+1);
	    PC += 2; goto next_switch;
/*b4*/	case MOViah:
	    AH = *(PC+1);
	    PC += 2; goto next_switch;
/*b5*/	case MOVich:
	    CH = *(PC+1);
	    PC += 2; goto next_switch;
/*b6*/	case MOVidh:
	    DH = *(PC+1);
	    PC += 2; goto next_switch;
/*b7*/	case MOVibh:
	    BH = *(PC+1);
	    PC += 2; goto next_switch;
/*b8*/	case MOViax:
	    AX = FETCH_WORD((PC+1));
	    PC += 3; goto next_switch;
/*b9*/	case MOVicx:
	    CX = FETCH_WORD((PC+1));
	    PC += 3; goto next_switch;
/*ba*/	case MOVidx:
	    DX = FETCH_WORD((PC+1));
	    PC += 3; goto next_switch;
/*bb*/	case MOVibx:
	    BX = FETCH_WORD((PC+1));
	    PC += 3; goto next_switch;
/*bc*/	case MOVisp:
	    SP = FETCH_WORD((PC+1)); 
	    PC += 3; goto next_switch;
/*bd*/	case MOVibp:
	    BP = FETCH_WORD((PC+1)); 
	    PC += 3; goto next_switch;
/*be*/	case MOVisi:
	    SI = FETCH_WORD((PC+1));
	    PC += 3; goto next_switch;
/*bf*/	case MOVidi:
	    DI = FETCH_WORD((PC+1));
	    PC += 3; goto next_switch;

/*d0*/	case SHIFTb:
/*d2*/	case SHIFTbv:
/*c0*/	case SHIFTbi: {
	    int temp, count;
	    DWORD rbef, raft; BYTE opc;
	    opc = *PC;
	    temp = (*(PC+1)>>3)& 0x7;
	    PC += hsw_modrm_16_byte(MODARGS);
	    if (opc==SHIFTb) count = 1;
	      else if (opc==SHIFTbv) count = CX & 0x1f;
		else { count = *PC & 0x1f; PC += 1; }
	    /* are we sure that for count==0 CF is not changed? */
	    if (count) {
		mem_ref = MEM_REF;
		switch (temp) {
		    case 0: /* ROL */
			rbef = *mem_ref;
			SRC1_8 = SRC2_8 = rbef;		/* OF: 00x or 11x */
			count &= 7;	/* rotation modulo 8 */
			raft = (rbef << count) | (rbef >> (8 - count));
			*mem_ref = RES_8 = (BYTE)raft;
			BYTE_FLAG = BYTE_OP;
			CARRYB = (BYTE)(raft & 1);
			goto next_switch;
		    case 1: /* ROR */
			rbef = *mem_ref; 
			SRC1_8 = SRC2_8 = rbef;
			count &= 7;
			raft = (rbef >> count) | (rbef << (8 - count));
			*mem_ref = RES_8 = (BYTE)raft;
			BYTE_FLAG = BYTE_OP;
			CARRYB = (BYTE)((raft >> 7) & 1);
			goto next_switch;
		    case 2: /* RCL */
			rbef = *mem_ref; 
			SRC1_8 = SRC2_8 = rbef;
			count &= 7;
			raft = (rbef << count) | ((rbef >> (9 - count))
			    | ((CARRY & 1) << (count - 1)));
			*mem_ref = RES_8 = (BYTE)raft;
			BYTE_FLAG = BYTE_OP;
			CARRYB = (BYTE)((rbef >> (8 - count)) & 1);
			goto next_switch;
		    case 3: /* RCR */
			rbef = *mem_ref; 
			SRC1_8 = SRC2_8 = rbef;
			count &= 7;
			raft = (rbef >> count) | ((rbef << (9 - count))
			    | ((CARRY & 1) << (8 - count)));
			*mem_ref = RES_8 = (BYTE)raft;
			BYTE_FLAG = BYTE_OP;
			CARRYB = (BYTE)((rbef >> (count - 1)) & 1);
			goto next_switch;
		    case 6: /* Undocumented */
			if (opc==SHIFTbv) goto illegal_op;
			/* fall thru for SHIFTbi */
		    case 4: /* SHL/SAL */
			temp = *mem_ref; 
			SRC1_8 = SRC2_8 = temp;
			CARRYB = (temp >> (8 - count)) & 1;
			temp = (temp << count);
			*mem_ref = temp;
			/* if cnt==1 OF=1 if CF after shift != MSB 1st op */
			RES_8 = temp; BYTE_FLAG = BYTE_OP;
			goto next_switch;
		    case 5: /* SHR */
			temp = *mem_ref; 
			SRC1_8 = SRC2_8 = temp;
			CARRYB = (temp >> (count-1)) & 1;
			temp = (temp >> count);
			*mem_ref = temp;
			/* if cnt==1 OF=1 if MSB was changed */
			RES_8 = temp; BYTE_FLAG = BYTE_OP;
			goto next_switch;
		    case 7: /* SAR */
			temp = *(signed char *)mem_ref; 
			CARRYB = (temp >> (count-1)) & 1;
			temp = (temp >> count);
			*mem_ref = (BYTE)temp;
			/* if cnt==1 OF=0 */
			SRC1_8 = SRC2_8 = RES_8 = (BYTE)temp;
			BYTE_FLAG = BYTE_OP;
			goto next_switch;
		    default: goto not_implemented;
		} } else  goto next_switch;
	    }
/*d1*/	case SHIFTw:
/*d3*/	case SHIFTwv:
/*c1*/	case SHIFTwi: {
	    int temp, count;
	    DWORD rbef, raft; BYTE opc;
	    opc = *PC;
	    temp = (*(PC+1)>>3)& 0x7;
	    PC += hsw_modrm_16_word(MODARGS);
	    mem_ref = MEM_REF;
	    if (opc==SHIFTw) count = 1;
	      else if (opc==SHIFTwv) count = CX & 0x1f;
		else { count = *PC & 0x1f; PC += 1; }
	    /* are we sure that for count==0 CF is not changed? */
	    if (count) {
		mem_ref = MEM_REF;
		switch (temp) {
		    case 0: /* ROL */
			rbef = mFETCH_WORD(mem_ref);
			SRC1_16 = SRC2_16 = rbef;
			count &= 0xf;	/* rotation modulo 16 */
			raft = (rbef << count) | (rbef >> (16 - count));
			mPUT_WORD(mem_ref,raft);
			RES_16 = (WORD)raft;
			CARRY = (WORD)(raft & 1);	/* -> BYTE_FLAG=0 */
			goto next_switch;
		    case 1: /* ROR */
			rbef = mFETCH_WORD(mem_ref);
			SRC1_16 = SRC2_16 = rbef;
			count &= 0xf;
			raft = (rbef >> count) | (rbef << (16 - count));
			mPUT_WORD(mem_ref,raft);
			RES_16 = (WORD)raft;
			CARRY = (WORD)((raft >> 15) & 1);
			goto next_switch;
		    case 2: /* RCL */
			rbef = mFETCH_WORD(mem_ref);
			SRC1_16 = SRC2_16 = rbef;
			count &= 0xf;
			raft = (rbef << count) | ((rbef >> (17 - count))
			    | ((CARRY & 1) << (count - 1)));
			mPUT_WORD(mem_ref,raft);
			RES_16 = (WORD)raft;
			CARRY = (WORD)((rbef >> (16 - count)) & 1);
			goto next_switch;
		    case 3: /* RCR */
			rbef = mFETCH_WORD(mem_ref);
			SRC1_16 = SRC2_16 = rbef;
			count &= 0xf;
			raft = (rbef >> count) | ((rbef << (17 - count))
			    | ((CARRY & 1) << (16 - count)));
			mPUT_WORD(mem_ref,raft);
			RES_16 = (WORD)raft;
			CARRY = (WORD)((rbef >> (count - 1)) & 1);
			goto next_switch;
		    case 6: /* Undocumented */
			if ((opc==SHIFTw)||(opc==SHIFTwv))
			    goto illegal_op;
			/* fall thru for SHIFTwi */
		    case 4: /* SHL/SAL */
			temp = mFETCH_WORD(mem_ref);
			SRC1_16 = SRC2_16 = temp;
			CARRY = (temp >> (16 - count)) & 1;
			temp = (temp << count);
			mPUT_WORD(mem_ref,temp);
			RES_16 = temp;
			goto next_switch;
		    case 5: /* SHR */
			temp = mFETCH_WORD(mem_ref); 
			SRC1_16 = SRC2_16 = temp;
			CARRY = (temp >> (count-1)) & 1;
			temp = (temp >> count);
			mPUT_WORD(mem_ref,temp);
			RES_16 = temp;
			goto next_switch;
		    case 7: /* SAR */
			temp = (signed short)mFETCH_WORD(mem_ref);
			CARRY = (temp >> (count-1)) & 1;
			temp = (temp >> count);
			mPUT_WORD(mem_ref,temp);
			/* if cnt==1 OF=0 */
			SRC1_16 = SRC2_16 = RES_16 = (WORD)temp;
			goto next_switch;
		    default: goto not_implemented;
	      } } else  goto next_switch;
	    }
/*c2*/	case RETisp: {
	    DWORD ip, dr;
	    POPWORD(ip);
	    dr = (signed short)(FETCH_WORD((PC+1)));
	    if (BIG_SS) ESP+=dr; else SP+=(WORD)dr;
	    PC = LONG_CS + ip;
	    } goto next_switch;
/*c3*/	case RET: {
	    DWORD ip;
	    POPWORD(ip);
	    PC = LONG_CS + ip;
	    } goto next_switch;
/*c4*/	case LES: {
	    register WORD wtemp;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) {
		/* Illegal */
		goto illegal_op;
	    }
	    mem_ref = MEM_REF;
	    wtemp = FETCH_WORD(mem_ref+2);
	    if ((*err = SET_SEGREG(LONG_ES,BIG_ES,MK_ES,wtemp))) {
		iVAR->error_addr=wtemp&0xfffc; return P0; }
	    *XREG1 = FETCH_WORD(mem_ref);
	    SHORT_ES_32 = wtemp;
	    } goto next_switch;
/*c5*/	case LDS: {
	    register WORD wtemp;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) {
		/* Illegal */
		goto illegal_op;
		}
	    mem_ref = MEM_REF;
	    wtemp = FETCH_WORD(mem_ref+2);
	    if ((*err = SET_SEGREG(LONG_DS,BIG_DS,MK_DS,wtemp))) {
		iVAR->error_addr=wtemp&0xfffc; return P0; }
	    *XREG1 = FETCH_WORD(mem_ref);
	    SHORT_DS_32 = wtemp;
	    } goto next_switch;
/*c6*/	case MOVbirm:
	    PC += hsw_modrm_16_byte(MODARGS);
	    *MEM_REF = *PC; PC += 1; goto next_switch;
/*c7*/	case MOVwirm:
	    PC += hsw_modrm_16_word(MODARGS);
	    mPUT_WORD(MEM_REF, mFETCH_WORD(PC));
	    PC += 2; goto next_switch;
/*c8*/	case ENTER: {
	    BYTE *sp, *bp, *ss;
	    DWORD FrameTemp;
	    BYTE level = *(PC+3) & 0x1f;
	    ss = LONG_SS;
	    if (VM86F || (BIG_SS==0)) {
		sp = ss + SP - 2;
		bp = ss + BP;
	      } else {
		sp = ss + ESP - 2;
		bp = ss + EBP;		/* Intel manual */
	    }
	    PUSHWORD(BP);		/* -> sp-=2 */
	    FrameTemp = sp - ss;
	    if (level) {
		sp -= (2 + 2*level);
		while (level--) {
		    bp -= 2; 
		    PUSHWORD((bp-ss));
		}
		PUSHWORD(FrameTemp);
	    }
	    BP = (WORD)FrameTemp;
	    sp -= (FETCH_WORD((PC + 1)));
	    if (VM86F || (BIG_SS==0)) SP=sp-ss; else ESP=sp-ss;
	    } PC += 4; goto next_switch;
/*c9*/	case LEAVE: {   /* 0xc9 */
	    register WORD wtemp;
	    SP = BP;
	    POPWORD(wtemp);
	    BP = wtemp;
	    } PC += 1; goto next_switch;
/*ca*/	case RETlisp: { /* restartable */
	    DWORD cs, ip, dr;
	    WORD transfer_magic;
	    POPWORD(ip);	/* so to use TOS_WORD */
	    dr = FETCH_WORD((PC+1));
#ifdef DOSEMU
	    cs = TOS_WORD;
	    if ((*err = SET_SEGREG(LONG_CS,BIG_CS,MK_CS,cs))==EXCP0B_NOSEG) {
		    PUSHWORD(ip); iVAR->error_addr=cs&0xfffc; return P0; }
#endif
	    POPWORD(cs);
	    if (BIG_SS) ESP+=dr; else SP+=(WORD)dr;
#ifdef DOSEMU
	    SHORT_CS_16 = (WORD)cs;
	    PC = ip + LONG_CS;
#endif
	    transfer_magic = (WORD)GetSelectorType(cs);
	    if (VM86F || (transfer_magic==TRANSFER_CODE16)) {
#ifndef DOSEMU
		SHORT_CS_16 = (WORD)cs;
		SET_SEGREG(LONG_CS,BIG_CS,MK_CS,cs);
		PC = ip + LONG_CS;
#endif
		goto next_switch;
	    }
	    else if (transfer_magic==TRANSFER_CODE32) {
#ifdef DOSEMU
		*err = EXCP_GOBACK; return (PC);
#else
		goto not_implemented;
#endif
	    }
	    else {
#ifdef DOSEMU
	        invoke_data(ENVARGS);    /* TRANSFER_DATA or garbage */
#else
		iENV->return_addr = ip | (cs << 16);
		trans_interp_flags(ENVARGS);
		EBP = EBP + (long)LONG_SS;
		ESP = ESP + (long)LONG_SS;
		return PC;
#endif
	    }
	    }
/*cb*/	case RETl: { /* restartable */
	    DWORD cs, ip;
	    WORD transfer_magic;
	    POPWORD(ip);
#ifdef DOSEMU
	    cs = TOS_WORD;
	    if ((*err = SET_SEGREG(LONG_CS,BIG_CS,MK_CS,cs))==EXCP0B_NOSEG) {
		    PUSHWORD(ip); iVAR->error_addr=cs&0xfffc; return P0; }
#endif
	    POPWORD(cs);
#ifdef DOSEMU
	    SHORT_CS_16 = (WORD)cs;
	    PC = ip + LONG_CS;
#endif
	    transfer_magic = (WORD)GetSelectorType(cs);
	    if (VM86F || (transfer_magic==TRANSFER_CODE16)) {
#ifndef DOSEMU
		SHORT_CS_16 = (WORD)cs;
		SET_SEGREG(LONG_CS,BIG_CS,MK_CS,cs);
		PC = ip + LONG_CS;
#endif
		goto next_switch;
	    }
	    else if (transfer_magic==TRANSFER_CODE32) {
#ifdef DOSEMU
		*err = EXCP_GOBACK; return (PC);
#else
		goto not_implemented;
#endif
	    }
	    else {
#ifdef DOSEMU
	        invoke_data(ENVARGS);    /* TRANSFER_DATA or garbage */
#else
		iENV->return_addr = ip | (cs << 16);
		trans_interp_flags(ENVARGS);
		EBP = EBP + (long)LONG_SS;
		ESP = ESP + (long)LONG_SS;
		return PC;
#endif
	    }
	    } 
/*cc*/	case INT3:
#ifdef DOSEMU
	    *err=EXCP03_INT3;
	    PC += 1; return PC;
#else
	    goto not_implemented;
#endif
/*cd*/	case INT:
#ifdef DOSEMU
	    *err=(PC[1]==1? EXCP01_SSTP:EXCP0D_GPF);
	    return P0;
#else
	    { unsigned int temp, cs, ip = (unsigned int)(PC - LONG_CS);
	    PUSHWORD(ip);
	    cs = SHORT_CS_16;
	    PUSHWORD(cs);
	    iENV->return_addr = (cs << 16)|ip;
	    temp =     trans_interp_flags(ENVARGS);
	    PUSHWORD(temp);
	    EBP = EBP + (long)LONG_SS;
	    ESP = ESP + (long)LONG_SS;
	    INT_handler((unsigned int)(*(PC+1)),(void *)env);
	    EBP = EBP - (long)LONG_SS;
	    ESP = ESP - (long)LONG_SS;
	    trans_flags_to_interp(ENVARGS, iENV->flags);
	    } PC += 2; goto next_switch;
#endif
/*ce*/	case INTO:
#ifdef DOSEMU
	    *err=EXCP04_INTO; return P0;
#else
	    goto not_implemented;
#endif
/*cf*/	case IRET: /* restartable */
	    if (VM86F) goto not_permitted;
	    else {
		DWORD cs, ip, flags;
		WORD transfer_magic;
		POPWORD(ip);
#ifdef DOSEMU
		cs = TOS_WORD;
		if ((*err = SET_SEGREG(LONG_CS,BIG_CS,MK_CS,cs))==EXCP0B_NOSEG) {
			PUSHWORD(ip); iVAR->error_addr=cs&0xfffc; return P0; }
#endif
		POPWORD(cs);
		POPWORD(flags);
		/* skip reserved bits, in case of PUSH XX..POPF */
		flags = (flags & 0x7fd5) | 2;
		BYTE_FLAG=0;
		trans_flags_to_interp(ENVARGS, flags);
#ifdef DOSEMU
		SHORT_CS_16 = (WORD)cs;
		PC = ip + LONG_CS;
		if (iENV->flags & TRAP_FLAG) {
		    return (PC);
		}
#endif
		transfer_magic = (WORD)GetSelectorType(cs);
		if (VM86F || (transfer_magic==TRANSFER_CODE16)) {
#ifndef DOSEMU
		    SHORT_CS_16 = (WORD)cs;
		    SET_SEGREG(LONG_CS,BIG_CS,MK_CS,cs);
		    PC = ip + LONG_CS;
#endif
			goto next_switch;
	    }
		else if (transfer_magic==TRANSFER_CODE32) {
#ifdef DOSEMU
		    *err = EXCP_GOBACK; return (PC);
#else
		    goto not_implemented;
#endif
	    }
#ifndef DOSEMU
		else {
		    iENV->return_addr = ip | (cs << 16);
		    trans_interp_flags(ENVARGS);
		    EBP = EBP + (long)LONG_SS;
		    ESP = ESP + (long)LONG_SS;
		    return PC;
		}
#else
		else
		    invoke_data(ENVARGS);    /* TRANSFER_DATA or garbage */
#endif
	    }

/*d0*/	    /* see before */
/*d1*/	    /* see before */
/*d2*/	    /* see before */
/*d3*/	    /* see before */
/*d4*/	case AAM: {
	    int base = PC[1];
	    BYTE tmp = AL;
	    AH = tmp / base;
	    RES_8 = AL = tmp % base;
	    BYTE_FLAG=BYTE_OP;
	    } PC += 2; goto next_switch;
/*d5*/	case AAD: {
	    int base = PC[1];
	    AX = ((AH * base) + AL) & 0xff;
	    RES_8 = AL; /* for flags */
	    BYTE_FLAG=BYTE_OP;
	    } PC += 2; goto next_switch;
/*d6*/	case 0xd6:    /* Undocumented */
	    AL = (CARRYB & 1? 0xff:0x00);
	    PC += 1; goto next_switch;
/*d7*/	case XLAT: {
	    mem_ref = OVERR_DS + (BX) + (AL);
	    AL = *mem_ref; }
	    PC += 1; goto next_switch;

/*d8*/	case ESC0:
/*d9*/	case ESC1:
/*da*/	case ESC2:
/*db*/	case ESC3:
/*dc*/	case ESC4:
/*dd*/	case ESC5:
/*de*/	case ESC6:
/*df*/	case ESC7:
    {
   switch(*PC){
/*d8*/	case ESC0: {
	    int reg = (*(PC+1) & 7);
	    DWORD funct = (DWORD)(*(PC+1) & 0x38) >> 3;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) hsw_fp87_reg0[funct](reg);
	    else hsw_fp87_mem0[funct](MEM_REF);
	    } break;
/*d9*/	case ESC1: {
	    int reg = (*(PC+1) & 7);
	    DWORD funct = (DWORD)(*(PC+1) & 0x38) >> 3;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) hsw_fp87_reg1[funct](reg);
	    else hsw_fp87_mem1[funct](MEM_REF);
	    } break;
/*da*/	case ESC2: {
	    int reg = (*(PC+1) & 7);
	    DWORD funct = (DWORD)(*(PC+1) & 0x38) >> 3;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) hsw_fp87_reg2[funct](reg);
	    else hsw_fp87_mem2[funct](MEM_REF);
	    } break;
/*db*/	case ESC3: {
	    int reg = (*(PC+1) & 7);
	    DWORD funct = (DWORD)(*(PC+1) & 0x38) >> 3;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) hsw_fp87_reg3[funct](reg);
	    else hsw_fp87_mem3[funct](MEM_REF);
	    } break;
/*dc*/	case ESC4: {
	    int reg = (*(PC+1) & 7);
	    DWORD funct = (DWORD)(*(PC+1) & 0x38) >> 3;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) hsw_fp87_reg4[funct](reg);
	    else hsw_fp87_mem4[funct](MEM_REF);
	    } break;
/*dd*/	case ESC5: {
	    int reg = (*(PC+1) & 7);
	    DWORD funct = (DWORD)(*(PC+1) & 0x38) >> 3;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) hsw_fp87_reg5[funct](reg);
	    else hsw_fp87_mem5[funct](MEM_REF);
	    } break;
/*de*/	case ESC6: {
	    int reg = (*(PC+1) & 7);
	    DWORD funct = (DWORD)(*(PC+1) & 0x38) >> 3;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) hsw_fp87_reg6[funct](reg);
	    else hsw_fp87_mem6[funct](MEM_REF);
	    } break;
/*df*/	case ESC7: {
	    int reg = (*(PC+1) & 7);
	    DWORD funct = (DWORD)(*(PC+1) & 0x38) >> 3;
	    PC += hsw_modrm_16_word(MODARGS);
	    if (IS_MODE_REG) hsw_fp87_reg7[funct](reg);
	    else hsw_fp87_mem7[funct](MEM_REF);
	    } break;
    }
#ifndef NO_TRACE_MSGS
    /*if (d_emu>3)*/ e_trace_fp(&hsw_env87);
#endif
    if ((*PC==WAIT) || (*PC==NOP)) PC += 1;
    goto next_switch; }

/*e0*/	case LOOPNZ_LOOPNE: 
	    if ((--CX != 0) && (!IS_ZF_SET)) {
		JUMP((PC+1)); goto next_switch;
	    } PC += 2; goto next_switch;
/*e1*/	case LOOPZ_LOOPE: 
	    if ((--CX != 0) && (IS_ZF_SET)) {
		JUMP((PC+1)); goto next_switch;
	    } PC += 2; goto next_switch;
/*e2*/	case LOOP: 
	    if (--CX != 0) {
		JUMP((PC+1)); goto next_switch;
	    } PC += 2; goto next_switch;
/*e3*/	case JCXZ: 
	    if (CX == 0) {
		JUMP((PC+1)); goto next_switch;
	    } PC += 2; goto next_switch;

/*e4*/	case INb: {
	      BYTE a = FETCH_BYTE(PC+1);
	      AL = hsw_port_inb(a);
	      PC += 2; goto next_switch;
	    }
/*e5*/	case INw: {
	      BYTE a = FETCH_BYTE(PC+1);
	      AX = hsw_port_inw(a);
	      PC += 2; goto next_switch;
	    }
/*e6*/	case OUTb: {
	      BYTE a = FETCH_BYTE(PC+1);
	      hsw_port_outb(a, AL);
	      PC += 2; goto next_switch;
	    }
/*e7*/	case OUTw: {
	      BYTE a = FETCH_BYTE(PC+1);
	      hsw_port_outw(a, AX);
	      PC += 2; goto next_switch;
	    }

/*6c*/	case INSb:
/*ec*/	case INvb:
	    atmp = hsw_port_inb(DX);
	    if (FETCH_BYTE(PC)==INvb)
		AL = (BYTE)atmp;
	    else {
		PUT_BYTE(LONG_ES+DI,atmp);
		(iENV->flags & DIRECTION_FLAG)?(DI--):(DI++);
	    }
	    PC += 1;
	    goto next_switch;

/*6d*/	case INSw:
/*ed*/	case INvw:
	    atmp = hsw_port_inw(DX);
	    if (FETCH_BYTE(PC)==INvw)
		AX = atmp;
	    else {
		PUT_WORD(LONG_ES+DI, atmp);
		(iENV->flags & DIRECTION_FLAG)?(DI-=2):(DI+=2);
	    }
	    PC += 1;
	    goto next_switch;

/*6e*/	case OUTSb:
	    atmp = LONG_ES[DI];
	    goto com6ee0;
/*ee*/	case OUTvb:
	    atmp = AL;
com6ee0:
	    hsw_port_outb(DX, atmp);
	    if (FETCH_BYTE(PC)==OUTSb) {
		(iENV->flags & DIRECTION_FLAG)?(DI--):(DI++);
	    }
	    PC += 1;
	    goto next_switch;

/*6f*/	case OUTSw:
	    atmp = FETCH_WORD(LONG_ES+DI);
	    goto com6fe0;
/*ef*/	case OUTvw:
	    atmp = AX;
com6fe0:
	    hsw_port_outw(DX, atmp);
	    if (FETCH_BYTE(PC)==OUTSw) {
		(iENV->flags & DIRECTION_FLAG)?(DI-=2):(DI+=2);
	    }
	    PC += 1;
	    goto next_switch;

/*e8*/	case CALLd: {
	    DWORD ip = PC - LONG_CS + 3;
	    PUSHWORD(ip);
	    PC = LONG_CS + ((ip + (signed short)FETCH_WORD((PC+1))) & 0xffff);
	    } goto next_switch;
/*e9*/	case JMPd: {
	    DWORD ip = PC - LONG_CS + 3;
	    PC = LONG_CS + ((ip + (signed short)FETCH_WORD((PC+1))) & 0xffff);
	    } goto next_switch;
/*ea*/	case JMPld: {
	    DWORD cs, ip;
	    WORD transfer_magic;
	    ip = FETCH_WORD(PC+1);
	    cs = (FETCH_WORD(PC+3) & 0xffff);
	    transfer_magic = (WORD)GetSelectorType(cs);
	    if (VM86F || (transfer_magic == TRANSFER_CODE16)) {
		if ((*err = SET_SEGREG(LONG_CS,BIG_CS,MK_CS,cs))==EXCP0B_NOSEG) {
		    iVAR->error_addr=cs&0xfffc; return P0; }
		SHORT_CS_16 = (WORD)cs;
		PC = ip + LONG_CS;
		goto next_switch;
	    }
#ifndef DOSEMU
	    else if ((transfer_magic == TRANSFER_CALLBACK) || 
	    		(transfer_magic == TRANSFER_BINARY))  {
		LONGPROC conv,targ;
		SEGIMAGE *lpSegImage = &((SEGIMAGE *)
	    		(*(long *)(SELECTOR_PADDRESS(cs))))[ip>>3];
		targ = (LONGPROC)lpSegImage->targ;
		conv = (LONGPROC)lpSegImage->conv;
		EBP = (long)LONG_SS + EBP;
		ESP = (long)LONG_SS + ESP;
		trans_interp_flags(ENVARGS);   
		if (transfer_magic == TRANSFER_CALLBACK) {
		    iENV->trans_addr = (BINADDR)MAKELONG(ip,cs);
#ifndef	NO_TRACE_MSGS
		    e_printf("do_ext: jump to %s\n", GetProcName(cs,ip>>3));
#endif
		}
#ifndef	NO_TRACE_MSGS
		else    /* TRANSFER_BINARY */
		    e_printf("do_ext: jumping to binary thunk %x:%x\n",
		    	(int)cs, (int)ip);
#endif
		(conv)(env,targ);
#ifndef NO_DEBUG_MSGS
		LogProcName(AX,DX,0);
#endif
                SHORT_CS_16 = cs = iENV->return_addr >> 16;
                ip = iENV->return_addr & 0xffff;
		SET_SEGREG(LONG_CS,BIG_CS,MK_CS,cs);
		SET_SEGREG(LONG_DS,BIG_DS,MK_DS,SHORT_DS_16);
		SET_SEGREG(LONG_ES,BIG_ES,MK_ES,SHORT_ES_16);
		SET_SEGREG(LONG_SS,BIG_SS,MK_SS,SHORT_SS_16);
                EBP = EBP - (long)LONG_SS;
                ESP = ESP - (long)LONG_SS;
                trans_flags_to_interp(ENVARGS, iENV->flags);
                PC = LONG_CS + ip; goto next_switch;
	    }
	    else if (transfer_magic == TRANSFER_RETURN) {
		iENV->return_addr = (cs << 16) | ip;
		trans_interp_flags(ENVARGS);
		EBP = EBP + (long)LONG_SS;
		ESP = ESP + (long)LONG_SS;
		return PC;
	    }
#endif
	    else
		invoke_data(ENVARGS);    /* TRANSFER_DATA or garbage */
	    }
/*eb*/	case JMPsid: 
/**/	if (PC[1]==0xfe) {
	    FatalAppExit(0, "LOOP");
	    exit(1);
	}
	JUMP((PC+1)); goto fast_switch;

/*f0*/	case LOCK:
	    CEmuStat |= CeS_LOCK;
	    PC += 1; goto next_switch;
	case 0xf1:    /* illegal on 8086 and 80x86 */
	    goto illegal_op;

/*f2*/	case REPNE:
/*f3*/	case REP:     /* also is REPE */
	{
	    unsigned int count = CX;	/* UNsigned! */
	    register unsigned int i;
	    int longd = 2;
	    BYTE repop,test;

	    repop = (*PC-REPNE);	/* 0 test !=, 1 test == */
	    IncEMUtime(count);
	    PC += 2;
segrep:
	    switch (*(PC-1)){
		case INSb: {
		    BYTE *dest;
		    if (count == 0) goto next_switch;
		    dest = (BYTE *)(LONG_ES+DI);
		    i = count;
		    if (iENV->flags & DIRECTION_FLAG) {
			while (i--) {PUT_BYTE(dest,hsw_port_inb(DX)); dest--;}
			DI -= count;
		    } else {
			while (i--) {PUT_BYTE(dest,hsw_port_inb(DX)); dest++;}
			DI += count;
		    }
		    CX = 0; instr_count += count;
		    }
		case INSw: {
		    int lcount = count * longd;
		    BYTE *dest = (BYTE *)(LONG_ES+DI);
		    if (count == 0) goto next_switch;
		    if (iENV->flags & DIRECTION_FLAG) lcount = -lcount;
		    i = count;
		    if (longd==2) {
		      if (iENV->flags & DIRECTION_FLAG) {
			while (i--) {PUT_WORD(dest,hsw_port_inw(DX)); dest-=2;}
		      } else {
			while (i--) {PUT_WORD(dest,hsw_port_inw(DX)); dest+=2;}
		      }
		    }
		    else {
		      if (iENV->flags & DIRECTION_FLAG) {
			while (i--) {PUT_QUAD(dest,hsw_port_ind(DX)); dest-=4;}
		      } else {
			while (i--) {PUT_QUAD(dest,hsw_port_ind(DX)); dest+=4;}
		      }
		    }
		    DI += lcount; instr_count += count;
		    CX = 0; goto next_switch;
		    }
		case OUTSb: {
		    BYTE *src;
		    if (count == 0) goto next_switch;
		    i = count;
		    src = (BYTE *)(LONG_DS+SI);
		    if (iENV->flags & DIRECTION_FLAG) {
			while (i--) {hsw_port_outb(DX,FETCH_BYTE(src)); src--;}
			SI -= count;
		    } else {
			while (i--) {hsw_port_outb(DX,FETCH_BYTE(src)); src++;}
			SI += count;
		    }
		    CX = 0; instr_count += count;
		    }
		case OUTSw: {
		    int lcount = count * longd;
		    BYTE *src = (BYTE *)(LONG_DS + SI);
		    if (count == 0) goto next_switch;
		    if (iENV->flags & DIRECTION_FLAG) lcount = -lcount;
		    i = count;
		    if (longd==2) {
		      if (iENV->flags & DIRECTION_FLAG) {
			while (i--) {hsw_port_outw(DX,FETCH_WORD(src)); src-=2;}
		      } else {
			while (i--) {hsw_port_outw(DX,FETCH_WORD(src)); src+=2;}
		      }
		    }
		    else {
		      if (iENV->flags & DIRECTION_FLAG) {
			while (i--) {hsw_port_outd(DX,FETCH_QUAD(src)); src-=4;}
		      } else {
			while (i--) {hsw_port_outd(DX,FETCH_QUAD(src)); src+=4;}
		      }
		    }
		    SI += lcount; instr_count += count;
		    CX = 0; goto next_switch;
		    } 
		case MOVSb: {
		    BYTE *src, *dest;
		    if (count == 0) goto next_switch;
		    src  = (BYTE *)(OVERR_DS+SI);
		    dest = (BYTE *)(LONG_ES+DI);
		    i = count;
#ifndef	NO_TRACE_MSGS
		    if (d_emu>4) e_printf("------- [EA=%p->%p]\n",(ea_=src),dest);
#endif
		    if (iENV->flags & DIRECTION_FLAG) {
			while (i--) {PUT_BYTE(dest,FETCH_BYTE(src)); src--,dest--;}
			DI -= count; SI -= count;
		    } else {
			while (i--) {PUT_BYTE(dest,FETCH_BYTE(src)); src++,dest++;}
			DI += count; SI += count;
		    }
		    CX = 0; instr_count += count;
		    goto next_switch;
		    } 
		case MOVSw: {
		    int lcount = count * longd;
		    BYTE *src  = (BYTE *)(OVERR_DS+SI);
		    BYTE *dest = (BYTE *)(LONG_ES+DI);
		    if (count == 0) goto next_switch;
		    if (iENV->flags & DIRECTION_FLAG) lcount = -lcount;
		    i = count;
		    if (longd==2) {
#ifndef	NO_TRACE_MSGS
		      if (d_emu>4) e_printf("------- [EA=%p->%p]\n",(ea_=(BYTE *)src),dest);
#endif
		      if (lcount < 0) {
			while (i--) {PUT_WORD(dest,FETCH_WORD(src)); src-=2,dest-=2;}
		      } else {
			while (i--) {PUT_WORD(dest,FETCH_WORD(src)); src+=2,dest+=2;}
		      }
		    } else {
#ifndef	NO_TRACE_MSGS
		      if (d_emu>4) e_printf("------- [EA=%p->%p]\n",(ea_=(BYTE *)src),dest);
#endif
		      if (lcount < 0) {
			while (i--) {PUT_QUAD(dest,FETCH_QUAD(src)); src-=4,dest-=4;}
		      } else {
			while (i--) {PUT_QUAD(dest,FETCH_QUAD(src)); src+=4,dest+=4;}
		      }
		    } 
		    DI += lcount; SI += lcount;
		    CX = 0; instr_count += count;
		    goto next_switch;
		    }
		case CMPSb: {
		    BYTE *src, *dest, *ovr;
		    DWORD src1=0, src2=0, oldcnt; int res;
		    if (count == 0) goto next_switch;
		    oldcnt = count;
		    src = (ovr=OVERR_DS) + (SI);
		    dest = LONG_ES + DI;
		    if (iENV->flags & DIRECTION_FLAG) { /* backwards */
			while (count) {
			    src1 = FETCH_BYTE(src); src--;
			    src2 = FETCH_BYTE(dest); dest--;
			    test = (src1 != src2) ^ repop;
			    if (test) count--;
			    else {
				res = src1 - src2;
				instr_count += (oldcnt-count);
				CX = count - 1;
				DI = dest - LONG_ES;
				SI = src - ovr;
				SETBFLAGS(1);
				goto next_switch;
			    }
			}
		    } else {		      /* forwards */
			while (count) {
			    src1 = FETCH_BYTE(src); src++;
			    src2 = FETCH_BYTE(dest); dest++;
			    test = (src1 != src2) ^ repop;
			    if (test) count--;
			    else {
				res = src1 - src2;
				instr_count += (oldcnt-count);
				CX = count - 1;
				DI = dest - LONG_ES;
				SI = src - ovr;
				SETBFLAGS(1);
				goto next_switch;
			    }
			}
		    }
		    res = src1 - src2;
		    instr_count += count;
		    CX = 0; DI = dest - LONG_ES;
		    SI = src - ovr;
		    SETBFLAGS(1);
		    } goto next_switch;
		case CMPSw: {
		    BYTE *src, *dest, *ovr;
		    DWORD res, src1=0, src2=0, oldcnt;
		    if (count == 0) goto next_switch;
		    oldcnt = count;
		    src = (ovr=OVERR_DS) + (SI);
		    dest = LONG_ES + DI;
		    if (iENV->flags & DIRECTION_FLAG) { /* backwards */
			while (count) {
			    if (longd==4) {
			      src1 = FETCH_QUAD(src);
			      src2 = FETCH_QUAD(dest);
			    }
			    else {
			      src1 = FETCH_WORD(src);
			      src2 = FETCH_WORD(dest);
			    }
			    src -= longd; dest -= longd;
			    test = (src1 != src2) ^ repop;
			    if (test) count--;
			    else {
				res = src1 - src2;
				instr_count += (oldcnt-count);
				CX = count - 1;
				DI = dest - LONG_ES;
				SI = src - ovr;
				if (longd==2) {
				  SETWFLAGS(1);
				}
				else {
				  SETDFLAGS(1,1);
				}
				goto next_switch;
			    }
			}
		    } else {		      /* forwards */
			while (count) {
			    if (longd==4) {
			      src1 = FETCH_QUAD(src);
			      src2 = FETCH_QUAD(dest);
			    }
			    else {
			      src1 = FETCH_WORD(src);
			      src2 = FETCH_WORD(dest);
			    }
			    src += longd; dest += longd;
			    test = (src1 != src2) ^ repop;
			    if (test) count--;
			    else {
				res = src1 - src2;
				instr_count += (oldcnt-count);
				CX = count - 1;
				DI = dest - LONG_ES;
				SI = src - ovr;
				if (longd==2) {
				  SETWFLAGS(1);
				}
				else {
				  SETDFLAGS(1,1);
				}
				goto next_switch;
			    }
			}
		    }
		    res = src1 - src2;
		    instr_count += count;
		    CX = 0; DI = dest - LONG_ES;
		    SI = src - ovr;
		    if (longd==2) {
		      SETWFLAGS(1);
		    }
		    else {
		      SETDFLAGS(1,1);
		    }
		    } goto next_switch;
		case STOSb: {
		    BYTE *dest;
		    register BYTE al;
		    if (count == 0) goto next_switch;
		    i = count;
		    dest = (BYTE *)(LONG_ES + DI);
#ifndef	NO_TRACE_MSGS
		    if (d_emu>4) e_printf("------- [EA=%p]\n",(ea_=dest));
#endif
		    al = AL;
		    if (iENV->flags & DIRECTION_FLAG) { /* backwards */
			while (i--) {PUT_BYTE(dest,al); dest--;}
			DI -= count;
		    } else {		      /* forwards */
			while (i--) {PUT_BYTE(dest,al); dest++;}
			DI += count;
		    }
		    CX = 0; instr_count += count;
		    goto next_switch;
		    } 
		case STOSw: {
		    BYTE *dest;
		    register DWORD ddat=EAX;
		    if (count == 0) goto next_switch;
		    i = count;
		    dest = (BYTE *)(LONG_ES + DI);
#ifndef	NO_TRACE_MSGS
		    if (d_emu>4) e_printf("------- [EA=%p]\n",(ea_=dest));
#endif
		    if (iENV->flags & DIRECTION_FLAG) { /* backwards */
			if (longd==4)
			    while (i--) {PUT_QUAD(dest,ddat); dest-=4;}
			else
			    while (i--) {PUT_WORD(dest,ddat); dest-=2;}
			DI -= count * longd;
		    } else {		      /* forwards */
			if (longd==4)
			    while (i--) {PUT_QUAD(dest,ddat); dest+=4;}
			else
			    while (i--) {PUT_WORD(dest,ddat); dest+=2;}
			DI += count * longd;
		    }
		    CX = 0; instr_count += count;
		    goto next_switch;
		    }
		case LODSb:
		    goto not_implemented;
		case LODSw:
		    goto not_implemented;
		case SCASb: {
		    BYTE *dest;
		    DWORD src1, src2=0, oldcnt; int res;
		    if (count == 0) goto next_switch;
		    oldcnt = count;
		    dest = LONG_ES + DI;
#ifndef	NO_TRACE_MSGS
		    if (d_emu>4) e_printf("------- [EA=%p]\n",(ea_=dest));
#endif
		    src1 = AL;
		    if (iENV->flags & DIRECTION_FLAG) { /* backwards */
			while (count) {
			    src2 = FETCH_BYTE(dest); dest--;
			    test = (src1 != src2) ^ repop;
			    if (test) count--;
			    else {
				res = src1 - src2;
				instr_count += (oldcnt-count);
				CX = count - 1;
				DI = dest - LONG_ES;
				SETBFLAGS(1);
                                if((src1 & 0xf) < src2) SET_AF
                                else CLEAR_AF
				goto next_switch;
			    }
			}
		    } else {		      /* forwards */
			while (count) {
			    src2 = FETCH_BYTE(dest); dest++;
			    test = (src1 != src2) ^ repop;
			    if (test) count--;
			    else {
				res = src1 - src2;
				instr_count += (oldcnt-count);
				CX = count - 1;
				DI = dest - LONG_ES;
				SETBFLAGS(1);
                                if((src1 & 0xf) < src2) SET_AF
                                else CLEAR_AF
				goto next_switch;
			    }
			}
		    }
		    res = src1 - src2;
		    instr_count += count;
		    CX = 0; DI = dest - LONG_ES;
		    SETBFLAGS(1);
                    if((src1 & 0xf) < src2) SET_AF
                    else CLEAR_AF
		    } goto next_switch;
		case SCASw: {
		    BYTE *dest;
		    DWORD res, src1, oldcnt;
		    int src2=0;
		    if (count == 0) goto next_switch;
		    oldcnt = count;
		    dest = LONG_ES + DI;
#ifndef	NO_TRACE_MSGS
		    if (d_emu>4) e_printf("------- [EA=%p]\n",(ea_=dest));
#endif
		    src1 = AX;
		    if (iENV->flags & DIRECTION_FLAG) { /* backwards */
			while (count) {
			    src2 = (longd==4? FETCH_QUAD(dest) : FETCH_WORD(dest));
			    dest -= longd;
			    test = ((int)src1 != src2) ^ repop;
			    if (test) count--;
			    else {
				res = src1 - src2;
				instr_count += (oldcnt-count);
				CX = count - 1;
				DI = dest - LONG_ES;
				if (longd==2) {
				  SETWFLAGS(1);
				}
				else {
				  SETDFLAGS(1,1);
				}
				if(((int)src1 & 0xf) < src2) SET_AF
                                else CLEAR_AF
				goto next_switch;
			    }
			}
		    } else {		      /* forwards */
			while (count) {
			    src2 = (longd==4? FETCH_QUAD(dest) : FETCH_WORD(dest));
			    dest += longd;
			    test = ((int)src1 != src2) ^ repop;
			    if (test) count--;
			    else {
				res = src1 - src2;
				instr_count += (oldcnt-count);
				CX = count - 1;
				DI = dest - LONG_ES;
				if (longd==2) {
				  SETWFLAGS(1);
				}
				else {
				  SETDFLAGS(1,1);
				}
				if(((int)src1 & 0xf) < src2) SET_AF
                                else CLEAR_AF
				goto next_switch;
			    }
			}
		    } 
		    res = src1 - src2;
		    instr_count += count;
		    CX = 0; DI = dest - LONG_ES;
		    if (longd==2) {
		      SETWFLAGS(1);
		    }
		    else {
		      SETDFLAGS(1,1);
		    }
		    if(((int)src1 & 0xf) < src2) SET_AF
                    else CLEAR_AF
		    } goto next_switch;
/*------------------IA-----------------------------------*/
        	case SEGes:
		    OVERR_DS = OVERR_SS = LONG_ES;
            	    PC+=1; goto segrep;
        	case SEGcs:
		    OVERR_DS = OVERR_SS = LONG_CS;
            	    PC+=1; goto segrep;
        	case SEGss:
		    OVERR_DS = OVERR_SS = LONG_SS;
            	    PC+=1; goto segrep;
        	case SEGds:
		    OVERR_DS = OVERR_SS = LONG_DS;
            	    PC+=1; goto segrep;
        	case SEGfs:
		    OVERR_DS = OVERR_SS = LONG_FS;
            	    PC+=1; goto segrep;
        	case SEGgs:
		    OVERR_DS = OVERR_SS = LONG_GS;
            	    PC+=1; goto segrep;
		case OPERoverride:	/* 0x66 */
		    longd = 4;
		    PC+=1; goto segrep;
		case ADDRoverride:	/* 0x67 */
		    PC=P0;	/* re-parse all */
		    code32 = 1;
		    PC = hsw_interp_16_32 (ENVARGS, P0, PC, err);
		    code32 = 0;
		    if (*err) return (*err==EXCP_GOBACK? PC:P0);
		    goto next_switch;
/*------------------IA------------------------------------*/
		default: PC--; goto not_implemented;
	    } }

/*f4*/	case HLT:
#ifdef DOSEMU
	    goto not_permitted;
#else
	    EBP = EBP + (long)LONG_SS;
	    ESP = ESP + (long)LONG_SS;
	    return PC;
#endif
/*f5*/	case CMC:
	    CARRY ^= CARRY_FLAG;
	    PC += 1; goto next_switch;
/*f6*/	case GRP1brm: {
	    DWORD src1, src2; int res;
	    res = (*(PC+1)>>3)& 0x7;
	    PC += hsw_modrm_16_byte(MODARGS);
	    mem_ref = MEM_REF; 
	    switch (res) {
		case 0: /* TEST */
		case 1: /* Undocumented */
		    res = *mem_ref & *PC; PC += 1;
		    SRC1_32 = SRC2_32 = RES_32 = BYTEDUP(res);	/* CF=OF=0 */
		    goto next_switch;
		case 2: /* NOT */
		    src1 = ~(*mem_ref);
		    *mem_ref = (BYTE)src1;
		    goto next_switch;
		case 3: /* NEG */
		    src1 = 0; src2 = *mem_ref;
		    res = (BYTE)(0x100-src2);
		    *mem_ref = (BYTE)res;
		    SETBFLAGS(1);
		    CARRYB = (src2 != 0);	/* not CARRY! */
		    goto next_switch;
		case 4: /* MUL AL */
		    src1 = *mem_ref;
		    src2 = AL;
		    res = src1 * src2;
		    AX = res;
		    if (res&0xff00) {	/* CF=OF=1 */
			CARRY=1; SRC1_16 = SRC2_16 = ~RES_16; /* 001,110 */
		    }
		    else {
			CARRY=0; SRC1_16 = SRC2_16 = RES_16;  /* 000,111 */
		    }
		    goto next_switch;
		case 5: /* IMUL AL */
		    src1 = *(signed char *)mem_ref;
		    src2 = (signed char)AL;
		    res = src1 * src2;
		    AX = res;
		    res = res >> 7;
		    if ((res==0)||(res==0xffffffff)) {  /* CF=OF=0 */
			CARRY=0; SRC1_16 = SRC2_16 = RES_16; /* 000,111 */
		    }
		    else {
			CARRY=1; SRC1_16 = SRC2_16 = ~RES_16;  /* 001,110 */
		    }
		    goto next_switch;
		case 6: /* DIV AL */
		    src1 = *mem_ref;
		    if (src1==0) goto div_by_zero;
		    res = AX;
		    AL = (BYTE)(res / src1);
		    AH = (BYTE)(res % src1);
		    goto next_switch;
		case 7: /* IDIV AX */
		    src1 = *(signed char *)mem_ref;
		    if (src1==0) goto div_by_zero;
		    res = (signed short)AX;
		    AL = (BYTE)(res / src1);
		    AH = (BYTE)(res % src1);
		    goto next_switch;
	    } }
/*f7*/	case GRP1wrm: {
	    int src1, src2, res;
	    res = (*(PC+1)>>3)& 0x7;
	    PC += hsw_modrm_16_word(MODARGS);
	    mem_ref = MEM_REF;
	      switch (res) {
		case 1: /* TEST (Is this Illegal?) */
		    if (!IS_MODE_REG) goto illegal_op; 
		case 0: /* TEST */
		    src1 = mFETCH_WORD(mem_ref);
		    src2 = FETCH_WORD(PC); PC += 2;
		    res = src1 & src2;
		    SRC1_32 = SRC2_32 = RES_32 = res & 0xffff;	/* CF=OF=0 */
		    goto next_switch;
		case 2: /* NOT */
		    src1 = mFETCH_WORD(mem_ref);
		    src1 = ~(src1);
		    mPUT_WORD(mem_ref, src1);
		    goto next_switch;
		case 3: /* NEG */
		    src1 = 0; /* equiv to SUB 0,src2 */
		    src2 = mFETCH_WORD(mem_ref);
		    res = (WORD)(0x10000-src2);
		    mPUT_WORD(mem_ref, res);
		    SETWFLAGS(1);
		    CARRY = (src2 != 0);	/* not CARRYB! */
		    goto next_switch;
		case 4: /* MUL AX */
		    src1 = mFETCH_WORD(mem_ref);
		    src2 = AX;
		    res = src1 * src2;
		    AX = res; DX = res >> 16;
		    if (res&0xffff0000) {	/* CF=OF=1 */
			CARRY=1; SRC1_16 = SRC2_16 = ~RES_16; /* 001,110 */
		    }
		    else {
			CARRY=0; SRC1_16 = SRC2_16 = RES_16;  /* 000,111 */
	      }
		    goto next_switch;
		case 5: /* IMUL AX */
		    src1 = (signed short)mFETCH_WORD(mem_ref);
		    src2 = (signed short)AX;
		    res = src1 * src2;
		    AX = res; DX = res >> 16;
		    res >>= 15;
		    if ((res==0)||(res==0xffffffff)) {  /* CF=OF=0 */
			CARRY=0; SRC1_16 = SRC2_16 = RES_16; /* 000,111 */
		    }
		    else {
			CARRY=1; SRC1_16 = SRC2_16 = ~RES_16;  /* 001,110 */
		    }
		    goto next_switch;
		case 6: /* DIV DX+AX */
		    src1 = mFETCH_WORD(mem_ref);
		    if (src1==0) goto div_by_zero;
		    res = AX;
		    res |= (DX<<16);
		    AX = res / src1;
		    DX = res % src1;
		    goto next_switch;
		case 7: /* IDIV DX+AX */
		    src1 = (signed short)mFETCH_WORD(mem_ref);
		    if (src1==0) goto div_by_zero;
		    res = AX;
		    res |= (DX<<16);
		    AX = res / src1;
		    DX = res % src1;
		    goto next_switch;
	      }
	      }

/*f8*/	case CLC:
	    CLEAR_CF;
	    PC += 1; goto next_switch;
/*f9*/	case STC:
	    SET_CF;
	    PC += 1; goto next_switch;
/*fa*/	case CLI:
#ifdef DOSEMU
	    if (!VM86F && (CPL <= IOPL))
		iENV->flags &= ~INTERRUPT_FLAG;
	    else
		goto not_permitted;
#endif
	    PC += 1; goto next_switch;
/*fb*/	case STI:
#ifdef DOSEMU
	    if (!VM86F && (CPL <= IOPL))
		iENV->flags |= INTERRUPT_FLAG;
	    else
		goto not_permitted;
#endif
	    PC += 1; goto next_switch;
/*fc*/	case CLD:
	    CLEAR_DF;
	    PC += 1; goto next_switch;
/*fd*/	case STD:
	    SET_DF;
	    PC += 1; goto next_switch;
/*fe*/	case GRP2brm: { /* only INC and DEC are legal on bytes */
	    register int temp;
	    temp = (*(PC+1)>>3)& 0x7;
	    PC += hsw_modrm_16_byte(MODARGS);
	    mem_ref = MEM_REF;
	    switch (temp) {
		case 0: /* INC */
		    SRC1_8 = temp = *mem_ref; SRC2_8 = 1;
		    temp += 1; *mem_ref = temp;
		    RES_16 = temp << 8; BYTE_FLAG = BYTE_OP;
		    goto next_switch;
		case 1: /* DEC */
		    SRC1_8 = temp = *mem_ref; SRC2_8 = -1;
		    temp -= 1; *mem_ref = temp;
		    RES_16 = temp << 8; BYTE_FLAG = BYTE_OP;
		    goto next_switch;
		case 2: /* CALL indirect */
		case 3: /* CALL long indirect */
		case 4: /* JMP indirect */
		case 5: /* JMP long indirect */
		case 6: /* PUSH */
		case 7: /* Illegal */
                    goto illegal_op; 
	    }}
/*ff*/	case GRP2wrm: {
	    register int temp;
	    DWORD jcs, jip, ocs=0, oip=0;
	    temp = (*(PC+1)>>3)& 0x7;
	    PC += hsw_modrm_16_word(MODARGS);
	    mem_ref = MEM_REF;
	      switch (temp) {
		case 0: /* INC */
		    temp = mFETCH_WORD(mem_ref);
		    SRC1_16 = (WORD)temp; SRC2_16 = 1;
		    temp += 1; RES_16 = (WORD)temp;
		    mPUT_WORD(mem_ref, temp);
		    goto next_switch;
		case 1: /* DEC */
		    temp = mFETCH_WORD(mem_ref);
		    SRC1_16 = (WORD)temp; SRC2_16 = -1;
		    temp -= 1; RES_16 = (WORD)temp;
		    mPUT_WORD(mem_ref, temp);
		    goto next_switch;
		case 2: /* CALL near indirect */
		    temp = PC - LONG_CS;
		    PUSHWORD(temp);
		    PC = LONG_CS + mFETCH_WORD(mem_ref);
		    goto next_switch;
		case 4: /* JMP near indirect */
		    PC = LONG_CS + mFETCH_WORD(mem_ref);
		    goto next_switch;
		case 3: /* CALL long indirect restartable */
		    ocs = SHORT_CS_16;
		    oip = PC - LONG_CS;
#ifndef DOSEMU
		    iENV->return_addr = (ocs << 16) | oip;
#endif
		    /* fall through */
		case 5:  { /* JMP long indirect restartable */
		    WORD transfer_magic;
		    jip = FETCH_WORD(mem_ref);
		    jcs = FETCH_WORD(mem_ref+2);
		    transfer_magic = (WORD)GetSelectorType(jcs);
		    if (VM86F || (transfer_magic==TRANSFER_CODE16)) {
			if ((*err = SET_SEGREG(LONG_CS,BIG_CS,MK_CS,jcs))==EXCP0B_NOSEG) {
			    iVAR->error_addr=jcs&0xfffc; return P0; }
			if (temp==3) {
			    PUSHWORD(ocs);
			    PUSHWORD(oip);
			}
			SHORT_CS_16 = (WORD)jcs;
			PC = jip + LONG_CS;
			goto next_switch;
		    }
#ifndef DOSEMU
		    if (temp==3) {
			PUSHWORD(ocs);
			PUSHWORD(oip);
		    }
		    if ((transfer_magic == TRANSFER_CALLBACK) ||
				(transfer_magic == TRANSFER_BINARY))  {
			LONGPROC conv,targ;
			SEGIMAGE *lpSegImage = &((SEGIMAGE *)
				(*(long *)(SELECTOR_PADDRESS(jcs))))[jip>>3];
			EBP = (long)LONG_SS + EBP;
			ESP = (long)LONG_SS + ESP;
			trans_interp_flags(ENVARGS);    
			targ = (LONGPROC)lpSegImage->targ;
			conv = (LONGPROC)lpSegImage->conv;
			if (transfer_magic == TRANSFER_CALLBACK) {
		    	    iENV->trans_addr = (BINADDR)MAKELONG(jip,jcs);
#ifndef	NO_TRACE_MSGS
			    LogProcName(jcs,jip,1);
#endif
			}
#ifndef	NO_TRACE_MSGS
			else    /* TRANSFER_BINARY */
			    e_printf("do_ext: calling binary thunk %x:%x\n",
			    	(int)jcs, (int)jip);
#endif
			(conv)(env,targ);
#ifndef NO_DEBUG_MSGS
			LogProcName(AX,DX,0);
#endif
			SHORT_CS_16 = jcs = iENV->return_addr >> 16;
			jip = iENV->return_addr & 0xffff;
			SET_SEGREG(LONG_CS,BIG_CS,MK_CS,jcs);
			SET_SEGREG(LONG_DS,BIG_DS,MK_DS,SHORT_DS_16);
			SET_SEGREG(LONG_ES,BIG_ES,MK_ES,SHORT_ES_16);
			SET_SEGREG(LONG_SS,BIG_SS,MK_SS,SHORT_SS_16);
			EBP = EBP - (long)LONG_SS;
			ESP = ESP - (long)LONG_SS;
			trans_flags_to_interp(ENVARGS, iENV->flags);
			PC = LONG_CS + jip; goto next_switch;
		    }
		    if (transfer_magic == TRANSFER_RETURN) {
			SHORT_CS_16 = jcs;
			iENV->return_addr = (jcs << 16) | jip;
			trans_interp_flags(ENVARGS);    
			EBP = EBP + (long)LONG_SS;
			ESP = ESP + (long)LONG_SS;
			return PC;
		    }
#endif
		    else
			invoke_data(ENVARGS);    /* TRANSFER_DATA or garbage */
		}
		case 6: /* PUSH */
		    temp = mFETCH_WORD(mem_ref);
		    PUSHWORD(temp);
		    goto next_switch;
		case 7: /* Illegal */
		    goto illegal_op;
	      }
	    }
	default:
	    goto not_implemented;
    } /* end of the switch statement */

not_implemented:
	d_emu=9;
	error(" 16/16 nonimplemented instruction %2x %2x %2x at %4x:%4x long PC %x\n",*P0,*(P0+1),*(P0+2),
		SHORT_CS_16,P0-LONG_CS,(int)P0);
#ifndef NO_TRACE_MSGS
	e_trace(ENVARGS, P0, P0, 0);
#endif
	FatalAppExit(0, "INSTR");
	exit(1);

bad_override:
	d_emu=9;
	error(" 16/16 bad code/data sizes at %4x:%4x long PC %x\n",
		SHORT_CS_16,P0-LONG_CS,(int)P0);
#ifndef NO_TRACE_MSGS
	e_trace(ENVARGS, P0, P0, 0);
#endif
	FatalAppExit(0, "SIZE");
	exit(1);
/*
bad_data:
	d_emu=9;
	e_printf(" 16/16 bad data at %4x:%4x long PC %x\n",
		SHORT_CS_16,P0-LONG_CS,(int)P0);
#ifndef NO_TRACE_MSGS
	e_trace(ENVARGS, P0, P0, 0);
#endif
	FatalAppExit(0, "OOPS");
        exit(1); 
*/
not_permitted:
#ifdef DOSEMU
	*err=EXCP0D_GPF; return P0;
#else
	FatalAppExit(0, "GPF");
	exit(1);
#endif

div_by_zero:
#ifdef DOSEMU
	*err=EXCP00_DIVZ; return P0;
#else
	FatalAppExit(0, "DIVZ");
	exit(1);
#endif

illegal_op:
	error(" 16/16 illegal instruction %2x %2x %2x at %4x:%4x long PC %x\n",*P0,*(P0+1),*(P0+2), 
		SHORT_CS_16,P0-LONG_CS,(int)P0);
#ifdef DOSEMU
	*err=EXCP06_ILLOP; return P0;
#else
	FatalAppExit(0, "ILLEG");
	exit(1);
#endif
}

#ifdef DOSEMU
int
invoke_code16 (ENVPARAMS, int vf, int cycmax)
{
#else	/* TWIN */
/**************************************************************************
    The interpreter is actually called with a BINADDR as its second
    parameter, but we will re-use this register as our PC instead,
    since that is all we really will use the passed address for
    anyway. mfh
**************************************************************************/
int
invoke_code16 (Interp_ENV *env)
{
#define vf	0	/* vf=0 for PM, vf=1 for Virtual/RM */
  int cycmax=1;	/* not used if called from here */
  Interp_VAR *interp_var = &interp_variables;
/*  extern void print_ldt(void); */
#endif
  static int first = 1;
  static int err = 0;
  BYTE *PC0;

#ifdef DOSEMU
  CEmuStat = (CEmuStat & ~CeS_MODE_MASK) | (vf? MODE_VM86:MODE_DPMI16);
  SHORT_CS_16 = (VM86F? (WORD)((DWORD)iENV->trans_addr>>16) : iENV->cs.cs);
#else
  CEmuStat = (CEmuStat & ~CeS_MODE_MASK) | MODE_DPMI16;
  SHORT_CS_16 = (WORD)((DWORD)iENV->trans_addr>>16);
#endif
  code32 = data32 = 0;
  SET_SEGREG (LONG_CS,BIG_CS,MK_CS,SHORT_CS_16);	/* unchecked */
  SET_SEGREG (LONG_DS,BIG_DS,MK_DS,SHORT_DS_16);
  SET_SEGREG (LONG_ES,BIG_ES,MK_ES,SHORT_ES_16);
  SET_SEGREG (LONG_SS,BIG_SS,MK_SS,SHORT_SS_16);
  SET_SEGREG (LONG_FS,BIG_FS,MK_FS,SHORT_FS_16);
  SET_SEGREG (LONG_GS,BIG_GS,MK_GS,SHORT_GS_16);
#ifndef DOSEMU
  EBP = EBP - (unsigned long)LONG_SS;
  ESP = ESP - (unsigned long)LONG_SS;
  instr_count = 1;
#endif

#ifdef __GNUC__
  if (first) {
#ifndef DOSEMU	/* TEMP init, to be moved to a better place */
    init_cpuemu();
/*    print_ldt();*/
#endif
    if (d_emu>1) {
      e_printf("CS: phys.addr for sel %03x is %08lx..%08lx\n",SHORT_CS_16,(long)LONG_CS,
	(long)GetSelectorAddrMax(SHORT_CS_16));
      e_printf("DS: phys.addr for sel %03x is %08lx..%08lx\n",SHORT_DS_16,(long)LONG_DS,
	(long)GetSelectorAddrMax(SHORT_DS_16));
      e_printf("ES: phys.addr for sel %03x is %08lx..%08lx\n",SHORT_ES_16,(long)LONG_ES,
	(long)GetSelectorAddrMax(SHORT_ES_16));
      e_printf("SS: phys.addr for sel %03x is %08lx..%08lx\n",SHORT_SS_16,(long)LONG_SS,
	(long)GetSelectorAddrMax(SHORT_SS_16));
    }
    first = 0;
  }
#endif
  PC0 = (BYTE *) LONG_CS + LOWORD (iENV->trans_addr);
  trans_flags_to_interp (ENVARGS, iENV->flags);

/**/ e_printf("INVOKE code16\n");
  PC0 = hsw_interp_16_16 (ENVARGS, PC0, PC0, &err, cycmax);

  if (VM86F) {
    if (err) iENV->return_addr = (SHORT_CS_16 << 16) | (PC0-LONG_CS);
  }
  else {
    iENV->return_addr = (PC0-LONG_CS);
  }
  if (err==EXCP_GOBACK) first=1;
  trans_interp_flags (ENVARGS);
  return err;
}

#ifdef DOSEMU
int
invoke_code32 (ENVPARAMS, int cycmax)
{
#else
int
invoke_code32 (Interp_ENV *env)
{
  int cycmax=1;	/* not used if called from here */
  Interp_VAR *interp_var = &interp_variables;
#endif
  static int first = 1;
  static int err = 0;
  BYTE *PC0;

  CEmuStat = (CEmuStat & ~CeS_MODE_MASK) | MODE_DPMI32;
  code32 = data32 = 1;
  SHORT_CS_16 = iENV->cs.cs;	/* Twin: should use trans_addr */
  SET_SEGREG (LONG_CS,BIG_CS,MK_CS,SHORT_CS_16);	/* unchecked */
  SET_SEGREG (LONG_DS,BIG_DS,MK_DS,SHORT_DS_16);
  SET_SEGREG (LONG_ES,BIG_ES,MK_ES,SHORT_ES_16);
  SET_SEGREG (LONG_SS,BIG_SS,MK_SS,SHORT_SS_16);
  SET_SEGREG (LONG_FS,BIG_FS,MK_FS,SHORT_FS_16);
  SET_SEGREG (LONG_GS,BIG_GS,MK_GS,SHORT_GS_16);
/* Twin: we cannot use 16/16 tricks for ESP/EBP like above */

#ifdef __GNUC__
  if (first) {
    if (d_emu>1) {
      e_printf("CS: physical addr for sel %03x is %08lx..%08lx\n",SHORT_CS_16,(long)LONG_CS,
	(long)GetSelectorAddrMax(SHORT_CS_16));
      e_printf("DS: physical addr for sel %03x is %08lx..%08lx\n",SHORT_DS_16,(long)LONG_DS,
	(long)GetSelectorAddrMax(SHORT_DS_16));
      e_printf("ES: physical addr for sel %03x is %08lx..%08lx\n",SHORT_ES_16,(long)LONG_ES,
	(long)GetSelectorAddrMax(SHORT_ES_16));
      e_printf("SS: physical addr for sel %03x is %08lx..%08lx\n",SHORT_SS_16,(long)LONG_SS,
	(long)GetSelectorAddrMax(SHORT_SS_16));
    }
    first = 0;
  }
#endif
  PC0 = (BYTE *) LONG_CS + iENV->trans_addr;
  trans_flags_to_interp (ENVARGS, iENV->flags);

/**/ e_printf("INVOKE code32\n");
  PC0 = hsw_interp_32_32 (ENVARGS, PC0, PC0, &err, cycmax);

  iENV->return_addr = (PC0-LONG_CS);
  if (err==EXCP_GOBACK) first=1;
  trans_interp_flags (ENVARGS);
  return err;
}

/**************************************************************************/

DWORD
trans_interp_flags(ENVPARAMS)
{
    register Interp_VAR_flags_czsp czsp = iVAR->flags_czsp;
    DWORD flags;

    /* turn off flag bits that we update here */
#ifdef DOSEMU
    /* clear OF,SF,ZF,AF,PF,CF,byte flag */
    flags = (env->flags & 0x3f7700) | 2;
#else
    flags = env->flags & (DIRECTION_FLAG | INTERRUPT_FLAG);
#endif

    /* byte operation? */
    /* convert into a word op, BYTE_FL is lost */
    if (czsp.byte.byte_op==BYTE_OP) {
	AUX1_8 = SRC1_8; AUX2_8 = SRC2_8;
	AUXR_8 = RES_8; BYTE_FLAG = 0;
    }
    flags |= (parity[czsp.byte.parity16] |
	(((SRC1_16 & 0xf) + (SRC2_16 & 0xf)) & AUX_CARRY_FLAG) |
	(czsp.byte.carryb & CARRY_FLAG) |
	(czsp.byte.res8 & SIGN_FLAG));
    if ((((czsp.byte.res8^SRC1_8)&0x80)!=0) &&
	(((SRC1_8^SRC2_8)&0x80)==0)) flags |= OVERFLOW_FLAG;
    if (czsp.word16.res16 == 0) flags |= ZERO_FLAG;

    /* put flags into the env structure */
    env->flags = flags;

    /* return flags */
    return(flags & ~BYTE_FL);
}

void
trans_flags_to_interp(ENVPARAMS, DWORD flags)
{
    register Interp_VAR_flags_czsp czsp;

    env->flags = (flags & ~BYTE_FL) | 0x202;
    /* initialize byte=carry=res8=parity16=0 */
    czsp.res = 0;
    SRC1_8 = 0; SRC2_8 = 0xff;

    /* deal with carry */
    czsp.byte.carryb = (BYTE)(flags & CARRY_FLAG);

    /* deal with overflow */
    if (flags & OVERFLOW_FLAG) {
	if (flags & SIGN_FLAG) SRC2_8 = 0; else SRC1_8 = 0xff;
    }

    if (flags & BYTE_FL) {
	czsp.byte.byte_op = BYTE_OP;
	/* deal with zero and sign - note that ZF|SF is not possible;
	 * also, when ZF set, PF will be 1 */
	if ((flags & ZERO_FLAG)==0) {
	    czsp.byte.res8 = (BYTE)((flags & SIGN_FLAG) | 1);
	    if ((flags & (SIGN_FLAG|PARITY_FLAG))==SIGN_FLAG)
		czsp.byte.res8 |= 2;	/* res8==0x83 -> parity=0 */
	    if (flags & AUX_CARRY_FLAG) {
		SRC2_8 |= 8; SRC1_8 |= 8;
	}
	}
	/* else (if zero) SF=0, PF=1 and AC=0 */
    }
    else {
	if ((flags & ZERO_FLAG)==0) {
	    czsp.byte.res8 = (BYTE)((flags & SIGN_FLAG) | 1);
	    if ((flags & PARITY_FLAG)==0) czsp.byte.parity16 = 1;
	    if (flags & AUX_CARRY_FLAG) {
		SRC2_16 |= 8; SRC1_16 |= 8;
	}
    }
    }
#ifdef DOSEMU
    if (flags & TRAP_FLAG) CEmuStat |= CeS_TRAP;
#endif
    iVAR->flags_czsp.res = czsp.res;
}

