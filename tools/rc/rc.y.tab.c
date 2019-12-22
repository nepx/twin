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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "rcyacc.y" /* yacc.c:339  */

/*************************************************************************

	rcyacc.y	1.31
	yacc resource file parser
  
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "windows.h"
#include "rc.h"
#include "define.h"
#include "buffer.h"
#include "resfunc.h"

extern int yyerror(char *);
extern int yylex();

void ignore_until_semicolon();
void ignore_until_brace(int);
void typedef_mode();
void initial_mode();
void ignore_rest_of_file();

int hex2dec(char *);

#define YYPRINT(file,type,val) if ((type == kLONGINT) || (type == kSHORTINT)) fprintf(file, " %d", val.i); else \
                               if (type == kSTRING) fprintf(file, " %s", val.str);

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

#line 123 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    kSTRING = 258,
    kQSTRING = 259,
    kASTRING = 260,
    kHEXCHARS = 261,
    kLONGINT = 262,
    kSHORTINT = 263,
    kFLAG = 264,
    kTABLENAME = 265,
    kACCELERATORS = 266,
    kBITMAP = 267,
    kCURSOR = 268,
    kDIALOG = 269,
    kDIALOGEX = 270,
    kICON = 271,
    kMENU = 272,
    kSTRINGTABLE = 273,
    kVERSIONINFO = 274,
    kRCDATA = 275,
    kDLGINIT = 276,
    kTOOLBAR = 277,
    kBUTTON = 278,
    kFILEVERSION = 279,
    kPRODUCTVERSION = 280,
    kFILEFLAGSMASK = 281,
    kFILEFLAGS = 282,
    kFILEOS = 283,
    kFILETYPE = 284,
    kFILESUBTYPE = 285,
    kBLOCK = 286,
    kVALUE = 287,
    kPOPUP = 288,
    kMENUITEM = 289,
    kSEPARATOR = 290,
    kCHECKED = 291,
    kGRAYED = 292,
    kHELP = 293,
    kINACTIVE = 294,
    kSTYLE = 295,
    kCAPTION = 296,
    kCLASS = 297,
    kFONT = 298,
    kLANGUAGE = 299,
    kLTEXT = 300,
    kRTEXT = 301,
    kCTEXT = 302,
    kEDITTEXT = 303,
    kLISTBOX = 304,
    kCOMBOBOX = 305,
    kGROUPBOX = 306,
    kPUSHBUTTON = 307,
    kDEFPUSHBUTTON = 308,
    kCHECKBOX = 309,
    kAUTOCHECKBOX = 310,
    kSTATE3 = 311,
    kAUTO3STATE = 312,
    kRADIOBUTTON = 313,
    kAUTORADIOBUTTON = 314,
    kSCROLLBAR = 315,
    kCONTROL = 316,
    kVIRTKEY = 317,
    kASCII = 318,
    kSHIFT = 319,
    kALT = 320,
    kNOINVERT = 321,
    kALIGN = 322,
    kENDIAN = 323,
    kBEGIN = 324,
    kEND = 325,
    kNOT = 326,
    kTYPEDEF = 327,
    kSTRUCT = 328,
    kCTYPE = 329,
    kNOTAVALIDTOKEN = 330
  };
#endif
/* Tokens.  */
#define kSTRING 258
#define kQSTRING 259
#define kASTRING 260
#define kHEXCHARS 261
#define kLONGINT 262
#define kSHORTINT 263
#define kFLAG 264
#define kTABLENAME 265
#define kACCELERATORS 266
#define kBITMAP 267
#define kCURSOR 268
#define kDIALOG 269
#define kDIALOGEX 270
#define kICON 271
#define kMENU 272
#define kSTRINGTABLE 273
#define kVERSIONINFO 274
#define kRCDATA 275
#define kDLGINIT 276
#define kTOOLBAR 277
#define kBUTTON 278
#define kFILEVERSION 279
#define kPRODUCTVERSION 280
#define kFILEFLAGSMASK 281
#define kFILEFLAGS 282
#define kFILEOS 283
#define kFILETYPE 284
#define kFILESUBTYPE 285
#define kBLOCK 286
#define kVALUE 287
#define kPOPUP 288
#define kMENUITEM 289
#define kSEPARATOR 290
#define kCHECKED 291
#define kGRAYED 292
#define kHELP 293
#define kINACTIVE 294
#define kSTYLE 295
#define kCAPTION 296
#define kCLASS 297
#define kFONT 298
#define kLANGUAGE 299
#define kLTEXT 300
#define kRTEXT 301
#define kCTEXT 302
#define kEDITTEXT 303
#define kLISTBOX 304
#define kCOMBOBOX 305
#define kGROUPBOX 306
#define kPUSHBUTTON 307
#define kDEFPUSHBUTTON 308
#define kCHECKBOX 309
#define kAUTOCHECKBOX 310
#define kSTATE3 311
#define kAUTO3STATE 312
#define kRADIOBUTTON 313
#define kAUTORADIOBUTTON 314
#define kSCROLLBAR 315
#define kCONTROL 316
#define kVIRTKEY 317
#define kASCII 318
#define kSHIFT 319
#define kALT 320
#define kNOINVERT 321
#define kALIGN 322
#define kENDIAN 323
#define kBEGIN 324
#define kEND 325
#define kNOT 326
#define kTYPEDEF 327
#define kSTRUCT 328
#define kCTYPE 329
#define kNOTAVALIDTOKEN 330

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 60 "rcyacc.y" /* yacc.c:355  */

  strp str;
  buffp b;
  resp resource;
  struct maskvals mask;
  struct dimvals dim;
  struct xyvals xy;
  struct binflagvals binflags;
  struct userresbuffer *urb;
  unsigned long i;
  char c;

