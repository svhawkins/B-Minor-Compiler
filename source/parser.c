/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "source/grammar.bison"

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h>
  #include <ctype.h>
  #include "decl.h"
  #include "expr.h"
  #include "param_list.h"
  #include "stmt.h"
  #include "symbol.h"
  #include "type.h"
  #define YYLMAX 256
  extern char* yytext;
  extern int yylex();
  extern int yyerror(const char* str);
  char* name_clean(char* s); // cleans yytext. 
  char error_text[YYLMAX];
  unsigned char eof = 0;
  bool bool_convert(char* s); /* converts given TOKEN_BOOL string to proper integer boolean value */
  void print_error_message(void);

  // parser results
  struct stmt* test_parser_result = NULL; // for testing
  struct decl* parser_result = NULL;
  

#line 98 "source/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif


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
    TOKEN_ERROR = 258,
    TOKEN_EOF = 259,
    TOKEN_ARRAY = 260,
    TOKEN_AUTO = 261,
    TOKEN_BOOLEAN = 262,
    TOKEN_CHAR = 263,
    TOKEN_ELSE = 264,
    TOKEN_FOR = 265,
    TOKEN_FUNCTION = 266,
    TOKEN_IF = 267,
    TOKEN_INTEGER = 268,
    TOKEN_PRINT = 269,
    TOKEN_RETURN = 270,
    TOKEN_STRING = 271,
    TOKEN_VOID = 272,
    TOKEN_WHILE = 273,
    TOKEN_IDENT = 274,
    TOKEN_BOOL = 275,
    TOKEN_NUMBER = 276,
    TOKEN_STR = 277,
    TOKEN_CH = 278,
    TOKEN_INC = 279,
    TOKEN_DEC = 280,
    TOKEN_SUB = 281,
    TOKEN_EXP = 282,
    TOKEN_NOT = 283,
    TOKEN_MULT = 284,
    TOKEN_DIV = 285,
    TOKEN_MOD = 286,
    TOKEN_ADD = 287,
    TOKEN_LEQ = 288,
    TOKEN_LESS = 289,
    TOKEN_GEQ = 290,
    TOKEN_GREAT = 291,
    TOKEN_EQ = 292,
    TOKEN_NEQ = 293,
    TOKEN_AND = 294,
    TOKEN_OR = 295,
    TOKEN_ASSIGN = 296,
    TOKEN_LBRACK = 297,
    TOKEN_RBRACK = 298,
    TOKEN_LPAR = 299,
    TOKEN_RPAR = 300,
    TOKEN_LCURL = 301,
    TOKEN_RCURL = 302,
    TOKEN_COLON = 303,
    TOKEN_SEMI = 304,
    TOKEN_COMMA = 305
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 86 "source/grammar.bison"

  struct expr* expr;
  struct stmt* stmt;
  struct decl* decl;
  struct type* type;
  struct param_list* param_list;
  char* name;

#line 207 "source/parser.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);





#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   463

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  119
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  228

#define YYUNDEFTOK  2
#define YYMAXUTOK   305


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   102,   102,   103,   104,   107,   108,   111,   112,   115,
     116,   119,   120,   123,   124,   127,   128,   131,   132,   133,
     134,   135,   136,   139,   142,   145,   146,   149,   150,   153,
     154,   157,   158,   159,   160,   163,   164,   165,   166,   167,
     168,   171,   172,   173,   176,   177,   178,   179,   182,   183,
     186,   187,   190,   191,   192,   195,   196,   197,   198,   199,
     202,   203,   204,   207,   208,   209,   210,   213,   214,   217,
     221,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   242,   243,   246,   247,   250,   251,
     254,   255,   259,   265,   272,   273,   274,   277,   282,   285,
     288,   289,   292,   293,   298,   301,   304,   305,   308,   309,
     310,   311,   314,   315,   316,   321,   324,   329,   332,   333
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOKEN_ERROR", "TOKEN_EOF",
  "TOKEN_ARRAY", "TOKEN_AUTO", "TOKEN_BOOLEAN", "TOKEN_CHAR", "TOKEN_ELSE",
  "TOKEN_FOR", "TOKEN_FUNCTION", "TOKEN_IF", "TOKEN_INTEGER",
  "TOKEN_PRINT", "TOKEN_RETURN", "TOKEN_STRING", "TOKEN_VOID",
  "TOKEN_WHILE", "TOKEN_IDENT", "TOKEN_BOOL", "TOKEN_NUMBER", "TOKEN_STR",
  "TOKEN_CH", "TOKEN_INC", "TOKEN_DEC", "TOKEN_SUB", "TOKEN_EXP",
  "TOKEN_NOT", "TOKEN_MULT", "TOKEN_DIV", "TOKEN_MOD", "TOKEN_ADD",
  "TOKEN_LEQ", "TOKEN_LESS", "TOKEN_GEQ", "TOKEN_GREAT", "TOKEN_EQ",
  "TOKEN_NEQ", "TOKEN_AND", "TOKEN_OR", "TOKEN_ASSIGN", "TOKEN_LBRACK",
  "TOKEN_RBRACK", "TOKEN_LPAR", "TOKEN_RPAR", "TOKEN_LCURL", "TOKEN_RCURL",
  "TOKEN_COLON", "TOKEN_SEMI", "TOKEN_COMMA", "$accept", "name", "program",
  "ext_decl_list", "test_program", "decl", "init", "ext_decl", "stmt",
  "print_stmt", "expr_stmt", "expr", "init_expr", "assign_expr",
  "unary_expr", "postfix_expr", "primary_expr", "primitive", "lor_expr",
  "land_expr", "eq_expr", "rel_expr", "add_expr", "mult_expr", "exp_expr",
  "select_stmt", "iter_stmt", "jump_stmt", "block_stmt", "block_list",
  "block", "function_decl", "param_list", "params", "param", "param_type",
  "type", "function_type", "array_type", "primitive_type", "ret_type",
  "array_list", "array", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305
};
# endif

