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


/**********************************************************************

    @(#)hsw_interp.h	1.23
  
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

The maintainer of the Willows TWIN Libraries may be reached (Email) 
at the address twin@willows.com	

changes for use with dosemu-0.67 1997/10/20 vignani@tin.it
changes for use with dosemu-0.99 1998/12/13 vignani@tin.it

*********************************************************************/

#ifndef hsw_interp__h
#define hsw_interp__h
/* "@(#)hsw_interp.h	1.23 :/users/sccs/src/win/intp32/s.hsw_interp.h 1/23/97 17:25:08"  */

#include "internal.h"
#include <setjmp.h>

#if !defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
#error undefined sex
#endif

#ifdef DOSEMU
#define ONESTRUCT
#endif

#if !defined(DOSEMU) || defined(_IN_INTP32)

#define ADDbfrm		0x00
#define ADDwfrm		0x01
#define ADDbtrm		0x02
#define ADDwtrm		0x03
#define ADDbia		0x04
#define ADDwia		0x05
#define PUSHes		0x06
#define POPes		0x07
#define ORbfrm		0x08
#define ORwfrm		0x09
#define ORbtrm		0x0a
#define ORwtrm		0x0b
#define ORbi		0x0c
#define ORwi		0x0d
#define PUSHcs		0x0e
#define TwoByteESC	0x0f

#define ADCbfrm		0x10
#define ADCwfrm		0x11
#define ADCbtrm		0x12
#define ADCwtrm		0x13
#define ADCbi		0x14
#define ADCwi		0x15
#define PUSHss		0x16
#define POPss		0x17
#define SBBbfrm		0x18
#define SBBwfrm		0x19
#define SBBbtrm		0x1a
#define SBBwtrm		0x1b
#define SBBbi		0x1c
#define SBBwi		0x1d
#define PUSHds		0x1e
#define POPds		0x1f

#define ANDbfrm		0x20
#define ANDwfrm		0x21
#define ANDbtrm		0x22
#define ANDwtrm		0x23
#define ANDbi		0x24
#define ANDwi		0x25
#define SEGes		0x26
#define DAA		0x27
#define SUBbfrm		0x28
#define SUBwfrm		0x29
#define SUBbtrm		0x2a
#define SUBwtrm		0x2b
#define SUBbi		0x2c
#define SUBwi		0x2d
#define SEGcs		0x2e
#define DAS		0x2f

#define XORbfrm		0x30
#define XORwfrm		0x31
#define XORbtrm		0x32
#define XORwtrm		0x33
#define XORbi		0x34
#define XORwi		0x35
#define SEGss		0x36
#define AAA		0x37
#define CMPbfrm		0x38
#define CMPwfrm		0x39
#define CMPbtrm		0x3a
#define CMPwtrm		0x3b
#define CMPbi		0x3c
#define CMPwi		0x3d
#define SEGds		0x3e
#define AAS		0x3f

#define INCax		0x40
#define INCcx		0x41
#define INCdx		0x42
#define INCbx		0x43
#define INCsp		0x44
#define INCbp		0x45
#define INCsi		0x46
#define INCdi		0x47
#define DECax		0x48
#define DECcx		0x49
#define DECdx		0x4a
#define DECbx		0x4b
#define DECsp		0x4c
#define DECbp		0x4d
#define DECsi		0x4e
#define DECdi		0x4f

#define PUSHax		0x50
#define PUSHcx		0x51
#define PUSHdx		0x52
#define PUSHbx		0x53
#define PUSHsp		0x54
#define PUSHbp		0x55
#define PUSHsi		0x56
#define PUSHdi		0x57
#define POPax		0x58
#define POPcx		0x59
#define POPdx		0x5a
#define POPbx		0x5b
#define POPsp		0x5c
#define POPbp		0x5d
#define POPsi		0x5e
#define POPdi		0x5f

#define PUSHA		0x60
#define POPA		0x61
#define BOUND		0x62
#define ARPL		0x63
#define SEGfs		0x64
#define SEGgs		0x65
#define OPERoverride	0x66
#define ADDRoverride	0x67
#define PUSHwi		0x68
#define IMULwrm		0x69 
#define PUSHbi		0x6a
#define IMULbrm		0x6b
#define INSb		0x6c
#define INSw		0x6d
#define OUTSb		0x6e
#define OUTSw		0x6f

#define JO		0x70
#define JNO		0x71
#define JB_JNAE		0x72
#define JNB_JAE		0x73
#define JE_JZ		0x74
#define JNE_JNZ		0x75
#define JBE_JNA		0x76
#define JNBE_JA		0x77
#define JS		0x78
#define JNS		0x79
#define JP_JPE		0x7a
#define JNP_JPO		0x7b
#define JL_JNGE		0x7c
#define JNL_JGE		0x7d
#define JLE_JNG		0x7e
#define JNLE_JG		0x7f

#define IMMEDbrm	0x80
#define IMMEDwrm	0x81
#define IMMEDbrm2	0x82
#define IMMEDisrm	0x83
#define TESTbrm		0x84
#define TESTwrm		0x85
#define XCHGbrm		0x86
#define XCHGwrm		0x87
#define MOVbfrm		0x88
#define MOVwfrm		0x89
#define MOVbtrm		0x8a
#define MOVwtrm		0x8b
#define MOVsrtrm	0x8c
#define LEA		0x8d
#define MOVsrfrm	0x8e
#define POPrm		0x8f

#define NOP		0x90
#define XCHGcx		0x91
#define XCHGdx		0x92
#define XCHGbx		0x93
#define XCHGsp		0x94
#define XCHGbp		0x95
#define XCHGsi		0x96
#define XCHGdi		0x97
#define CBW		0x98
#define CWD		0x99
#define CALLl		0x9a
#define WAIT		0x9b
#define PUSHF		0x9c
#define POPF		0x9d
#define SAHF		0x9e
#define LAHF		0x9f

#define MOVmal		0xa0
#define MOVmax		0xa1
#define MOValm		0xa2
#define MOVaxm		0xa3
#define MOVSb		0xa4
#define MOVSw		0xa5
#define CMPSb		0xa6
#define CMPSw		0xa7
#define	TESTbi		0xa8
#define TESTwi		0xa9
#define STOSb		0xaa
#define STOSw		0xab
#define LODSb		0xac
#define LODSw		0xad
#define SCASb		0xae
#define SCASw		0xaf

#define MOVial		0xb0
#define MOVicl		0xb1
#define MOVidl		0xb2
#define MOVibl		0xb3
#define MOViah		0xb4
#define MOVich		0xb5
#define MOVidh		0xb6
#define MOVibh		0xb7
#define MOViax		0xb8
#define MOVicx		0xb9
#define MOVidx		0xba
#define MOVibx		0xbb
#define MOVisp		0xbc
#define MOVibp		0xbd
#define MOVisi		0xbe
#define MOVidi		0xbf

#define SHIFTbi		0xc0
#define SHIFTwi		0xc1 
#define RETisp		0xc2
#define RET		0xc3
#define LES		0xc4
#define LDS		0xc5
#define MOVbirm		0xc6
#define MOVwirm		0xc7
#define ENTER		0xc8
#define LEAVE		0xc9 
#define RETlisp		0xca
#define RETl		0xcb
#define INT3		0xcc
#define INT		0xcd
#define INTO		0xce
#define IRET		0xcf

#define SHIFTb		0xd0
#define SHIFTw		0xd1
#define SHIFTbv		0xd2
#define SHIFTwv		0xd3
#define AAM		0xd4
#define AAD		0xd5
#define RESERVED1	0xd6
#define XLAT		0xd7
#define ESC0		0xd8
#define ESC1		0xd9
#define ESC2		0xda
#define ESC3		0xdb
#define ESC4		0xdc
#define ESC5		0xdd
#define ESC6		0xde
#define ESC7		0xdf

#define LOOPNZ_LOOPNE	0xe0
#define LOOPZ_LOOPE	0xe1
#define LOOP		0xe2
#define JCXZ		0xe3
#define INb		0xe4
#define INw		0xe5
#define OUTb		0xe6
#define OUTw		0xe7
#define CALLd		0xe8
#define JMPd		0xe9
#define JMPld		0xea
#define JMPsid		0xeb
#define INvb		0xec
#define INvw		0xed
#define OUTvb		0xee
#define OUTvw		0xef

#define LOCK		0xf0
#define REPNE		0xf2
#define REP		0xf3
#define HLT		0xf4
#define CMC		0xf5
#define GRP1brm 	0xf6
#define GRP1wrm 	0xf7
#define CLC		0xf8
#define STC		0xf9
#define CLI		0xfa
#define STI		0xfb
#define CLD		0xfc
#define STD		0xfd
#define GRP2brm		0xfe
#define GRP2wrm		0xff

#ifdef FP87 /* [ */
#define FADDm32r_sti	((ESC0<<3) | (0x0 & 0x38) >> 3)
#define FMULm32r_sti	((ESC0<<3) | (0x8 & 0x38) >> 3)
#define FCOMm32r_sti	((ESC0<<3) | (0x10 /*or 0xd0*/ & 0x38) >> 3)
#define FCOMPm32r_sti	((ESC0<<3) | (0x18 /*or 0xd8*/ & 0x38) >> 3)
#define FSUBm32r_sti	((ESC0<<3) | (0x20 /*or 0xe0*/ & 0x38) >> 3)
#define FSUBRm32r_sti	((ESC0<<3) | (0x28 /*or 0xe8*/ & 0x38) >> 3)
#define FDIVm32r_sti	((ESC0<<3) | (0x30 /*or 0xf0*/ & 0x38) >> 3)
#define FDIVRm32r_sti	((ESC0<<3) | (0x38 /*or 0xf8*/ & 0x38) >> 3)