#line 326 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 343 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
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

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1009

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  92
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  87
/* YYNRULES -- Number of rules.  */
#define YYNRULES  241
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  566

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   330

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    76,     2,     2,     2,    81,    82,    90,
      87,    88,    79,    77,    89,    78,     2,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    91,
      84,    86,    85,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    83,     2,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   195,   195,   196,   197,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   218,   219,   222,   223,   226,   229,   230,   233,   234,
     237,   238,   239,   243,   244,   247,   248,   249,   250,   251,
     254,   257,   258,   261,   262,   265,   266,   269,   270,   273,
     274,   275,   276,   277,   278,   281,   284,   285,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   299,   302,   303,
     304,   307,   308,   311,   312,   313,   316,   317,   318,   322,
     325,   327,   329,   331,   333,   335,   337,   339,   341,   343,
     345,   347,   352,   354,   359,   364,   369,   371,   375,   376,
     380,   385,   387,   393,   394,   400,   402,   407,   412,   414,
     419,   425,   426,   431,   433,   437,   438,   441,   442,   445,
     448,   451,   454,   457,   460,   461,   464,   465,   468,   469,
     472,   473,   474,   475,   478,   479,   480,   481,   485,   488,
     490,   491,   493,   494,   495,   496,   497,   498,   499,   501,
     502,   504,   505,   508,   509,   511,   512,   515,   516,   517,
     519,   520,   522,   525,   526,   529,   530,   533,   534,   537,
     538,   541,   542,   545,   548,   549,   551,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   568,   569,   570,   571,
     572,   573,   574,   575,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   598,   599,   600,   601,   604,   605,   608,   609,
     632,   633,   634,   640,   640,   644,   645,   650,   651,   650,
     660,   661,   660,   674,   675,   674,   678,   678,   690,   691,
     696,   697
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "kSTRING", "kQSTRING", "kASTRING",
  "kHEXCHARS", "kLONGINT", "kSHORTINT", "kFLAG", "kTABLENAME",
  "kACCELERATORS", "kBITMAP", "kCURSOR", "kDIALOG", "kDIALOGEX", "kICON",
  "kMENU", "kSTRINGTABLE", "kVERSIONINFO", "kRCDATA", "kDLGINIT",
  "kTOOLBAR", "kBUTTON", "kFILEVERSION", "kPRODUCTVERSION",
  "kFILEFLAGSMASK", "kFILEFLAGS", "kFILEOS", "kFILETYPE", "kFILESUBTYPE",
  "kBLOCK", "kVALUE", "kPOPUP", "kMENUITEM", "kSEPARATOR", "kCHECKED",
  "kGRAYED", "kHELP", "kINACTIVE", "kSTYLE", "kCAPTION", "kCLASS", "kFONT",
  "kLANGUAGE", "kLTEXT", "kRTEXT", "kCTEXT", "kEDITTEXT", "kLISTBOX",
  "kCOMBOBOX", "kGROUPBOX", "kPUSHBUTTON", "kDEFPUSHBUTTON", "kCHECKBOX",
  "kAUTOCHECKBOX", "kSTATE3", "kAUTO3STATE", "kRADIOBUTTON",
  "kAUTORADIOBUTTON", "kSCROLLBAR", "kCONTROL", "kVIRTKEY", "kASCII",
  "kSHIFT", "kALT", "kNOINVERT", "kALIGN", "kENDIAN", "kBEGIN", "kEND",
  "kNOT", "kTYPEDEF", "kSTRUCT", "kCTYPE", "kNOTAVALIDTOKEN", "'!'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'&'", "'|'", "'<'", "'>'", "'='", "'('",
  "')'", "','", "'\\''", "';'", "$accept", "resourcefile", "resource",
  "resourceid", "flags", "menudata", "menudef", "menuitemlist", "menuitem",
  "menuflags", "menuflag", "acceldata", "acceldef", "accelitemlist",
  "accelitem", "accelflags", "accelflag", "dialogdata", "dlgdef",
  "dlgbase", "dims", "coords", "controls", "controlstyle", "styleflags",
  "control", "controlid", "dlginitdata", "dlginitdef", "dlginititemlist",
  "toolbardata", "toolbardef", "toolbardims", "toolbaritemlist",
  "toolbaritem", "strtab", "strtablines", "strtabline", "bitmapdata",
  "cursordata", "icondata", "rcdatares", "binaryres", "userres",
  "userreslines", "userresline", "binresflags", "versioninfodata",
  "fixedinfo", "fistmnt", "versioninfoblock", "viblocks", "viblockstmnts",
  "valuestmnt", "viblockstmnt", "manyqstrings", "fileref", "binary",
  "binarydata", "binarylines", "binaryline", "binarybytes", "binarybyte",
  "maybelanguage", "languagespec", "aexpr", "nonparenexpr", "expr", "str",
  "astr", "integer", "c_declaration", "begin_block_end", "$@1",
  "begin_block_end_or_null", "struct_union_class", "$@2", "$@3", "typedef",
  "$@4", "$@5", "prototype", "$@6", "$@7", "$@8", "fieldsep", "Woptions", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,    33,    43,    45,    42,
      47,    37,    38,   124,    60,    62,    61,    40,    41,    44,
      39,    59
};
# endif

#define YYPACT_NINF -408

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-408)))