#define YYPACT_NINF (-136)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-115)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       8,  -136,  -136,   -30,    -8,   -20,    30,  -136,  -136,    -8,
    -136,   199,    35,   422,  -136,  -136,    -3,     9,     3,   113,
      39,  -136,  -136,  -136,  -136,     3,     3,     3,     3,    56,
     -20,  -136,  -136,  -136,  -136,  -136,    15,  -136,    -7,    66,
    -136,  -136,    58,    -2,    77,   420,    41,   432,  -136,  -136,
    -136,  -136,  -136,  -136,    67,  -136,  -136,  -136,   435,  -136,
    -136,  -136,   -31,  -136,    87,   103,   166,   106,   214,     3,
    -136,   150,  -136,   154,     3,  -136,  -136,  -136,    35,    47,
    -136,   105,  -136,  -136,   109,   199,  -136,     3,     3,     3,
    -136,  -136,     3,   295,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,   393,  -136,  -136,
     166,   280,  -136,     2,  -136,   119,  -136,  -136,   228,   247,
     174,    92,  -136,  -136,    93,  -136,    88,  -136,  -136,  -136,
     156,  -136,  -136,   143,  -136,    99,    -2,    77,   420,   420,
      41,    41,    41,    41,   432,   432,  -136,  -136,  -136,  -136,
     163,  -136,   280,   161,   176,  -136,   164,   171,  -136,   180,
     309,   179,   323,   189,   261,   199,   199,  -136,   166,  -136,
    -136,  -136,  -136,    -5,  -136,   153,    64,    -8,   199,   101,
     337,   199,   130,   351,   365,   203,   223,  -136,  -136,  -136,
     280,  -136,  -136,  -136,   195,  -136,  -136,  -136,   199,   199,
     139,  -136,   199,   199,   140,   199,   148,   379,   199,  -136,
    -136,  -136,  -136,   199,  -136,  -136,   199,  -136,   199,   199,
     151,  -136,  -136,  -136,  -136,  -136,   199,  -136
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,    13,     0,     0,     0,     0,     2,    15,     6,
      16,     0,     0,     0,     1,     5,     0,     0,     0,     0,
       0,    44,    46,    47,    45,     0,     0,     0,     0,     0,
      43,     3,     8,     7,    17,    18,     0,    25,    67,    31,
      35,    41,    29,    48,    50,    52,    55,    60,    63,    22,
      19,    20,    21,    14,     0,   104,   108,   109,     0,   110,
     111,   113,     0,   115,   102,     0,     0,   117,     0,     0,
      43,     0,    84,     0,     0,    34,    33,    32,    43,     0,
      86,    43,    90,    91,     0,    88,    24,     0,     0,     0,
      36,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   112,   105,
       0,     0,     9,    96,   107,   103,   106,   116,     0,     0,
       0,     0,    23,    85,     0,    42,     0,    87,    89,    26,
      67,    68,    30,     0,    40,     0,    49,    51,    53,    54,
      58,    56,    59,    57,    62,    61,    64,    65,    66,   119,
       0,   114,     0,     0,    12,    95,     0,     0,    94,    98,
       0,     0,     0,     0,     0,     0,     0,   102,     0,    38,
      39,   118,    27,     0,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    71,   103,    11,
       0,    99,   100,   101,     0,    92,    97,    72,     0,     0,
       0,    80,     0,     0,     0,     0,     0,     0,     0,    28,
      93,    73,    74,     0,    81,    82,     0,    76,     0,     0,
       0,    70,    75,    83,    77,    78,     0,    79
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -136,     4,  -136,   235,  -136,   -24,  -135,   244,   -11,  -136,
    -136,   -12,  -136,   -49,    24,  -136,  -136,  -136,  -136,   177,
     181,   165,   359,   162,   -19,  -136,  -136,  -136,    63,   193,
    -136,  -136,  -136,    82,  -136,  -136,   110,     1,  -108,   -10,
     216,    -4,  -136
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    70,     6,     7,    31,     8,   153,     9,    83,    34,
      35,    36,   173,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    84,
      85,    10,   157,   158,   159,   191,    62,   114,   115,   116,
      65,   168,    67
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      33,    11,   151,    64,     5,    82,    71,    73,    12,    66,
     111,     2,     1,     5,    63,    30,    79,   172,   112,   155,
      88,     2,     2,    21,    22,    23,    24,     2,    13,    25,
      14,    26,    78,    81,    89,    27,     4,    95,   129,     3,
     132,    68,   189,   133,   119,   190,     4,    28,   108,    75,
      76,    77,     4,    69,   110,   209,   120,   121,   150,    63,
     188,    82,   124,   117,    86,    87,    16,   102,    17,   131,
      18,    19,    81,   103,    20,     2,    21,    22,    23,    24,
      53,   135,    25,    74,    26,   146,   147,   148,    27,    81,
      90,    91,   125,    54,    55,    56,    57,    87,    94,   154,
      28,    59,    29,    80,    60,   194,   161,   163,    92,   107,
      93,    54,   130,   195,    96,    97,   167,   156,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,  -112,     2,    21,    22,    23,    24,   165,   166,    25,
     154,    26,    87,    87,   170,    27,   198,   113,   179,    87,
     182,    87,   185,   126,   186,   187,   127,    28,    54,    55,
      56,    57,    72,  -114,    58,   167,    59,   197,   200,    60,
     201,   204,   206,    56,    57,   202,   193,    58,   154,    59,
      87,   156,    60,    88,   213,   216,   169,   211,   212,    87,
      87,   214,   215,   218,   217,   220,   226,   221,    87,   122,
      87,    87,   222,   123,    87,   223,   171,   224,   225,    16,
     174,    17,   175,    18,    19,   227,   176,    20,     2,    21,
      22,    23,    24,   164,    87,    25,    87,    26,   180,    87,
     177,    27,   208,     2,    21,    22,    23,    24,   183,    87,
      25,    29,    26,    28,    15,    29,    27,     2,    21,    22,
      23,    24,   207,    87,    25,    32,    26,   210,    28,   196,
      27,   138,   139,   118,   144,   145,     2,    21,    22,    23,
      24,   136,    28,    25,   109,    26,   137,   160,   128,    27,
       2,    21,    22,    23,    24,   192,     0,    25,     0,    26,
       0,    28,     0,    27,     0,     0,   162,     0,     0,     2,
      21,    22,    23,    24,     0,    28,    25,     0,    26,     0,
     184,     0,    27,     0,     2,    21,    22,    23,    24,     0,
       0,    25,     0,    26,    28,     0,   152,    27,     2,    21,
      22,    23,    24,     0,     0,    25,     0,    26,     0,    28,
     134,    27,     2,    21,    22,    23,    24,     0,     0,    25,
       0,    26,     0,    28,   178,    27,     2,    21,    22,    23,
      24,     0,     0,    25,     0,    26,     0,    28,   181,    27,
       2,    21,    22,    23,    24,     0,     0,    25,     0,    26,
       0,    28,   199,    27,     2,    21,    22,    23,    24,     0,
       0,    25,     0,    26,     0,    28,   203,    27,     2,    21,
      22,    23,    24,     0,     0,    25,     0,    26,     0,    28,
     205,    27,     2,    21,    22,    23,    24,     0,     0,    25,
       0,    26,     0,    28,   219,    27,     0,    54,    55,    56,
      57,     0,     0,    58,     0,    59,   149,    28,    60,    61,
      54,     0,    56,    57,     0,     0,    58,     0,    59,     0,
       0,    60,    61,    98,    99,   100,   101,   140,   141,   142,
     143,   104,   105,   106
};

