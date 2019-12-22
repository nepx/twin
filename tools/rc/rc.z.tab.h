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
#line 45 "ifyacc.y" /* yacc.c:1909  */

  int i;
  char *str;

#line 73 "y.tab.h" /* yacc.c:1909  */
};

typedef union ZZSTYPE ZZSTYPE;
# define ZZSTYPE_IS_TRIVIAL 1
# define ZZSTYPE_IS_DECLARED 1
#endif


extern ZZSTYPE zzlval;

int zzparse (void);

#endif /* !ZZ_ZZ_Y_TAB_H_INCLUDED  */