#define YYTABLE_NINF -239

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -408,   408,  -408,  -408,  -408,   -80,  -408,  -408,  -408,   450,
    -408,  -408,   294,  -408,  -408,  -408,  -408,   768,  -408,  -408,
    -408,   742,  -408,  -408,  -408,  -408,  -408,  -408,  -408,     8,
    -408,   395,   294,   461,  -408,     2,     2,     2,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,
     294,   294,   294,   294,   294,   294,   294,   -42,  -408,  -408,
      -6,   294,   294,   294,   294,   294,   294,   294,  -408,  -408,
     569,   294,   294,   294,   294,   294,   294,   294,    -9,   294,
    -408,     2,  -408,  -408,  -408,     8,   336,   336,    78,    78,
     336,     8,  -408,   290,    -4,   398,   290,   757,   757,   757,
     757,   757,   757,   757,  -408,    28,  -408,   461,   461,   757,
     757,   757,   757,   757,    52,  -408,   267,   267,   327,   327,
     327,  -408,  -408,  -408,   461,     2,  -408,    59,    10,    37,
    -408,    91,  -408,  -408,    14,  -408,  -408,  -408,  -408,  -408,
    -408,  -408,    -9,  -408,  -408,   443,  -408,    -9,   461,  -408,
    -408,  -408,  -408,    95,  -408,  -408,  -408,   331,  -408,    29,
    -408,  -408,    65,  -408,  -408,   116,  -408,   120,   234,   294,
     294,   188,  -408,  -408,  -408,   128,  -408,  -408,  -408,    51,
    -408,   140,   -39,   294,   511,    53,   450,   450,   104,   554,
    -408,   106,   119,   161,  -408,   173,   450,    18,  -408,   148,
     167,   215,   181,  -408,  -408,  -408,   461,   461,   461,   -29,
    -408,   155,   102,  -408,  -408,    26,  -408,   757,  -408,  -408,
     242,   242,   715,  -408,  -408,  -408,    -9,   461,   157,   693,
    -408,  -408,   757,   177,   -20,   757,  -408,  -408,   182,  -408,
    -408,    99,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,
    -408,   228,   -17,   450,   294,   294,  -408,   294,  -408,    -9,
    -408,  -408,   461,   153,    -2,   715,   609,   110,   242,   242,
     242,   242,   242,   227,   232,   179,   237,   214,   450,   450,
    -408,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,
      -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,  -408,
    -408,  -408,   450,   348,  -408,  -408,  -408,  -408,    40,  -408,
     312,  -408,  -408,  -408,  -408,   461,   461,   168,   392,   294,
     294,  -408,  -408,  -408,  -408,  -408,   242,   647,   647,   346,
     346,   346,   242,   412,   242,   412,   242,   244,   242,   244,
     242,  -408,  -408,   511,   450,   450,   450,   124,   124,   124,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   124,
     450,  -408,  -408,    -9,  -408,  -408,   294,   294,   324,   328,
    -408,  -408,  -408,  -408,   757,   757,   244,   412,   412,   244,
     244,   244,    -9,    -9,    -9,    -9,  -408,    -9,   757,    -9,
      -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,
      -9,    -9,    67,   294,   757,   757,   257,    -9,   274,   137,
     159,   124,   124,   124,   124,   106,   106,   106,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   106,   124,  -408,
     225,   757,  -408,   294,   166,  -408,  -408,   765,    -9,    -9,
      -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,
      -9,    -9,    -9,    -9,    -9,    -9,  -408,  -408,  -408,  -408,
    -408,   757,    -9,   285,   556,  -408,   330,   461,  -408,  -408,
    -408,  -408,  -408,  -408,   106,   106,   106,   106,   251,   251,
     251,   106,   106,   106,   106,   106,   450,   450,   106,   106,
     251,   450,   225,  -408,  -408,  -408,   294,    -9,    -9,    -9,
      -9,   349,   251,  -408,   126,  -408,  -408,  -408,    -9,    -9,
      -9,    -9,    -9,    -9,    -9,    -9,    -9,  -408,    -9,   333,
     757,   251,   251,   251,   251,  -408,   -49,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,   253,  -408,  -408,  -408,
    -408,  -408,    34,    34,  -408,  -408,    34,   106,   106,   106,
    -408,  -408,   141,   251,  -408,   253
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,   212,   213,   214,   215,   218,   219,     0,
      23,   140,     0,   230,   227,   233,     3,     0,    20,    22,
      21,   186,     4,   220,   221,   222,   236,   214,    17,   174,
      19,     0,     0,   238,   177,   225,   225,   225,    23,    23,
      23,    23,    23,    23,    23,   140,    23,    23,    23,    23,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    14,
       0,     0,     0,     0,     0,     0,     0,     0,   149,   141,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     223,   225,   231,   228,   234,   174,     0,     0,     0,     0,
       0,   174,    18,   134,     0,     0,   134,   187,   188,   189,
     190,   191,   193,   192,   237,     0,   175,   238,   238,   144,
     145,   146,   147,   148,     0,   185,   178,   179,   180,   181,
     182,   184,   183,   239,   238,   225,   226,     0,     0,     0,
      13,     0,   216,   217,     0,     5,   164,   120,   163,   162,
       6,   121,   238,     9,    55,     0,    58,   238,   238,    10,
       7,   122,     8,     0,    15,   123,   125,     0,   124,     0,
      11,   100,     0,    12,   107,     0,    16,     0,     0,     0,
       0,     0,   139,   150,   176,     0,   232,   229,   235,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,     0,     0,    25,     0,     0,     0,   138,     0,
       0,     0,     0,   116,   115,   118,   238,   238,   238,     0,
     224,     0,     0,   166,   165,     0,   168,    69,    63,    60,
       0,     0,    59,   194,    61,    62,   238,   238,     0,     0,
      67,    70,    68,     0,     0,   135,   136,   137,     0,   132,
     131,   238,   128,   133,   130,   102,   105,   104,   101,   106,
     110,     0,     0,     0,     0,     0,   153,     0,    42,   238,
      41,    44,   238,     0,     0,   205,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,    56,
      72,    27,     0,     0,    26,    29,   127,   126,     0,   109,
       0,   114,   108,   112,   119,   238,   238,     0,     0,     0,
       0,   170,   173,   169,   172,   204,     0,   195,   196,   197,
     198,   199,     0,   201,     0,   200,     0,   207,     0,   206,
       0,    65,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    33,    31,   238,   129,   113,     0,     0,     0,     0,
     151,   159,   154,   153,    47,    47,   211,   203,   202,   209,
     208,   210,   238,   238,   238,   238,    99,   238,    98,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,     0,   142,   143,     0,   238,     0,   238,
     238,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    30,
       0,    33,   153,     0,     0,   152,    48,     0,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,    36,    37,    39,    38,
      34,    35,   238,     0,     0,   160,   156,   238,    51,    49,
      50,    52,    53,    54,     0,     0,     0,     0,    73,    73,
      73,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      73,     0,    32,   157,   153,   161,     0,   238,   238,   238,
     238,     0,     0,    84,    74,    76,    85,    86,   238,   238,
     238,   238,   238,   238,   238,   238,   238,    96,   238,     0,
     155,    73,    73,    73,    73,    77,     0,     0,     0,    73,
      73,    73,    73,    73,     0,     0,    73,    73,     0,   158,
      83,    80,    81,    82,    79,    78,    75,    87,    88,    89,
      92,    93,   238,   238,    90,    91,   238,     0,     0,     0,
      94,    95,   240,     0,    97,   241
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -408,  -408,  -408,   -16,   674,  -408,    15,  -408,  -408,   -68,
    -408,  -408,  -408,  -408,  -408,    76,  -408,   315,  -408,  -408,
    -407,   210,  -408,   139,    93,  -408,   -52,  -408,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,
    -408,  -408,   362,  -408,  -408,   151,  -408,   418,  -408,  -408,
    -408,  -408,  -370,  -408,  -408,  -408,   112,   127,  -408,  -408,
     283,  -408,  -408,   -19,     1,   513,  -408,  -179,   458,  -408,
      -1,  -408,  -408,  -408,   -25,  -408,  -408,  -408,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,    -5,  -408
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    16,    17,    29,   152,   194,   234,   305,   402,
     460,   130,   180,   212,   261,   409,   436,   143,   144,   145,
     146,   147,   229,   503,   504,   300,   387,   160,   161,   200,
     163,   164,   165,   252,   313,    59,   168,   205,   135,   140,
     150,   154,   155,   156,   241,   242,   157,    30,    31,    69,
     114,   173,   317,   371,   372,   466,   136,   137,   138,   182,
     243,   264,   324,    60,   106,   148,    19,   222,    20,   139,
      34,    22,    81,   125,    82,    23,    36,   128,    24,    35,
     127,    25,    37,   129,    57,   437,   564
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      21,    49,    18,   408,   322,    58,   310,    26,   442,   443,
     444,    83,    84,   302,   303,   181,    21,    58,   311,   238,
     454,     3,     4,    27,     6,   239,   240,   263,    79,   167,
     199,   214,  -171,  -103,   527,  -117,  -117,  -103,    12,   544,
     256,   265,   266,     3,     4,    27,     6,   239,   240,   104,
     304,   215,   211,   312,   219,   -43,   126,   257,   -43,   -43,
       7,     8,   463,   105,   176,   159,   131,   497,   498,   499,
     500,    80,   153,   123,   508,   509,   510,   511,   512,   142,
      78,   515,   516,   171,  -167,     7,     8,    58,   323,   327,
     328,   329,   330,   331,   333,   335,   337,   339,  -117,  -103,
     175,   177,   169,   170,  -167,   226,   259,   142,   215,     7,
       8,     7,     8,     7,     8,  -117,  -171,   527,  -103,   174,
     231,   -43,   172,    78,   519,   386,     7,     8,   178,   220,
     215,     7,     8,  -238,  -238,    12,   193,   183,   -43,    12,
     221,   -45,   191,   192,   -45,   -45,   190,   376,  -238,  -238,
     560,   561,   562,   377,   201,   378,    78,   379,   198,   380,
     179,   381,   233,   -46,   193,    32,   -46,   -46,   218,   307,
     465,   246,   260,     7,     8,   247,     3,     4,    27,     6,
       7,     8,   251,    21,   223,   202,     7,     8,    78,    32,
     203,    32,   209,    32,   -28,   -28,   326,  -238,   210,   368,
     369,   253,   254,   255,  -111,   158,    32,   -45,   158,   527,
     213,    32,  -238,  -238,   141,    78,  -111,   151,   245,   223,
     223,   278,   279,   250,   -45,   258,    78,   280,  -238,   -46,
      78,   -28,     7,     8,     7,     8,   308,   248,   370,     7,
       8,     7,     8,   321,     7,     8,   -46,   301,    78,     7,
       8,  -111,   306,    32,   319,   220,   249,   320,     7,     8,
      32,   456,   457,   458,   459,   336,   221,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   132,   133,   389,   390,   309,    58,
     340,     7,     8,   220,   204,   368,   369,   400,   220,   332,
     366,   367,    32,   220,   221,   334,   368,   369,   220,   221,
     365,    32,   501,   338,   221,   223,   432,   382,   406,   221,
     277,   223,   407,   223,   495,   223,   527,   223,   502,   223,
     132,   133,    21,   433,   435,    58,    73,    74,    75,    76,
      77,     3,     4,    27,     6,   493,     7,     8,   403,   438,
     439,   440,   441,   462,   368,   369,   445,   446,   447,   448,
     449,   450,   451,   452,   453,    12,   455,   411,   412,   413,
     414,    32,   415,   362,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   430,   195,   196,
     197,   230,   434,   539,   149,   134,   162,    58,     2,    76,
      77,     3,     4,     5,     6,     7,     8,   429,     9,    61,
      62,    63,    64,    65,    66,    67,    10,    11,   273,   274,
     275,   276,   277,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   410,    12,     3,     4,    27,     6,   492,   166,   364,
     184,   373,   496,    92,    68,   216,     0,    28,     0,    71,
      72,    73,    74,    75,    76,    77,     0,   505,   505,   505,
      13,    14,    15,   185,   186,   187,   188,    12,     0,   505,
       0,     0,   521,   522,   523,   524,   275,   276,   277,   528,
     525,   505,     0,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   189,   538,     3,     4,    27,     6,     7,     8,
     505,   505,   505,   505,     0,    33,   505,   505,   505,   505,
     505,   505,   505,   505,   505,   505,   505,   505,    71,    72,
      73,    74,    75,    76,    77,    70,     0,   557,   558,     0,
      78,   559,     0,     0,     0,   228,     0,   563,     0,     0,
       0,     0,   505,    97,    98,    99,   100,   101,   102,   103,
     -71,     0,     0,     0,   107,   108,   109,   110,   111,   112,
     113,     0,     0,     0,   116,   117,   118,   119,   120,   121,
     122,     0,   124,     0,     0,   526,     0,     0,     0,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,     0,     0,   506,   507,
     545,   546,     0,     0,   -71,   494,     0,   552,   553,   517,
       0,   556,     0,    71,    72,    73,    74,    75,    76,    77,
       0,     0,     0,     0,   224,   225,    71,    72,    73,    74,
      75,    76,    77,   236,   237,   244,   565,   115,     0,     0,
     540,   541,   542,   543,     0,     0,     0,     0,   547,   548,
     549,   550,   551,     0,     0,   554,   555,     0,     0,     0,
       0,   206,   207,   208,     0,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   217,   325,     0,     0,
       0,   227,     0,     0,     0,   232,     0,     0,   235,   281,
       0,   314,    85,    86,    87,    88,    89,    90,    91,     0,
      93,    94,    95,    96,     0,   262,   270,   271,   272,   273,
     274,   275,   276,   277,     0,     0,   341,   342,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,     0,     0,     0,     0,     0,
     361,   363,     0,   299,     0,     0,   244,   315,   316,     0,
     318,     3,     4,    27,     6,     7,     8,     0,     0,    38,
      39,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   383,   384,   385,     0,     0,     0,   391,   392,
     393,   394,   395,   396,   397,   398,   399,     0,   401,    50,
      51,    52,    53,    54,    55,    56,   468,   469,   470,   471,
     472,   473,   374,   375,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     388,   388,   388,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   388,     0,     0,     0,     0,     0,     0,   404,
     405,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,   388,   388,   388,   388,     0,     0,
       0,   388,   388,   388,   388,   388,   388,   388,   388,   388,
       0,   388,     0,   461,   513,   514,   464,   467,     0,   518,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   461,     0,     0,     0,   520
};