#define FLDm32r_sti  	((ESC1<<3) | (0x0 & 0x38) >> 3)
#define FXCH        	((ESC1<<3) | (0xc8 & 0x38) >> 3)
#define FSTm32r_FNOP	((ESC1<<3) | (0x10 /*or 0xd0*/ & 0x38) >> 3)
#define FSTPm32r    	((ESC1<<3) | (0x18 & 0x38) >> 3)
#define FD9SLASH4 		((ESC1<<3) | (0x20 /*or 0xe0-0xe5*/ & 0x38) >> 3)
#define    FLDENV 		0x20
#define    FCHS 		0xe0
#define    FABS 		0xe1
#define    FTST 		0xe4
#define    FXAM 		0xe5
#define FLDCONST		((ESC1<<3) | (0xe8 /*or 0xe9-0xee*/ & 0x38) >> 3)
#define    FLDCW		0x28
#define    FLD1 		0xe8
#define    FLDL2T		0xe9
#define    FLDL2E		0xea
#define    FLDPI		0xeb
#define    FLDLG2		0xec
#define    FLDLN2		0xed
#define    FLDZ 		0xee
#define FD9SLASH6		((ESC1<<3) | (0x30 /*or 0xf0-0xf7*/ & 0x38) >> 3)
#define    FSTENV 		0x30
#define    F2XM1 		0xf0
#define    FYL2X 		0xf1
#define    FPTAN 		0xf2
#define    FPATAN 		0xf3
#define    FXTRACT 		0xf4
#define    FPREM1 		0xf5
#define    FDECSTP		0xf6
#define    FINCSTP		0xf7
#define FD9SLASH7		((ESC1<<3) | (0x38 /*or 0xf8-0xff*/ & 0x38) >> 3)
#define    FSTCW 		0x38
#define    FPREM 		0xf8
#define    FYL2XP1 		0xf9
#define    FSQRT 		0xfa
#define    FSINCOS 		0xfb
#define    FRNDINT 		0xfc
#define    FSCALE 		0xfd
#define    FSIN			0xfe
#define    FCOS			0xff

#define FADDm32i		((ESC2<<3) | (0x0 & 0x38) >> 3)
#define FMULm32i		((ESC2<<3) | (0x8 & 0x38) >> 3)
#define FICOMm32i		((ESC2<<3) | (0x10 & 0x38) >> 3)
#define FICOMPm32i		((ESC2<<3) | (0x18 & 0x38) >> 3)
#define FISUBm32i		((ESC2<<3) | (0x20 & 0x38) >> 3)
#define FISUBRm32i_FUCOMPPst1	((ESC2<<3) | (0x28 /*or 0xe9*/ & 0x38) >> 3)
#define FIDIVm32i		((ESC2<<3) | (0x30 & 0x38) >> 3)
#define FIDIVRm32i		((ESC2<<3) | (0x38 & 0x38) >> 3)

#define FILDm32i		((ESC3<<3) | (0x0 & 0x38) >> 3)
#define FISTm32i		((ESC3<<3) | (0x10 & 0x38) >> 3)
#define FISTPm32i		((ESC3<<3) | (0x18 & 0x38) >> 3)
#define FRSTORm94B_FINIT_FCLEX	((ESC3<<3) | (0x20 /*or 0xe3-2*/ & 0x38) >> 3)
#define FLDm80r  		((ESC3<<3) | (0x28 & 0x38) >> 3)
#define FSTPm80r  		((ESC3<<3) | (0x38 & 0x38) >> 3)

#define FADDm64r_tosti	((ESC4<<3) | (0x0 & 0x38) >> 3)
#define FMULm64r_tosti	((ESC4<<3) | (0x8 & 0x38) >> 3)
#define FCOMm64r     	((ESC4<<3) | (0x10 & 0x38) >> 3)
#define FCOMPm64r    	((ESC4<<3) | (0x18 & 0x38) >> 3)
#define FSUBm64r_FSUBRfromsti ((ESC4<<3) | (0x20 /*or 0xe0*/ & 0x38) >> 3)
#define FSUBRm64r_FSUBfromsti ((ESC4<<3) | (0x28 /*or 0xe8*/ & 0x38) >> 3)
#define FDIVm64r_FDIVRtosti   ((ESC4<<3) | (0x30 /*or 0xf0*/ & 0x38) >> 3)
#define FDIVRm64r_FDIVtosti   ((ESC4<<3) | (0x38 /*or 0xf8*/ & 0x38) >> 3)

#define FLDm64r_FFREE         ((ESC5<<3) | (0x0 /*or 0xc0*/ & 0x38) >> 3)
#define FSTm64r_sti           ((ESC5<<3) | (0x10 /*or 0xd0+i*/ & 0x38) >> 3)
#define FSTPm64r_sti          ((ESC5<<3) | (0x18 /*or 0xd8+i*/ & 0x38) >> 3)
#define FUCOMsti              ((ESC5<<3) | (0x20 /*or 0xe0+i*/ & 0x38) >> 3)
#define FUCOMPsti             ((ESC5<<3) | (0x28 /*or 0xe8+i*/ & 0x38) >> 3)
#define FSAVEm94B             ((ESC5<<3) | (0x30 & 0x38) >> 3)
#define FSTSWm16i             ((ESC5<<3) | (0x38 & 0x38) >> 3)

#define FADDm16i_tostipop     ((ESC6<<3) | (0x0 & 0x38) >> 3)
#define FMULm16i_tostipop     ((ESC6<<3) | (0x8 & 0x38) >> 3)
#define FICOMm16i             ((ESC6<<3) | (0x10 & 0x38) >> 3)
#define FICOMPm16i_FCOMPPst1  ((ESC6<<3) | (0x18 /*or 0xd9*/ & 0x38) >> 3)
#define FISUBm16i_FSUBRPfromsti ((ESC6<<3) | (0x20 /*or 0xe0*/ & 0x38) >> 3)
#define FISUBRm16i_FSUBPfromsti ((ESC6<<3) | (0x28 /*or 0xe8*/ & 0x38) >> 3)
#define FIDIVm16i_FDIVRPtosti ((ESC6<<3) | (0x30 /*or 0xf0*/ & 0x38) >> 3)
#define FIDIVRm16i_FDIVPtosti ((ESC6<<3) | (0x38 /*or 0xf8*/ & 0x38) >> 3)

#define FILDm16i		((ESC7<<3) | (0x0 & 0x38) >> 3)
#define FISTm16i		((ESC7<<3) | (0x10 & 0x38) >> 3)
#define FISTPm16i		((ESC7<<3) | (0x18 & 0x38) >> 3)
#define FBLDm80dec_FSTSWax		((ESC7<<3) | (0x20 /*or 0xe0*/ & 0x38) >> 3)
#define FILDm64i		((ESC7<<3) | (0x28 & 0x38) >> 3)
#define FBSTPm80dec		((ESC7<<3) | (0x30 & 0x38) >> 3)
#define FISTPm64i		((ESC7<<3) | (0x38 & 0x38) >> 3)

#endif	/* ] */

/*
 *   control registers
 */
#define CR0_PE         0x00000001
#define CR0_MP         0x00000002
#define CR0_EM         0x00000004
#define CR0_TS         0x00000008
#define CR0_ET         0x00000010
#define CR0_NE         0x00000020
#define CR0_WP         0x00010000
#define CR0_AM         0x00040000
#define CR0_NW         0x20000000
#define CR0_CD         0x40000000
#define CR0_PG         0x80000000
#define CR0_RESERVED   0x1ffaffc0
#define CR0_NOT_IMP    0x00000000

#define CR3_PWT        0x00000008
#define CR3_PCD        0x00000010
#define CR3_PDB_SHIFT  12
#define CR3_PDB_MASK   0xfffff000
#define CR3_RESERVED   0x00000fe7
#define CR3_NOT_IMP    0x00000018

#define CR4_VME        0x00000001
#define CR4_PVI        0x00000002
#define CR4_TSD        0x00000004
#define CR4_DE         0x00000008
#define CR4_PSE        0x00000010
#define CR4_PAE        0x00000020
#define CR4_MCE        0x00000040
#define CR4_PGE        0x00000080
#define CR4_PCE        0x00000100
#define CR4_OSFXSR     0x00000200
#define CR4_RESERVED   0xfffffc00
#define CR4_NOT_IMP    0x0000012e

#define EFLAGS_CF      0x00000001
#define EFLAGS_SET     0x00000002
#define EFLAGS_PF      0x00000004
#define EFLAGS_AF      0x00000010
#define EFLAGS_ZF      0x00000040
#define EFLAGS_SF      0x00000080
#define EFLAGS_TF      0x00000100
#define EFLAGS_IF      0x00000200
#define EFLAGS_DF      0x00000400
#define EFLAGS_OF      0x00000800
#define EFLAGS_IOPL    0x00003000
#define EFLAGS_NT      0x00004000
#define EFLAGS_RF      0x00010000
#define EFLAGS_VM      0x00020000
#define EFLAGS_AC      0x00040000
#define EFLAGS_VIF     0x00080000
#define EFLAGS_VIP     0x00100000
#define EFLAGS_ID      0x00200000

#define EFLAGS_ALL       (0x003f7fd5)

#define EFLAGS_REAL_32   (EFLAGS_ALL & ~(EFLAGS_VIP|EFLAGS_VIF|EFLAGS_VM))
#define EFLAGS_V8086_32  (EFLAGS_ALL & ~(EFLAGS_VIP|EFLAGS_VIF|EFLAGS_VM|EFLAGS_IOPL))
#define EFLAGS_ALL_16    LOWORD(EFLAGS_ALL)
#define EFLAGS_REAL_16   LOWORD(EFLAGS_REAL_32)
#define EFLAGS_V8086_16  LOWORD(EFLAGS_V8086_32)
#define EFLAGS_CC        0x00000cd5

#define EFLAGS_IOPL_SHIFT 12
#define EFLAGS_IOPL_MASK  0x3000

