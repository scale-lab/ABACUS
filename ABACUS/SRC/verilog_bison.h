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

#ifndef YY_YY_SRC_VERILOG_BISON_H_INCLUDED
# define YY_YY_SRC_VERILOG_BISON_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    vSYMBOL_ID = 258,
    vNUMBER_ID = 259,
    vDELAY_ID = 260,
    vALWAYS = 261,
    vAND = 262,
    vASSIGN = 263,
    vGENERATE = 264,
    vBEGIN = 265,
    vCASE = 266,
    vDEFAULT = 267,
    vDEFINE = 268,
    vELSE = 269,
    vEND = 270,
    vENDCASE = 271,
    vENDGENERATE = 272,
    vENDMODULE = 273,
    vIF = 274,
    vFOR = 275,
    vINOUT = 276,
    vINPUT = 277,
    vINPUT_SIGNED = 278,
    vMODULE = 279,
    vNAND = 280,
    vNEGEDGE = 281,
    vNOR = 282,
    vNOT = 283,
    vOR = 284,
    vNO_ID = 285,
    vSIGNED = 286,
    vOUTPUT = 287,
    vPARAMETER = 288,
    vPOSEDGE = 289,
    vREG = 290,
    vOUTPUT_REG = 291,
    vWIRE = 292,
    vINTEGER = 293,
    vGENVAR = 294,
    vREG_SIGNED = 295,
    vWIRE_SIGNED = 296,
    vXNOR = 297,
    vXOR = 298,
    vDEFPARAM = 299,
    voANDAND = 300,
    voOROR = 301,
    voLTE = 302,
    voGTE = 303,
    voSLEFT = 304,
    voSRIGHT = 305,
    voEQUAL = 306,
    voNOTEQUAL = 307,
    voCASEEQUAL = 308,
    voCASENOTEQUAL = 309,
    voXNOR = 310,
    voNAND = 311,
    voNOR = 312,
    vNOT_SUPPORT = 313,
    UOR = 314,
    UAND = 315,
    UNOT = 316,
    UNAND = 317,
    UNOR = 318,
    UXNOR = 319,
    UXOR = 320,
    ULNOT = 321,
    UADD = 322,
    UMINUS = 323,
    LOWER_THAN_ELSE = 324
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 61 "SRC/verilog_bison.y" /* yacc.c:1909  */

	char *id_name;
	char *num_value;
	ast_node_t *node;

#line 130 "SRC/verilog_bison.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_VERILOG_BISON_H_INCLUDED  */
