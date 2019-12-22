/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 60 "rcyacc.y" /* yacc.c:1909  */

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

#line 217 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