/* www.sandpile.org/80x86/cpuid.shtml */
#define CPUID_FEATURE_FPU    0x00000001
#define CPUID_FEATURE_VME    0x00000002
#define CPUID_FEATURE_DBGE   0x00000004
#define CPUID_FEATURE_PGSZE  0X00000008
#define CPUID_FEATURE_TSC    0x00000010
#define CPUID_FEATURE_MSR    0x00000020
#define CPUID_FEATURE_PAE    0x00000040
#define CPUID_FEATURE_MCK    0x00000080
#define CPUID_FEATURE_CPMX   0x00000100
#define CPUID_FEATURE_APIC   0x00000200
#define CPUID_FEATURE_RSVD1  0x00000400
#define CPUID_FEATURE_SEP    0x00000800
#define CPUID_FEATURE_MTTR   0x00001000
#define CPUID_FEATURE_PGE    0x00002000 
#define CPUID_FEATURE_MCA    0x00004000
#define CPUID_FEATURE_CMOV   0x00008000 

#define CPUID_FEATURE_PAT    0x00010000
#define CPUID_FEATURE_36PG   0x00020000
#define CPUID_FEATURE_RSVD2  0x00040000 /* pentium II, mendocino? */
#define CPUID_FEATURE_RSVD3  0x00080000

#define CPUID_FEATURE_RSVD4  0x00100000
#define CPUID_FEATURE_RSVD5  0x00200000
#define CPUID_FEATURE_RSVD6  0x00400000
#define CPUID_FEATURE_MMX    0x00800000 
#define CPUID_FEATURE_FXSAVE 0x01000000
#define CPUID_FEATURE_PIII   0x02000000  /* pentium III, unknown */
#define CPUID_FEATURE_RSVD   0xfc780400 

#define MSR_TSC              0x00000010
#define MSR_MTRR_CAP         0x000000fe
#define MSR_SYSENTER_CS      0x00000174
#define MSR_SYSENTER_ESP     0x00000175
#define MSR_SYSENTER_EIP     0x00000176

/*
 *   segment selectors
 */
#define SELECTOR_GDT             0
#define SELECTOR_LDT             1
#define SELECTOR_RPL_SHIFT       0
#define SELECTOR_RPL_MASK        0x03
#define SELECTOR_TI_SHIFT        2
#define SELECTOR_TI_MASK         0x4
#define SELECTOR_INDEX_SHIFT     3
#define SELECTOR_INDEX_MASK      0xfff8

#define SELECTOR_RPL(_sel)       ((_sel) & SELECTOR_RPL_MASK)
#define SELECTOR_INDEX(_sel)     ((_sel) >> SELECTOR_INDEX_SHIFT)

#define NULL_SELECTOR(_sel)      (!((_sel) & ~SELECTOR_RPL_MASK))

/*
 *   segment descriptors
 */
typedef struct Descriptor {
   unsigned   limit_lo  : 16;
   unsigned   base_lo   : 16;
   unsigned   base_mid  : 8;
   unsigned   type      : 4;
   unsigned   S         : 1;
   unsigned   DPL       : 2;
   unsigned   present   : 1;
   unsigned   limit_hi  : 4;
   unsigned   AVL       : 1;
   unsigned   unused    : 1;
   unsigned   DB        : 1;
   unsigned   gran      : 1;
   unsigned   base_hi   : 8;
} Descriptor;

#define LIMIT(_desc)        (((_desc).limit_hi<<16) | (_desc).limit_lo)
#define BASE(_desc)         (((_desc).base_hi<<24) | ((_desc).base_mid<<16) | (_desc).base_lo)

#define LIMIT_LO(_dw)       (((int)(_dw)) & 0xffff)
#define LIMIT_HI(_dw)       ((((int)(_dw)) >> 16) & 0xf)

#define BASE_LO(_dw)        (((int)(_dw)) & 0xffff)
#define BASE_MID(_dw)       ((((int)(_dw)) >> 16) & 0xff)
#define BASE_HI(_dw)        ((((int)(_dw)) >> 24) & 0xff)

#define DT_CODE(_desc)               ( ((_desc).S) && (((_desc).type & 0x8) == 0x8))
#define DT_CONFORMING_CODE(_desc)    ( ((_desc).S) && (((_desc).type & 0xc) == 0xc))
#define DT_NONCONFORMING_CODE(_desc) ( ((_desc).S) && (((_desc).type & 0xc) == 0x8))
#define DT_READABLE_CODE(_desc)      ( ((_desc).S) && (((_desc).type & 0xa) == 0xa))
#define DT_DATA(_desc)               ( ((_desc).S) && (((_desc).type & 0x8) == 0x0))
#define DT_WRITEABLE_DATA(_desc)     ( ((_desc).S) && (((_desc).type & 0xa) == 0x2))
#define DT_EXPAND_DOWN(_desc)        ( ((_desc).S) && (((_desc).type & 0xc) == 0x4))
#define DT_CALL_GATE(_desc)          (!((_desc).S) && (((_desc).type & 0x7) == 0x4))
#define DT_LDT(_desc)                (!((_desc).S) && (((_desc).type & 0xf) == 0x2))
#define DT_TASK_GATE(_desc)          (!((_desc).S) && (((_desc).type & 0xf) == 0x5))
#define DT_TSS(_desc)                (!((_desc).S) && (((_desc).type & 0x5) == 0x1))
#define DT_AVAIL_TSS(_desc)          (!((_desc).S) && (((_desc).type & 0x7) == 0x1))

#define DT_ACCESS                    0x2
#define DT_32BIT                     0x8
#define DT_TSS_BUSY                  0x2

#define GT_TASK(_gate)               (((_gate).type & 0x1f) == 0x05)
#define GT_INTR(_gate)               (((_gate).type & 0x17) == 0x06)
#define GT_TRAP(_gate)               (((_gate).type & 0x17) == 0x07)
#define GT_32BIT                     0x08

#define DATA_DESC           0x2
#define CODE_DESC           0xa
#define TASK_DESC           0x9  /* TSS available */
#define TASK_DESC_BUSY      0xb  /* TSS busy */


#define CARRY_FLAG		0x0001
/* reserved			0x0002 */
#define PARITY_FLAG		0x0004
/* reserved			0x0008 */
#define AUX_CARRY_FLAG		0x0010
/* reserved			0x0020 */
#define ZERO_FLAG		0x0040
#define SIGN_FLAG		0x0080
#define TRAP_FLAG		0x0100
#define INTERRUPT_FLAG		0x0200
#define DIRECTION_FLAG		0x0400
#define OVERFLOW_FLAG		0x0800
#define IOPL_FLAG_MASK		0x3000
#define NESTED_FLAG		0x4000
#define BYTE_FL			0x8000	/* Intel reserved! */
#define RF_FLAG			0x10000
#define VM_FLAG			0x20000
/* AC				0x40000 */

#define CPL	0x3000
#define IOPL	(iENV->flags&IOPL_FLAG_MASK)

#define BYTE_OP 0x45

/*
 * Test condition uses:	RES	SRC1,SRC2
 *	Z,NZ		 Y	    N
 *	S,NS		 Y	    N
 *	CY,NC		 Y	    N
 *	P,NP		 Y	    N
 *	OV,NO		 Y	    Y
 *	AF		 N	    Y
 *	CX,LOOP		 N	    N
 *
 * beware of logical operations - all conditions _should_ return 0 or 1
 *
 *		[                  ][------word--------] SRCn_16
 *		[        ][        ][--byte--][--aux---]
 *				      SRCn_8
 */
#define IS_AF_SET (((SRC1_16 & 0xf0f) + (SRC2_16 & 0xf0f)) & \
	(BYTE_FLAG==BYTE_OP? 0x1000:0x10))

#define IS_CF_SET (BOOL)(CARRY & CARRY_FLAG)

/* overflow rule using bit15:
 *	src1 src2  res	b15
 *	  0    0    0    0
 *	  0    0    1    1
 *	  0    1    0    0
 *	  0    1    1    0
 *	  1    0    0    0
 *	  1    0    1    0
 *	  1    1    0    1
 *	  1    1    1    0
 */ 
#define IS_OF_SET (BOOL)((((~(SRC1_16 ^ SRC2_16))&(SRC1_16 ^ RES_16)) >> 15) & 1)

#define IS_ZF_SET (BOOL)(RES_16 == 0)
#define IS_SF_SET (BOOL)((RES_8 >> 7) & 1)
#define IS_PF_SET (parity[((BYTE_FLAG==BYTE_OP)?RES_8:PAR_16)]==PARITY_FLAG)

#define CLEAR_CF CARRYB=0
/* Wow. this changes also parity */
#define CLEAR_AF { SRC1_16 &= ~0x808; SRC2_16 &= ~0x808; }
#define CLEAR_DF iENV->flags &= ~DIRECTION_FLAG
/* this is safe */
#define CLEAR_ZF RES_8|=1

#define SET_CF CARRYB=1
/* Wow. this changes also parity */
#define SET_AF { SRC1_16 |= 0x808; SRC2_16 |= 0x808; }
#define SET_DF iENV->flags |= DIRECTION_FLAG
/* this way of setting ZF affects also SF,OF,PF,AF. Some operations like
 * LAR,LSL specify that ONLY ZF is affected. God save the Queen. */
#define SET_ZF RES_16=0

#define ROL	0
#define ROR	1
#define RCL	2
#define RCR	3
#define SHL	4
#define SHR	5
#define SAR	7

#define ADD	0
#define OR	1
#define ADC	2
#define SBB	3
#define AND	4
#define SUB	5
#define XOR	6
#define CMP	7

#define INC	0
#define DEC	1

#define TEST	0
#define NOT	2
#define NEG	3
#define MUL	4
#define IMUL	5
#define DIV	6
#define IDIV	7

