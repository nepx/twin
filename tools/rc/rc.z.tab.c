/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with zz or ZZ, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define ZZBISON 1

/* Bison version.  */
#define ZZBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define ZZSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define ZZPURE 0

/* Push parsers.  */
#define ZZPUSH 0

/* Pull parsers.  */
#define ZZPULL 1




/* Copy the first part of user declarations.  */
#line 1 "ifyacc.y" /* yacc.c:339  */

/*************************************************************************

    ifyacc.y	1.9
    yacc parser for #if statements

    Copyright 1997 Willows Software, Inc. 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.

The maintainer of the Willows TWIN Libraries may be reached (Email) 
at the address twin@willows.com	

**************************************************************************/

#include <stdio.h>
#include "rc.h"
#include "define.h"
#include "buffer.h"

extern int zzerror(char *);
extern int zzlex();

extern int ifResult;

#if defined(TWIN_HAVEALLOCA)
#include <alloca.h>
#endif

#line 109 "y.tab.c" /* yacc.c:339  */

# ifndef ZZ_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define ZZ_NULLPTR nullptr
#  else
#   define ZZ_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef ZZERROR_VERBOSE
# undef ZZERROR_VERBOSE
# define ZZERROR_VERBOSE 1
#else
# define ZZERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef ZZ_ZZ_Y_TAB_H_INCLUDED
# define ZZ_ZZ_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef ZZDEBUG
# define ZZDEBUG 0
#endif
#if ZZDEBUG
extern int zzdebug;
#endif

/* Token type.  */
#ifndef ZZTOKENTYPE
# define ZZTOKENTYPE
  enum zztokentype
  {
    ifINT = 258,
    ifSTRING = 259,
    ifDEFINED = 260,
    ifnDEFINED = 261,
    ifPREPROCESSORIF = 262
  };
#endif
/* Tokens.  */
#define ifINT 258
#define ifSTRING 259
#define ifDEFINED 260
#define ifnDEFINED 261
#define ifPREPROCESSORIF 262

/* Value type.  */
#if ! defined ZZSTYPE && ! defined ZZSTYPE_IS_DECLARED

union ZZSTYPE
{
#line 45 "ifyacc.y" /* yacc.c:355  */

  int i;
  char *str;

#line 168 "y.tab.c" /* yacc.c:355  */
};

typedef union ZZSTYPE ZZSTYPE;
# define ZZSTYPE_IS_TRIVIAL 1
# define ZZSTYPE_IS_DECLARED 1
#endif


extern ZZSTYPE zzlval;

int zzparse (void);

#endif /* !ZZ_ZZ_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 185 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef ZZTYPE_UINT8
typedef ZZTYPE_UINT8 zztype_uint8;
#else
typedef unsigned char zztype_uint8;
#endif

#ifdef ZZTYPE_INT8
typedef ZZTYPE_INT8 zztype_int8;
#else
typedef signed char zztype_int8;
#endif

#ifdef ZZTYPE_UINT16
typedef ZZTYPE_UINT16 zztype_uint16;
#else
typedef unsigned short int zztype_uint16;
#endif

#ifdef ZZTYPE_INT16
typedef ZZTYPE_INT16 zztype_int16;
#else
typedef short int zztype_int16;
#endif

#ifndef ZZSIZE_T
# ifdef __SIZE_TYPE__
#  define ZZSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define ZZSIZE_T size_t
# elif ! defined ZZSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define ZZSIZE_T size_t
# else
#  define ZZSIZE_T unsigned int
# endif
#endif

#define ZZSIZE_MAXIMUM ((ZZSIZE_T) -1)

#ifndef ZZ_
# if defined ZZENABLE_NLS && ZZENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define ZZ_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef ZZ_
#  define ZZ_(Msgid) Msgid
# endif
#endif

#ifndef ZZ_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define ZZ_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define ZZ_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef ZZ_ATTRIBUTE_PURE
# define ZZ_ATTRIBUTE_PURE   ZZ_ATTRIBUTE ((__pure__))
#endif