static const yytype_int16 yycheck[] =
{
       1,    17,     1,   373,     6,     9,    23,    87,   415,   416,
     417,    36,    37,    33,    34,     1,    17,     9,    35,     1,
     427,     3,     4,     5,     6,     7,     8,     1,    33,     1,
       1,    70,     6,     4,    83,     7,     8,     8,    44,    88,
      69,   220,   221,     3,     4,     5,     6,     7,     8,    91,
      70,    90,     1,    70,     1,     4,    81,    86,     7,     8,
       7,     8,   432,    69,     5,    69,    85,   474,   475,   476,
     477,    69,    91,    78,   481,   482,   483,   484,   485,     1,
      89,   488,   489,    31,    70,     7,     8,     9,    90,   268,
     269,   270,   271,   272,   273,   274,   275,   276,    70,    70,
     125,    91,   107,   108,    90,     1,     4,     1,    90,     7,
       8,     7,     8,     7,     8,    87,    90,    83,    89,   124,
       1,    70,    70,    89,   494,     1,     7,     8,    91,    76,
      90,     7,     8,     7,     8,    44,    69,   142,    87,    44,
      87,     4,   147,   148,     7,     8,   145,   326,     7,     8,
     557,   558,   559,   332,    89,   334,    89,   336,   157,   338,
      69,   340,     1,     4,    69,    87,     7,     8,   184,    70,
       4,     4,    70,     7,     8,     8,     3,     4,     5,     6,
       7,     8,     1,   184,   185,    69,     7,     8,    89,    87,
      70,    87,     4,    87,    33,    34,    86,    71,    70,    31,
      32,   206,   207,   208,    23,    93,    87,    70,    96,    83,
      70,    87,    71,    87,    87,    89,    35,    90,    70,   220,
     221,   226,   227,     8,    87,    70,    89,    70,    87,    70,
      89,    70,     7,     8,     7,     8,   241,    70,    70,     7,
       8,     7,     8,    90,     7,     8,    87,    70,    89,     7,
       8,    70,    70,    87,   259,    76,    89,   262,     7,     8,
      87,    36,    37,    38,    39,    86,    87,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,     4,     5,   348,   349,    70,     9,
      86,     7,     8,    76,    70,    31,    32,   359,    76,    82,
     315,   316,    87,    76,    87,    83,    31,    32,    76,    87,
       8,    87,    71,    86,    87,   326,    69,   343,     4,    87,
      86,   332,     4,   334,     4,   336,    83,   338,    87,   340,
       4,     5,   343,    86,    70,     9,    79,    80,    81,    82,
      83,     3,     4,     5,     6,    70,     7,     8,   363,   411,
     412,   413,   414,   431,    31,    32,   418,   419,   420,   421,
     422,   423,   424,   425,   426,    44,   428,   382,   383,   384,
     385,    87,   387,    35,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,    67,    68,
      69,   191,   407,    70,    89,    69,     8,     9,     0,    82,
      83,     3,     4,     5,     6,     7,     8,   402,    10,    24,
      25,    26,    27,    28,    29,    30,    18,    19,    82,    83,
      84,    85,    86,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   375,    44,     3,     4,     5,     6,   462,    96,   308,
      17,    69,   467,    45,    69,   182,    -1,     9,    -1,    77,
      78,    79,    80,    81,    82,    83,    -1,   478,   479,   480,
      72,    73,    74,    40,    41,    42,    43,    44,    -1,   490,
      -1,    -1,   497,   498,   499,   500,    84,    85,    86,   504,
     501,   502,    -1,   508,   509,   510,   511,   512,   513,   514,
     515,   516,    69,   518,     3,     4,     5,     6,     7,     8,
     521,   522,   523,   524,    -1,    12,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,    77,    78,
      79,    80,    81,    82,    83,    32,    -1,   552,   553,    -1,
      89,   556,    -1,    -1,    -1,     1,    -1,   562,    -1,    -1,
      -1,    -1,   563,    50,    51,    52,    53,    54,    55,    56,
      16,    -1,    -1,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,    -1,    -1,   502,    -1,    -1,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,   479,   480,
     527,   528,    -1,    -1,    70,    69,    -1,   534,   535,   490,
      -1,   538,    -1,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,   186,   187,    77,    78,    79,    80,
      81,    82,    83,   195,   196,   197,   563,    88,    -1,    -1,
     521,   522,   523,   524,    -1,    -1,    -1,    -1,   529,   530,
     531,   532,   533,    -1,    -1,   536,   537,    -1,    -1,    -1,
      -1,   168,   169,   170,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,   183,    88,    -1,    -1,
      -1,   188,    -1,    -1,    -1,   192,    -1,    -1,   195,    16,
      -1,   253,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    47,    48,    49,    -1,   212,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    -1,   278,   279,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,
     302,   303,    -1,    70,    -1,    -1,   308,   254,   255,    -1,
     257,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    -1,    19,    20,    21,
      22,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,   344,   345,   346,    -1,    -1,    -1,   350,   351,
     352,   353,   354,   355,   356,   357,   358,    -1,   360,    77,
      78,    79,    80,    81,    82,    83,    61,    62,    63,    64,
      65,    66,   319,   320,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     347,   348,   349,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   359,    -1,    -1,    -1,    -1,    -1,    -1,   366,
     367,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   403,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   411,   412,   413,   414,    -1,    -1,
      -1,   418,   419,   420,   421,   422,   423,   424,   425,   426,
      -1,   428,    -1,   430,   486,   487,   433,   434,    -1,   491,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   492,    -1,    -1,    -1,   496
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    93,     0,     3,     4,     5,     6,     7,     8,    10,
      18,    19,    44,    72,    73,    74,    94,    95,   156,   158,
     160,   162,   163,   167,   170,   173,    87,     5,   160,    96,
     139,   140,    87,   157,   162,   171,   168,   174,    11,    12,
      13,    14,    15,    16,    17,    19,    20,    21,    22,    95,
      77,    78,    79,    80,    81,    82,    83,   176,     9,   127,
     155,    24,    25,    26,    27,    28,    29,    30,    69,   141,
     157,    77,    78,    79,    80,    81,    82,    83,    89,   177,
      69,   164,   166,   166,   166,    96,    96,    96,    96,    96,
      96,    96,   139,    96,    96,    96,    96,   157,   157,   157,
     157,   157,   157,   157,    91,    69,   156,   157,   157,   157,
     157,   157,   157,   157,   142,    88,   157,   157,   157,   157,
     157,   157,   157,   177,   157,   165,   166,   172,   169,   175,
     103,   155,     4,     5,    69,   130,   148,   149,   150,   161,
     131,   149,     1,   109,   110,   111,   112,   113,   157,   109,
     132,   149,    97,   155,   133,   134,   135,   138,   148,    69,
     119,   120,     8,   122,   123,   124,   134,     1,   128,   177,
     177,    31,    70,   143,   177,   166,     5,    91,    91,    69,
     104,     1,   151,   177,    17,    40,    41,    42,    43,    69,
     156,   177,   177,    69,    98,    67,    68,    69,   156,     1,
     121,    89,    69,    70,    70,   129,   157,   157,   157,     4,
      70,     1,   105,    70,    70,    90,   152,   157,    95,     1,
      76,    87,   159,   162,   160,   160,     1,   157,     1,   114,
     113,     1,   157,     1,    99,   157,   160,   160,     1,     7,
       8,   136,   137,   152,   160,    70,     4,     8,    70,    89,
       8,     1,   125,   177,   177,   177,    69,    86,    70,     4,
      70,   106,   157,     1,   153,   159,   159,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,   177,   177,
      70,    16,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    70,
     117,    70,    33,    34,    70,   100,    70,    70,   177,    70,
      23,    35,    70,   126,   160,   157,   157,   144,   157,   177,
     177,    90,     6,    90,   154,    88,    86,   159,   159,   159,
     159,   159,    82,   159,    83,   159,    86,   159,    86,   159,
      86,   160,   160,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   160,    35,   160,   137,     8,   177,   177,    31,    32,
      70,   145,   146,    69,   157,   157,   159,   159,   159,   159,
     159,   159,    95,   160,   160,   160,     1,   118,   157,   118,
     118,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     118,   160,   101,   177,   157,   157,     4,     4,   144,   107,
     107,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,    98,
     177,   157,    69,    86,   177,    70,   108,   177,   118,   118,
     118,   118,   112,   112,   112,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   112,   118,    36,    37,    38,    39,
     102,   157,   101,   144,   157,     4,   147,   157,    61,    62,
      63,    64,    65,    66,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,    70,    69,     4,   177,   112,   112,   112,
     112,    71,    87,   115,   116,   162,   115,   115,   112,   112,
     112,   112,   112,   160,   160,   112,   112,   115,   160,   144,
     157,   177,   177,   177,   177,   162,   116,    83,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,    70,
     115,   115,   115,   115,    88,   116,   116,   115,   115,   115,
     115,   115,   116,   116,   115,   115,   116,   177,   177,   177,
     112,   112,   112,   177,   178,   116
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    92,    93,    93,    93,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    95,    95,    96,    96,    97,    98,    98,    99,    99,
     100,   100,   100,   101,   101,   102,   102,   102,   102,   102,
     103,   104,   104,   105,   105,   106,   106,   107,   107,   108,
     108,   108,   108,   108,   108,   109,   110,   110,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   112,   113,   113,
     113,   114,   114,   115,   115,   115,   116,   116,   116,   116,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   118,   118,
     119,   120,   120,   121,   121,   121,   121,   122,   123,   123,
     124,   125,   125,   126,   126,   127,   127,   128,   128,   129,
     130,   131,   132,   133,   134,   134,   135,   135,   136,   136,
     137,   137,   137,   137,   138,   138,   138,   138,   138,   139,
     140,   140,   141,   141,   141,   141,   141,   141,   141,   142,
     142,   143,   143,   144,   144,   145,   145,   146,   146,   146,
     147,   147,   148,   149,   149,   150,   150,   151,   151,   152,
     152,   153,   153,   154,   155,   155,   156,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   158,   158,   158,   158,
     158,   158,   158,   158,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   160,   160,   160,   160,   161,   161,   162,   162,
     163,   163,   163,   165,   164,   166,   166,   168,   169,   167,
     171,   172,   170,   174,   175,   173,   176,   173,   177,   177,
     178,   178
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     3,     4,     4,     2,     3,     2,
       1,     1,     1,     0,     2,     2,     3,     3,     0,     2,
       4,     2,     6,     0,     3,     1,     1,     1,     1,     1,
       2,     3,     3,     0,     2,     4,     4,     0,     2,     2,
       2,     2,     2,     2,     2,     1,     4,     4,     1,     3,
       3,     3,     3,     3,     5,     5,     2,     3,     3,     3,
       3,     0,     2,     0,     1,     3,     1,     2,     3,     3,
       9,     9,     9,     9,     7,     7,     7,     9,     9,     9,
       9,     9,     9,     9,    11,    11,     7,    12,     1,     1,
       1,     3,     3,     0,     2,     2,     2,     1,     4,     4,
       3,     0,     2,     2,     1,     4,     4,     0,     2,     3,
       1,     1,     1,     1,     1,     1,     4,     4,     1,     3,
       1,     1,     1,     1,     0,     3,     3,     3,     2,     4,
       0,     2,     8,     8,     2,     2,     2,     2,     2,     0,
       2,     5,     7,     0,     2,     6,     4,     5,     7,     1,
       1,     2,     1,     1,     1,     3,     3,     0,     2,     3,
       3,     0,     2,     1,     0,     2,     5,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     3,     2,     3,     3,     4,     4,
       4,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     4,     0,     2,     0,     0,     5,
       0,     0,     5,     0,     0,     5,     0,     4,     0,     2,
       0,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:
#line 200 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1911 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 201 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 202 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1923 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 203 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1929 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 204 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1935 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 205 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource));}
#line 1941 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 206 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1947 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 207 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 208 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1959 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 209 "rcyacc.y" /* yacc.c:1646  */
    { set_string_flags((yyvsp[-1].i)); }
#line 1965 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 210 "rcyacc.y" /* yacc.c:1646  */
    { add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1971 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 211 "rcyacc.y" /* yacc.c:1646  */
    { type_resource((yyvsp[0].resource), (yyvsp[-2].str)); add_resource((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].resource)); }
#line 1977 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 212 "rcyacc.y" /* yacc.c:1646  */
    { VPRINT(("Table name is now %s", (yyvsp[0].str))); strcpy(tbl_name, (yyvsp[0].str)); }
#line 1983 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 213 "rcyacc.y" /* yacc.c:1646  */
    { }
#line 1989 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 214 "rcyacc.y" /* yacc.c:1646  */
    { }
#line 1995 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 215 "rcyacc.y" /* yacc.c:1646  */
    { }
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 219 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.str) = (char *) makeintresource((yyvsp[0].i)); }
#line 2007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 222 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0x10; /* These seem to be default flags */ }
#line 2013 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 223 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = ((yyvsp[-1].i) & (yyvsp[0].mask).and) | (yyvsp[0].mask).or; }
#line 2019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 226 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_menu_resource((yyvsp[0].b)); }
#line 2025 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 229 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = set_menu_last((yyvsp[-1].b)); }
#line 2031 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 230 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Error processing menu definition")); }
#line 2037 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 233 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = NULL; }
#line 2043 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 234 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = append_menu_node((yyvsp[-1].b), (yyvsp[0].b)); }
#line 2049 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 237 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_menu_popup((yyvsp[-2].str), (yyvsp[0].b), (yyvsp[-1].i)); }
#line 2055 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 238 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_menu_separator(); }
#line 2061 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 240 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_menu_item((yyvsp[-4].str), (yyvsp[-2].i), (yyvsp[-1].i)); }
#line 2067 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 243 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2073 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 244 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 2079 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 248 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = menu_checked(); }
#line 2085 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 249 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = menu_grayed(); }
#line 2091 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 250 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = menu_inactive(); }
#line 2097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 251 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = menu_help(); }
#line 2103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 254 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_accelerator_resource((yyvsp[0].b)); }
#line 2109 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 257 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = (yyvsp[-1].b); }
#line 2115 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 258 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Error processing accelerator table definition")); }
#line 2121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 261 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = NULL; }
#line 2127 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 262 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = append_accel((yyvsp[-1].b), (yyvsp[0].b)); }
#line 2133 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 265 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = make_accel_s((yyvsp[-3].str), (yyvsp[-1].i), (yyvsp[0].i)); }
#line 2139 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 266 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = make_accel_i((yyvsp[-3].i), (yyvsp[-1].i), (yyvsp[0].i)); }
#line 2145 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 269 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2151 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 270 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i) | (yyvsp[0].i); }
#line 2157 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 273 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0x01; }
#line 2163 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 274 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0x00; }
#line 2169 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 275 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0x08; }
#line 2175 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 276 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0x04; }
#line 2181 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 277 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0x10; }
#line 2187 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 278 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0x02; }
#line 2193 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 281 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_dialog_resource((yyvsp[0].b)); }
#line 2199 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 284 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = set_dialog_controls((yyvsp[-3].b), (yyvsp[-1].b)); }
#line 2205 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 285 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Error processing dialog box definition")); }
#line 2211 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 288 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = make_dialog((yyvsp[0].dim)); }
#line 2217 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 289 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = set_dialog_style((yyvsp[-2].b), (yyvsp[0].i)); }
#line 2223 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 290 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Expected integer after STYLE")); }
#line 2229 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 291 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = set_dialog_caption((yyvsp[-2].b), (yyvsp[0].str)); }
#line 2235 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 292 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = set_dialog_class((yyvsp[-2].b), (yyvsp[0].str)); }
#line 2241 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 293 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = set_dialog_menu((yyvsp[-2].b), (yyvsp[0].str)); }
#line 2247 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 294 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = set_dialog_font((yyvsp[-4].b), (yyvsp[0].str), (yyvsp[-2].i)); }
#line 2253 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 295 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Need integer point size for FONT")); }
#line 2259 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 299 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.dim).corner = (yyvsp[-2].xy); (yyval.dim).extent = (yyvsp[0].xy); }
#line 2265 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 302 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.xy).x = (yyvsp[-2].i); (yyval.xy).y = (yyvsp[0].i); }
#line 2271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 303 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Coordinates must be integers")); }
#line 2277 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 304 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Coordinates must be integers")); }
#line 2283 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 307 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = NULL; }
#line 2289 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 308 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = append_control((yyvsp[-1].b), (yyvsp[0].b)); }
#line 2295 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 311 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.mask).or = 0; (yyval.mask).and = -1;(yyval.mask).extendedFlags = 0; }
#line 2301 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 312 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.mask) = (yyvsp[0].mask); }
#line 2307 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 313 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.mask) = (yyvsp[-2].mask); (yyval.mask).extendedFlags = (yyvsp[0].mask).or; }
#line 2313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 316 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.mask).or = (yyvsp[0].i); (yyval.mask).and = -1; }
#line 2319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 317 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.mask).or = 0; (yyval.mask).and = ~(yyvsp[0].i); }
#line 2325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 318 "rcyacc.y" /* yacc.c:1646  */
    { 
					  (yyval.mask).or = (yyvsp[-2].mask).or | (yyvsp[0].mask).or; 
					  (yyval.mask).and = (yyvsp[-2].mask).and & (yyvsp[0].mask).and; 
					}