#define AL  iENV->rax.x.x.h.l
#define AH  iENV->rax.x.x.h.h
#define AX  iENV->rax.x.x.x
#define EAX iENV->rax.e
#define BL  iENV->rbx.x.x.h.l
#define BH  iENV->rbx.x.x.h.h
#define BX  iENV->rbx.x.x.x
#define EBX iENV->rbx.e
#define CL  iENV->rcx.x.x.h.l
#define CH  iENV->rcx.x.x.h.h
#define CX  iENV->rcx.x.x.x
#define ECX iENV->rcx.e
#define DL  iENV->rdx.x.x.h.l
#define DH  iENV->rdx.x.x.h.h
#define DX  iENV->rdx.x.x.x
#define EDX iENV->rdx.e
#define SI  iENV->rsi.si.si
#define ESI iENV->rsi.esi
#define DI  iENV->rdi.di.di
#define EDI iENV->rdi.edi
#ifdef	BP
#undef	BP
#endif
#define BP  iENV->rbp.bp.bp
#define EBP iENV->rbp.ebp
#ifdef	SP
#undef	SP
#endif
#define SP  iENV->rsp.sp.sp
#define ESP iENV->rsp.esp
#define SHORT_CS_16 iENV->cs.word16.cs
#define SHORT_CS_32 iENV->cs.cs
#define SHORT_DS_16 iENV->ds.word16.ds
#define SHORT_DS_32 iENV->ds.ds
#define SHORT_ES_16 iENV->es.word16.es
#define SHORT_ES_32 iENV->es.es
#define SHORT_SS_16 iENV->ss.word16.ss
#define SHORT_SS_32 iENV->ss.ss
#define SHORT_FS_16 iENV->fs.word16.fs
#define SHORT_FS_32 iENV->fs.fs
#define SHORT_GS_16 iENV->gs.word16.gs
#define SHORT_GS_32 iENV->gs.gs
#define LONG_CS iVAR->seg_regs[0]
#define LONG_DS iVAR->seg_regs[1]
#define LONG_ES iVAR->seg_regs[2]
#define LONG_SS iVAR->seg_regs[3]
#define LONG_FS iVAR->seg_regs[4]
#define LONG_GS iVAR->seg_regs[5]
#define BIG_CS iVAR->bigseg[0]
#define BIG_DS iVAR->bigseg[1]
#define BIG_ES iVAR->bigseg[2]
#define BIG_SS iVAR->bigseg[3]
#define BIG_FS iVAR->bigseg[4]
#define BIG_GS iVAR->bigseg[5]

#define OVERR_DS iVAR->seg_regs[6]
#define OVERR_SS iVAR->seg_regs[7]
#define INVALID_OVR	((unsigned char *)(-1l))

#define SIGNb (res & 0x80)
#define SIGNw ((res & 0x8000) >> 8)
#define CARRYb ((res >> 8) & 0x1)
#define CARRYw ((res >> 16) & 0x1)
#define ZEROb (((unsigned char)(res) == 0) ? 0x40 : 0)
#define ZEROw (((unsigned short)(res) == 0) ? 0x40 : 0)
#define AUXCARRY 0
#define OVFLOWb (((!(res ^ src1)) & (!(src2 ^ src1)) << 4) & 0x800)
#define OVFLOWw (((!(res ^ src1)) & (!(src2 ^ src1)) >> 4) & 0x800)
#define PARITY (parity [(unsigned char)(res)])
#define ADDbFLAGS SIGNb | CARRYb | ZEROb | OVFLOWb | AUXCARRY | PARITY | flags & (~(DIRECTION_FLAG | INTERRUPT_FLAG))
#define ADDwFLAGS SIGNw | CARRYw | ZEROw | OVFLOWw | AUXCARRY | PARITY | flags & (~(DIRECTION_FLAG | INTERRUPT_FLAG))
#define LOGICbFLAGS SIGNb | ZEROb | PARITY | flags & (~(DIRECTION_FLAG | INTERRUPT_FLAG))
#define LOGICwFLAGS SIGNw | ZEROw | PARITY | flags & (~(DIRECTION_FLAG | INTERRUPT_FLAG))

#define AT_EXECUTE	1
#define AT_READ		2
#define AT_WRITE	3

#ifndef DOSEMU
#define ERRORLOG(s,o,m,r) fprintf(stderr,"Error accessing selector %x, offset %x, mode %x, errorcode %x\n",s,o,m,r)
#endif

#define UINT_86		2
#define INT_86		2
#define LP_86		4

#define JUMP(lp)	PC = PC + 2 +(signed char)(*(lp)); 

#define SELECTOR_PADDRESS(sel) GetPhysicalAddress(sel)

extern int SetSegreg(unsigned char **lp, unsigned char *big,
	unsigned long csel);
#define SET_SEGREG(lp,big,mk,sel)	SetSegreg(&(lp),&(big),(mk|sel))

#ifdef DOSEMU
int hsw_verr(unsigned short sel);
int hsw_verw(unsigned short sel);
#else
#define hsw_verr(s)	(1)
#define hsw_verw(s)	(1)
#endif

#endif

/* ======================= target-specific stuff ====================== */

#ifdef __i386__
/* use true 80-bit registers on i386 */
typedef	long double	Ldouble;
#else
/* use 64-bit doubles on other targets */
typedef	double	Ldouble;
#endif

typedef struct tagENV87
{   	Ldouble         fpregs[8];  /* floating point register stack */
	int             fpstt;      /* top of register stack */
	unsigned short  fpus, fpuc, fptag;
} ENV87;


#if defined(sparc) || defined(mips) || defined(ppc) || defined(hppa) || defined(arm)
/* BE layout:
 *		    b0        b1        b2       b3
 *		[-----------------res------------------] RES_32
 *	CARRY	[------carry-------][------res16-------] RES_16
 *		[byte_op-][-carryb-][--res8--][parity16]
 *		BYTE_FLAG	      RES_8
 */
typedef	union {
		unsigned long res;
		struct {
			unsigned short carry;
			unsigned short res16;
		} word16;
		struct {
			unsigned char byte_op;
			unsigned char carryb;
			unsigned char res8;
			unsigned char parity16;
		} byte;
	} Interp_VAR_flags_czsp;
#elif defined(alpha)
typedef	union {
		unsigned long res;
		struct {
			unsigned short res16;
			unsigned short carry;
			unsigned int dummy1;
		} word16;
		struct {
			unsigned char parity16;
			unsigned char res8;
			unsigned char carryb;
			unsigned char byte_op;
			unsigned int dummy1;
		} byte;
	} Interp_VAR_flags_czsp;
#elif defined(__i386__) 
/* LE layout:
 *		    b0        b1        b2       b3
 *		[-----------------res------------------] RES_32
 *	 RES_16 [------res16-------][------carry-------] CARRY
 *		[parity16][--res8--][-carryb-][byte_op-]
 *			    RES_8	      BYTE_FLAG
 */
typedef	union {
		unsigned long res;
		struct {
			unsigned short res16;
			unsigned short carry;
		} word16;
		struct {
			unsigned char parity16;
			unsigned char res8;
			unsigned char carryb;
			unsigned char byte_op;
		} byte;
	} Interp_VAR_flags_czsp;
#endif

#if defined(sparc) || defined(mips) || defined(ppc) || defined(hppa) || defined(arm)
typedef struct keyENV{
	union {
		unsigned int ds;
		struct {
			unsigned short ds_top;
			unsigned short ds;
		} word16;
	} ds;
	union {
		unsigned int es;
		struct {
			unsigned short es_top;
			unsigned short es;
		} word16;
	} es;
	union {
		unsigned int ss;
		struct {
			unsigned short ss_top;
			unsigned short ss;
		} word16;
	} ss;
	unsigned long flags;
	union {
		unsigned long e;
		struct {
			unsigned short dummy;
			union {
				unsigned short x;
				struct {
					unsigned char h;
					unsigned char l;
				} h;
			} x;
		} x;
	} rax;
	union {
		unsigned long e;
		struct {
			unsigned short dummy;
			union {
				unsigned short x;
				struct {
					unsigned char h;
					unsigned char l;
				} h;
			} x;
		} x;
	} rbx;
	union {
		unsigned long e;
		struct {
			unsigned short dummy;
			union {
				unsigned short x;
				struct {
					unsigned char h;
					unsigned char l;
				} h;
			} x;
		} x;
	} rcx;
	union {
		unsigned long e;
		struct {
			unsigned short dummy;
			union {
				unsigned short x;
				struct {
					unsigned char h;
					unsigned char l;
				} h;
			} x;
		} x;
	} rdx;
	union {
		unsigned long esi;
		struct {
			unsigned short si_top;
			unsigned short si;
		} si;
	} rsi;
	union {
		unsigned long edi;
		struct {
			unsigned short di_top;
			unsigned short di;
		} di;
	} rdi;
	union {
		unsigned long ebp;
		struct {
			unsigned short bp_top;
			unsigned short bp;
		} bp;
	} rbp;
	union {
		unsigned long esp;
		struct {
			unsigned short sp_top;
			unsigned short sp;
		} sp;
	} rsp;
	union {
		unsigned int cs;
		struct {
			unsigned short cs_top;
			unsigned short cs;
		} word16;
	} cs;
	union {
		unsigned int fs;
		struct {
			unsigned short fs_top;
			unsigned short fs;
		} word16;
	} fs;
	union {
		unsigned int gs;
		struct {
			unsigned short gs_top;
			unsigned short gs;
		} word16;
	} gs;
/* WARNING - trans_address MUST have an offset of 0x60 because this number is
   hardcoded in the ppc thunk code!! */
	BINADDR		trans_addr;
	BINADDR		return_addr;
	LPVOID		machine_stack;
	struct ENV	*prev_env;
	unsigned long   active;
	unsigned long   is_catch;
	unsigned long   level;
	struct HSW_86_CATCHBUF *buf;
	jmp_buf		jump_buffer;
#ifndef ONESTRUCT
} Interp_ENV;
typedef struct keyVAR {
#endif
	unsigned char *seg_regs[8];
	Interp_VAR_flags_czsp flags_czsp;
/*
 *		    b0        b1        b2       b3
 *	BE	[--------------longword----------------] SRC1_32
 *		[                  ][------word--------] SRC1_16
 *		[        ][        ][--byte--][--aux---]
 *				      SRC1_8    AUX1_8
 */
	union {
		unsigned long longword;
		struct {
			unsigned short dummy;
			unsigned short word;
		} word16;
		struct {
			unsigned char dummy1;
			unsigned char dummy2;
			unsigned char byte;
 			unsigned char aux;
		} byte;
	} src1;
/*
 *		    b0        b1        b2       b3
 *	BE	[--------------longword----------------] SRC2_32
 *		[                  ][------word--------] SRC2_16
 *		[        ][        ][--byte--][--aux---]
 *				      SRC2_8    AUX2_8
 */
	union {
		unsigned long longword;
		struct {
			unsigned short dummy;
			unsigned short word;
		} word16;
		struct {
			unsigned char dummy1;
			unsigned char dummy2;
			unsigned char byte;
 			unsigned char aux;
		} byte;
	} src2;
	unsigned char *reg1;	/* from REG field of mod_rm */
	unsigned char *mem_ref;	/* from RM field of mod_rm */
	int mode_reg;
	unsigned long error_addr;
	unsigned char bigseg[6];
#ifdef ONESTRUCT
} Interp_ENV;
#else
} Interp_VAR;
#endif
#endif

