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
#line 1 "SRC/verilog_bison.y" /* yacc.c:339  */

/*
Copyright (c) 2009 Peter Andrew Jamieson (jamieson.peter@gmail.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "parse_making_ast.h"
 
#define PARSE {printf("here\n");}

#ifndef YYLINENO
int yylineno;
#define YYLINENO yylineno
#else
extern int yylineno;
#endif

void yyerror(const char *str);
int yywrap();
int yyparse();
int yylex(void);

 // RESPONCE in an error
void yyerror(const char *str)
{
	fprintf(stderr,"error in parsing: %s - on line number %d\n",str, yylineno);
	exit(-1);
}
 
// point of continued file reading
int yywrap()
{
	return 1;
}


#line 126 "SRC/verilog_bison.c" /* yacc.c:339  */

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
   by #include "verilog_bison.h".  */
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
#line 61 "SRC/verilog_bison.y" /* yacc.c:355  */

	char *id_name;
	char *num_value;
	ast_node_t *node;

#line 242 "SRC/verilog_bison.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_VERILOG_BISON_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 259 "SRC/verilog_bison.c" /* yacc.c:358  */

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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1727

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  96
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  148
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  368

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   324

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    78,     2,    93,     2,    70,    63,     2,
      71,    72,    68,    66,    91,    67,    94,    69,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    60,    90,
      64,    92,    65,    59,    95,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    75,     2,    76,    62,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    73,    61,    74,    77,     2,     2,     2,
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
      55,    56,    57,    58,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   119,   119,   122,   132,   133,   134,   137,   140,   141,
     142,   145,   146,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   162,   165,   166,   169,   170,   173,   176,
     177,   178,   179,   180,   181,   184,   185,   188,   189,   190,
     191,   192,   193,   196,   200,   201,   202,   203,   204,   205,
     206,   209,   210,   211,   212,   216,   219,   220,   221,   222,
     225,   226,   229,   230,   233,   234,   237,   238,   242,   245,
     247,   248,   249,   250,   251,   252,   253,   254,   257,   258,
     261,   262,   263,   264,   268,   269,   272,   273,   276,   277,
     280,   281,   284,   285,   286,   290,   291,   292,   295,   296,
     297,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   339,   340,   343,
     344,   345,   346,   347,   348,   349,   350,   353,   354
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "vSYMBOL_ID", "vNUMBER_ID", "vDELAY_ID",
  "vALWAYS", "vAND", "vASSIGN", "vGENERATE", "vBEGIN", "vCASE", "vDEFAULT",
  "vDEFINE", "vELSE", "vEND", "vENDCASE", "vENDGENERATE", "vENDMODULE",
  "vIF", "vFOR", "vINOUT", "vINPUT", "vINPUT_SIGNED", "vMODULE", "vNAND",
  "vNEGEDGE", "vNOR", "vNOT", "vOR", "vNO_ID", "vSIGNED", "vOUTPUT",
  "vPARAMETER", "vPOSEDGE", "vREG", "vOUTPUT_REG", "vWIRE", "vINTEGER",
  "vGENVAR", "vREG_SIGNED", "vWIRE_SIGNED", "vXNOR", "vXOR", "vDEFPARAM",
  "voANDAND", "voOROR", "voLTE", "voGTE", "voSLEFT", "voSRIGHT", "voEQUAL",
  "voNOTEQUAL", "voCASEEQUAL", "voCASENOTEQUAL", "voXNOR", "voNAND",
  "voNOR", "vNOT_SUPPORT", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'('", "')'", "'{'", "'}'", "'['",
  "']'", "'~'", "'!'", "UOR", "UAND", "UNOT", "UNAND", "UNOR", "UXNOR",
  "UXOR", "ULNOT", "UADD", "UMINUS", "LOWER_THAN_ELSE", "';'", "','",
  "'='", "'#'", "'.'", "'@'", "$accept", "source_text", "items", "define",
  "module", "list_of_module_items", "module_item", "parameter_declaration",
  "input_declaration", "output_declaration", "inout_declaration",
  "net_declaration", "variable_list", "variable", "continuous_assign",
  "gate_declaration", "gate_instance", "module_instantiation",
  "module_instance", "list_of_module_connections", "module_connection",
  "list_of_module_parameters", "module_parameter", "always", "gen_block",
  "statement", "blocking_assignment", "non_blocking_assignment",
  "case_item_list", "case_items", "seq_block", "stmt_list",
  "delay_control", "event_expression_list", "event_expression",
  "expression", "primary", "expression_list", YY_NULLPTR
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
     305,   306,   307,   308,   309,   310,   311,   312,   313,    63,
      58,   124,    94,    38,    60,    62,    43,    45,    42,    47,
      37,    40,    41,   123,   125,    91,    93,   126,    33,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      59,    44,    61,    35,    46,    64
};
# endif

