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
  char clean_name[YYLMAX];

  char error_text[YYLMAX];
  unsigned char eof = 0;
  bool bool_convert(char* s); /* converts given TOKEN_BOOL string to proper integer boolean value */
  void print_error_message(void);
  struct stmt* test_parser_result = NULL; // for testing
  struct decl* parser_result = NULL;
  

#line 97 "source/parser.c"

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
#line 85 "source/grammar.bison"

  struct expr* expr;
  struct stmt* stmt;
  struct decl* decl;
  struct type* type;
  struct param_list* param_list;
  char* name;

#line 206 "source/parser.c"

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
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   498

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  226

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
       0,    99,    99,   100,   101,   102,   105,   106,   109,   110,
     113,   114,   117,   118,   121,   122,   125,   126,   129,   130,
     131,   132,   133,   134,   137,   140,   143,   144,   147,   148,
     151,   152,   153,   154,   157,   158,   159,   162,   163,   164,
     165,   166,   167,   170,   171,   172,   173,   176,   177,   180,
     183,   184,   187,   188,   191,   192,   193,   196,   197,   198,
     199,   200,   203,   204,   205,   208,   209,   210,   211,   214,
     215,   218,   219,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   237,   238,   241,   242,
     245,   246,   249,   250,   254,   255,   258,   259,   260,   263,
     264,   267,   270,   271,   274,   275,   276,   279,   282,   283,
     286,   287,   288,   289,   292,   293,   294,   295,   298,   299,
     302,   303
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
  "test_program", "decl", "init", "init_list", "ext_decl", "stmt",
  "print_stmt", "expr_stmt", "expr", "assign_expr", "unary_expr",
  "postfix_expr", "primary_expr", "primitive", "subscript_list",
  "subscript", "lor_expr", "land_expr", "eq_expr", "rel_expr", "add_expr",
  "mult_expr", "exp_expr", "select_stmt", "iter_stmt", "jump_stmt",
  "block_stmt", "block_list", "block", "function_decl", "param_list",
  "params", "param", "param_type", "type", "function_type", "array_type",
  "primitive_type", "ret_type", "array_list", "array", YY_NULLPTR
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