#if defined(__i386__) 
typedef struct keyENV{
	union {
		unsigned int ds;
		struct {
			unsigned short ds;
			unsigned short ds_top;
		} word16;
	} ds;
	union {
		unsigned int es;
		struct {
			unsigned short es;
			unsigned short es_top;
		} word16;
	} es;
	union {
		unsigned int ss;
		struct {
			unsigned short ss;
			unsigned short ss_top;
		} word16;
	} ss;
	unsigned long flags;
	union {
		unsigned long e;
		struct {
			union {
				unsigned short x;
				struct {
					unsigned char l;
					unsigned char h;
				} h;
			} x;
			unsigned short dummy;
		} x;
	} rax;
	union {
		unsigned long e;
		struct {
			union {
				unsigned short x;
				struct {
					unsigned char l;
					unsigned char h;
				} h;
			} x;
			unsigned short dummy;
		} x;
	} rbx;
	union {
		unsigned long e;
		struct {
			union {
				unsigned short x;
				struct {
					unsigned char l;
					unsigned char h;
				} h;
			} x;
			unsigned short dummy;
		} x;
	} rcx;
	union {
		unsigned long e;
		struct {
			union {
				unsigned short x;
				struct {
					unsigned char l;
					unsigned char h;
				} h;
			} x;
			unsigned short dummy;
		} x;
	} rdx;
	union {
		unsigned long esi;
		struct {
			unsigned short si;
			unsigned short si_top;
		} si;
	} rsi;
	union {
		unsigned long edi;
		struct {
			unsigned short di;
			unsigned short di_top;
		} di;
	} rdi;
	union {
		unsigned long ebp;
		struct {
			unsigned short bp;
			unsigned short bp_top;
		} bp;
	} rbp;
	union {
		unsigned long esp;
		struct {
			unsigned short sp;
			unsigned short sp_top;
		} sp;
	} rsp;
	union {
		unsigned int cs;
		struct {
			unsigned short cs;
			unsigned short cs_top;
		} word16;
	} cs;
	union {
		unsigned int fs;
		struct {
			unsigned short fs;
			unsigned short fs_top;
		} word16;
	} fs;
	union {
		unsigned int gs;
		struct {
			unsigned short gs;
			unsigned short gs_top;
		} word16;
	} gs;
	BINADDR		trans_addr;
	BINADDR		return_addr;
#ifndef DOSEMU
	LPVOID		machine_stack;
	struct ENV	*prev_env;
	unsigned long   active;
	unsigned long   is_catch;
	unsigned long   level;
	struct HSW_86_CATCHBUF *buf;
	jmp_buf		jump_buffer;
#endif
#ifndef ONESTRUCT
} Interp_ENV;
typedef struct keyVAR {
#endif
	unsigned char *seg_regs[8];
	Interp_VAR_flags_czsp flags_czsp;
/*
 *		    b0        b1        b2       b3
 *	LE	[--------------longword----------------] SRC1_32
 *	SRC1_16	[------word--------][                  ]
 *		[--aux---][--byte--][        ][        ]
 *		  AUX1_8    SRC1_8
 */
	union {
		unsigned long longword;
		struct {
			unsigned short word;
			unsigned short dummy;
		} word16;
		struct {
 			unsigned char aux;
			unsigned char byte;
			unsigned char dummy2;
			unsigned char dummy1;
		} byte;
	} src1;
/*
 *		    b0        b1        b2       b3
 *	LE	[--------------longword----------------] SRC2_32
 *	SRC2_16	[------word--------][                  ]
 *		[--aux---][--byte--][        ][        ]
 *		  AUX2_8    SRC2_8
 */
	union {
		unsigned long longword;
		struct {
			unsigned short word;
			unsigned short dummy;
		} word16;
		struct {
 			unsigned char aux;
			unsigned char byte;
			unsigned char dummy2;
			unsigned char dummy1;
		} byte;
	} src2;
	unsigned char *reg1;	/* from REG field of mod_rm */
	unsigned char *mem_ref;	/* from RM field of mod_rm */
	int mode_reg;
	unsigned long error_addr;
	unsigned char bigseg[6];
#ifdef ONESTRUCT
} Interp_ENV;
#else
} Interp_VAR;
#endif
#endif

#if defined(alpha)
typedef struct keyENV{
	union {
	    unsigned long ds;
		struct {
			unsigned short ds;
			unsigned short ds_top;
			unsigned int dummy1;
		} word16;
	} ds;
	union {
		unsigned long es;
		struct {
			unsigned short es;
			unsigned short es_top;
			unsigned int dummy1;
		} word16;
	} es;
	union {
		unsigned long ss;
		struct {
			unsigned short ss;
			unsigned short ss_top;
			unsigned int dummy1;
		} word16;
	} ss;
	unsigned long flags;
	union {
		unsigned long e;
		struct {
			union {
				unsigned short x;
				struct {
					unsigned char l;
					unsigned char h;
				} h;
			} x;
			unsigned short dummy;
			unsigned int dummy1;
		} x;
	} rax;
	union {
		unsigned long e;
		struct {
			union {
				unsigned short x;
				struct {
					unsigned char l;
					unsigned char h;
				} h;
			} x;
			unsigned short dummy;
			unsigned int dummy1;
		} x;
	} rbx;
	union {
		unsigned long e;
		struct {
			union {
				unsigned short x;
				struct {
					unsigned char l;
					unsigned char h;
				} h;
			} x;
			unsigned short dummy;
			unsigned int dummy1;
		} x;
	} rcx;
	union {
		unsigned long e;
		struct {
			union {
				unsigned short x;
				struct {
					unsigned char l;
					unsigned char h;
				} h;
			} x;
			unsigned short dummy;
			unsigned int dummy1;
		} x;
	} rdx;
	union {
		unsigned long esi;
		struct {
			unsigned short si;
			unsigned short si_top;
			unsigned int dummy1;
		} si;
	} rsi;
	union {
		unsigned long edi;
		struct {
			unsigned short di;
			unsigned short di_top;
			unsigned int dummy1;
		} di;
	} rdi;
	union {
		unsigned long ebp;
		struct {
			unsigned short bp;
			unsigned short bp_top;
			unsigned int dummy1;
		} bp;
	} rbp;
	union {
		unsigned long esp;
		struct {
			unsigned short sp;
			unsigned short sp_top;
			unsigned int dummy1;
		} sp;
	} rsp;
	union {
		unsigned long cs;
		struct {
			unsigned short cs;
			unsigned short cs_top;
			unsigned int dummy1;
		} word16;
	} cs;
	union {
		unsigned long fs;
		struct {
			unsigned short fs;
			unsigned short fs_top;
			unsigned int dummy1;
		} word16;
	} fs;
	union {
		unsigned long gs;
		struct {
			unsigned short gs;
			unsigned short gs_top;
			unsigned int dummy1;
		} word16;
	} gs;
	BINADDR		trans_addr;
	BINADDR		return_addr;
	LPVOID		machine_stack;
	struct ENV	*prev_env;
	unsigned long   active;
	unsigned long   is_catch;
	unsigned long   level;
	struct HSW_86_CATCHBUF *buf;
	jmp_buf		jump_buffer;
#ifndef ONESTRUCT
} Interp_ENV;
typedef struct keyVAR {
#endif
	unsigned char *seg_regs[8];
	Interp_VAR_flags_czsp flags_czsp;
	union {
		unsigned long longword;
		struct {
			unsigned short word;
			unsigned short dummy;
			unsigned int dummy1;
		} word16;
		struct {
 			unsigned char aux;
			unsigned char byte;
			unsigned char dummy2;
			unsigned char dummy1;
			unsigned int dummy3;
		} byte;
	} src1;
	union {
		unsigned long longword;
		struct {
			unsigned short word;
			unsigned short dummy;
			unsigned int dummy1;
		} word16;
		struct {
 			unsigned char aux;
			unsigned char byte;
			unsigned char dummy2;
			unsigned char dummy1;
			unsigned int dummy3;
		} byte;
	} src2;
	unsigned char *reg1;	/* from REG field of mod_rm */
	unsigned char *mem_ref;	/* from RM field of mod_rm */
	int mode_reg;
#ifdef ONESTRUCT
} Interp_ENV;
#else
} Interp_VAR;
#endif
#endif

#ifdef ONESTRUCT
#define iENV	env
#define iVAR	env
#define ENVPARAMS	Interp_ENV *env
#define ENVARGS		env
#else
#define iENV	env
#define iVAR	interp_var
#define ENVPARAMS	Interp_ENV *env,Interp_VAR *interp_var
#define ENVARGS		env,interp_var
#endif
#ifdef USE_GLOBAL_EBP
#define MODPARAMS	ENVPARAMS
#define MODARGS		ENVARGS
#else
#define MODPARAMS	ENVPARAMS,BYTE *PC
#define MODARGS		ENVARGS,PC
#endif

#if !defined(DOSEMU) || defined(_IN_INTP32)