#line 2334 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 322 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.mask).or = (yyvsp[-1].mask).or; (yyval.mask).and = (yyvsp[-1].mask).and; }
#line 2340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 326 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_ltext_control((yyvsp[-6].str), (yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2346 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 328 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_rtext_control((yyvsp[-6].str), (yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2352 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 330 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_ctext_control((yyvsp[-6].str), (yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2358 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 332 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_icon_control((yyvsp[-6].str), (yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2364 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 334 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_edit_control((yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2370 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 336 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_listbox_control((yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2376 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 338 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_combobox_control((yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2382 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 340 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_groupbox_control((yyvsp[-6].str), (yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 342 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_pushbutton_control((yyvsp[-6].str), (yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2394 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 344 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_defpushbutton_control((yyvsp[-6].str), (yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2400 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 346 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_radiobutton_control((yyvsp[-6].str), (yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2406 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 348 "rcyacc.y" /* yacc.c:1646  */
    {
	   (yyvsp[0].mask).or |= BS_AUTORADIOBUTTON;
	   (yyval.b) = create_autoradiobutton_control((yyvsp[-6].str),(yyvsp[-4].i),(yyvsp[-2].dim),(yyvsp[0].mask));
	 }
#line 2415 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 353 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_checkbox_control((yyvsp[-6].str), (yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2421 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 355 "rcyacc.y" /* yacc.c:1646  */
    {
	   (yyvsp[0].mask).or |= BS_AUTOCHECKBOX;
	   (yyval.b) = create_checkbox_control((yyvsp[-6].str),(yyvsp[-4].i),(yyvsp[-2].dim),(yyvsp[0].mask));
	 }