#define YYPACT_NINF (-142)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-117)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      20,  -142,  -142,    34,     0,   -23,    17,  -142,  -142,  -142,
     235,    14,    68,  -142,  -142,    51,    71,   445,   250,    83,
    -142,  -142,  -142,  -142,   445,   445,   445,   445,   110,    38,
    -142,  -142,  -142,  -142,  -142,   -12,  -142,    -6,   162,  -142,
    -142,    37,    78,   184,   458,    28,   114,  -142,  -142,  -142,
    -142,  -142,  -142,   111,  -142,  -142,  -142,   219,  -142,  -142,
    -142,   -27,  -142,   122,   124,   226,  -142,   264,   445,    48,
     210,  -142,   215,   445,  -142,  -142,  -142,    69,    12,  -142,
      45,  -142,  -142,   197,  -142,   445,   347,   113,  -142,  -142,
     445,   445,   445,  -142,  -142,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   170,  -142,
    -142,   226,   330,  -142,     1,  -142,   130,  -142,  -142,   283,
     297,   316,    33,  -142,  -142,    46,  -142,   156,  -142,  -142,
     137,  -142,    73,  -142,  -142,   149,  -142,  -142,    78,   184,
     458,   458,    28,    28,    28,    28,   114,   114,  -142,  -142,
    -142,  -142,   141,  -142,   330,   139,   144,  -142,   152,   159,
    -142,   158,   361,   375,   389,   403,   417,   431,   235,   235,
    -142,   226,  -142,  -142,  -142,  -142,    87,  -142,   474,   -18,
       0,   235,    76,   235,    90,   235,    96,   235,   115,   235,
     133,   235,   201,   229,  -142,  -142,  -142,   330,  -142,  -142,
    -142,   164,  -142,  -142,  -142,   235,  -142,   235,  -142,   235,
    -142,   235,  -142,   235,  -142,   235,   235,  -142,  -142,  -142,
    -142,  -142,  -142,  -142,  -142,  -142
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     5,    14,     0,     0,     0,     0,    16,     2,    17,
       0,     0,     0,     1,     3,     0,     0,     0,     0,     0,
      43,    45,    46,    44,     0,     0,     0,     0,     0,    39,
       4,     7,     6,    18,    19,     0,    26,    69,    30,    34,
      37,    28,    50,    52,    54,    57,    62,    65,    23,    20,
      21,    22,    15,     0,   106,   110,   111,     0,   112,   113,
     115,     0,   117,   104,     0,     0,   118,     0,     0,    39,
       0,    86,     0,     0,    33,    32,    31,    39,     0,    88,
      39,    92,    93,     0,    90,     0,     0,    42,    47,    25,
       0,     0,     0,    35,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
     107,     0,     0,     8,    98,   109,   105,   108,   119,     0,
       0,     0,     0,    24,    87,     0,    38,     0,    89,    91,
       0,    40,     0,    48,    27,    69,    70,    29,    51,    53,
      55,    56,    60,    58,    61,    59,    64,    63,    66,    67,
      68,   121,     0,   116,     0,     0,    11,    97,     0,     0,
      96,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,    49,    41,   120,    12,     0,     9,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    71,    73,   105,    10,     0,   101,   102,
     103,     0,    94,    99,    74,     0,    76,     0,    82,     0,
      84,     0,    78,     0,    80,     0,     0,    13,    95,    75,
      77,    83,    85,    79,    81,    72
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -142,     5,  -142,  -142,   -20,  -141,  -142,    24,   -10,  -142,
     -51,   -15,   -42,     3,  -142,  -142,  -142,  -142,   153,  -142,
     157,   132,   177,   396,   185,   -65,  -142,  -142,  -142,    47,
    -142,   179,  -142,  -142,    86,  -142,  -142,    99,    -8,  -104,
     -11,   211,    -2,   -59
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    69,     6,    30,     7,   155,   176,     8,    82,    33,
      34,    35,    36,    37,    38,    39,    40,    87,    88,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      83,    84,     9,   159,   160,   161,   198,    61,   115,   116,
     117,    64,   171,    66
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,    63,    70,    72,    62,     5,   118,   153,    81,    11,
      65,     5,    78,   175,   112,    29,   121,    13,   157,     2,
       2,    91,   113,   201,     1,    12,   136,    74,    75,    76,
      14,   202,    77,    80,    31,    92,     2,    89,    90,     2,
     148,   149,   150,   130,     4,     4,   109,   120,   134,    62,
     137,     3,   118,   122,   103,   111,   217,   126,   125,    52,
     104,     4,    90,    81,     4,    10,   152,   195,   163,   165,
     167,   132,    80,    53,    54,    55,    56,    95,   168,    57,
      85,    58,    86,    90,    59,    60,    12,    85,    80,    86,
      85,   169,    86,   127,   135,    67,    90,   156,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,    85,   118,    86,    52,    68,   170,    96,   173,   158,
      15,   205,    16,    90,    17,    18,    90,    73,    19,     2,
      20,    21,    22,    23,   196,   207,    24,   197,    25,   156,
      90,   209,    26,   105,   106,   107,    90,   182,   184,   186,
     188,   190,   192,   108,    27,    85,    28,    79,   193,   194,
     211,    53,    54,    55,    56,    90,  -114,   170,   114,    58,
     200,   204,    59,   206,  -116,   208,    91,   210,   213,   212,
     172,   214,   156,    90,   174,   158,    93,    94,   177,     2,
      20,    21,    22,    23,    90,   219,    24,   220,    25,   221,
     178,   222,    26,   223,   179,   224,   225,    15,   180,    16,
      28,    17,    18,   151,    27,    19,     2,    20,    21,    22,
      23,    97,    98,    24,    53,    25,    55,    56,   139,    26,
      57,    53,    58,    55,    56,    59,    60,    57,   216,    58,
     133,    27,    59,    28,   128,    15,   215,    16,   218,    17,
      18,    90,   138,    19,     2,    20,    21,    22,    23,   123,
      90,    24,   129,    25,   124,    90,   203,    26,   110,     2,
      20,    21,    22,    23,   140,   141,    24,   199,    25,    27,
       0,    28,    26,     2,    20,    21,    22,    23,   146,   147,
      24,     0,    25,     0,    27,     0,    26,     0,     0,    71,
       0,     0,     2,    20,    21,    22,    23,     0,    27,    24,
       0,    25,     0,   119,     0,    26,     2,    20,    21,    22,
      23,     0,     0,    24,     0,    25,     0,    27,     0,    26,
       0,     0,   162,     0,     0,     2,    20,    21,    22,    23,
       0,    27,    24,     0,    25,     0,   164,     0,    26,     2,
      20,    21,    22,    23,     0,     0,    24,     0,    25,     0,
      27,     0,    26,     0,     0,   166,     2,    20,    21,    22,
      23,     0,     0,    24,    27,    25,   154,     0,     0,    26,
       2,    20,    21,    22,    23,     0,     0,    24,     0,    25,
       0,    27,   131,    26,     2,    20,    21,    22,    23,     0,
       0,    24,     0,    25,     0,    27,   181,    26,     2,    20,
      21,    22,    23,     0,     0,    24,     0,    25,     0,    27,
     183,    26,     2,    20,    21,    22,    23,     0,     0,    24,
       0,    25,     0,    27,   185,    26,     2,    20,    21,    22,
      23,     0,     0,    24,     0,    25,     0,    27,   187,    26,
       2,    20,    21,    22,    23,     0,     0,    24,     0,    25,
       0,    27,   189,    26,     2,    20,    21,    22,    23,     0,
       0,    24,     0,    25,     0,    27,   191,    26,     0,    53,
      54,    55,    56,     0,     0,    57,     0,    58,     0,    27,
      59,    99,   100,   101,   102,   142,   143,   144,   145
};