/* CARRY is tricky, as it accesses also byte 3 (BYTE_OP); use with care */
#define CARRY  iVAR->flags_czsp.word16.carry
#define CARRYB iVAR->flags_czsp.byte.carryb
#define RES_32 iVAR->flags_czsp.res
#define RES_16 iVAR->flags_czsp.word16.res16
#define RES_8  iVAR->flags_czsp.byte.res8
#define AUXR_8 iVAR->flags_czsp.byte.parity16
#define PAR_16 iVAR->flags_czsp.byte.parity16
#define AUX1_8 iVAR->src1.byte.aux
#define AUX2_8 iVAR->src2.byte.aux
#define BYTE_FLAG iVAR->flags_czsp.byte.byte_op
#define SRC1_32 iVAR->src1.longword
#define SRC2_32 iVAR->src2.longword
#define SRC1_16 iVAR->src1.word16.word
#define SRC2_16 iVAR->src2.word16.word
#define SRC1_8  iVAR->src1.byte.byte
#define SRC2_8  iVAR->src2.byte.byte
/* these are pointers to the actual mem/reg src/dest location */
#define REG1 iVAR->reg1
#define HREG1 ((BYTE *)REG1)
#define XREG1 ((WORD *)REG1)
#define EREG1 ((DWORD *)REG1)
#define MEM_REF iVAR->mem_ref

#define IS_MODE_REG iVAR->mode_reg

/*
#define ALLOW_OVERRIDE(default) ((OVERRIDE!=INVALID_OVR)?OVERRIDE:default)
*/

/* ========================== FLAG setting ============================ */
/* Use similar macros to save the op state for later flag evaluation -
 * this greatly clarifies the source and avoids mistakes -- AV */
/*
 *		[-----------------res------------------]
 *	     	[----------0000000c][--res(0)-----00---]
 *		[01000101][        ][        ][        ]
 *
 * The parameter s is for the SUB flags setting (1) vs the ADD case (0).
 * s should always be a compile-time constant to make gcc skip the code
 * we don't need.
 *
 * SETBFLAGS requires definition of variables: src1,src2,res
 */
#ifdef __i386__
/* no-carry version */
#define BYTEDUP(sr)	({__asm__ __volatile__ ("\
	movzbl	%b1,%0\n\
	movb	%b1,%h0"\
	: "=q"(sr) \
	: "0"(sr) ); sr;})

/* carry version */
#define BYTEDUP_C(sr)	({__asm__ __volatile__ ("\
	andl	$0x1ff,%0\n\
	shll	$8,%0\n\
	movb	%h1,%b0"\
	: "=q"(sr) \
	: "0"(sr) ); sr;})
#else
#define BYTEDUP(sr)	((((sr)&0xff)<<8)|((sr)&0xff))
#define BYTEDUP_C(sr)	((((sr)&0x1ff)<<8)|((sr)&0xff))
#endif

/* New: align flag handling for byte ops to the default 16-bit word case.
 * We do this by duplicating the bits 0..7 into 8..15. Carry goes always
 * into bit 16; high byte is now cleared.
 * Flags: SF: uses bit 15 as before
 *	  ZF: if result byte is 0, so is the extended word
 *	  OF: uses bit 15 as before. Low bytes of src1,src2 ignored
 *	  PF: uses low byte, which is the same as the result
 *	  CF: into bit 16 as before
 *	  AF: uses low byte
 */
#define SETBFLAGS(s) { register DWORD _res1=res;\
	RES_32=BYTEDUP_C(_res1);\
	_res1=src1; SRC1_32=BYTEDUP(_res1);\
	if (s) {_res1=-src2; if ((_res1&0x7f)==0) _res1^=0x80;}\
	else _res1=src2; SRC2_32=BYTEDUP(_res1);}

/*
 *		[-----------------res------------------]
 *	     	[----------0000000c][--res(1)---res(0)-]
 *		[00000000][        ][        ][        ]
 *
 * The parameter s is for the SUB flags setting (1) vs the ADD case (0).
 * s should always be a compile-time constant to make gcc skip the code
 * we don't need.
 *
 * SETWFLAGS requires definition of variables: src1,src2,res
 * High 16 bits of SRC1,SRC2 are don't cares
 */
#define SETWFLAGS(s) { register DWORD _res1;\
	RES_32=res&0x1ffff; SRC1_32=src1;\
	if (s) {_res1=-src2; if ((_res1&0x7fff)==0) _res1^=0x8000;}\
	else _res1=src2; SRC2_32=_res1;}

/* add/sub rule for carry using bit31:
 *	src1 src2  res	b31(a) b31(s)
 *	  0    0    0    0	0
 *	  0    0    1    0	1
 *	  0    1    0    1	1
 *	  0    1    1    0	1
 *	  1    0    0    1	0
 *	  1    0    1    0	0
 *	  1    1    0    1	0
 *	  1    1    1    1	1
 *
 * This add/sub flag evaluation is tricky too. In 16-bit mode, the carry
 * flag is always at its correct position (bit 16), while in 32-bit mode
 * it has to be calculated from src1,src2,res. The original Willows code
 * had the correct definition for the ADD case, but totally failed in the
 * SUB/CMP case. It turned out that there isn't any simple expression
 * covering both cases, BUT that the sub carry is the inverse of what we
 * get for ADD if src2 is inverted (NOT negated!); see for yourself.
 */ 
#define IS_CF_SETD(s1,s2,r) ((unsigned)((((s1)^(s2)) & ~(r)) | ((s1)&(s2))) >> 31)

/*
 * for 32-bit ops we pack hi and lo bytes into a 16-bit value; this way
 * we can use the same tests for sign (bit 15) and parity (lo byte). It
 * is only necessary to add a case for the 32-bit zero condition; this is
 * done by setting bit 8 if the original value was not zero, so that 16-bit
 * zero test will give the correct answer -- AV
 */
#ifdef __i386__
/* latest version... move only sign bit from b31 to b15; if sign=1, then
 * of course ZF=0, else test and set bit 8 if 32-bit result is not zero.
 * The trick works because rcr doesn't change the ZF coming from shl.
 * Be careful not to change the LSByte - it is used by AF and PF */
#define PACK32_16(sr,de)	__asm__ __volatile__ ("\
	shll	%0\n\
	rcrw	%w0\n\
	jz	1f\n\
	orb	$1,%h0\n\
1:	movw	%w0,%w1"\
	: "=q"(sr), "=g"(*((short *)&de))\
	: "0"(sr) \
	: "memory" )
#else
#define PACK32_16(sr,de)	de=((WORD)(((sr)>>16)&0xff00) |\
				(WORD)((sr)&0xff) | (WORD)((sr)!=0? 0x100:0))
#endif

/*
 *		[-----------------res------------------]
 *	     	[----------sssssssc][--res(3)---res(0)-]
 *		[00000000][        ][-------1][        ]
 *
 * There are two parameters for the flag setting macros; c controls if
 * carry-setting flag has to be compiled in, s is for the SUB flags
 * setting (1) vs the ADD case (0). c ad s should always be compile-time
 * constants to make gcc skip the code we don't need.
 *
 * SETDFLAGS requires definition of variables: src1,src2,res
 */
#define SETDFLAGS(c,s) { register DWORD _res1=res;\
	PACK32_16(_res1,RES_16);\
	_res1=src1; PACK32_16(_res1,SRC1_16);\
	if (s) {_res1=-src2; if ((_res1<<1)==0) _res1^=0x80000000; src2=~src2;}\
	else _res1=src2; PACK32_16(_res1,SRC2_16);\
	if (c) CARRY = IS_CF_SETD(src1,src2,res) ^ (s); else BYTE_FLAG=0;}


/* ================== MEMORY and REGISTERS access ===================== */
/*
 * on x86, the CPU registers are stored in memory; there is no
 * difference in accessing. We keep things separate for compatibility.
 */

#ifdef ULTRA_DEBUG
static __inline__ BYTE GETBYTE(void *addr)
{
	BYTE val = (BYTE)*((BYTE *)(addr));
/**/	e_printf("GETBYTE %p = %x\n",addr,val);
	return val;
}
#else
#define	GETBYTE(p)	(BYTE)*((BYTE *)(p))
#endif

/* for non-gcc systems, where you don't have the ({..}) extension, it
 * will be better to transform these forms into inline functions. For
 * the moment we give an error when not using gcc. */
#ifndef __GNUC__
#error No hope, you die here.
#endif
/* little-endian or native version */
#ifdef ULTRA_DEBUG
static __inline__ void lPUTBYTE(volatile void *addr, BYTE val)
{
	*((BYTE *)(addr))=(BYTE)(val);
/**/	e_printf("PUTBYTE %p, %x = %x\n",addr,val,*(BYTE *)addr);
}
#else
#define lPUTBYTE(p,b)	({*((BYTE *)(p))=(BYTE)(b); 0;})
#endif
#define	lGETWORD(p)	(WORD)*((WORD *)(p))
#define lGETDWORD(p)	(DWORD)*((DWORD *)(p))
#define lPUTWORD(p,w)	({*((WORD *)(p))=(WORD)(w); 0;})
#define lPUTDWORD(p,dw)	({*((DWORD *)(p))=(DWORD)(dw); 0;})
#define lGETOCTA(p)	(*((QLONG *)(p)))
#define lPUTOCTA(p,ow)	({*((QLONG *)(p))=(QLONG)(ow); 0;})

#define bPUTBYTE	lPUTBYTE
#ifdef ppc
/*
 *  $Id: hsw_interp.h,v 1.3 1999/10/23 10:11:18 jching Exp $
 */
static __inline__ unsigned bGETWORD(/*volatile*/ void *addr)
{
	unsigned val;
	__asm__ __volatile__ ("lhbrx %0,0,%1" : "=r" (val) :
		 "r" ((WORD *)addr), "m" (*(WORD *)addr));
#ifdef ULTRA_DEBUG
/**/	e_printf("GETWORD %p = %x\n",addr,val);
#endif
	return val;
}