#define YYPACT_NINF -256

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-256)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      37,    60,    65,    72,    37,  -256,  -256,   113,    48,  -256,
    -256,  -256,  -256,    64,    58,    70,   494,    41,  -256,    30,
     845,    88,  -256,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,  1355,  -256,    91,    66,   494,
    -256,    21,    92,     2,    30,   190,    26,    26,    26,     2,
       2,     2,     2,    26,    26,    26,    26,    26,    26,    26,
      26,    26,     2,     2,   722,  -256,  -256,  -256,  -256,  -256,
    -256,  -256,  -256,  -256,  -256,  -256,   494,  -256,  -256,  -256,
    -256,  -256,  -256,  -256,  -256,  1159,  1130,   -48,  -256,  -256,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   845,    93,  -256,  1511,    84,
     124,  -256,   -40,   190,   131,   494,   123,   145,   147,   190,
     190,   169,   180,   183,  -256,  -256,   158,   168,  -256,    45,
     -41,   -54,   -20,    17,   170,   172,   176,   178,    67,    76,
      80,    82,   116,   121,   127,   129,   138,   179,   181,  -256,
    -256,   844,  -256,   494,  -256,   494,  1561,  1537,   -28,   -28,
     177,   177,  1641,  1641,  1641,  1641,  1609,  1633,  1609,  1381,
     908,  1585,  1609,  1633,   -28,   -28,   -11,   -11,  -256,  -256,
    -256,   763,   845,    49,   297,  -256,   130,  -256,   494,   590,
    -256,  -256,   410,   142,   165,   494,   494,    30,  -256,  -256,
    -256,  -256,   368,  -256,    26,  -256,  -256,  -256,  -256,  -256,
    -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,
    -256,  -256,   494,   184,  1100,   -47,  1511,   494,   261,  -256,
     804,   186,   264,    51,  -256,  1511,   267,    53,  -256,  1511,
     270,   271,   207,   -25,  -256,  -256,   621,   494,   494,  1511,
    -256,  -256,  1187,  1215,   191,   447,   494,  1511,   940,   494,
     208,   209,  1511,    62,  -256,  -256,   213,   199,   339,   220,
     289,   297,  -256,  -256,  -256,   188,  -256,   188,   494,   528,
    1511,   481,   190,   494,   494,  1511,  1511,  -256,   876,  -256,
    -256,   494,    30,   494,  -256,  -256,   494,   224,  -256,  -256,
    -256,   559,  -256,   494,   236,   381,  -256,  1407,   284,   652,
    1511,   494,   227,  1433,  -256,  1243,  1271,   253,  -256,   494,
    1299,   190,  -256,  -256,   190,   190,    30,   972,   494,   494,
    -256,  -256,   214,    57,  1327,  -256,  -256,  -256,  -256,   234,
    -256,  1459,  1004,  -256,   217,  -256,   190,   494,   237,  -256,
    -256,  1036,   494,  -256,  1485,   494,  1068,  -256
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     6,     5,     0,     0,     1,
       4,     3,     7,     0,    37,     0,     0,     0,    36,     0,
       0,   140,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     0,   109,   107,   108,
     106,   111,   105,   102,   103,     0,   148,     0,   104,   110,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,   148,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    91,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,   136,     0,   146,     0,   134,   133,   129,   130,
     126,   125,   127,   128,   131,   132,   122,   120,   121,     0,
       0,   119,   112,   118,   123,   124,   116,   117,   113,   114,
     115,     0,     0,     0,     0,    94,     0,    68,     0,     0,
      44,    43,     0,     0,     0,     0,     0,     0,    71,    72,
      69,    90,     0,    28,     0,    24,    25,    45,    46,    47,
      48,    26,    23,    30,    27,    29,    31,    32,    33,    34,
      49,    50,     0,   141,   148,     0,   147,     0,     0,     8,
       0,     0,     0,     0,    61,    63,     0,     0,    65,    67,
       0,     0,     0,     0,    97,    98,     0,     0,     0,    78,
      89,    88,     0,     0,     0,     0,     0,    80,     0,     0,
       0,     0,   135,    38,     9,    58,     0,     0,     0,     0,
       0,     0,   100,    99,    93,     0,    92,     0,     0,     0,
      79,     0,     0,     0,     0,    82,    81,   143,     0,   137,
     138,     0,     0,     0,    56,    60,     0,     0,    64,    95,
      96,     0,    54,     0,     0,     0,    85,     0,    73,     0,
      83,     0,   142,     0,    41,     0,     0,     0,    53,     0,
       0,     0,    75,    84,     0,     0,     0,     0,     0,     0,
      62,    66,     0,     0,     0,    52,    87,    86,    74,     0,
     144,     0,     0,    59,     0,    51,     0,     0,    39,    57,
      76,     0,     0,   145,     0,     0,     0,    40
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -256,  -256,  -256,   307,   313,   -90,   -63,  -256,  -256,  -256,
    -256,  -256,  1666,   -35,  -256,  -256,   174,  -256,  -256,    -9,
      43,  -256,    46,  -256,  -256,  -104,   -42,  -256,  -256,    13,
    -256,   104,  -256,  -256,  -255,   -16,     1,   166
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    64,    65,    66,    67,    68,
      69,    70,    17,    18,    71,    72,   126,    73,   121,   243,
     244,   247,   248,    74,    75,   135,   136,   137,   315,   316,
     138,   139,   123,   253,   254,   245,    36,    87
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      35,   160,   127,   117,   285,   124,   212,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    88,    89,   197,
      40,    94,    95,   118,   119,   191,   164,   271,   195,    14,
     309,   196,   310,    21,    22,   211,   213,   214,   110,   111,
     112,   113,   114,   165,   165,   128,   140,   286,    21,    22,
       1,   202,    21,    22,   129,   130,   131,   112,   113,   114,
     161,     2,   210,     7,   132,   133,   287,    14,     8,    14,
     215,   214,     9,   125,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   211,
     211,    16,   240,    39,    23,    24,    25,   216,   214,   199,
      26,    27,    28,    37,   120,    29,    30,    12,    39,    13,
      31,   241,    32,   277,   140,   280,    33,    34,   160,   354,
     140,   140,    38,    21,    22,   134,    15,   301,   116,    16,
     140,    16,   278,   242,   281,    21,    22,   234,   278,   236,
      19,   129,   130,   131,   302,   193,   250,   221,   214,   260,
      20,   132,   133,    76,   251,   264,   222,   214,    21,    22,
     223,   214,   224,   214,   129,   130,   131,   160,   249,   117,
     261,   115,   256,   192,   132,   133,   259,   122,   318,   262,
     263,    21,    22,    21,    22,   194,   267,   255,   252,   129,
     130,   131,   198,    39,   140,   140,   225,   214,   128,   132,
     133,   226,   214,   200,   250,    39,   268,   227,   214,   228,
     214,   272,   251,   144,   145,   146,   147,   346,   229,   214,
     347,   348,   134,   203,   204,   201,   157,   158,    39,   202,
     205,   289,   290,   110,   111,   112,   113,   114,   208,   295,
     296,   206,   360,   298,   207,   134,    21,    22,   209,   269,
     217,    39,   218,    39,   273,   249,   219,   276,   220,   230,
     279,   231,   311,   282,   283,   317,   275,   319,   320,   284,
     134,   293,   299,   300,   303,   323,   255,   325,   255,   304,
     326,   306,   307,   140,   349,   327,   331,   330,   335,   317,
      21,    22,   338,   324,   353,   337,   356,   359,    23,    24,
      25,    10,   362,   344,    26,    27,    28,    11,   343,    29,
      30,   305,   351,   352,    31,   342,    32,   308,   333,   235,
      33,    34,   140,     0,     0,   140,   140,   128,     0,     0,
       0,   361,    21,    22,     0,     0,   364,   242,     0,   366,
       0,     0,    23,    24,    25,     0,     0,   140,    26,    27,
      28,     0,     0,    29,    30,     0,     0,     0,    31,     0,
      32,    21,    22,   265,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    22,     0,     0,     0,     0,
       0,   246,     0,   314,    23,    24,    25,   332,     0,   266,
      26,    27,    28,     0,     0,    29,    30,     0,     0,     0,
      31,     0,    32,    21,    22,   258,    33,    34,     0,     0,
       0,     0,     0,    23,    24,    25,     0,     0,     0,    26,
      27,    28,     0,   242,    29,    30,    23,    24,    25,    31,
       0,    32,    26,    27,    28,    33,    34,    29,    30,     0,
      21,    22,    31,     0,    32,     0,     0,     0,    33,    34,
       0,     0,     0,     0,     0,    23,    24,    25,     0,     0,
       0,    26,    27,    28,     0,     0,    29,    30,   294,     0,
       0,    31,     0,    32,    21,    22,     0,    33,    34,     0,
       0,     0,     0,   314,     0,     0,     0,    21,    22,     0,
       0,     0,    23,    24,    25,     0,     0,     0,    26,    27,
      28,     0,     0,    29,    30,     0,     0,     0,    31,     0,
      32,     0,     0,     0,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,    25,     0,
       0,     0,    26,    27,    28,     0,     0,    29,    30,    23,
      24,    25,    31,     0,    32,    26,    27,    28,    33,    34,
      29,    30,     0,     0,     0,    31,     0,    32,     0,     0,
       0,    33,    34,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,     0,   103,     0,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
     312,     0,     0,     0,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,     0,   103,   313,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       0,   328,     0,     0,     0,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,     0,   103,
     329,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,     0,
     103,   257,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
       0,   103,   288,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,     0,     0,    41,     0,     0,    42,    43,
      44,    45,     0,     0,     0,     0,     0,     0,     0,     0,
     159,     0,   336,    46,    47,    48,     0,    49,     0,    50,
      51,    52,     0,     0,    53,    54,     0,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    41,     0,     0,    42,
      43,    44,    45,     0,     0,     0,     0,     0,     0,     0,
       0,   239,     0,     0,    46,    47,    48,     0,    49,     0,
      50,    51,    52,     0,     0,    53,    54,     0,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    41,     0,     0,
      42,    43,    44,    45,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,     0,    46,    47,    48,     0,    49,
       0,    50,    51,    52,     0,     0,    53,    54,     0,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    41,     0,
       0,    42,    43,    44,    45,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,     0,
      49,     0,    50,    51,    52,     0,     0,    53,    54,     0,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,     0,   103,   232,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,     0,     0,     0,     0,
     233,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,     0,   103,   321,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,     0,     0,
       0,     0,   322,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,     0,   103,     0,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
       0,     0,     0,     0,   238,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,     0,   103,
       0,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,     0,     0,     0,     0,   297,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
       0,   103,     0,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,     0,     0,     0,     0,     0,   350,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,     0,   103,     0,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,     0,     0,     0,     0,
     358,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,     0,   103,     0,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,     0,     0,     0,
       0,     0,   363,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,     0,   103,     0,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
       0,     0,     0,     0,   367,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,     0,   103,
       0,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,     0,     0,   270,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,     0,   103,
       0,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,     0,   163,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,     0,   103,     0,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       0,   162,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,     0,   103,     0,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,   291,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,     0,   103,     0,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,     0,   292,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,     0,   103,     0,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,   340,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,     0,
     103,     0,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,     0,   341,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,     0,   103,     0,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       0,   345,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,     0,   103,     0,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,   355,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,     0,
     103,   237,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,     0,   103,   334,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,     0,   103,   339,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,     0,   103,   357,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,     0,   103,   365,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,     0,
     103,     0,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    90,     0,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,     0,     0,     0,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,     0,
       0,     0,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,     0,     0,     0,     0,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    92,    93,    94,    95,
      96,    97,    98,    99,     0,   101,     0,     0,     0,     0,
       0,     0,   107,   108,   109,   110,   111,   112,   113,   114,
      92,    93,    94,    95,    96,    97,    98,    99,    92,    93,
      94,    95,     0,     0,     0,     0,     0,   108,   109,   110,
     111,   112,   113,   114,     0,   108,   109,   110,   111,   112,
     113,   114,   141,   142,   143,     0,     0,     0,     0,   148,
     149,   150,   151,   152,   153,   154,   155,   156
};