#line 2430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 360 "rcyacc.y" /* yacc.c:1646  */
    {
	   (yyvsp[-2].mask).or |= BS_3STATE;
	   (yyval.b) = create_generic_control((yyvsp[-8].str),(yyvsp[-6].i),(yyvsp[-4].str),(yyvsp[-2].mask), (yyvsp[0].dim));
	 }
#line 2439 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 365 "rcyacc.y" /* yacc.c:1646  */
    {
	   (yyvsp[-2].mask).or |= BS_AUTO3STATE;
	   (yyval.b) = create_generic_control((yyvsp[-8].str),(yyvsp[-6].i),(yyvsp[-4].str),(yyvsp[-2].mask), (yyvsp[0].dim));
	 }
#line 2448 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 370 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = create_scrollbar_control((yyvsp[-4].i), (yyvsp[-2].dim), (yyvsp[0].mask)); }
#line 2454 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 372 "rcyacc.y" /* yacc.c:1646  */
    {(yyval.b) = create_generic_control((yyvsp[-9].str), (yyvsp[-7].i), (yyvsp[-5].str), (yyvsp[-3].mask), (yyvsp[-1].dim));}
#line 2460 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 376 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Control identifiers must be integers")); }
#line 2466 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 381 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_dlginit_resource((yyvsp[0].b)); }
#line 2472 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 386 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = (yyvsp[-1].b); }
#line 2478 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 388 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Error processing dialog init definition")); }
#line 2484 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 393 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = make_dlginit(); }
#line 2490 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 395 "rcyacc.y" /* yacc.c:1646  */
    {
		  (yyval.b) = add_dlginit_item(add_dlginit_item((yyvsp[-1].b),
			(BYTE)((WORD)(yyvsp[0].i)) & 0xff),
			(BYTE)(((WORD)(yyvsp[0].i)) >> 8) & 0xff);
		}