static const yytype_int16 yycheck[] =
{
      10,    12,    17,    18,    12,     0,    65,   111,    28,     4,
      12,     6,    27,   154,    41,    10,    67,     0,    17,    19,
      19,    27,    49,    41,     4,    48,    91,    24,    25,    26,
       6,    49,    27,    28,    10,    41,    19,    49,    50,    19,
     105,   106,   107,    85,    44,    44,    57,    67,    90,    57,
      92,    31,   111,    68,    26,    57,   197,    45,    73,    45,
      32,    44,    50,    83,    44,    31,   108,   171,   119,   120,
     121,    86,    67,     5,     6,     7,     8,    40,    45,    11,
      42,    13,    44,    50,    16,    17,    48,    42,    83,    44,
      42,    45,    44,    48,    91,    44,    50,   112,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,    42,   171,    44,    45,    44,   127,    39,    45,   114,
      10,    45,    12,    50,    14,    15,    50,    44,    18,    19,
      20,    21,    22,    23,    47,    45,    26,    50,    28,   154,
      50,    45,    32,    29,    30,    31,    50,   162,   163,   164,
     165,   166,   167,    42,    44,    42,    46,    47,   168,   169,
      45,     5,     6,     7,     8,    50,    44,   178,    44,    13,
     178,   181,    16,   183,    44,   185,    27,   187,    45,   189,
      43,   191,   197,    50,    43,   180,    24,    25,    49,    19,
      20,    21,    22,    23,    50,   205,    26,   207,    28,   209,
      48,   211,    32,   213,    45,   215,   216,    10,    50,    12,
      46,    14,    15,    43,    44,    18,    19,    20,    21,    22,
      23,    37,    38,    26,     5,    28,     7,     8,    96,    32,
      11,     5,    13,     7,     8,    16,    17,    11,     9,    13,
      87,    44,    16,    46,    47,    10,    45,    12,   201,    14,
      15,    50,    95,    18,    19,    20,    21,    22,    23,    49,
      50,    26,    83,    28,    49,    50,   180,    32,    57,    19,
      20,    21,    22,    23,    97,    98,    26,   178,    28,    44,
      -1,    46,    32,    19,    20,    21,    22,    23,   103,   104,
      26,    -1,    28,    -1,    44,    -1,    32,    -1,    -1,    49,
      -1,    -1,    19,    20,    21,    22,    23,    -1,    44,    26,
      -1,    28,    -1,    49,    -1,    32,    19,    20,    21,    22,
      23,    -1,    -1,    26,    -1,    28,    -1,    44,    -1,    32,
      -1,    -1,    49,    -1,    -1,    19,    20,    21,    22,    23,
      -1,    44,    26,    -1,    28,    -1,    49,    -1,    32,    19,
      20,    21,    22,    23,    -1,    -1,    26,    -1,    28,    -1,
      44,    -1,    32,    -1,    -1,    49,    19,    20,    21,    22,
      23,    -1,    -1,    26,    44,    28,    46,    -1,    -1,    32,
      19,    20,    21,    22,    23,    -1,    -1,    26,    -1,    28,
      -1,    44,    45,    32,    19,    20,    21,    22,    23,    -1,
      -1,    26,    -1,    28,    -1,    44,    45,    32,    19,    20,
      21,    22,    23,    -1,    -1,    26,    -1,    28,    -1,    44,
      45,    32,    19,    20,    21,    22,    23,    -1,    -1,    26,
      -1,    28,    -1,    44,    45,    32,    19,    20,    21,    22,
      23,    -1,    -1,    26,    -1,    28,    -1,    44,    45,    32,
      19,    20,    21,    22,    23,    -1,    -1,    26,    -1,    28,
      -1,    44,    45,    32,    19,    20,    21,    22,    23,    -1,
      -1,    26,    -1,    28,    -1,    44,    45,    32,    -1,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    13,    -1,    44,
      16,    33,    34,    35,    36,    99,   100,   101,   102
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    19,    31,    44,    52,    53,    55,    58,    83,
      31,    52,    48,     0,    58,    10,    12,    14,    15,    18,
      20,    21,    22,    23,    26,    28,    32,    44,    46,    52,
      54,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    45,     5,     6,     7,     8,    11,    13,    16,
      17,    88,    89,    91,    92,    93,    94,    44,    44,    52,
      62,    49,    62,    44,    64,    64,    64,    52,    62,    47,
      52,    55,    59,    81,    82,    42,    44,    68,    69,    49,
      50,    27,    41,    24,    25,    40,    39,    37,    38,    33,
      34,    35,    36,    26,    32,    29,    30,    31,    42,    91,
      92,    93,    41,    49,    44,    89,    90,    91,    94,    49,
      55,    61,    62,    49,    49,    62,    45,    48,    47,    82,
      63,    45,    62,    69,    63,    64,    76,    63,    71,    72,
      73,    73,    74,    74,    74,    74,    75,    75,    76,    76,
      76,    43,    63,    90,    46,    56,    62,    17,    52,    84,
      85,    86,    49,    61,    49,    61,    49,    61,    45,    45,
      91,    93,    43,    45,    43,    56,    57,    49,    48,    45,
      50,    45,    62,    45,    62,    45,    62,    45,    62,    45,
      62,    45,    62,    59,    59,    90,    47,    50,    87,    88,
      89,    41,    49,    85,    59,    45,    59,    45,    59,    45,
      59,    45,    59,    45,    59,    45,     9,    56,    80,    59,
      59,    59,    59,    59,    59,    59
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    53,    53,    53,    53,    54,    54,    55,    55,
      56,    56,    57,    57,    52,    52,    58,    58,    59,    59,
      59,    59,    59,    59,    60,    61,    62,    62,    63,    63,
      64,    64,    64,    64,    65,    65,    65,    66,    66,    66,
      66,    66,    66,    67,    67,    67,    67,    68,    68,    69,
      70,    70,    71,    71,    72,    72,    72,    73,    73,    73,
      73,    73,    74,    74,    74,    75,    75,    75,    75,    76,
      76,    77,    77,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    79,    79,    80,    80,
      81,    81,    82,    82,    83,    83,    84,    84,    84,    85,
      85,    86,    87,    87,    88,    88,    88,    89,    90,    90,
      91,    91,    91,    91,    92,    92,    92,    92,    93,    93,
      94,    94
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     3,     1,     1,     1,     4,     6,
       3,     1,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     1,     3,     1,     3,
       1,     2,     2,     2,     1,     2,     2,     1,     3,     1,
       3,     4,     2,     1,     1,     1,     1,     1,     2,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       3,     5,     7,     5,     6,     7,     6,     7,     6,     7,
       6,     7,     6,     7,     6,     7,     2,     3,     2,     3,
       1,     2,     1,     1,     7,     8,     1,     1,     0,     3,
       1,     3,     1,     1,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     2,
       4,     3
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
#line 99 "source/grammar.bison"
                   { return 0; }
#line 1603 "source/parser.c"
    break;

  case 4:
#line 101 "source/grammar.bison"
                                           { test_parser_result = (yyvsp[0].stmt); return 0; }
#line 1609 "source/parser.c"
    break;

  case 5:
#line 102 "source/grammar.bison"
                    { parser_result = NULL; test_parser_result = NULL; eof = 1; return 0; }
#line 1615 "source/parser.c"
    break;

  case 6:
#line 105 "source/grammar.bison"
                    { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1621 "source/parser.c"
    break;

  case 14:
#line 121 "source/grammar.bison"
                   { (yyval.name) = yytext; }
#line 1627 "source/parser.c"
    break;

  case 15:
#line 122 "source/grammar.bison"
                                  { (yyval.name) = (yyvsp[-1].name); }
#line 1633 "source/parser.c"
    break;

  case 19:
#line 130 "source/grammar.bison"
                 { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1639 "source/parser.c"
    break;

  case 25:
#line 140 "source/grammar.bison"
                            { (yyval.stmt) = stmt_create(STMT_EXPR, NULL, NULL, (yyvsp[-1].expr), NULL, NULL, NULL, NULL); }
#line 1645 "source/parser.c"
    break;

  case 26:
#line 143 "source/grammar.bison"
                   { (yyval.expr) = (yyvsp[0].expr); }
#line 1651 "source/parser.c"
    break;

  case 27:
#line 144 "source/grammar.bison"
                                    { (yyval.expr) = expr_create(EXPR_COMMA, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1657 "source/parser.c"
    break;

  case 28:
#line 147 "source/grammar.bison"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 1663 "source/parser.c"
    break;

  case 29:
#line 148 "source/grammar.bison"
                                                  { (yyval.expr) = expr_create(EXPR_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1669 "source/parser.c"
    break;

  case 30:
#line 151 "source/grammar.bison"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 1675 "source/parser.c"
    break;

  case 31:
#line 152 "source/grammar.bison"
                                  { (yyval.expr) = expr_create(EXPR_POS, (yyvsp[0].expr), NULL); }
#line 1681 "source/parser.c"
    break;

  case 32:
#line 153 "source/grammar.bison"
                                  { (yyval.expr) = expr_create(EXPR_NOT, (yyvsp[0].expr), NULL); }
#line 1687 "source/parser.c"
    break;

  case 33:
#line 154 "source/grammar.bison"
                                  { (yyval.expr) = expr_create(EXPR_NEG, (yyvsp[0].expr), NULL); }
#line 1693 "source/parser.c"
    break;

  case 34:
#line 157 "source/grammar.bison"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 1699 "source/parser.c"
    break;

  case 35:
#line 158 "source/grammar.bison"
                                      { (yyval.expr) = expr_create(EXPR_INC, (yyvsp[-1].expr), NULL); }
#line 1705 "source/parser.c"
    break;

  case 36:
#line 159 "source/grammar.bison"
                                      { (yyval.expr) = expr_create(EXPR_DEC, (yyvsp[-1].expr), NULL); }
#line 1711 "source/parser.c"
    break;

  case 37:
#line 162 "source/grammar.bison"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 1717 "source/parser.c"
    break;

  case 38:
#line 163 "source/grammar.bison"
                                          { (yyval.expr) = (yyvsp[-1].expr); }
#line 1723 "source/parser.c"
    break;

  case 39:
#line 164 "source/grammar.bison"
                    { (yyval.expr) = expr_create_name((yyvsp[0].name)); }
#line 1729 "source/parser.c"
    break;

  case 40:
#line 165 "source/grammar.bison"
                                           { (yyval.expr) = expr_create(EXPR_FCALL, expr_create_name((yyvsp[-2].name)), NULL); }
#line 1735 "source/parser.c"
    break;

  case 41:
#line 166 "source/grammar.bison"
                                               { (yyval.expr) = expr_create(EXPR_FCALL, expr_create_name((yyvsp[-3].name)), (yyvsp[-1].expr)); }
#line 1741 "source/parser.c"
    break;

  case 42:
#line 167 "source/grammar.bison"
                                   { (yyvsp[0].expr)->left = expr_create_name((yyvsp[-1].name)); (yyval.expr) = (yyvsp[0].expr); }
#line 1747 "source/parser.c"
    break;

  case 43:
#line 170 "source/grammar.bison"
                       { (yyval.expr) = expr_create_boolean_literal(bool_convert(yytext)); }
#line 1753 "source/parser.c"
    break;

  case 44:
#line 171 "source/grammar.bison"
                     { (yyval.expr) = expr_create_char_literal((char)atoi(yytext)); }
#line 1759 "source/parser.c"
    break;

  case 45:
#line 172 "source/grammar.bison"
                         { (yyval.expr) = expr_create_integer_literal(atoi(yytext)); }
#line 1765 "source/parser.c"
    break;

  case 46:
#line 173 "source/grammar.bison"
                      { (yyval.expr) = expr_create_string_literal(yytext); }
#line 1771 "source/parser.c"
    break;

  case 47:
#line 176 "source/grammar.bison"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 1777 "source/parser.c"
    break;

  case 48:
#line 177 "source/grammar.bison"
                                          { (yyval.expr) = (yyvsp[-1].expr); (yyvsp[-1].expr)->left = (yyvsp[0].expr); }
#line 1783 "source/parser.c"
    break;

  case 49:
#line 180 "source/grammar.bison"
                                                  { (yyval.expr) = expr_create(EXPR_SUBSCRIPT, NULL, (yyvsp[-1].expr)); }
#line 1789 "source/parser.c"
    break;

  case 50:
#line 183 "source/grammar.bison"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1795 "source/parser.c"
    break;

  case 51:
#line 184 "source/grammar.bison"
                                       { (yyval.expr) = expr_create(EXPR_OR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1801 "source/parser.c"
    break;

  case 52:
#line 187 "source/grammar.bison"
                    { (yyval.expr) = (yyvsp[0].expr); }
#line 1807 "source/parser.c"
    break;

  case 53:
#line 188 "source/grammar.bison"
                                        { (yyval.expr) = expr_create(EXPR_AND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1813 "source/parser.c"
    break;

  case 54:
#line 191 "source/grammar.bison"
                   { (yyval.expr) = (yyvsp[0].expr); }
#line 1819 "source/parser.c"
    break;

  case 55:
#line 192 "source/grammar.bison"
                                    { (yyval.expr) = expr_create(EXPR_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1825 "source/parser.c"
    break;

  case 56:
#line 193 "source/grammar.bison"
                                     { (yyval.expr) = expr_create(EXPR_NEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1831 "source/parser.c"
    break;

  case 57:
#line 196 "source/grammar.bison"
                    { (yyval.expr) = (yyvsp[0].expr); }
#line 1837 "source/parser.c"
    break;

  case 58:
#line 197 "source/grammar.bison"
                                        { (yyval.expr) = expr_create(EXPR_LESS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1843 "source/parser.c"
    break;

  case 59:
#line 198 "source/grammar.bison"
                                         { (yyval.expr) = expr_create(EXPR_GREAT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1849 "source/parser.c"
    break;

  case 60:
#line 199 "source/grammar.bison"
                                       { (yyval.expr) = expr_create(EXPR_LEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1855 "source/parser.c"
    break;

  case 61:
#line 200 "source/grammar.bison"
                                       { (yyval.expr) = expr_create(EXPR_GEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1861 "source/parser.c"
    break;

  case 62:
#line 203 "source/grammar.bison"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1867 "source/parser.c"
    break;

  case 63:
#line 204 "source/grammar.bison"
                                        { (yyval.expr) = expr_create(EXPR_ADD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1873 "source/parser.c"
    break;

  case 64:
#line 205 "source/grammar.bison"
                                        { (yyval.expr) = expr_create(EXPR_SUB, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1879 "source/parser.c"
    break;

  case 65:
#line 208 "source/grammar.bison"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1885 "source/parser.c"
    break;

  case 66:
#line 209 "source/grammar.bison"
                                          { (yyval.expr) = expr_create(EXPR_MULT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1891 "source/parser.c"
    break;

  case 67:
#line 210 "source/grammar.bison"
                                         { (yyval.expr) = expr_create(EXPR_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1897 "source/parser.c"
    break;

  case 68:
#line 211 "source/grammar.bison"
                                         { (yyval.expr) = expr_create(EXPR_MOD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1903 "source/parser.c"
    break;

  case 69:
#line 214 "source/grammar.bison"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 1909 "source/parser.c"
    break;

  case 70:
#line 215 "source/grammar.bison"
                                         { (yyval.expr) = expr_create(EXPR_EXP, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1915 "source/parser.c"
    break;


#line 1919 "source/parser.c"

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
#line 306 "source/grammar.bison"

int yyerror(const char* str) {
  fflush(stdout);
  if (!eof) { sprintf(error_text, "%s", str); eof = 0; }
  return 0;
}

/* prints error messages that are NOT eof */
void print_error_message(void) { fprintf(stderr, "%s\n", error_text); }

bool bool_convert(char* s) { return (!strcmp(s, "true")); }

void name_clean(char* s) {
  int i, j; char new_str[strlen(s)];
  for (i = 0; isspace(s[i]); i++) {} // skip whitespace
  for (j = 0; isalnum(s[i]) || s[i] == '_'; i++, j++) new_str[j] = s[i];
  new_str[j] = '\0';
  strcpy(clean_name, new_str);
}
  