static const yytype_int16 yycheck[] =
{
      16,    64,    44,    38,    29,     3,    47,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,   123,
      19,    49,    50,    39,     3,   115,    74,    74,    68,     3,
     285,    71,   287,     3,     4,   139,    90,    91,    66,    67,
      68,    69,    70,    91,    91,    44,    45,    72,     3,     4,
      13,    92,     3,     4,     9,    10,    11,    68,    69,    70,
      76,    24,    17,     3,    19,    20,    91,     3,     3,     3,
      90,    91,     0,    71,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   203,
     204,    75,   192,    73,    55,    56,    57,    90,    91,   125,
      61,    62,    63,    72,    93,    66,    67,     4,    73,    71,
      71,    72,    73,    72,   123,    72,    77,    78,   191,    72,
     129,   130,    91,     3,     4,    90,    72,    75,    72,    75,
     139,    75,    91,    94,    91,     3,     4,   163,    91,   165,
      92,     9,    10,    11,    92,    71,    26,    90,    91,    17,
      90,    19,    20,    75,    34,   207,    90,    91,     3,     4,
      90,    91,    90,    91,     9,    10,    11,   240,   194,   214,
      15,    90,   198,    90,    19,    20,   202,    95,   292,   205,
     206,     3,     4,     3,     4,    71,   212,   196,    68,     9,
      10,    11,    71,    73,   203,   204,    90,    91,   207,    19,
      20,    90,    91,    90,    26,    73,   232,    90,    91,    90,
      91,   237,    34,    49,    50,    51,    52,   331,    90,    91,
     334,   335,    90,   129,   130,    90,    62,    63,    73,    92,
      71,   257,   258,    66,    67,    68,    69,    70,    90,   265,
     266,    71,   356,   269,    71,    90,     3,     4,    90,    75,
      90,    73,    90,    73,     3,   281,    90,     3,    90,    90,
       3,    90,   288,     3,     3,   291,    90,   293,   294,    72,
      90,    90,    74,    74,    71,   301,   285,   303,   287,    90,
     306,    71,     3,   292,   336,    71,    60,   313,    14,   315,
       3,     4,    75,   302,    90,   321,    72,    90,    55,    56,
      57,     4,    75,   329,    61,    62,    63,     4,   327,    66,
      67,   278,   338,   339,    71,    72,    73,   281,   315,   163,
      77,    78,   331,    -1,    -1,   334,   335,   336,    -1,    -1,
      -1,   357,     3,     4,    -1,    -1,   362,    94,    -1,   365,
      -1,    -1,    55,    56,    57,    -1,    -1,   356,    61,    62,
      63,    -1,    -1,    66,    67,    -1,    -1,    -1,    71,    -1,
      73,     3,     4,     5,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    12,    55,    56,    57,    16,    -1,    31,
      61,    62,    63,    -1,    -1,    66,    67,    -1,    -1,    -1,
      71,    -1,    73,     3,     4,     5,    77,    78,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    -1,    -1,    -1,    61,
      62,    63,    -1,    94,    66,    67,    55,    56,    57,    71,
      -1,    73,    61,    62,    63,    77,    78,    66,    67,    -1,
       3,     4,    71,    -1,    73,    -1,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    -1,
      -1,    61,    62,    63,    -1,    -1,    66,    67,    31,    -1,
      -1,    71,    -1,    73,     3,     4,    -1,    77,    78,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,     3,     4,    -1,
      -1,    -1,    55,    56,    57,    -1,    -1,    -1,    61,    62,
      63,    -1,    -1,    66,    67,    -1,    -1,    -1,    71,    -1,
      73,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
      -1,    -1,    61,    62,    63,    -1,    -1,    66,    67,    55,
      56,    57,    71,    -1,    73,    61,    62,    63,    77,    78,
      66,    67,    -1,    -1,    -1,    71,    -1,    73,    -1,    -1,
      -1,    77,    78,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    -1,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    91,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    -1,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      91,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    91,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    91,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    -1,     3,    -1,    -1,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    90,    21,    22,    23,    -1,    25,    -1,    27,
      28,    29,    -1,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,     3,    -1,    -1,     6,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    21,    22,    23,    -1,    25,    -1,
      27,    28,    29,    -1,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,     3,    -1,    -1,
       6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    21,    22,    23,    -1,    25,
      -1,    27,    28,    29,    -1,    -1,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,     3,    -1,
      -1,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    23,    -1,
      25,    -1,    27,    28,    29,    -1,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      76,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    76,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    76,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    76,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    76,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      76,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    76,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    76,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    74,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    -1,    73,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    72,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    72,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    45,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    -1,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    47,    48,    49,    50,
      51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      47,    48,    49,    50,    51,    52,    53,    54,    47,    48,
      49,    50,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    70,    -1,    64,    65,    66,    67,    68,
      69,    70,    46,    47,    48,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    13,    24,    97,    98,    99,   100,     3,     3,     0,
      99,   100,     4,    71,     3,    72,    75,   108,   109,    92,
      90,     3,     4,    55,    56,    57,    61,    62,    63,    66,
      67,    71,    73,    77,    78,   131,   132,    72,    91,    73,
     132,     3,     6,     7,     8,     9,    21,    22,    23,    25,
      27,    28,    29,    32,    33,    35,    36,    37,    38,    39,
      40,    41,    42,    43,   101,   102,   103,   104,   105,   106,
     107,   110,   111,   113,   119,   120,    75,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   133,   131,   131,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    90,    72,   109,   131,     3,
      93,   114,    95,   128,     3,    71,   112,   122,   132,     9,
      10,    11,    19,    20,    90,   121,   122,   123,   126,   127,
     132,   108,   108,   108,   112,   112,   112,   112,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   112,   112,    18,
     102,   131,    72,    73,    74,    91,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   101,    90,    71,    71,    68,    71,   121,    71,   131,
      90,    90,    92,   127,   127,    71,    71,    71,    90,    90,
      17,   121,    47,    90,    91,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    60,    76,   131,   133,   131,    60,    76,    18,
     101,    72,    94,   115,   116,   131,    94,   117,   118,   131,
      26,    34,    68,   129,   130,   132,   131,    91,     5,   131,
      17,    15,   131,   131,   122,     5,    31,   131,   131,    75,
      74,    74,   131,     3,    18,    90,     3,    72,    91,     3,
      72,    91,     3,     3,    72,    29,    72,    91,    91,   131,
     131,    72,    72,    90,    31,   131,   131,    76,   131,    74,
      74,    75,    92,    71,    90,   116,    71,     3,   118,   130,
     130,   131,    72,    91,    12,   124,   125,   131,   121,   131,
     131,    60,    76,   131,   132,   131,   131,    71,    72,    91,
     131,    60,    16,   125,    60,    14,    90,   131,    75,    60,
      72,    72,    72,   115,   131,    72,   121,   121,   121,   122,
      76,   131,   131,    90,    72,    72,    72,    60,    76,    90,
     121,   131,    75,    76,   131,    60,   131,    76
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    96,    97,    98,    98,    98,    98,    99,   100,   100,
     100,   101,   101,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   103,   104,   104,   105,   105,   106,   107,
     107,   107,   107,   107,   107,   108,   108,   109,   109,   109,
     109,   109,   109,   110,   111,   111,   111,   111,   111,   111,
     111,   112,   112,   112,   112,   113,   114,   114,   114,   114,
     115,   115,   116,   116,   117,   117,   118,   118,   119,   120,
     121,   121,   121,   121,   121,   121,   121,   121,   122,   122,
     123,   123,   123,   123,   124,   124,   125,   125,   126,   126,
     127,   127,   128,   128,   128,   129,   129,   129,   130,   130,
     130,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   132,
     132,   132,   132,   132,   132,   132,   132,   133,   133
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     1,     1,     3,     8,     9,
       7,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     6,    11,
      16,     8,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     8,     7,     6,     5,     2,     5,     9,     4,     8,
       3,     1,     5,     1,     3,     1,     5,     1,     3,     3,
       1,     2,     2,     5,     7,     6,     9,     1,     3,     4,
       3,     4,     4,     5,     2,     1,     3,     3,     3,     3,
       2,     1,     4,     4,     2,     3,     3,     1,     1,     2,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     5,     3,     6,     6,     1,
       1,     4,     7,     6,     9,    12,     3,     3,     1
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
        case 2:
#line 119 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {next_parsed_verilog_file((yyvsp[0].node));}
#line 1862 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 3:
#line 122 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {
											if ((yyvsp[-1].node) != NULL)
											{
												(yyval.node) = newList_entry((yyvsp[-1].node), (yyvsp[0].node));
											}
											else
											{
												(yyval.node) = newList(FILE_ITEMS, (yyvsp[0].node));
											}
										}