#line 2500 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 401 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = add_dlginit_item((yyvsp[-1].b), '\0'); }
#line 2506 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 403 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = (yyvsp[-1].b); }
#line 2512 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 408 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_toolbar_resource((yyvsp[0].b)); }
#line 2518 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 413 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = set_toolbar_dims((yyvsp[-1].b), LOWORD((yyvsp[-3].i)), HIWORD((yyvsp[-3].i))); }
#line 2524 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 415 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Error processing tool bar definition")); }
#line 2530 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 420 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = MAKELONG((yyvsp[-2].i), (yyvsp[0].i)); }
#line 2536 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 425 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = make_toolbar(1, 16, 15); }
#line 2542 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 427 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = add_toolbar_item((yyvsp[-1].b), (WORD)(yyvsp[0].i)); }
#line 2548 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 432 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 2554 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 434 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2560 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 438 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Error processing string table")); }
#line 2566 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 445 "rcyacc.y" /* yacc.c:1646  */
    { add_table_string((yyvsp[-2].i), (yyvsp[0].str)); }
#line 2572 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 448 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_bitmap_resource((yyvsp[0].b)); }
#line 2578 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 451 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_cursor_resource((yyvsp[0].b)); }
#line 2584 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 454 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_icon_resource((yyvsp[0].b)); }
#line 2590 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 457 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = type_resource_rcdata((yyvsp[0].resource)); }
#line 2596 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 460 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_binary_resource(buff_load((yyvsp[0].str), 1)); }
#line 2602 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 461 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.resource) = make_binary_resource((yyvsp[0].b)); }
#line 2608 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 464 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = pack_rcdata((yyvsp[-3].binflags), (yyvsp[-1].urb)); }
#line 2614 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 465 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Error in user-defined resource definition")); }
#line 2620 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 468 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.urb) = (yyvsp[0].urb); }
#line 2626 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 469 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.urb) = urb_cat((yyvsp[-2].urb), (yyvsp[0].urb)); }
#line 2632 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 472 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.urb) = urb_string((yyvsp[0].str)); }
#line 2638 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 473 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.urb) = urb_short((yyvsp[0].i)); }
#line 2644 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 474 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.urb) = urb_long((yyvsp[0].i)); }
#line 2650 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 475 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.urb) = urb_binary((yyvsp[0].b)); }
#line 2656 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 478 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.binflags) = defbinflags; }
#line 2662 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 479 "rcyacc.y" /* yacc.c:1646  */
    { (yyvsp[-2].binflags).align = (yyvsp[0].i); (yyval.binflags) = (yyvsp[-2].binflags); }