static __inline__ void bPUTWORD(volatile void *addr, WORD val)
{
	__asm__ __volatile__ ("sthbrx %1,0,%2" : "=m" (*(WORD *)addr) :
		 "r" (val), "r" ((WORD *)addr));
#ifdef ULTRA_DEBUG
/**/	e_printf("PUTWORD %p, %x = %x\n",addr,val,*(WORD *)addr);
#endif
}

static __inline__ DWORD bGETDWORD(/*volatile*/ void *addr)
{
	unsigned val;
	__asm__ __volatile__ ("lwbrx %0,0,%1" : "=r" (val) :
		 "r" ((DWORD *)addr), "m" (*(DWORD *)addr));
#ifdef ULTRA_DEBUG
/**/	e_printf("GETDWORD %p = %x\n",addr,val);
#endif
	return val;
}

static __inline__ QLONG bGETOCTA(volatile void *addr)
{
	union {	QLONG lq; struct {DWORD ll,lh;} lw; } val;
	__asm__ __volatile__ ("
		lwbrx %0,0,%2\n
		addi  %2,%2,4\n
		lwbrx %1,0,%2"
		: "=r" (val.lw.lh), "=r" (val.lw.ll)
		: "r" ((DWORD *)addr), "m" (*(DWORD *)addr) );
#ifdef ULTRA_DEBUG
/**/	e_printf("GETQWORD %p = %Lx\n",addr,val.lq);
#endif
	return val.lq;
}

static __inline__ void bPUTDWORD(volatile void *addr, DWORD val)
{
	__asm__ __volatile__ ("stwbrx %1,0,%2" : "=m" (*(DWORD *)addr) :
		 "r" (val), "r" ((DWORD *)addr));
#ifdef ULTRA_DEBUG
/**/	e_printf("PUTDWORD %p, %lx = %lx\n",addr,val,*(DWORD *)addr);
#endif
}

static __inline__ void bPUTOCTA(volatile void *addr, QLONG val)
{
	union { QLONG lq; struct {DWORD lh,ll;} lw; } v;
	v.lq = val;
	__asm__ __volatile__ ("
		stwbrx %1,0,%3\n
		addi   %3,%3,4\n
		stwbrx %2,0,%3"
		: "=m" (*(DWORD *)addr)
		: "r" (v.lw.ll), "r" (v.lw.lh), "r" ((DWORD *)addr) );
#ifdef ULTRA_DEBUG
/**/	e_printf("PUTQWORD %p, %Lx = %Lx\n",addr,val,*(QLONG *)addr);
#endif
}
#else	/* general-purpose version */
#define bGETWORD(p)	((WORD)(((BYTE *)p)[0])|((WORD)(((BYTE *)p)[1])<<8))
#define bGETDWORD(p)	((DWORD)(((BYTE *)p)[0])|((DWORD)(((BYTE *)p)[1])<<8)|\
			((DWORD)(((BYTE *)p)[2])<<16)|((DWORD)(((BYTE *)p)[3])<<24))

#define bPUTWORD(pw,w)	({BYTE *p=(BYTE *)pw; register WORD v=(WORD)(w);\
			p[0]=v; p[1]=(v>>8); 0;})

#define bPUTDWORD(pd,dw) ({BYTE *p=(BYTE *)pd; register DWORD v=(DWORD)(dw);\
			p[0]=v; p[1]=(v>>8); p[2]=(v>>16); p[3]=(v>>24); 0;})

/* get/put a 64-bit long long/double with byte swap. Used only by fp87.c.
 * The parameter p MUST be a BYTE pointer */
#define bGETOCTA(p)	({QLONG lq; \
			((BYTE*)&lq)[7] = (p)[0], ((BYTE*)&lq)[6] = (p)[1], \
			((BYTE*)&lq)[5] = (p)[2], ((BYTE*)&lq)[4] = (p)[3], \
			((BYTE*)&lq)[3] = (p)[4], ((BYTE*)&lq)[2] = (p)[5], \
			((BYTE*)&lq)[1] = (p)[6], ((BYTE*)&lq)[0] = (p)[7], \
			lq; })
#define bPUTOCTA(pq,qw)	((pq)[0] = ((BYTE*)&qw)[7], (pq)[1] = ((BYTE*)&qw)[6], \
			(pq)[2] = ((BYTE*)&qw)[5], (pq)[3] = ((BYTE*)&qw)[4], \
			(pq)[4] = ((BYTE*)&qw)[3], (pq)[5] = ((BYTE*)&qw)[2], \
			(pq)[6] = ((BYTE*)&qw)[1], (pq)[7] = ((BYTE*)&qw)[0]  )
#endif

/* register definitions - use always native endian mode */
#define FETCH_HREG	GETBYTE
#define FETCH_XREG	lGETWORD
#define FETCH_EREG	lGETDWORD
#define PUT_HREG	lPUTBYTE
#define PUT_XREG	lPUTWORD
#define PUT_EREG	lPUTDWORD

/* memory definitions - use always little-endian memory order */
#define FETCH_BYTE	GETBYTE
#ifdef _LITTLE_ENDIAN
#define FETCH_WORD	lGETWORD
#define FETCH_QUAD	lGETDWORD
#define FETCH_OCTA	lGETOCTA
#define PUT_BYTE	lPUTBYTE
#define PUT_WORD	lPUTWORD
#define PUT_QUAD	lPUTDWORD
#define PUT_OCTA	lPUTOCTA
#else
#define FETCH_WORD	bGETWORD
#define FETCH_QUAD	bGETDWORD
#define FETCH_OCTA	bGETOCTA
#define PUT_BYTE	bPUTBYTE
#define PUT_WORD	bPUTWORD
#define PUT_QUAD	bPUTDWORD
#define PUT_OCTA	bPUTOCTA
#endif

/* register/memory selection via macro */
#ifdef DOSEMU	/* currently x86 only, this simplifies things */
#define mFETCH_BYTE	GETBYTE
#define mFETCH_WORD	lGETWORD
#define mFETCH_QUAD	lGETDWORD
#define mPUT_BYTE	lPUTBYTE
#define mPUT_WORD	lPUTWORD
#define mPUT_QUAD	lPUTDWORD
#else
#define mFETCH_BYTE	GETBYTE
#define mFETCH_WORD(p)	(IS_MODE_REG? FETCH_XREG((p)):FETCH_WORD((p)))
#define mFETCH_QUAD(p)	(IS_MODE_REG? FETCH_EREG((p)):FETCH_QUAD((p)))
#define mPUT_BYTE(p,b)	(IS_MODE_REG? PUT_HREG((p),(b)):PUT_BYTE((p),(b)))
#define mPUT_WORD(p,b)	(IS_MODE_REG? PUT_XREG((p),(b)):PUT_WORD((p),(b)))
#define mPUT_QUAD(p,b)	(IS_MODE_REG? PUT_EREG((p),(b)):PUT_QUAD((p),(b)))
#endif

#if defined(__i386__) || defined(ppc)	/* known asm shortcuts */
#define TOS_WORD	(BIG_SS? FETCH_WORD(LONG_SS+ESP):\
				FETCH_WORD(LONG_SS+SP))
/* PAGING warning - possible page fault on push, should NOT move (e)sp
 * until address is fixed! */
#define PUSHWORD(w) if (BIG_SS) {PUT_WORD(LONG_SS+ESP-2, w); ESP-=2;} else \
				{WORD _z=SP-2; PUT_WORD(LONG_SS+_z, w); SP=_z;}
#define PUSHQUAD(w) if (BIG_SS) {PUT_QUAD(LONG_SS+ESP-4, w); ESP-=4;} else \
				{WORD _z=SP-4; PUT_QUAD(LONG_SS+_z, w); SP=_z;}

#define POPWORD(w)  if (BIG_SS) {w=FETCH_WORD(LONG_SS+ESP); ESP+=2;} else \
				{w=FETCH_WORD(LONG_SS+SP); SP+=2;}
#define POPQUAD(w)  if (BIG_SS) {w=FETCH_QUAD(LONG_SS+ESP); ESP+=4;} else \
				{w=FETCH_QUAD(LONG_SS+SP); SP+=4;}
#else	/* general-purpose */
#define TOS_WORD	({unsigned short w;\
		unsigned char *sp=(BIG_SS? LONG_SS+ESP:LONG_SS+SP);\
		w=sp[0]|((unsigned short)sp[1]<<8); w; })

#define PUSHWORD(w) if (BIG_SS) {unsigned char *sp; ESP-=2; \
		sp=LONG_SS+ESP; sp[0]=w; sp[1]=w>>8;} else \
		{unsigned char *sp; SP-=2; sp=LONG_SS+SP; \
		sp[0]=w; sp[1]=w>>8;}
#define PUSHQUAD(w) if (BIG_SS) {unsigned char *sp; ESP-=4; sp=LONG_SS+ESP; \
		sp[0]=w; sp[1]=w>>8; sp[2]=w>>16; sp[3]=w>>24;} else \
		{unsigned char *sp; SP-=4; sp=LONG_SS+SP; \
		sp[0]=w; sp[1]=w>>8; sp[2]=w>>16; sp[3]=w>>24;}

#define POPWORD(w)  if (BIG_SS) {unsigned char *sp=LONG_SS+ESP; \
		w=sp[0]|((unsigned short)sp[1]<<8); ESP+=2;} else \
		{unsigned char *sp=LONG_SS+SP; \
		w=sp[0]|((unsigned short)sp[1]<<8); SP+=2;}
#define POPQUAD(w)  if (BIG_SS) {unsigned char *sp=LONG_SS+ESP; \
		w=sp[0]|((unsigned long)sp[1]<<8)| \
		((unsigned long)sp[2]<<16)|((unsigned long)sp[3]<<24); \
		ESP+=4;} else {unsigned char *sp=LONG_SS+SP; \
		w=sp[0]|((unsigned long)sp[1]<<8)| \
		((unsigned long)sp[2]<<16)|((unsigned long)sp[3]<<24); \
		SP+=4;}
#endif