#line 1877 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 4:
#line 132 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);}
#line 1883 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 5:
#line 133 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList(FILE_ITEMS, (yyvsp[0].node));}
#line 1889 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 6:
#line 134 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = NULL;}
#line 1895 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 7:
#line 137 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = NULL; newDefine((yyvsp[-1].id_name), (yyvsp[0].num_value), yylineno);}
#line 1901 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 8:
#line 140 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModule((yyvsp[-6].id_name), (yyvsp[-4].node), (yyvsp[-1].node), yylineno);}
#line 1907 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 9:
#line 141 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModule((yyvsp[-7].id_name), (yyvsp[-5].node), (yyvsp[-1].node), yylineno);}
#line 1913 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 10:
#line 142 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModule((yyvsp[-5].id_name), NULL, (yyvsp[-1].node), yylineno);}
#line 1919 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 11:
#line 145 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList_entry((yyvsp[-1].node), (yyvsp[0].node));}
#line 1925 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 12:
#line 146 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList(MODULE_ITEMS, (yyvsp[0].node));}
#line 1931 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 13:
#line 149 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1937 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 14:
#line 150 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1943 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 15:
#line 151 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1949 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 16:
#line 152 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1955 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 17:
#line 153 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1961 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 18:
#line 154 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1967 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 19:
#line 155 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1973 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 20:
#line 156 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1979 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 21:
#line 157 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1985 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 22:
#line 158 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1991 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 23:
#line 162 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(PARAMETER, (yyvsp[-1].node));}
#line 1997 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 24:
#line 165 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(INPUT, (yyvsp[-1].node));}
#line 2003 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 25:
#line 166 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(INPUT_SIGNED, (yyvsp[-1].node));}
#line 2009 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 26:
#line 169 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(OUTPUT, (yyvsp[-1].node));}
#line 2015 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 27:
#line 170 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(OUTPUT_REG, (yyvsp[-1].node));}
#line 2021 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 28:
#line 173 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(INOUT, (yyvsp[-1].node));}
#line 2027 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 29:
#line 176 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(WIRE, (yyvsp[-1].node));}
#line 2033 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 30:
#line 177 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(REG, (yyvsp[-1].node));}
#line 2039 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 31:
#line 178 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(INTEGER, (yyvsp[-1].node));}
#line 2045 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 32:
#line 179 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(GENVAR, (yyvsp[-1].node));}
#line 2051 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 33:
#line 180 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(REG_SIGNED, (yyvsp[-1].node));}
#line 2057 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 34:
#line 181 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = markAndProcessSymbolListWith(WIRE_SIGNED, (yyvsp[-1].node));}
#line 2063 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 35:
#line 184 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList_entry((yyvsp[-2].node), (yyvsp[0].node));}
#line 2069 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 36:
#line 185 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList(VAR_DECLARE_LIST, (yyvsp[0].node));}
#line 2075 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 37:
#line 188 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newVarDeclare((yyvsp[0].id_name), NULL, NULL, NULL, NULL, NULL, yylineno);}
#line 2081 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 38:
#line 189 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newVarDeclare((yyvsp[0].id_name), (yyvsp[-4].node), (yyvsp[-2].node), NULL, NULL, NULL, yylineno);}
#line 2087 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 39:
#line 190 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newVarDeclare((yyvsp[-5].id_name), (yyvsp[-9].node), (yyvsp[-7].node), (yyvsp[-3].node), (yyvsp[-1].node), NULL, yylineno);}
#line 2093 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 40:
#line 191 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newDoubleArray((yyvsp[-10].id_name), (yyvsp[-14].node), (yyvsp[-12].node), (yyvsp[-8].node), (yyvsp[-6].node), (yyvsp[-3].node), (yyvsp[-1].node), NULL, yylineno);}
#line 2099 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 41:
#line 192 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newVarDeclare((yyvsp[-2].id_name), (yyvsp[-6].node), (yyvsp[-4].node), NULL, NULL, (yyvsp[0].node), yylineno);}
#line 2105 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 42:
#line 193 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newVarDeclare((yyvsp[-2].id_name), NULL, NULL, NULL, NULL, (yyvsp[0].node), yylineno);}
#line 2111 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 43:
#line 196 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newAssign((yyvsp[-1].node), yylineno);}
#line 2117 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 44:
#line 200 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGate(BITWISE_AND, (yyvsp[-1].node), yylineno);}
#line 2123 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 45:
#line 201 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGate(BITWISE_NAND, (yyvsp[-1].node), yylineno);}
#line 2129 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 46:
#line 202 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGate(BITWISE_NOR, (yyvsp[-1].node), yylineno);}
#line 2135 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 47:
#line 203 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGate(BITWISE_NOT, (yyvsp[-1].node), yylineno);}
#line 2141 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 48:
#line 204 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGate(BITWISE_OR, (yyvsp[-1].node), yylineno);}
#line 2147 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 49:
#line 205 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGate(BITWISE_XNOR, (yyvsp[-1].node), yylineno);}
#line 2153 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 50:
#line 206 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGate(BITWISE_XOR, (yyvsp[-1].node), yylineno);}
#line 2159 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 51:
#line 209 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGateInstance((yyvsp[-7].id_name), (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].node), yylineno);}
#line 2165 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 52:
#line 210 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGateInstance(NULL, (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].node), yylineno);}
#line 2171 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 53:
#line 211 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGateInstance((yyvsp[-5].id_name), (yyvsp[-3].node), (yyvsp[-1].node), NULL, yylineno);}
#line 2177 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 54:
#line 212 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGateInstance(NULL, (yyvsp[-3].node), (yyvsp[-1].node), NULL, yylineno);}
#line 2183 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 55:
#line 216 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModuleInstance((yyvsp[-1].id_name), (yyvsp[0].node), yylineno);}
#line 2189 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 56:
#line 219 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModuleNamedInstance((yyvsp[-4].id_name), (yyvsp[-2].node), NULL, yylineno);}
#line 2195 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 57:
#line 220 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModuleNamedInstance((yyvsp[-4].id_name), (yyvsp[-2].node), (yyvsp[-6].node), yylineno); }
#line 2201 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 58:
#line 221 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModuleNamedInstance((yyvsp[-3].id_name), NULL, NULL, yylineno);}
#line 2207 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 59:
#line 222 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModuleNamedInstance((yyvsp[-3].id_name), NULL, (yyvsp[-5].node), yylineno);}
#line 2213 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 60:
#line 225 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList_entry((yyvsp[-2].node), (yyvsp[0].node));}
#line 2219 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 61:
#line 226 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList(MODULE_CONNECT_LIST, (yyvsp[0].node));}
#line 2225 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 62:
#line 229 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModuleConnection((yyvsp[-3].id_name), (yyvsp[-1].node), yylineno);}
#line 2231 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 63:
#line 230 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModuleConnection(NULL, (yyvsp[0].node), yylineno);}
#line 2237 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 64:
#line 233 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList_entry((yyvsp[-2].node), (yyvsp[0].node));}
#line 2243 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 65:
#line 234 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList(MODULE_PARAMETER_LIST, (yyvsp[0].node));}
#line 2249 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 66:
#line 237 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModuleParameter((yyvsp[-3].id_name), (yyvsp[-1].node), yylineno);}
#line 2255 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 67:
#line 238 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newModuleParameter(NULL, (yyvsp[0].node), yylineno);}
#line 2261 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 68:
#line 242 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newAlways((yyvsp[-1].node), (yyvsp[0].node), yylineno);}
#line 2267 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 69:
#line 245 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newGenerate((yyvsp[-1].node),yylineno);}
#line 2273 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 70:
#line 247 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 2279 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 71:
#line 248 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);}
#line 2285 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 72:
#line 249 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);}
#line 2291 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 73:
#line 250 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newIf((yyvsp[-2].node), (yyvsp[0].node), NULL, yylineno);}
#line 2297 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 74:
#line 251 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newIf((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2303 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 75:
#line 252 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newCase((yyvsp[-3].node), (yyvsp[-1].node), yylineno);}
#line 2309 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 76:
#line 253 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newFor((yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2315 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 77:
#line 254 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = NULL;}
#line 2321 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 78:
#line 257 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBlocking((yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2327 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 79:
#line 258 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBlocking((yyvsp[-3].node), (yyvsp[0].node), yylineno);}
#line 2333 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 80:
#line 261 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newNonBlocking(" ", (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2339 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 81:
#line 262 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newNonBlocking("$signed", (yyvsp[-3].node), (yyvsp[0].node), yylineno);}
#line 2345 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 82:
#line 263 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newNonBlocking(" ", (yyvsp[-3].node), (yyvsp[0].node), yylineno);}
#line 2351 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 83:
#line 264 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newNonBlocking("$signed", (yyvsp[-4].node), (yyvsp[0].node), yylineno);}
#line 2357 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 84:
#line 268 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList_entry((yyvsp[-1].node), (yyvsp[0].node));}
#line 2363 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 85:
#line 269 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList(CASE_LIST, (yyvsp[0].node));}
#line 2369 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 86:
#line 272 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newCaseItem((yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2375 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 87:
#line 273 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newDefaultCase((yyvsp[0].node), yylineno);}
#line 2381 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 88:
#line 276 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);}
#line 2387 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 89:
#line 277 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);}
#line 2393 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 90:
#line 280 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList_entry((yyvsp[-1].node), (yyvsp[0].node));}
#line 2399 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 91:
#line 281 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList(BLOCK, (yyvsp[0].node));}
#line 2405 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 92:
#line 284 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);}
#line 2411 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 93:
#line 285 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = NULL;}
#line 2417 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 94:
#line 286 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = NULL;}
#line 2423 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 95:
#line 290 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList_entry((yyvsp[-2].node), (yyvsp[0].node));}
#line 2429 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 96:
#line 291 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList_entry((yyvsp[-2].node), (yyvsp[0].node));}
#line 2435 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 97:
#line 292 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList(DELAY_CONTROL, (yyvsp[0].node));}
#line 2441 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 98:
#line 295 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 2447 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 99:
#line 296 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newPosedgeSymbol((yyvsp[0].id_name), yylineno);}
#line 2453 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 100:
#line 297 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newNegedgeSymbol((yyvsp[0].id_name), yylineno);}
#line 2459 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 101:
#line 300 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 2465 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 102:
#line 301 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(ADD, (yyvsp[0].node), yylineno);}
#line 2471 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 103:
#line 302 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(MINUS, (yyvsp[0].node), yylineno);}
#line 2477 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 104:
#line 303 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(BITWISE_NOT, (yyvsp[0].node), yylineno);}
#line 2483 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 105:
#line 304 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(BITWISE_AND, (yyvsp[0].node), yylineno);}
#line 2489 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 106:
#line 305 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(BITWISE_OR, (yyvsp[0].node), yylineno);}
#line 2495 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 107:
#line 306 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(BITWISE_NAND, (yyvsp[0].node), yylineno);}
#line 2501 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 108:
#line 307 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(BITWISE_NOR, (yyvsp[0].node), yylineno);}
#line 2507 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 109:
#line 308 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(BITWISE_XNOR, (yyvsp[0].node), yylineno);}
#line 2513 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 110:
#line 309 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(LOGICAL_NOT, (yyvsp[0].node), yylineno);}
#line 2519 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 111:
#line 310 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newUnaryOperation(BITWISE_XOR, (yyvsp[0].node), yylineno);}
#line 2525 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 112:
#line 311 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(BITWISE_XOR, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2531 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 113:
#line 312 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(MULTIPLY, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2537 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 114:
#line 313 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(DIVIDE, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2543 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 115:
#line 314 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(MODULO, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2549 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 116:
#line 315 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(ADD, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2555 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 117:
#line 316 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(MINUS, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2561 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 118:
#line 317 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(BITWISE_AND, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2567 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 119:
#line 318 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(BITWISE_OR, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2573 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 120:
#line 319 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(BITWISE_NAND, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2579 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 121:
#line 320 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(BITWISE_NOR, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2585 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 122:
#line 321 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(BITWISE_XNOR, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2591 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 123:
#line 322 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(LT, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2597 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 124:
#line 323 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(GT, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2603 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 125:
#line 324 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(SR, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2609 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 126:
#line 325 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(SL, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2615 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 127:
#line 326 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(LOGICAL_EQUAL, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2621 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 128:
#line 327 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(NOT_EQUAL, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2627 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 129:
#line 328 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(LTE, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2633 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 130:
#line 329 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(GTE, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2639 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 131:
#line 330 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(CASE_EQUAL, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2645 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 132:
#line 331 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(CASE_NOT_EQUAL, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2651 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 133:
#line 332 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(LOGICAL_OR, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2657 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 134:
#line 333 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newBinaryOperation(LOGICAL_AND, (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2663 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 135:
#line 334 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newIfQuestion((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node), yylineno);}
#line 2669 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 136:
#line 335 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);}
#line 2675 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 137:
#line 339 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newListReplicate( (yyvsp[-4].node), (yyvsp[-2].node) ); }
#line 2681 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 138:
#line 340 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newListReplicate( (yyvsp[-4].node), (yyvsp[-2].node) ); }
#line 2687 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 139:
#line 343 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newNumberNode((yyvsp[0].num_value), yylineno);}
#line 2693 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 140:
#line 344 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newSymbolNode((yyvsp[0].id_name), yylineno);}
#line 2699 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 141:
#line 345 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newArrayRef((yyvsp[-3].id_name), (yyvsp[-1].node), yylineno);}
#line 2705 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 142:
#line 346 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newArrayArrayRef((yyvsp[-6].id_name), (yyvsp[-4].node), (yyvsp[-1].node), yylineno);}
#line 2711 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 143:
#line 347 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newRangeRef((yyvsp[-5].id_name), (yyvsp[-3].node), (yyvsp[-1].node), yylineno);}
#line 2717 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 144:
#line 348 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newArrayRangeRef((yyvsp[-8].id_name), (yyvsp[-6].node), (yyvsp[-3].node), (yyvsp[-1].node), yylineno);}
#line 2723 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 145:
#line 349 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newMatrixRef((yyvsp[-11].id_name), (yyvsp[-9].node), (yyvsp[-6].node), (yyvsp[-3].node), (yyvsp[-1].node), yylineno);}
#line 2729 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 146:
#line 350 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node); ((yyvsp[-1].node))->types.concat.num_bit_strings = -1;}
#line 2735 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 147:
#line 353 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList_entry((yyvsp[-2].node), (yyvsp[0].node)); /* note this will be in order lsb = greatest to msb = 0 in the node child list */}
#line 2741 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;

  case 148:
#line 354 "SRC/verilog_bison.y" /* yacc.c:1646  */
    {(yyval.node) = newList(CONCATENATE, (yyvsp[0].node));}
#line 2747 "SRC/verilog_bison.c" /* yacc.c:1646  */
    break;


#line 2751 "SRC/verilog_bison.c" /* yacc.c:1646  */
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
#line 357 "SRC/verilog_bison.y" /* yacc.c:1906  */