#ifndef ZZ_ATTRIBUTE_UNUSED
# define ZZ_ATTRIBUTE_UNUSED ZZ_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn ZZ_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define ZZUSE(E) ((void) (E))
#else
# define ZZUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about zzlval being uninitialized.  */
# define ZZ_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define ZZ_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define ZZ_INITIAL_VALUE(Value) Value
#endif
#ifndef ZZ_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define ZZ_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define ZZ_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef ZZ_INITIAL_VALUE
# define ZZ_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined zzoverflow || ZZERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef ZZSTACK_USE_ALLOCA
#  if ZZSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define ZZSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define ZZSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define ZZSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef ZZSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define ZZSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef ZZSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define ZZSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define ZZSTACK_ALLOC ZZMALLOC
#  define ZZSTACK_FREE ZZFREE
#  ifndef ZZSTACK_ALLOC_MAXIMUM
#   define ZZSTACK_ALLOC_MAXIMUM ZZSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined ZZMALLOC || defined malloc) \
             && (defined ZZFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef ZZMALLOC
#   define ZZMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (ZZSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef ZZFREE
#   define ZZFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined zzoverflow || ZZERROR_VERBOSE */


#if (! defined zzoverflow \
     && (! defined __cplusplus \
         || (defined ZZSTYPE_IS_TRIVIAL && ZZSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union zzalloc
{
  zztype_int16 zzss_alloc;
  ZZSTYPE zzvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define ZZSTACK_GAP_MAXIMUM (sizeof (union zzalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define ZZSTACK_BYTES(N) \
     ((N) * (sizeof (zztype_int16) + sizeof (ZZSTYPE)) \
      + ZZSTACK_GAP_MAXIMUM)

# define ZZCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables ZZSIZE and ZZSTACKSIZE give the old and new number of
   elements in the stack, and ZZPTR gives the new location of the
   stack.  Advance ZZPTR to a properly aligned location for the next
   stack.  */
# define ZZSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        ZZSIZE_T zznewbytes;                                            \
        ZZCOPY (&zzptr->Stack_alloc, Stack, zzsize);                    \
        Stack = &zzptr->Stack_alloc;                                    \
        zznewbytes = zzstacksize * sizeof (*Stack) + ZZSTACK_GAP_MAXIMUM; \
        zzptr += zznewbytes / sizeof (*zzptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined ZZCOPY_NEEDED && ZZCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef ZZCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define ZZCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define ZZCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          ZZSIZE_T zzi;                         \
          for (zzi = 0; zzi < (Count); zzi++)   \
            (Dst)[zzi] = (Src)[zzi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !ZZCOPY_NEEDED */

/* ZZFINAL -- State number of the termination state.  */
#define ZZFINAL  10
/* ZZLAST -- Last index in ZZTABLE.  */
#define ZZLAST   111

/* ZZNTOKENS -- Number of terminals.  */
#define ZZNTOKENS  21
/* ZZNNTS -- Number of nonterminals.  */
#define ZZNNTS  3
/* ZZNRULES -- Number of rules.  */
#define ZZNRULES  25
/* ZZNSTATES -- Number of states.  */
#define ZZNSTATES  54

/* ZZTRANSLATE[ZZX] -- Symbol number corresponding to ZZX as returned
   by zzlex, with out-of-bounds checking.  */
#define ZZUNDEFTOK  2
#define ZZMAXUTOK   262

#define ZZTRANSLATE(ZZX)                                                \
  ((unsigned int) (ZZX) <= ZZMAXUTOK ? zztranslate[ZZX] : ZZUNDEFTOK)

/* ZZTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by zzlex, without out-of-bounds checking.  */
static const zztype_uint8 zztranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     8,     2,     2,     2,    13,    14,     2,
      19,    20,    11,     9,     2,    10,     2,    12,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      16,    18,    17,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    15,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7
};

#if ZZDEBUG
  /* ZZRLINE[ZZN] -- Source line where rule number ZZN was defined.  */
static const zztype_uint8 zzrline[] =
{
       0,    67,    67,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91
};
#endif

#if ZZDEBUG || ZZERROR_VERBOSE || 0
/* ZZTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at ZZNTOKENS, nonterminals.  */
static const char *const zztname[] =
{
  "$end", "error", "$undefined", "ifINT", "ifSTRING", "ifDEFINED",
  "ifnDEFINED", "ifPREPROCESSORIF", "'!'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'&'", "'|'", "'<'", "'>'", "'='", "'('", "')'", "$accept",
  "preprocessorifclause", "expr", ZZ_NULLPTR
};
#endif

# ifdef ZZPRINT
/* ZZTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const zztype_uint16 zztoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,    33,    43,
      45,    42,    47,    37,    38,   124,    60,    62,    61,    40,
      41
};
# endif

#define ZZPACT_NINF -15

#define zzpact_value_is_default(Yystate) \
  (!!((Yystate) == (-15)))

#define ZZTABLE_NINF -1

#define zztable_value_is_error(Yytable_value) \
  0

  /* ZZPACT[STATE-NUM] -- Index in ZZTABLE of the portion describing
     STATE-NUM.  */
static const zztype_int8 zzpact[] =
{
      15,    63,     9,   -15,   -15,    21,    37,    63,    63,    88,
     -15,   -15,    19,   -15,    29,    88,    75,    17,    63,    63,
      63,    63,    63,    23,    40,     2,    46,    35,    27,    38,
     -15,    63,    61,    61,    93,    93,    93,    63,   -14,    63,
     -14,    63,    39,    63,    39,    63,   -15,   -15,    39,   -14,
     -14,    39,    39,    39
};

  /* ZZDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when ZZTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const zztype_uint8 zzdefact[] =
{
       0,     0,     0,     3,     4,     0,     0,     0,     0,     2,
       1,    23,     0,    25,     0,    15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,     0,     5,     6,     7,     8,     9,     0,    11,     0,
      10,     0,    17,     0,    16,     0,    22,    24,    21,    13,
      12,    19,    18,    20
};

  /* ZZPGOTO[NTERM-NUM].  */
static const zztype_int8 zzpgoto[] =
{
     -15,   -15,    -7
};

  /* ZZDEFGOTO[NTERM-NUM].  */
static const zztype_int8 zzdefgoto[] =
{
      -1,     2,     9
};

  /* ZZTABLE[ZZPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If ZZTABLE_NINF, syntax error.  */
static const zztype_uint8 zztable[] =
{
      15,    16,    25,    26,    27,     3,     4,     5,     6,    10,
       7,    32,    33,    34,    35,    36,    38,    40,    42,    44,
      41,     8,     1,    28,    48,    11,     3,     4,     5,     6,
      49,     7,    50,    29,    51,    31,    52,    37,    53,     0,
      12,    13,     8,     3,     4,     5,     6,    46,     7,     3,
       4,     5,     6,    45,     7,    39,    14,    27,    47,     8,
       0,     0,     0,     0,    43,     8,     3,     4,     5,     6,
       0,     7,    20,    21,    22,    23,    24,    25,    26,    27,
       0,     0,     8,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    30,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    23,    24,    25,
      26,    27
};

static const zztype_int8 zzcheck[] =
{
       7,     8,    16,    17,    18,     3,     4,     5,     6,     0,
       8,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      18,    19,     7,     4,    31,     4,     3,     4,     5,     6,
      37,     8,    39,     4,    41,    18,    43,    14,    45,    -1,
      19,     4,    19,     3,     4,     5,     6,    20,     8,     3,
       4,     5,     6,    18,     8,    15,    19,    18,    20,    19,
      -1,    -1,    -1,    -1,    18,    19,     3,     4,     5,     6,
      -1,     8,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    19,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    20,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    14,    15,    16,
      17,    18
};

  /* ZZSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const zztype_uint8 zzstos[] =
{
       0,     7,    22,     3,     4,     5,     6,     8,    19,    23,
       0,     4,    19,     4,    19,    23,    23,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,     4,     4,
      20,    18,    23,    23,    23,    23,    23,    14,    23,    15,
      23,    18,    23,    18,    23,    18,    20,    20,    23,    23,
      23,    23,    23,    23
};

  /* ZZR1[ZZN] -- Symbol number of symbol that rule ZZN derives.  */
static const zztype_uint8 zzr1[] =
{
       0,    21,    22,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23
};

  /* ZZR2[ZZN] -- Number of symbols on the right hand side of rule ZZN.  */
static const zztype_uint8 zzr2[] =
{
       0,     2,     2,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     3,     2,     3,     3,     4,     4,
       4,     4,     4,     2,     4,     2
};


#define zzerrok         (zzerrstatus = 0)
#define zzclearin       (zzchar = ZZEMPTY)
#define ZZEMPTY         (-2)
#define ZZEOF           0

#define ZZACCEPT        goto zzacceptlab
#define ZZABORT         goto zzabortlab
#define ZZERROR         goto zzerrorlab


#define ZZRECOVERING()  (!!zzerrstatus)

#define ZZBACKUP(Token, Value)                                  \
do                                                              \
  if (zzchar == ZZEMPTY)                                        \
    {                                                           \
      zzchar = (Token);                                         \
      zzlval = (Value);                                         \
      ZZPOPSTACK (zzlen);                                       \
      zzstate = *zzssp;                                         \
      goto zzbackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      zzerror (ZZ_("syntax error: cannot back up")); \
      ZZERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define ZZTERROR        1
#define ZZERRCODE       256



/* Enable debugging if requested.  */
#if ZZDEBUG

# ifndef ZZFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define ZZFPRINTF fprintf
# endif

# define ZZDPRINTF(Args)                        \
do {                                            \
  if (zzdebug)                                  \
    ZZFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef ZZ_LOCATION_PRINT
# define ZZ_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define ZZ_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (zzdebug)                                                            \
    {                                                                     \
      ZZFPRINTF (stderr, "%s ", Title);                                   \
      zz_symbol_print (stderr,                                            \
                  Type, Value); \
      ZZFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on ZZOUTPUT.  |
`----------------------------------------*/

static void
zz_symbol_value_print (FILE *zzoutput, int zztype, ZZSTYPE const * const zzvaluep)
{
  FILE *zzo = zzoutput;
  ZZUSE (zzo);
  if (!zzvaluep)
    return;
# ifdef ZZPRINT
  if (zztype < ZZNTOKENS)
    ZZPRINT (zzoutput, zztoknum[zztype], *zzvaluep);
# endif
  ZZUSE (zztype);
}


/*--------------------------------.
| Print this symbol on ZZOUTPUT.  |
`--------------------------------*/

static void
zz_symbol_print (FILE *zzoutput, int zztype, ZZSTYPE const * const zzvaluep)
{
  ZZFPRINTF (zzoutput, "%s %s (",
             zztype < ZZNTOKENS ? "token" : "nterm", zztname[zztype]);

  zz_symbol_value_print (zzoutput, zztype, zzvaluep);
  ZZFPRINTF (zzoutput, ")");
}

/*------------------------------------------------------------------.
| zz_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
zz_stack_print (zztype_int16 *zzbottom, zztype_int16 *zztop)
{
  ZZFPRINTF (stderr, "Stack now");
  for (; zzbottom <= zztop; zzbottom++)
    {
      int zzbot = *zzbottom;
      ZZFPRINTF (stderr, " %d", zzbot);
    }
  ZZFPRINTF (stderr, "\n");
}

# define ZZ_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (zzdebug)                                                  \
    zz_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the ZZRULE is going to be reduced.  |
`------------------------------------------------*/

static void
zz_reduce_print (zztype_int16 *zzssp, ZZSTYPE *zzvsp, int zzrule)
{
  unsigned long int zzlno = zzrline[zzrule];
  int zznrhs = zzr2[zzrule];
  int zzi;
  ZZFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             zzrule - 1, zzlno);
  /* The symbols being reduced.  */
  for (zzi = 0; zzi < zznrhs; zzi++)
    {
      ZZFPRINTF (stderr, "   $%d = ", zzi + 1);
      zz_symbol_print (stderr,
                       zzstos[zzssp[zzi + 1 - zznrhs]],
                       &(zzvsp[(zzi + 1) - (zznrhs)])
                                              );
      ZZFPRINTF (stderr, "\n");
    }
}

# define ZZ_REDUCE_PRINT(Rule)          \
do {                                    \
  if (zzdebug)                          \
    zz_reduce_print (zzssp, zzvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int zzdebug;
#else /* !ZZDEBUG */
# define ZZDPRINTF(Args)
# define ZZ_SYMBOL_PRINT(Title, Type, Value, Location)
# define ZZ_STACK_PRINT(Bottom, Top)
# define ZZ_REDUCE_PRINT(Rule)
#endif /* !ZZDEBUG */


/* ZZINITDEPTH -- initial size of the parser's stacks.  */
#ifndef ZZINITDEPTH
# define ZZINITDEPTH 200
#endif

/* ZZMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   ZZSTACK_ALLOC_MAXIMUM < ZZSTACK_BYTES (ZZMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef ZZMAXDEPTH
# define ZZMAXDEPTH 10000
#endif


#if ZZERROR_VERBOSE

# ifndef zzstrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define zzstrlen strlen
#  else
/* Return the length of ZZSTR.  */
static ZZSIZE_T
zzstrlen (const char *zzstr)
{
  ZZSIZE_T zzlen;
  for (zzlen = 0; zzstr[zzlen]; zzlen++)
    continue;
  return zzlen;
}
#  endif
# endif

# ifndef zzstpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define zzstpcpy stpcpy
#  else
/* Copy ZZSRC to ZZDEST, returning the address of the terminating '\0' in
   ZZDEST.  */
static char *
zzstpcpy (char *zzdest, const char *zzsrc)
{
  char *zzd = zzdest;
  const char *zzs = zzsrc;

  while ((*zzd++ = *zzs++) != '\0')
    continue;

  return zzd - 1;
}
#  endif
# endif

# ifndef zztnamerr
/* Copy to ZZRES the contents of ZZSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for zzerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  ZZSTR is taken from zztname.  If ZZRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static ZZSIZE_T
zztnamerr (char *zzres, const char *zzstr)
{
  if (*zzstr == '"')
    {
      ZZSIZE_T zzn = 0;
      char const *zzp = zzstr;

      for (;;)
        switch (*++zzp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++zzp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (zzres)
              zzres[zzn] = *zzp;
            zzn++;
            break;

          case '"':
            if (zzres)
              zzres[zzn] = '\0';
            return zzn;
          }
    do_not_strip_quotes: ;
    }

  if (! zzres)
    return zzstrlen (zzstr);

  return zzstpcpy (zzres, zzstr) - zzres;
}
# endif

/* Copy into *ZZMSG, which is of size *ZZMSG_ALLOC, an error message
   about the unexpected token ZZTOKEN for the state stack whose top is
   ZZSSP.

   Return 0 if *ZZMSG was successfully written.  Return 1 if *ZZMSG is
   not large enough to hold the message.  In that case, also set
   *ZZMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
zzsyntax_error (ZZSIZE_T *zzmsg_alloc, char **zzmsg,
                zztype_int16 *zzssp, int zztoken)
{
  ZZSIZE_T zzsize0 = zztnamerr (ZZ_NULLPTR, zztname[zztoken]);
  ZZSIZE_T zzsize = zzsize0;
  enum { ZZERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *zzformat = ZZ_NULLPTR;
  /* Arguments of zzformat. */
  char const *zzarg[ZZERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int zzcount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in zzchar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated zzchar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (zztoken != ZZEMPTY)
    {
      int zzn = zzpact[*zzssp];
      zzarg[zzcount++] = zztname[zztoken];
      if (!zzpact_value_is_default (zzn))
        {
          /* Start ZZX at -ZZN if negative to avoid negative indexes in
             ZZCHECK.  In other words, skip the first -ZZN actions for
             this state because they are default actions.  */
          int zzxbegin = zzn < 0 ? -zzn : 0;
          /* Stay within bounds of both zzcheck and zztname.  */
          int zzchecklim = ZZLAST - zzn + 1;
          int zzxend = zzchecklim < ZZNTOKENS ? zzchecklim : ZZNTOKENS;
          int zzx;

          for (zzx = zzxbegin; zzx < zzxend; ++zzx)
            if (zzcheck[zzx + zzn] == zzx && zzx != ZZTERROR
                && !zztable_value_is_error (zztable[zzx + zzn]))
              {
                if (zzcount == ZZERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    zzcount = 1;
                    zzsize = zzsize0;
                    break;
                  }
                zzarg[zzcount++] = zztname[zzx];
                {
                  ZZSIZE_T zzsize1 = zzsize + zztnamerr (ZZ_NULLPTR, zztname[zzx]);
                  if (! (zzsize <= zzsize1
                         && zzsize1 <= ZZSTACK_ALLOC_MAXIMUM))
                    return 2;
                  zzsize = zzsize1;
                }
              }
        }
    }

  switch (zzcount)
    {
# define ZZCASE_(N, S)                      \
      case N:                               \
        zzformat = S;                       \
      break
      ZZCASE_(0, ZZ_("syntax error"));
      ZZCASE_(1, ZZ_("syntax error, unexpected %s"));
      ZZCASE_(2, ZZ_("syntax error, unexpected %s, expecting %s"));
      ZZCASE_(3, ZZ_("syntax error, unexpected %s, expecting %s or %s"));
      ZZCASE_(4, ZZ_("syntax error, unexpected %s, expecting %s or %s or %s"));
      ZZCASE_(5, ZZ_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef ZZCASE_
    }

  {
    ZZSIZE_T zzsize1 = zzsize + zzstrlen (zzformat);
    if (! (zzsize <= zzsize1 && zzsize1 <= ZZSTACK_ALLOC_MAXIMUM))
      return 2;
    zzsize = zzsize1;
  }

  if (*zzmsg_alloc < zzsize)
    {
      *zzmsg_alloc = 2 * zzsize;
      if (! (zzsize <= *zzmsg_alloc
             && *zzmsg_alloc <= ZZSTACK_ALLOC_MAXIMUM))
        *zzmsg_alloc = ZZSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *zzp = *zzmsg;
    int zzi = 0;
    while ((*zzp = *zzformat) != '\0')
      if (*zzp == '%' && zzformat[1] == 's' && zzi < zzcount)
        {
          zzp += zztnamerr (zzp, zzarg[zzi++]);
          zzformat += 2;
        }
      else
        {
          zzp++;
          zzformat++;
        }
  }
  return 0;
}
#endif /* ZZERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
zzdestruct (const char *zzmsg, int zztype, ZZSTYPE *zzvaluep)
{
  ZZUSE (zzvaluep);
  if (!zzmsg)
    zzmsg = "Deleting";
  ZZ_SYMBOL_PRINT (zzmsg, zztype, zzvaluep, zzlocationp);

  ZZ_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ZZUSE (zztype);
  ZZ_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int zzchar;

/* The semantic value of the lookahead symbol.  */
ZZSTYPE zzlval;
/* Number of syntax errors so far.  */
int zznerrs;


/*----------.
| zzparse.  |
`----------*/

int
zzparse (void)
{
    int zzstate;
    /* Number of tokens to shift before error messages enabled.  */
    int zzerrstatus;

    /* The stacks and their tools:
       'zzss': related to states.
       'zzvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow zzoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    zztype_int16 zzssa[ZZINITDEPTH];
    zztype_int16 *zzss;
    zztype_int16 *zzssp;

    /* The semantic value stack.  */
    ZZSTYPE zzvsa[ZZINITDEPTH];
    ZZSTYPE *zzvs;
    ZZSTYPE *zzvsp;

    ZZSIZE_T zzstacksize;

  int zzn;
  int zzresult;
  /* Lookahead token as an internal (translated) token number.  */
  int zztoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  ZZSTYPE zzval;

#if ZZERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char zzmsgbuf[128];
  char *zzmsg = zzmsgbuf;
  ZZSIZE_T zzmsg_alloc = sizeof zzmsgbuf;
#endif

#define ZZPOPSTACK(N)   (zzvsp -= (N), zzssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int zzlen = 0;

  zzssp = zzss = zzssa;
  zzvsp = zzvs = zzvsa;
  zzstacksize = ZZINITDEPTH;

  ZZDPRINTF ((stderr, "Starting parse\n"));

  zzstate = 0;
  zzerrstatus = 0;
  zznerrs = 0;
  zzchar = ZZEMPTY; /* Cause a token to be read.  */
  goto zzsetstate;

/*------------------------------------------------------------.
| zznewstate -- Push a new state, which is found in zzstate.  |
`------------------------------------------------------------*/
 zznewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  zzssp++;

 zzsetstate:
  *zzssp = zzstate;

  if (zzss + zzstacksize - 1 <= zzssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      ZZSIZE_T zzsize = zzssp - zzss + 1;

#ifdef zzoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        ZZSTYPE *zzvs1 = zzvs;
        zztype_int16 *zzss1 = zzss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if zzoverflow is a macro.  */
        zzoverflow (ZZ_("memory exhausted"),
                    &zzss1, zzsize * sizeof (*zzssp),
                    &zzvs1, zzsize * sizeof (*zzvsp),
                    &zzstacksize);

        zzss = zzss1;
        zzvs = zzvs1;
      }
#else /* no zzoverflow */
# ifndef ZZSTACK_RELOCATE
      goto zzexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (ZZMAXDEPTH <= zzstacksize)
        goto zzexhaustedlab;
      zzstacksize *= 2;
      if (ZZMAXDEPTH < zzstacksize)
        zzstacksize = ZZMAXDEPTH;

      {
        zztype_int16 *zzss1 = zzss;
        union zzalloc *zzptr =
          (union zzalloc *) ZZSTACK_ALLOC (ZZSTACK_BYTES (zzstacksize));
        if (! zzptr)
          goto zzexhaustedlab;
        ZZSTACK_RELOCATE (zzss_alloc, zzss);
        ZZSTACK_RELOCATE (zzvs_alloc, zzvs);
#  undef ZZSTACK_RELOCATE
        if (zzss1 != zzssa)
          ZZSTACK_FREE (zzss1);
      }
# endif
#endif /* no zzoverflow */

      zzssp = zzss + zzsize - 1;
      zzvsp = zzvs + zzsize - 1;

      ZZDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) zzstacksize));

      if (zzss + zzstacksize - 1 <= zzssp)
        ZZABORT;
    }

  ZZDPRINTF ((stderr, "Entering state %d\n", zzstate));

  if (zzstate == ZZFINAL)
    ZZACCEPT;

  goto zzbackup;

/*-----------.
| zzbackup.  |
`-----------*/
zzbackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  zzn = zzpact[zzstate];
  if (zzpact_value_is_default (zzn))
    goto zzdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* ZZCHAR is either ZZEMPTY or ZZEOF or a valid lookahead symbol.  */
  if (zzchar == ZZEMPTY)
    {
      ZZDPRINTF ((stderr, "Reading a token: "));
      zzchar = zzlex ();
    }

  if (zzchar <= ZZEOF)
    {
      zzchar = zztoken = ZZEOF;
      ZZDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      zztoken = ZZTRANSLATE (zzchar);
      ZZ_SYMBOL_PRINT ("Next token is", zztoken, &zzlval, &zzlloc);
    }

  /* If the proper action on seeing token ZZTOKEN is to reduce or to
     detect an error, take that action.  */
  zzn += zztoken;
  if (zzn < 0 || ZZLAST < zzn || zzcheck[zzn] != zztoken)
    goto zzdefault;
  zzn = zztable[zzn];
  if (zzn <= 0)
    {
      if (zztable_value_is_error (zzn))
        goto zzerrlab;
      zzn = -zzn;
      goto zzreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (zzerrstatus)
    zzerrstatus--;

  /* Shift the lookahead token.  */
  ZZ_SYMBOL_PRINT ("Shifting", zztoken, &zzlval, &zzlloc);

  /* Discard the shifted token.  */
  zzchar = ZZEMPTY;

  zzstate = zzn;
  ZZ_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++zzvsp = zzlval;
  ZZ_IGNORE_MAYBE_UNINITIALIZED_END

  goto zznewstate;


/*-----------------------------------------------------------.
| zzdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
zzdefault:
  zzn = zzdefact[zzstate];
  if (zzn == 0)
    goto zzerrlab;
  goto zzreduce;


/*-----------------------------.
| zzreduce -- Do a reduction.  |
`-----------------------------*/
zzreduce:
  /* zzn is the number of a rule to reduce with.  */
  zzlen = zzr2[zzn];

  /* If ZZLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets ZZVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to ZZVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that ZZVAL may be used uninitialized.  */
  zzval = zzvsp[1-zzlen];


  ZZ_REDUCE_PRINT (zzn);
  switch (zzn)
    {
        case 2:
#line 67 "ifyacc.y" /* yacc.c:1646  */
    {ifResult = (zzvsp[0].i); }
#line 1298 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 70 "ifyacc.y" /* yacc.c:1646  */
    { LOG(("Undefined constant %s", (zzvsp[0].str))); (zzval.i) = 0; }
#line 1304 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 71 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-2].i) + (zzvsp[0].i); }
#line 1310 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 72 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-2].i) - (zzvsp[0].i); }
#line 1316 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 73 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-2].i) * (zzvsp[0].i); }
#line 1322 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 74 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-2].i) / (zzvsp[0].i); }
#line 1328 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 75 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-2].i) % (zzvsp[0].i); }
#line 1334 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 76 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-2].i) | (zzvsp[0].i); }
#line 1340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 77 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-2].i) & (zzvsp[0].i); }
#line 1346 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 78 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-3].i) || (zzvsp[0].i); }
#line 1352 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 79 "ifyacc.y" /* yacc.c:1646  */
    {(zzval.i) = (zzvsp[-3].i) && (zzvsp[0].i); }
#line 1358 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 80 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-1].i); }
#line 1364 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 81 "ifyacc.y" /* yacc.c:1646  */
    {(zzval.i) = !(zzvsp[0].i); }
#line 1370 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 82 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-2].i) > (zzvsp[0].i); }
#line 1376 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 83 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = (zzvsp[-2].i) < (zzvsp[0].i); }
#line 1382 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 84 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = ((zzvsp[-3].i) >= (zzvsp[0].i)); }
#line 1388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 85 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = ((zzvsp[-3].i) <= (zzvsp[0].i)); }
#line 1394 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 86 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = ((zzvsp[-3].i) == (zzvsp[0].i)); }
#line 1400 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 87 "ifyacc.y" /* yacc.c:1646  */
    { (zzval.i) = ((zzvsp[-3].i) != (zzvsp[0].i)); }
#line 1406 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 88 "ifyacc.y" /* yacc.c:1646  */
    { LOG(("%s is%s defined", (zzvsp[-1].str), match_define((zzvsp[-1].str)) ? "" : " not")); (zzval.i) = match_define((zzvsp[-1].str)) ? 1 : 0; str_free((zzvsp[-1].str)); }
#line 1412 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 89 "ifyacc.y" /* yacc.c:1646  */
    { LOG(("%s is%s defined", (zzvsp[0].str), match_define((zzvsp[0].str)) ? "" : " not")); (zzval.i) = match_define((zzvsp[0].str)) ? 1 : 0; str_free((zzvsp[0].str)); }
#line 1418 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 90 "ifyacc.y" /* yacc.c:1646  */
    { LOG(("%s is%s defined", (zzvsp[-1].str), match_define((zzvsp[-1].str)) ? "" : " not")); (zzval.i) = !match_define((zzvsp[-1].str)) ? 1 : 0; str_free((zzvsp[-1].str)); }
#line 1424 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 91 "ifyacc.y" /* yacc.c:1646  */
    { LOG(("%s is%s defined", (zzvsp[0].str), match_define((zzvsp[0].str)) ? "" : " not")); (zzval.i) = !match_define((zzvsp[0].str)) ? 1 : 0; str_free((zzvsp[0].str)); }
#line 1430 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1434 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter zzchar, and that requires
     that zztoken be updated with the new translation.  We take the
     approach of translating immediately before every use of zztoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     ZZABORT, ZZACCEPT, or ZZERROR immediately after altering zzchar or
     if it invokes ZZBACKUP.  In the case of ZZABORT or ZZACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of ZZERROR or ZZBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  ZZ_SYMBOL_PRINT ("-> $$ =", zzr1[zzn], &zzval, &zzloc);

  ZZPOPSTACK (zzlen);
  zzlen = 0;
  ZZ_STACK_PRINT (zzss, zzssp);

  *++zzvsp = zzval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  zzn = zzr1[zzn];

  zzstate = zzpgoto[zzn - ZZNTOKENS] + *zzssp;
  if (0 <= zzstate && zzstate <= ZZLAST && zzcheck[zzstate] == *zzssp)
    zzstate = zztable[zzstate];
  else
    zzstate = zzdefgoto[zzn - ZZNTOKENS];

  goto zznewstate;


/*--------------------------------------.
| zzerrlab -- here on detecting error.  |
`--------------------------------------*/
zzerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  zztoken = zzchar == ZZEMPTY ? ZZEMPTY : ZZTRANSLATE (zzchar);

  /* If not already recovering from an error, report this error.  */
  if (!zzerrstatus)
    {
      ++zznerrs;
#if ! ZZERROR_VERBOSE
      zzerror (ZZ_("syntax error"));
#else
# define ZZSYNTAX_ERROR zzsyntax_error (&zzmsg_alloc, &zzmsg, \
                                        zzssp, zztoken)
      {
        char const *zzmsgp = ZZ_("syntax error");
        int zzsyntax_error_status;
        zzsyntax_error_status = ZZSYNTAX_ERROR;
        if (zzsyntax_error_status == 0)
          zzmsgp = zzmsg;
        else if (zzsyntax_error_status == 1)
          {
            if (zzmsg != zzmsgbuf)
              ZZSTACK_FREE (zzmsg);
            zzmsg = (char *) ZZSTACK_ALLOC (zzmsg_alloc);
            if (!zzmsg)
              {
                zzmsg = zzmsgbuf;
                zzmsg_alloc = sizeof zzmsgbuf;
                zzsyntax_error_status = 2;
              }
            else
              {
                zzsyntax_error_status = ZZSYNTAX_ERROR;
                zzmsgp = zzmsg;
              }
          }
        zzerror (zzmsgp);
        if (zzsyntax_error_status == 2)
          goto zzexhaustedlab;
      }
# undef ZZSYNTAX_ERROR
#endif
    }



  if (zzerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (zzchar <= ZZEOF)
        {
          /* Return failure if at end of input.  */
          if (zzchar == ZZEOF)
            ZZABORT;
        }
      else
        {
          zzdestruct ("Error: discarding",
                      zztoken, &zzlval);
          zzchar = ZZEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto zzerrlab1;


/*---------------------------------------------------.
| zzerrorlab -- error raised explicitly by ZZERROR.  |
`---------------------------------------------------*/
zzerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     ZZERROR and the label zzerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto zzerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this ZZERROR.  */
  ZZPOPSTACK (zzlen);
  zzlen = 0;
  ZZ_STACK_PRINT (zzss, zzssp);
  zzstate = *zzssp;
  goto zzerrlab1;


/*-------------------------------------------------------------.
| zzerrlab1 -- common code for both syntax error and ZZERROR.  |
`-------------------------------------------------------------*/
zzerrlab1:
  zzerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      zzn = zzpact[zzstate];
      if (!zzpact_value_is_default (zzn))
        {
          zzn += ZZTERROR;
          if (0 <= zzn && zzn <= ZZLAST && zzcheck[zzn] == ZZTERROR)
            {
              zzn = zztable[zzn];
              if (0 < zzn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (zzssp == zzss)
        ZZABORT;


      zzdestruct ("Error: popping",
                  zzstos[zzstate], zzvsp);
      ZZPOPSTACK (1);
      zzstate = *zzssp;
      ZZ_STACK_PRINT (zzss, zzssp);
    }

  ZZ_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++zzvsp = zzlval;
  ZZ_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  ZZ_SYMBOL_PRINT ("Shifting", zzstos[zzn], zzvsp, zzlsp);

  zzstate = zzn;
  goto zznewstate;


/*-------------------------------------.
| zzacceptlab -- ZZACCEPT comes here.  |
`-------------------------------------*/
zzacceptlab:
  zzresult = 0;
  goto zzreturn;

/*-----------------------------------.
| zzabortlab -- ZZABORT comes here.  |
`-----------------------------------*/
zzabortlab:
  zzresult = 1;
  goto zzreturn;

#if !defined zzoverflow || ZZERROR_VERBOSE
/*-------------------------------------------------.
| zzexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
zzexhaustedlab:
  zzerror (ZZ_("memory exhausted"));
  zzresult = 2;
  /* Fall through.  */
#endif

zzreturn:
  if (zzchar != ZZEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      zztoken = ZZTRANSLATE (zzchar);
      zzdestruct ("Cleanup: discarding lookahead",
                  zztoken, &zzlval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this ZZABORT or ZZACCEPT.  */
  ZZPOPSTACK (zzlen);
  ZZ_STACK_PRINT (zzss, zzssp);
  while (zzssp != zzss)
    {
      zzdestruct ("Cleanup: popping",
                  zzstos[*zzssp], zzvsp);
      ZZPOPSTACK (1);
    }
#ifndef zzoverflow
  if (zzss != zzssa)
    ZZSTACK_FREE (zzss);
#endif
#if ZZERROR_VERBOSE
  if (zzmsg != zzmsgbuf)
    ZZSTACK_FREE (zzmsg);
#endif
  return zzresult;
}
#line 93 "ifyacc.y" /* yacc.c:1906  */


int zzerror(char *str)
{
  CERROR(("%s", str));
  return 0;
}