/* These are for multiply/divide. I ASSUME that every version of gcc on
 * every 32-bit target has a 'long long' 64-bit type */
typedef union {
  QLONG sd;
  struct { long sl,sh; } s;
} s_i64_u;

typedef union {
  QWORD ud;
  struct { long ul,uh; } u;
} u_i64_u;

#ifndef DOSEMU
#define error(s...)	fprintf(stderr,##s)
extern LPSTR	GetProcName(WORD wSel, WORD wOrd);
#else
#include "cpu-emu.h"
#endif

typedef void (*FUNCT_PTR)();

#ifndef NO_TRACE_MSGS
extern void e_trace (ENVPARAMS, unsigned char *, unsigned char *,
  	int);
extern void e_trace_fp (ENV87 *);
#endif
/* debug registers (DRs) emulation */
extern int e_debug_check(unsigned char *);

extern unsigned char *
  hsw_interp_16_16 (ENVPARAMS, unsigned char *,
	unsigned char *, int *, int);
extern unsigned char *
  hsw_interp_16_32 (ENVPARAMS, unsigned char *,
	unsigned char *, int *);
extern unsigned char *
  hsw_interp_32_16 (ENVPARAMS, unsigned char *,
	unsigned char *, int *);
extern unsigned char *
  hsw_interp_32_32 (ENVPARAMS, unsigned char *,
	unsigned char *, int *, int);

extern int
  hsw_modrm_16_byte (MODPARAMS);
extern int
  hsw_modrm_16_word (MODPARAMS);
extern int
  hsw_modrm_16_quad (MODPARAMS);
extern int
  hsw_modrm_32_byte (MODPARAMS);
extern int
  hsw_modrm_32_word (MODPARAMS);
extern int
  hsw_modrm_32_quad (MODPARAMS);

extern void invoke_data (ENVPARAMS);

extern void
trans_flags_to_interp(ENVPARAMS, DWORD);
extern DWORD
trans_interp_flags(ENVPARAMS);

extern int
init_cpuemu(void);

#endif	/* _IN_INTP32 */

extern BOOL code32, data32;
extern void FatalAppExit(UINT, LPCSTR);
extern QWORD EMUtime;

#define MK_CS	0x00000
#define MK_DS	0x10000
#define MK_ES	0x20000
#define MK_SS	0x30000
#define MK_FS	0x40000
#define MK_GS	0x50000

/*
 * INT 00 C  - CPU-generated - DIVIDE ERROR
 * INT 01 C  - CPU-generated - SINGLE STEP
 * INT 02 C  - external hardware - NON-MASKABLE INTERRUPT
 * INT 03 C  - CPU-generated - BREAKPOINT
 * INT 04 C  - CPU-generated - INTO DETECTED OVERFLOW
 * INT 05 C  - CPU-generated (80186+) - BOUND RANGE EXCEEDED
 * INT 06 C  - CPU-generated (80286+) - INVALID OPCODE
 * INT 07 C  - CPU-generated (80286+) - PROCESSOR EXTENSION NOT AVAILABLE
 * INT 08 C  - CPU-generated (80286+) - DOUBLE EXCEPTION DETECTED
 * INT 09 C  - CPU-generated (80286,80386) - PROCESSOR EXTENSION PROTECTION ERROR
 * INT 0A CP - CPU-generated (80286+) - INVALID TASK STATE SEGMENT
 * INT 0B CP - CPU-generated (80286+) - SEGMENT NOT PRESENT
 * INT 0C C  - CPU-generated (80286+) - STACK FAULT
 * INT 0D C  - CPU-generated (80286+) - GENERAL PROTECTION VIOLATION
 * INT 0E C  - CPU-generated (80386+ native mode) - PAGE FAULT
 * INT 10 C  - CPU-generated (80286+) - COPROCESSOR ERROR
 * INT 11    - CPU-generated (80486+) - ALIGNMENT CHECK
 * INT 12    - CPU-generated (Pentium) - MACHINE CHECK EXCEPTION
 */
#define EXCP00_DIVZ	1
#define EXCP01_SSTP	2
#define EXCP02_NMI	3
#define EXCP03_INT3	4
#define EXCP04_INTO	5
#define EXCP05_BOUND	6
#define EXCP06_ILLOP	7
#define EXCP07_PREX	8
#define EXCP08_DBLE	9
#define EXCP09_XERR	10
#define EXCP0A_TSS	11
#define EXCP0B_NOSEG	12
#define EXCP0C_STACK	13
#define EXCP0D_GPF	14
#define EXCP0E_PAGE	15
#define EXCP10_COPR	17
#define EXCP11_ALGN	18
#define EXCP12_MCHK	19

#define EXCP_GOBACK	64
#define EXCP_SIGNAL	65

#if !defined(DOSEMU) || defined(_IN_INTP32)

extern void hsw_fp87_00m(), hsw_fp87_01m(), hsw_fp87_02m(), hsw_fp87_03m();
extern void hsw_fp87_04m(), hsw_fp87_05m(), hsw_fp87_06m(), hsw_fp87_07m();
extern void hsw_fp87_10m(), hsw_fp87_11m(), hsw_fp87_12m(), hsw_fp87_13m();
extern void hsw_fp87_14m(), hsw_fp87_15m(), hsw_fp87_16m(), hsw_fp87_17m();
extern void hsw_fp87_20m(), hsw_fp87_21m(), hsw_fp87_22m(), hsw_fp87_23m();
extern void hsw_fp87_24m(), hsw_fp87_25m(), hsw_fp87_26m(), hsw_fp87_27m();
extern void hsw_fp87_30m(), hsw_fp87_31m(), hsw_fp87_32m(), hsw_fp87_33m();
extern void hsw_fp87_34m(), hsw_fp87_35m(), hsw_fp87_36m(), hsw_fp87_37m();
extern void hsw_fp87_40m(), hsw_fp87_41m(), hsw_fp87_42m(), hsw_fp87_43m();
extern void hsw_fp87_44m(), hsw_fp87_45m(), hsw_fp87_46m(), hsw_fp87_47m();
extern void hsw_fp87_50m(), hsw_fp87_51m(), hsw_fp87_52m(), hsw_fp87_53m();
extern void hsw_fp87_54m(), hsw_fp87_55m(), hsw_fp87_56m(), hsw_fp87_57m();
extern void hsw_fp87_60m(), hsw_fp87_61m(), hsw_fp87_62m(), hsw_fp87_63m();
extern void hsw_fp87_64m(), hsw_fp87_65m(), hsw_fp87_66m(), hsw_fp87_67m();
extern void hsw_fp87_70m(), hsw_fp87_71m(), hsw_fp87_72m(), hsw_fp87_73m();
extern void hsw_fp87_74m(), hsw_fp87_75m(), hsw_fp87_76m(), hsw_fp87_77m();
extern void hsw_fp87_00r(), hsw_fp87_01r(), hsw_fp87_02r(), hsw_fp87_03r();
extern void hsw_fp87_04r(), hsw_fp87_05r(), hsw_fp87_06r(), hsw_fp87_07r();
extern void hsw_fp87_10r(), hsw_fp87_11r(), hsw_fp87_12r(), hsw_fp87_13r();
extern void hsw_fp87_14r(), hsw_fp87_15r(), hsw_fp87_16r(), hsw_fp87_17r();
extern void hsw_fp87_20r(), hsw_fp87_21r(), hsw_fp87_22r(), hsw_fp87_23r();
extern void hsw_fp87_24r(), hsw_fp87_25r(), hsw_fp87_26r(), hsw_fp87_27r();
extern void hsw_fp87_30r(), hsw_fp87_31r(), hsw_fp87_32r(), hsw_fp87_33r();
extern void hsw_fp87_34r(), hsw_fp87_35r(), hsw_fp87_36r(), hsw_fp87_37r();
extern void hsw_fp87_40r(), hsw_fp87_41r(), hsw_fp87_42r(), hsw_fp87_43r();
extern void hsw_fp87_44r(), hsw_fp87_45r(), hsw_fp87_46r(), hsw_fp87_47r();
extern void hsw_fp87_50r(), hsw_fp87_51r(), hsw_fp87_52r(), hsw_fp87_53r();
extern void hsw_fp87_54r(), hsw_fp87_55r(), hsw_fp87_56r(), hsw_fp87_57r();
extern void hsw_fp87_60r(), hsw_fp87_61r(), hsw_fp87_62r(), hsw_fp87_63r();
extern void hsw_fp87_64r(), hsw_fp87_65r(), hsw_fp87_66r(), hsw_fp87_67r();
extern void hsw_fp87_70r(), hsw_fp87_71r(), hsw_fp87_72r(), hsw_fp87_73r();
extern void hsw_fp87_74r(), hsw_fp87_75r(), hsw_fp87_76r(), hsw_fp87_77r();

extern FUNCT_PTR hsw_fp87_mem0[8];
extern FUNCT_PTR hsw_fp87_mem1[8];
extern FUNCT_PTR hsw_fp87_mem2[8];
extern FUNCT_PTR hsw_fp87_mem3[8];
extern FUNCT_PTR hsw_fp87_mem4[8];
extern FUNCT_PTR hsw_fp87_mem5[8];
extern FUNCT_PTR hsw_fp87_mem6[8];
extern FUNCT_PTR hsw_fp87_mem7[8];
extern FUNCT_PTR hsw_fp87_reg0[8];
extern FUNCT_PTR hsw_fp87_reg1[8];
extern FUNCT_PTR hsw_fp87_reg2[8];
extern FUNCT_PTR hsw_fp87_reg3[8];
extern FUNCT_PTR hsw_fp87_reg4[8];
extern FUNCT_PTR hsw_fp87_reg5[8];
extern FUNCT_PTR hsw_fp87_reg6[8];
extern FUNCT_PTR hsw_fp87_reg7[8];

#endif

extern BYTE parity[];
extern char unknown_msg[], illegal_msg[], unsupp_msg[];
extern ENV87 hsw_env87;

#endif /* hsw_interp__h */