static const yytype_int16 yycheck[] =
{
      11,    31,   110,    13,     0,    29,    18,    19,     4,    13,
      41,    19,     4,     9,    13,    11,    28,   152,    49,    17,
      27,    19,    19,    20,    21,    22,    23,    19,    48,    26,
       0,    28,    28,    29,    41,    32,    44,    39,    87,    31,
      89,    44,    47,    92,    68,    50,    44,    44,    58,    25,
      26,    27,    44,    44,    58,   190,    68,    69,   107,    58,
     168,    85,    74,    67,    49,    50,    10,    26,    12,    88,
      14,    15,    68,    32,    18,    19,    20,    21,    22,    23,
      45,    93,    26,    44,    28,   104,   105,   106,    32,    85,
      24,    25,    45,     5,     6,     7,     8,    50,    40,   111,
      44,    13,    46,    47,    16,    41,   118,   119,    42,    42,
      44,     5,    88,    49,    37,    38,   126,   113,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,    44,    19,    20,    21,    22,    23,    45,    45,    26,
     152,    28,    50,    50,    45,    32,    45,    44,   160,    50,
     162,    50,   164,    48,   165,   166,    47,    44,     5,     6,
       7,     8,    49,    44,    11,   175,    13,   178,   180,    16,
     181,   183,   184,     7,     8,    45,   175,    11,   190,    13,
      50,   177,    16,    27,    45,    45,    43,   198,   199,    50,
      50,   202,   203,    45,   205,   207,    45,   208,    50,    49,
      50,    50,   213,    49,    50,   216,    43,   218,   219,    10,
      49,    12,    48,    14,    15,   226,    45,    18,    19,    20,
      21,    22,    23,    49,    50,    26,    50,    28,    49,    50,
      50,    32,     9,    19,    20,    21,    22,    23,    49,    50,
      26,    46,    28,    44,     9,    46,    32,    19,    20,    21,
      22,    23,    49,    50,    26,    11,    28,   194,    44,   177,
      32,    96,    97,    49,   102,   103,    19,    20,    21,    22,
      23,    94,    44,    26,    58,    28,    95,    49,    85,    32,
      19,    20,    21,    22,    23,   175,    -1,    26,    -1,    28,
      -1,    44,    -1,    32,    -1,    -1,    49,    -1,    -1,    19,
      20,    21,    22,    23,    -1,    44,    26,    -1,    28,    -1,
      49,    -1,    32,    -1,    19,    20,    21,    22,    23,    -1,
      -1,    26,    -1,    28,    44,    -1,    46,    32,    19,    20,
      21,    22,    23,    -1,    -1,    26,    -1,    28,    -1,    44,
      45,    32,    19,    20,    21,    22,    23,    -1,    -1,    26,
      -1,    28,    -1,    44,    45,    32,    19,    20,    21,    22,
      23,    -1,    -1,    26,    -1,    28,    -1,    44,    45,    32,
      19,    20,    21,    22,    23,    -1,    -1,    26,    -1,    28,
      -1,    44,    45,    32,    19,    20,    21,    22,    23,    -1,
      -1,    26,    -1,    28,    -1,    44,    45,    32,    19,    20,
      21,    22,    23,    -1,    -1,    26,    -1,    28,    -1,    44,
      45,    32,    19,    20,    21,    22,    23,    -1,    -1,    26,
      -1,    28,    -1,    44,    45,    32,    -1,     5,     6,     7,
       8,    -1,    -1,    11,    -1,    13,    43,    44,    16,    17,
       5,    -1,     7,     8,    -1,    -1,    11,    -1,    13,    -1,
      -1,    16,    17,    33,    34,    35,    36,    98,    99,   100,
     101,    29,    30,    31
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    19,    31,    44,    52,    53,    54,    56,    58,
      82,    31,    52,    48,     0,    54,    10,    12,    14,    15,
      18,    20,    21,    22,    23,    26,    28,    32,    44,    46,
      52,    55,    58,    59,    60,    61,    62,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    45,     5,     6,     7,     8,    11,    13,
      16,    17,    87,    88,    90,    91,    92,    93,    44,    44,
      52,    62,    49,    62,    44,    65,    65,    65,    52,    62,
      47,    52,    56,    59,    80,    81,    49,    50,    27,    41,
      24,    25,    42,    44,    40,    39,    37,    38,    33,    34,
      35,    36,    26,    32,    29,    30,    31,    42,    90,    91,
      92,    41,    49,    44,    88,    89,    90,    92,    49,    56,
      62,    62,    49,    49,    62,    45,    48,    47,    80,    64,
      65,    75,    64,    64,    45,    62,    70,    71,    72,    72,
      73,    73,    73,    73,    74,    74,    75,    75,    75,    43,
      64,    89,    46,    57,    62,    17,    52,    83,    84,    85,
      49,    62,    49,    62,    49,    45,    45,    90,    92,    43,
      45,    43,    57,    63,    49,    48,    45,    50,    45,    62,
      49,    45,    62,    49,    49,    62,    59,    59,    89,    47,
      50,    86,    87,    88,    41,    49,    84,    59,    45,    45,
      62,    59,    45,    45,    62,    45,    62,    49,     9,    57,
      79,    59,    59,    45,    59,    59,    45,    59,    45,    45,
      62,    59,    59,    59,    59,    59,    45,    59
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    53,    53,    53,    54,    54,    55,    55,    56,
      56,    57,    57,    52,    52,    58,    58,    59,    59,    59,
      59,    59,    59,    60,    61,    62,    62,    63,    63,    64,
      64,    65,    65,    65,    65,    66,    66,    66,    66,    66,
      66,    67,    67,    67,    68,    68,    68,    68,    69,    69,
      70,    70,    71,    71,    71,    72,    72,    72,    72,    72,
      73,    73,    73,    74,    74,    74,    74,    75,    75,    76,
      76,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    78,    78,    79,    79,    80,    80,
      81,    81,    82,    82,    83,    83,    83,    84,    84,    85,
      86,    86,    87,    87,    87,    88,    89,    89,    90,    90,
      90,    90,    91,    91,    91,    91,    92,    92,    93,    93
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     1,     2,     1,     1,     1,     4,
       6,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     1,     3,     1,     3,     1,
       3,     1,     2,     2,     2,     1,     2,     2,     4,     4,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     3,     5,
       7,     5,     6,     7,     7,     8,     7,     8,     8,     9,
       6,     7,     7,     8,     2,     3,     2,     3,     1,     2,
       1,     1,     7,     8,     1,     1,     0,     3,     1,     3,
       1,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     4,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
          ++yyp;
          ++yyformat;
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

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
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
#line 102 "source/grammar.bison"
                        { parser_result = (yyvsp[0].decl); return 0; }
#line 1595 "source/parser.c"
    break;

  case 3:
#line 103 "source/grammar.bison"
                                           { test_parser_result = (yyvsp[0].stmt); return 0; }
#line 1601 "source/parser.c"
    break;

  case 4:
#line 104 "source/grammar.bison"
                    { eof = 1; return 0; }
#line 1607 "source/parser.c"
    break;

  case 5:
#line 107 "source/grammar.bison"
                                       { (yyval.decl) = (yyvsp[-1].decl); (yyvsp[-1].decl)->next = (yyvsp[0].decl); }
#line 1613 "source/parser.c"
    break;

  case 6:
#line 108 "source/grammar.bison"
                         { (yyval.decl) = (yyvsp[0].decl); }
#line 1619 "source/parser.c"
    break;

  case 7:
#line 111 "source/grammar.bison"
                    { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1625 "source/parser.c"
    break;

  case 8:
#line 112 "source/grammar.bison"
                        { (yyval.stmt) = stmt_create(STMT_DECL, (yyvsp[0].decl), NULL, NULL, NULL, NULL, NULL, NULL); }
#line 1631 "source/parser.c"
    break;

  case 9:
#line 115 "source/grammar.bison"
                                        { (yyval.decl) = decl_create((yyvsp[-3].name), (yyvsp[-1].type), NULL, NULL, NULL); }
#line 1637 "source/parser.c"
    break;

  case 10:
#line 116 "source/grammar.bison"
                                                          { (yyval.decl) = decl_create((yyvsp[-5].name), (yyvsp[-3].type), (yyvsp[-1].expr), NULL,  NULL); }
#line 1643 "source/parser.c"
    break;

  case 11:
#line 119 "source/grammar.bison"
                                         { (yyval.expr) = expr_create(EXPR_INIT, (yyvsp[-1].expr), NULL); }
#line 1649 "source/parser.c"
    break;

  case 12:
#line 120 "source/grammar.bison"
            { (yyval.expr) = (yyvsp[0].expr); }
#line 1655 "source/parser.c"
    break;

  case 13:
#line 123 "source/grammar.bison"
                   { (yyval.name) = strdup(yytext); }
#line 1661 "source/parser.c"
    break;

  case 14:
#line 124 "source/grammar.bison"
                                  { (yyval.name) = (yyvsp[-1].name); }
#line 1667 "source/parser.c"
    break;

  case 15:
#line 127 "source/grammar.bison"
                { (yyval.decl) = (yyvsp[0].decl); }
#line 1673 "source/parser.c"
    break;

  case 16:
#line 128 "source/grammar.bison"
                         { (yyval.decl) = (yyvsp[0].decl); }
#line 1679 "source/parser.c"
    break;

  case 17:
#line 131 "source/grammar.bison"
                  { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1685 "source/parser.c"
    break;

  case 18:
#line 132 "source/grammar.bison"
                 { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1691 "source/parser.c"
    break;

  case 19:
#line 133 "source/grammar.bison"
                 { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1697 "source/parser.c"
    break;

  case 20:
#line 134 "source/grammar.bison"
                 { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1703 "source/parser.c"
    break;

  case 21:
#line 135 "source/grammar.bison"
                  { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1709 "source/parser.c"
    break;

  case 22:
#line 136 "source/grammar.bison"
                   { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1715 "source/parser.c"
    break;

  case 23:
#line 139 "source/grammar.bison"
                                         { (yyval.stmt) = stmt_create(STMT_PRINT, NULL, NULL, (yyvsp[-1].expr), NULL, NULL, NULL, NULL); }
#line 1721 "source/parser.c"
    break;

  case 24:
#line 142 "source/grammar.bison"
                            { (yyval.stmt) = stmt_create(STMT_EXPR, NULL, NULL, (yyvsp[-1].expr), NULL, NULL, NULL, NULL); }
#line 1727 "source/parser.c"
    break;

  case 25:
#line 145 "source/grammar.bison"
                   { (yyval.expr) = (yyvsp[0].expr); }
#line 1733 "source/parser.c"
    break;

  case 26:
#line 146 "source/grammar.bison"
                                    { (yyval.expr) = expr_create(EXPR_COMMA, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1739 "source/parser.c"
    break;

  case 27:
#line 149 "source/grammar.bison"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 1745 "source/parser.c"
    break;

  case 28:
#line 150 "source/grammar.bison"
                                       { (yyval.expr) = expr_create(EXPR_COMMA, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1751 "source/parser.c"
    break;

  case 29:
#line 153 "source/grammar.bison"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 1757 "source/parser.c"
    break;

  case 30:
#line 154 "source/grammar.bison"
                                                  { (yyval.expr) = expr_create(EXPR_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1763 "source/parser.c"
    break;

  case 31:
#line 157 "source/grammar.bison"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 1769 "source/parser.c"
    break;

  case 32:
#line 158 "source/grammar.bison"
                                  { (yyval.expr) = expr_create(EXPR_POS, (yyvsp[0].expr), NULL); }
#line 1775 "source/parser.c"
    break;

  case 33:
#line 159 "source/grammar.bison"
                                  { (yyval.expr) = expr_create(EXPR_NOT, (yyvsp[0].expr), NULL); }
#line 1781 "source/parser.c"
    break;

  case 34:
#line 160 "source/grammar.bison"
                                  { (yyval.expr) = expr_create(EXPR_NEG, (yyvsp[0].expr), NULL); }
#line 1787 "source/parser.c"
    break;

  case 35:
#line 163 "source/grammar.bison"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 1793 "source/parser.c"
    break;

  case 36:
#line 164 "source/grammar.bison"
                                      { (yyval.expr) = expr_create(EXPR_INC, (yyvsp[-1].expr), NULL); }
#line 1799 "source/parser.c"
    break;

  case 37:
#line 165 "source/grammar.bison"
                                      { (yyval.expr) = expr_create(EXPR_DEC, (yyvsp[-1].expr), NULL); }
#line 1805 "source/parser.c"
    break;

  case 38:
#line 166 "source/grammar.bison"
                                                                  { (yyval.expr) = expr_create(EXPR_SUBSCRIPT, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1811 "source/parser.c"
    break;

  case 39:
#line 167 "source/grammar.bison"
                                                       { (yyval.expr) = expr_create(EXPR_FCALL, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1817 "source/parser.c"
    break;

  case 40:
#line 168 "source/grammar.bison"
                                                  { (yyval.expr) = expr_create(EXPR_FCALL, (yyvsp[-2].expr), NULL); }
#line 1823 "source/parser.c"
    break;

  case 41:
#line 171 "source/grammar.bison"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 1829 "source/parser.c"
    break;

  case 42:
#line 172 "source/grammar.bison"
                                          { (yyval.expr) = (yyvsp[-1].expr); }
#line 1835 "source/parser.c"
    break;

  case 43:
#line 173 "source/grammar.bison"
                    { (yyval.expr) = expr_create_name((yyvsp[0].name)); }
#line 1841 "source/parser.c"
    break;

  case 44:
#line 176 "source/grammar.bison"
                       { (yyval.expr) = expr_create_boolean_literal(bool_convert(yytext)); }
#line 1847 "source/parser.c"
    break;

  case 45:
#line 177 "source/grammar.bison"
                     { (yyval.expr) = expr_create_char_literal(yytext[0]); }
#line 1853 "source/parser.c"
    break;

  case 46:
#line 178 "source/grammar.bison"
                         { (yyval.expr) = expr_create_integer_literal(atoi(yytext)); }
#line 1859 "source/parser.c"
    break;

  case 47:
#line 179 "source/grammar.bison"
                      { (yyval.expr) = expr_create_string_literal(yytext); }
#line 1865 "source/parser.c"
    break;

  case 48:
#line 182 "source/grammar.bison"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1871 "source/parser.c"
    break;

  case 49:
#line 183 "source/grammar.bison"
                                       { (yyval.expr) = expr_create(EXPR_OR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1877 "source/parser.c"
    break;

  case 50:
#line 186 "source/grammar.bison"
                    { (yyval.expr) = (yyvsp[0].expr); }
#line 1883 "source/parser.c"
    break;

  case 51:
#line 187 "source/grammar.bison"
                                        { (yyval.expr) = expr_create(EXPR_AND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1889 "source/parser.c"
    break;

  case 52:
#line 190 "source/grammar.bison"
                   { (yyval.expr) = (yyvsp[0].expr); }
#line 1895 "source/parser.c"
    break;

  case 53:
#line 191 "source/grammar.bison"
                                    { (yyval.expr) = expr_create(EXPR_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1901 "source/parser.c"
    break;

  case 54:
#line 192 "source/grammar.bison"
                                     { (yyval.expr) = expr_create(EXPR_NEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1907 "source/parser.c"
    break;

  case 55:
#line 195 "source/grammar.bison"
                    { (yyval.expr) = (yyvsp[0].expr); }
#line 1913 "source/parser.c"
    break;

  case 56:
#line 196 "source/grammar.bison"
                                        { (yyval.expr) = expr_create(EXPR_LESS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1919 "source/parser.c"
    break;

  case 57:
#line 197 "source/grammar.bison"
                                         { (yyval.expr) = expr_create(EXPR_GREAT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1925 "source/parser.c"
    break;

  case 58:
#line 198 "source/grammar.bison"
                                       { (yyval.expr) = expr_create(EXPR_LEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1931 "source/parser.c"
    break;

  case 59:
#line 199 "source/grammar.bison"
                                       { (yyval.expr) = expr_create(EXPR_GEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1937 "source/parser.c"
    break;

  case 60:
#line 202 "source/grammar.bison"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1943 "source/parser.c"
    break;

  case 61:
#line 203 "source/grammar.bison"
                                        { (yyval.expr) = expr_create(EXPR_ADD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1949 "source/parser.c"
    break;

  case 62:
#line 204 "source/grammar.bison"
                                        { (yyval.expr) = expr_create(EXPR_SUB, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1955 "source/parser.c"
    break;

  case 63:
#line 207 "source/grammar.bison"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1961 "source/parser.c"
    break;

  case 64:
#line 208 "source/grammar.bison"
                                          { (yyval.expr) = expr_create(EXPR_MULT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1967 "source/parser.c"
    break;

  case 65:
#line 209 "source/grammar.bison"
                                         { (yyval.expr) = expr_create(EXPR_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1973 "source/parser.c"
    break;

  case 66:
#line 210 "source/grammar.bison"
                                         { (yyval.expr) = expr_create(EXPR_MOD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1979 "source/parser.c"
    break;

  case 67:
#line 213 "source/grammar.bison"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 1985 "source/parser.c"
    break;

  case 68:
#line 214 "source/grammar.bison"
                                         { (yyval.expr) = expr_create(EXPR_EXP, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1991 "source/parser.c"
    break;

  case 69:
#line 218 "source/grammar.bison"
               { 
		(yyval.stmt) = stmt_create(STMT_IF_ELSE, NULL, NULL, (yyvsp[-2].expr), NULL, (yyvsp[0].stmt), NULL, NULL);
	       }
#line 1999 "source/parser.c"
    break;

  case 70:
#line 222 "source/grammar.bison"
               {
		(yyval.stmt) = stmt_create(STMT_IF_ELSE, NULL, NULL, (yyvsp[-4].expr), NULL, (yyvsp[-2].stmt), (yyvsp[0].stmt), NULL);
	       }
#line 2007 "source/parser.c"
    break;

  case 71:
#line 227 "source/grammar.bison"
                                                        { (yyval.stmt) = stmt_create(STMT_WHILE, NULL, NULL, (yyvsp[-2].expr), NULL, (yyvsp[0].stmt), NULL, NULL); }
#line 2013 "source/parser.c"
    break;

  case 72:
#line 228 "source/grammar.bison"
                                                                       { (yyval.stmt) = stmt_create(STMT_FOR, NULL, NULL, NULL, NULL, (yyvsp[0].stmt), NULL, NULL); }
#line 2019 "source/parser.c"
    break;

  case 73:
#line 229 "source/grammar.bison"
                                                                            { (yyval.stmt) = stmt_create(STMT_FOR, NULL, NULL, NULL, (yyvsp[-2].expr), (yyvsp[0].stmt), NULL, NULL); }
#line 2025 "source/parser.c"
    break;

  case 74:
#line 230 "source/grammar.bison"
                                                                            { (yyval.stmt) = stmt_create(STMT_FOR, NULL, NULL, (yyvsp[-3].expr), NULL, (yyvsp[0].stmt), NULL, NULL); }
#line 2031 "source/parser.c"
    break;

  case 75:
#line 231 "source/grammar.bison"
                                                                                 { (yyval.stmt) = stmt_create(STMT_FOR, NULL, NULL, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt), NULL, NULL); }
#line 2037 "source/parser.c"
    break;

  case 76:
#line 232 "source/grammar.bison"
                                                                            { (yyval.stmt) = stmt_create(STMT_FOR, NULL, (yyvsp[-4].expr), NULL, NULL, (yyvsp[0].stmt), NULL, NULL); }
#line 2043 "source/parser.c"
    break;

  case 77:
#line 233 "source/grammar.bison"
                                                                                 { (yyval.stmt) = stmt_create(STMT_FOR, NULL, (yyvsp[-5].expr), NULL, (yyvsp[-2].expr), (yyvsp[0].stmt), NULL, NULL); }
#line 2049 "source/parser.c"
    break;

  case 78:
#line 234 "source/grammar.bison"
                                                                                 { (yyval.stmt) = stmt_create(STMT_FOR, NULL, (yyvsp[-5].expr), (yyvsp[-3].expr), NULL, (yyvsp[0].stmt), NULL, NULL); }
#line 2055 "source/parser.c"
    break;

  case 79:
#line 235 "source/grammar.bison"
                                                                                       { (yyval.stmt) = stmt_create(STMT_FOR, NULL, (yyvsp[-6].expr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt), NULL, NULL); }
#line 2061 "source/parser.c"
    break;

  case 80:
#line 236 "source/grammar.bison"
                                                                 { (yyval.stmt) = stmt_create(STMT_FOR, (yyvsp[-3].decl), NULL, NULL, NULL, (yyvsp[0].stmt), NULL, NULL); }
#line 2067 "source/parser.c"
    break;

  case 81:
#line 237 "source/grammar.bison"
                                                                      { (yyval.stmt) = stmt_create(STMT_FOR, (yyvsp[-4].decl), NULL, NULL, (yyvsp[-2].expr), (yyvsp[0].stmt), NULL, NULL); }
#line 2073 "source/parser.c"
    break;

  case 82:
#line 238 "source/grammar.bison"
                                                                      { (yyval.stmt) = stmt_create(STMT_FOR, (yyvsp[-4].decl), NULL, (yyvsp[-3].expr), NULL, (yyvsp[0].stmt), NULL, NULL); }
#line 2079 "source/parser.c"
    break;

  case 83:
#line 239 "source/grammar.bison"
                                                                           { (yyval.stmt) = stmt_create(STMT_FOR, (yyvsp[-5].decl), NULL, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt), NULL, NULL); }
#line 2085 "source/parser.c"
    break;

  case 84:
#line 242 "source/grammar.bison"
                                    { (yyval.stmt) = stmt_create(STMT_RETURN, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
#line 2091 "source/parser.c"
    break;

  case 85:
#line 243 "source/grammar.bison"
                                         { (yyval.stmt) = stmt_create(STMT_RETURN, NULL, NULL, (yyvsp[-1].expr), NULL, NULL, NULL, NULL); }
#line 2097 "source/parser.c"
    break;

  case 86:
#line 246 "source/grammar.bison"
                                     { (yyval.stmt) = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
#line 2103 "source/parser.c"
    break;

  case 87:
#line 247 "source/grammar.bison"
                                                { (yyval.stmt) = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, (yyvsp[-1].stmt), NULL, NULL); }
#line 2109 "source/parser.c"
    break;

  case 88:
#line 250 "source/grammar.bison"
                   { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2115 "source/parser.c"
    break;

  case 89:
#line 251 "source/grammar.bison"
                              { (yyval.stmt) = (yyvsp[-1].stmt); (yyvsp[-1].stmt)->next = (yyvsp[0].stmt); }
#line 2121 "source/parser.c"
    break;

  case 90:
#line 254 "source/grammar.bison"
             { (yyval.stmt) = stmt_create(STMT_DECL, (yyvsp[0].decl), NULL, NULL, NULL, NULL, NULL, NULL); }
#line 2127 "source/parser.c"
    break;

  case 91:
#line 255 "source/grammar.bison"
             { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2133 "source/parser.c"
    break;

  case 92:
#line 260 "source/grammar.bison"
                {
		/* assign the params for a function here */
		 (yyvsp[-4].type)->params = (yyvsp[-2].param_list);
		 (yyval.decl) = decl_create((yyvsp[-6].name), (yyvsp[-4].type), NULL, NULL, NULL); 
		}
#line 2143 "source/parser.c"
    break;

  case 93:
#line 266 "source/grammar.bison"
                { /* assign the params here in type */
		  (yyvsp[-5].type)->params = (yyvsp[-3].param_list);
		  (yyval.decl) = decl_create((yyvsp[-7].name), (yyvsp[-5].type), NULL, (yyvsp[0].stmt), NULL);
		}
#line 2152 "source/parser.c"
    break;

  case 94:
#line 272 "source/grammar.bison"
                    { (yyval.param_list) = (yyvsp[0].param_list); }
#line 2158 "source/parser.c"
    break;

  case 95:
#line 273 "source/grammar.bison"
                        { (yyval.param_list) = NULL; }
#line 2164 "source/parser.c"
    break;

  case 96:
#line 274 "source/grammar.bison"
                    { (yyval.param_list) = NULL; }
#line 2170 "source/parser.c"
    break;

  case 97:
#line 278 "source/grammar.bison"
         {
	   (yyvsp[-2].param_list)->next = (yyvsp[0].param_list); // originally null
	   (yyval.param_list) = (yyvsp[-2].param_list);
         }
#line 2179 "source/parser.c"
    break;

  case 98:
#line 282 "source/grammar.bison"
               { (yyval.param_list) = (yyvsp[0].param_list); }
#line 2185 "source/parser.c"
    break;

  case 99:
#line 285 "source/grammar.bison"
                                    { (yyval.param_list) = param_list_create((yyvsp[-2].name), (yyvsp[0].type), NULL); }
#line 2191 "source/parser.c"
    break;

  case 100:
#line 288 "source/grammar.bison"
                  { (yyval.type) = (yyvsp[0].type); }
#line 2197 "source/parser.c"
    break;

  case 101:
#line 289 "source/grammar.bison"
                           { (yyval.type) = (yyvsp[0].type); }
#line 2203 "source/parser.c"
    break;

  case 102:
#line 292 "source/grammar.bison"
                      { (yyval.type) = (yyvsp[0].type); }
#line 2209 "source/parser.c"
    break;

  case 103:
#line 294 "source/grammar.bison"
       { /* assign leaf subtype here */
         type_subtype_leaf_assign((yyvsp[-1].type), (yyvsp[0].type));
	 (yyval.type) = (yyvsp[-1].type);
       }
#line 2218 "source/parser.c"
    break;

  case 104:
#line 298 "source/grammar.bison"
                  { (yyval.type) = type_create(TYPE_AUTO, NULL, NULL, NULL); }
#line 2224 "source/parser.c"
    break;

  case 105:
#line 301 "source/grammar.bison"
                                        { (yyval.type) = type_create(TYPE_FUNCTION, (yyvsp[0].type), NULL, NULL); }
#line 2230 "source/parser.c"
    break;

  case 106:
#line 304 "source/grammar.bison"
                            { (yyval.type) = (yyvsp[0].type); }
#line 2236 "source/parser.c"
    break;

  case 107:
#line 305 "source/grammar.bison"
                           { (yyval.type) = (yyvsp[0].type); }
#line 2242 "source/parser.c"
    break;

  case 108:
#line 308 "source/grammar.bison"
                               { (yyval.type) = type_create(TYPE_BOOLEAN, NULL, NULL, NULL); }
#line 2248 "source/parser.c"
    break;

  case 109:
#line 309 "source/grammar.bison"
                         { (yyval.type) = type_create(TYPE_CHARACTER, NULL, NULL, NULL); }
#line 2254 "source/parser.c"
    break;

  case 110:
#line 310 "source/grammar.bison"
                            { (yyval.type) = type_create(TYPE_INTEGER, NULL, NULL, NULL); }
#line 2260 "source/parser.c"
    break;

  case 111:
#line 311 "source/grammar.bison"
                           { (yyval.type) = type_create(TYPE_STRING, NULL, NULL, NULL); }
#line 2266 "source/parser.c"
    break;

  case 112:
#line 314 "source/grammar.bison"
                          { (yyval.type) = (yyvsp[0].type); }
#line 2272 "source/parser.c"
    break;

  case 113:
#line 315 "source/grammar.bison"
                      { (yyval.type) = type_create(TYPE_VOID, NULL, NULL, NULL); }
#line 2278 "source/parser.c"
    break;

  case 114:
#line 317 "source/grammar.bison"
           { /* assign leaf subtype here */
	     type_subtype_leaf_assign((yyvsp[-1].type), (yyvsp[0].type));
	     (yyval.type) = (yyvsp[-1].type);
	   }
#line 2287 "source/parser.c"
    break;

  case 115:
#line 321 "source/grammar.bison"
                         { (yyval.type) = (yyvsp[0].type); }
#line 2293 "source/parser.c"
    break;

  case 116:
#line 325 "source/grammar.bison"
             { 
	       (yyvsp[-1].type)->subtype = (yyvsp[0].type);
	       (yyval.type) = (yyvsp[-1].type);
	     }
#line 2302 "source/parser.c"
    break;

  case 117:
#line 329 "source/grammar.bison"
                   { (yyval.type) = (yyvsp[0].type); }
#line 2308 "source/parser.c"
    break;

  case 118:
#line 332 "source/grammar.bison"
                                                          { (yyval.type) = type_create(TYPE_ARRAY, NULL, NULL, (yyvsp[-1].expr)); }
#line 2314 "source/parser.c"
    break;

  case 119:
#line 333 "source/grammar.bison"
                                              { (yyval.type) = type_create(TYPE_ARRAY, NULL, NULL, NULL); }
#line 2320 "source/parser.c"
    break;


#line 2324 "source/parser.c"

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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp);
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
#line 336 "source/grammar.bison"

int yyerror(const char* str) {
  fflush(stdout);
  if (!eof) { sprintf(error_text, "%s", str); eof = 0; }
  return 0;
}

/* prints error messages that are NOT eof */
void print_error_message(void) { fprintf(stderr, "%s\n", error_text); }

bool bool_convert(char* s) { return (!strcmp(s, "true")); }

char* name_clean(char* s) {
  int i, j; char new_str[strlen(s)];
  for (i = 0; isspace(s[i]); i++) {} // skip whitespace
  for (j = 0; isalnum(s[i]) || s[i] == '_'; i++, j++) new_str[j] = s[i];
  new_str[j] = '\0';
  strcpy(s, new_str);
  return s;
}
  