#line 2668 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 480 "rcyacc.y" /* yacc.c:1646  */
    { (yyvsp[-2].binflags).align = alignval((yyvsp[0].str)); (yyval.binflags) = (yyvsp[-2].binflags); }
#line 2674 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 481 "rcyacc.y" /* yacc.c:1646  */
    { if (((yyvsp[0].str)[0] == 'b') || ((yyvsp[0].str)[0] == 'B')) (yyvsp[-2].binflags).endian = big;
                                      else if (((yyvsp[0].str)[0] == 'l') || ((yyvsp[0].str)[0] == 'L')) (yyvsp[-2].binflags).endian = little;
                                      else CERROR(("Invalid endian specification %s", (yyvsp[0].str)));
                                      (yyval.binflags) = (yyvsp[-2].binflags); }
#line 2683 "y.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 519 "rcyacc.y" /* yacc.c:1646  */
    { }
#line 2689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 520 "rcyacc.y" /* yacc.c:1646  */
    { }
#line 2695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 526 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = buff_load((yyvsp[0].str), 1); }
#line 2701 "y.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 529 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = (yyvsp[-1].b); }
#line 2707 "y.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 530 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Error in binary data lines")); (yyval.b) = NULL; }
#line 2713 "y.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 533 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = NULL; }
#line 2719 "y.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 534 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = buff_cat((yyvsp[-1].b), (yyvsp[0].b)); }
#line 2725 "y.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 537 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = (yyvsp[-1].b); }
#line 2731 "y.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 538 "rcyacc.y" /* yacc.c:1646  */
    { CERROR(("Error in binary data")); (yyval.b) = '\0'; }
#line 2737 "y.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 541 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = NULL; }
#line 2743 "y.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 542 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.b) = buff_append_byte((yyvsp[-1].b), (yyvsp[0].c)); }
#line 2749 "y.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 545 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.c) = hex2dec((yyvsp[0].str)); }
#line 2755 "y.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 555 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) + (yyvsp[0].i); }
#line 2761 "y.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 556 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) - (yyvsp[0].i); }
#line 2767 "y.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 557 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) * (yyvsp[0].i); }
#line 2773 "y.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 558 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) / (yyvsp[0].i); }
#line 2779 "y.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 559 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) % (yyvsp[0].i); }
#line 2785 "y.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 560 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 2791 "y.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 561 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) & (yyvsp[0].i); }
#line 2797 "y.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 562 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 2803 "y.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 569 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) + (yyvsp[0].i); }
#line 2809 "y.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 570 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) - (yyvsp[0].i); }
#line 2815 "y.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 571 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) * (yyvsp[0].i); }
#line 2821 "y.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 572 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) / (yyvsp[0].i); }
#line 2827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 573 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) % (yyvsp[0].i); }
#line 2833 "y.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 574 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 2839 "y.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 575 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) & (yyvsp[0].i); }
#line 2845 "y.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 579 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) + (yyvsp[0].i); }
#line 2851 "y.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 580 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) - (yyvsp[0].i); }
#line 2857 "y.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 581 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) * (yyvsp[0].i); }
#line 2863 "y.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 582 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) / (yyvsp[0].i); }
#line 2869 "y.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 583 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) % (yyvsp[0].i); }
#line 2875 "y.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 584 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 2881 "y.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 585 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) & (yyvsp[0].i); }
#line 2887 "y.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 586 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-3].i) || (yyvsp[0].i); }
#line 2893 "y.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 587 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-3].i) && (yyvsp[0].i); }
#line 2899 "y.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 588 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 2905 "y.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 589 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = !(yyvsp[0].i); }
#line 2911 "y.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 590 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) > (yyvsp[0].i); }
#line 2917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 591 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) < (yyvsp[0].i); }
#line 2923 "y.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 592 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = ((yyvsp[-3].i) >= (yyvsp[0].i)); }
#line 2929 "y.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 593 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = ((yyvsp[-3].i) <= (yyvsp[0].i)); }
#line 2935 "y.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 594 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = ((yyvsp[-3].i) == (yyvsp[0].i)); }
#line 2941 "y.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 595 "rcyacc.y" /* yacc.c:1646  */
    { (yyval.i) = ((yyvsp[-3].i) != (yyvsp[0].i)); }
#line 2947 "y.tab.c" /* yacc.c:1646  */
    break;

  case 223:
#line 640 "rcyacc.y" /* yacc.c:1646  */
    { ignore_until_brace(0); }
#line 2953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 224:
#line 642 "rcyacc.y" /* yacc.c:1646  */
    { ignore_until_brace(1); }
#line 2959 "y.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 650 "rcyacc.y" /* yacc.c:1646  */
    { ignore_until_semicolon(); }
#line 2965 "y.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 651 "rcyacc.y" /* yacc.c:1646  */
    { ignore_until_semicolon(); }
#line 2971 "y.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 653 "rcyacc.y" /* yacc.c:1646  */
    { ignore_rest_of_file(); }
#line 2977 "y.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 660 "rcyacc.y" /* yacc.c:1646  */
    { typedef_mode(); }
#line 2983 "y.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 661 "rcyacc.y" /* yacc.c:1646  */
    { typedef_mode(); }
#line 2989 "y.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 662 "rcyacc.y" /* yacc.c:1646  */
    {
		     DEFINESTRUCT *defineNew = create_define((yyvsp[0].str));
		     defineNew->value = str_dup("extern");
		     /* initial_mode(); */
		     { ignore_rest_of_file(); }
		 }
#line 3000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 674 "rcyacc.y" /* yacc.c:1646  */
    { ignore_until_semicolon(); }
#line 3006 "y.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 675 "rcyacc.y" /* yacc.c:1646  */
    { ignore_until_semicolon(); }
#line 3012 "y.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 677 "rcyacc.y" /* yacc.c:1646  */
    { ignore_rest_of_file(); }
#line 3018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 678 "rcyacc.y" /* yacc.c:1646  */
    { ignore_until_semicolon(); }
#line 3024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 680 "rcyacc.y" /* yacc.c:1646  */
    { ignore_rest_of_file(); }
#line 3030 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3034 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 699 "rcyacc.y" /* yacc.c:1906  */


int yyerror(char *str)
{
  CERROR(("%s", str));
  return 0;
}

void set_position(int first_line, int last_line, int first_column, int last_column)
{
#ifdef NEED_BISON_POSITION_INFO
  yylloc.first_line = first_line;
  yylloc.last_line = last_line;
  yylloc.first_column = first_column;
  yylloc.last_column = last_column;
#endif
}

void rcyacc_debug_level(int level)
{
// http://lists.infradead.org/pipermail/b43-dev/2013-February/002877.html
// With bison 2.7 YYDEBUG will be set to 0 if it was not set, then #ifdef
// YYDEBUG returns true, but bison 2.7 treets YYDEBUG 0 as not set.
#if YYDEBUG // Change from #ifdef to #if
  extern int yydebug;
    
  yydebug = level;
#endif
}
