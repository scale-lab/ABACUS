/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
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
/* Tokens.  */
#define vSYMBOL_ID 258
#define vNUMBER_ID 259
#define vDELAY_ID 260
#define vALWAYS 261
#define vAND 262
#define vASSIGN 263
#define vGENERATE 264
#define vBEGIN 265
#define vCASE 266
#define vDEFAULT 267
#define vDEFINE 268
#define vELSE 269
#define vEND 270
#define vENDCASE 271
#define vENDGENERATE 272
#define vENDMODULE 273
#define vIF 274
#define vFOR 275
#define vINOUT 276
#define vINPUT 277
#define vINPUT_SIGNED 278
#define vMODULE 279
#define vNAND 280
#define vNEGEDGE 281
#define vNOR 282
#define vNOT 283
#define vOR 284
#define vNO_ID 285
#define vSIGNED 286
#define vOUTPUT 287
#define vPARAMETER 288
#define vPOSEDGE 289
#define vREG 290
#define vOUTPUT_REG 291
#define vWIRE 292
#define vINTEGER 293
#define vGENVAR 294
#define vREG_SIGNED 295
#define vWIRE_SIGNED 296
#define vXNOR 297
#define vXOR 298
#define vDEFPARAM 299
#define voANDAND 300
#define voOROR 301
#define voLTE 302
#define voGTE 303
#define voSLEFT 304
#define voSRIGHT 305
#define voEQUAL 306
#define voNOTEQUAL 307
#define voCASEEQUAL 308
#define voCASENOTEQUAL 309
#define voXNOR 310
#define voNAND 311
#define voNOR 312
#define vNOT_SUPPORT 313
#define UOR 314
#define UAND 315
#define UNOT 316
#define UNAND 317
#define UNOR 318
#define UXNOR 319
#define UXOR 320
#define ULNOT 321
#define UADD 322
#define UMINUS 323
#define LOWER_THAN_ELSE 324




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 61 "SRC/verilog_bison.y"
{
	char *id_name;
	char *num_value;
	ast_node_t *node;
}
/* Line 1529 of yacc.c.  */
#line 193 "SRC/verilog_bison.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

