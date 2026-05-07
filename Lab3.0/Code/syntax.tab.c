/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "./syntax.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Forward declarations: actual AST types/implementations live in main.c */
/* Define Node type so actions can access node fields directly. This duplicates
   the definition in main.c but keeps the grammar self-contained. */
typedef struct Node {
    char* name;
    int line;
    int is_terminal;      /* 1: 终结符, 0: 非终结符 */
    union {
        struct {
            struct Node** children;
            int num_children;
        } nonterm;
        struct {
            char* value;   /* ID, STRING */
            int int_val;
            float float_val;
        } term;
    } u;
} Node;

extern Node* root; /* parser will set this when building AST (defined in main.c) */
extern int has_syntax_error; /* set by yyerror */
extern int has_error; /* set by lexer or parser on any error */

/* strdup may be missing from strict C declarations; declare it to avoid warnings */
extern char* strdup(const char*);

extern int yylineno;
void yyerror(const char* msg);
extern int yylex();

/* Prototypes for AST helpers implemented in main.c */
Node* create_node(const char* name, int line, int num_children, ...);
Node* create_terminal_node(const char* name, int line);
Node* create_id_node(const char* value, int line);
Node* create_int_node(int val, int line);
Node* create_float_node(float val, int line);
Node* create_string_node(const char* value, int line);
void print_tree(Node* node, int depth);
void free_node(Node* node);


#line 120 "./syntax.tab.c"

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

#include "syntax.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_AUTO = 3,                       /* AUTO  */
  YYSYMBOL_BREAK = 4,                      /* BREAK  */
  YYSYMBOL_CASE = 5,                       /* CASE  */
  YYSYMBOL_CHAR = 6,                       /* CHAR  */
  YYSYMBOL_CONST = 7,                      /* CONST  */
  YYSYMBOL_CONTINUE = 8,                   /* CONTINUE  */
  YYSYMBOL_DEFAULT = 9,                    /* DEFAULT  */
  YYSYMBOL_DO = 10,                        /* DO  */
  YYSYMBOL_DOUBLE = 11,                    /* DOUBLE  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_ENUM = 13,                      /* ENUM  */
  YYSYMBOL_EXTERN = 14,                    /* EXTERN  */
  YYSYMBOL_FLOAT = 15,                     /* FLOAT  */
  YYSYMBOL_FOR = 16,                       /* FOR  */
  YYSYMBOL_GOTO = 17,                      /* GOTO  */
  YYSYMBOL_IF = 18,                        /* IF  */
  YYSYMBOL_INT = 19,                       /* INT  */
  YYSYMBOL_LONG = 20,                      /* LONG  */
  YYSYMBOL_REGISTER = 21,                  /* REGISTER  */
  YYSYMBOL_RETURN = 22,                    /* RETURN  */
  YYSYMBOL_SHORT = 23,                     /* SHORT  */
  YYSYMBOL_SIGNED = 24,                    /* SIGNED  */
  YYSYMBOL_SIZEOF = 25,                    /* SIZEOF  */
  YYSYMBOL_STATIC = 26,                    /* STATIC  */
  YYSYMBOL_STRUCT = 27,                    /* STRUCT  */
  YYSYMBOL_SWITCH = 28,                    /* SWITCH  */
  YYSYMBOL_TYPEDEF = 29,                   /* TYPEDEF  */
  YYSYMBOL_UNION = 30,                     /* UNION  */
  YYSYMBOL_UNSIGNED = 31,                  /* UNSIGNED  */
  YYSYMBOL_VOID = 32,                      /* VOID  */
  YYSYMBOL_VOLATILE = 33,                  /* VOLATILE  */
  YYSYMBOL_WHILE = 34,                     /* WHILE  */
  YYSYMBOL_ID = 35,                        /* ID  */
  YYSYMBOL_INT_CONST = 36,                 /* INT_CONST  */
  YYSYMBOL_FLOAT_CONST = 37,               /* FLOAT_CONST  */
  YYSYMBOL_STRING = 38,                    /* STRING  */
  YYSYMBOL_PLUS = 39,                      /* PLUS  */
  YYSYMBOL_MINUS = 40,                     /* MINUS  */
  YYSYMBOL_STAR = 41,                      /* STAR  */
  YYSYMBOL_DIV = 42,                       /* DIV  */
  YYSYMBOL_MOD = 43,                       /* MOD  */
  YYSYMBOL_LT = 44,                        /* LT  */
  YYSYMBOL_LE = 45,                        /* LE  */
  YYSYMBOL_GT = 46,                        /* GT  */
  YYSYMBOL_GE = 47,                        /* GE  */
  YYSYMBOL_EQ = 48,                        /* EQ  */
  YYSYMBOL_NE = 49,                        /* NE  */
  YYSYMBOL_AND = 50,                       /* AND  */
  YYSYMBOL_OR = 51,                        /* OR  */
  YYSYMBOL_NOT = 52,                       /* NOT  */
  YYSYMBOL_ASSIGNOP = 53,                  /* ASSIGNOP  */
  YYSYMBOL_LP = 54,                        /* LP  */
  YYSYMBOL_RP = 55,                        /* RP  */
  YYSYMBOL_LB = 56,                        /* LB  */
  YYSYMBOL_RB = 57,                        /* RB  */
  YYSYMBOL_LC = 58,                        /* LC  */
  YYSYMBOL_RC = 59,                        /* RC  */
  YYSYMBOL_COMMA = 60,                     /* COMMA  */
  YYSYMBOL_SEMI = 61,                      /* SEMI  */
  YYSYMBOL_DOT = 62,                       /* DOT  */
  YYSYMBOL_LOWER_THAN_ELSE = 63,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 64,                  /* $accept  */
  YYSYMBOL_Program = 65,                   /* Program  */
  YYSYMBOL_ExtDefList = 66,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 67,                    /* ExtDef  */
  YYSYMBOL_ExtDecList = 68,                /* ExtDecList  */
  YYSYMBOL_Specifier = 69,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 70,           /* StructSpecifier  */
  YYSYMBOL_OptTag = 71,                    /* OptTag  */
  YYSYMBOL_Tag = 72,                       /* Tag  */
  YYSYMBOL_VarDec = 73,                    /* VarDec  */
  YYSYMBOL_FunDec = 74,                    /* FunDec  */
  YYSYMBOL_VarList = 75,                   /* VarList  */
  YYSYMBOL_ParamDec = 76,                  /* ParamDec  */
  YYSYMBOL_CompSt = 77,                    /* CompSt  */
  YYSYMBOL_StmtList = 78,                  /* StmtList  */
  YYSYMBOL_Stmt = 79,                      /* Stmt  */
  YYSYMBOL_MatchedStmt = 80,               /* MatchedStmt  */
  YYSYMBOL_UnmatchedStmt = 81,             /* UnmatchedStmt  */
  YYSYMBOL_DefList = 82,                   /* DefList  */
  YYSYMBOL_Def = 83,                       /* Def  */
  YYSYMBOL_DecList = 84,                   /* DecList  */
  YYSYMBOL_Dec = 85,                       /* Dec  */
  YYSYMBOL_Exp = 86,                       /* Exp  */
  YYSYMBOL_Args = 87                       /* Args  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   440

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  143

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   318


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    84,    84,    87,    88,    91,    92,    93,    94,    95,
      98,    99,   102,   103,   104,   107,   108,   111,   112,   115,
     118,   119,   122,   123,   126,   127,   130,   133,   136,   137,
     141,   142,   145,   146,   147,   148,   149,   150,   153,   154,
     156,   159,   160,   163,   166,   167,   170,   171,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   198,   199,   202,   203
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "AUTO", "BREAK",
  "CASE", "CHAR", "CONST", "CONTINUE", "DEFAULT", "DO", "DOUBLE", "ELSE",
  "ENUM", "EXTERN", "FLOAT", "FOR", "GOTO", "IF", "INT", "LONG",
  "REGISTER", "RETURN", "SHORT", "SIGNED", "SIZEOF", "STATIC", "STRUCT",
  "SWITCH", "TYPEDEF", "UNION", "UNSIGNED", "VOID", "VOLATILE", "WHILE",
  "ID", "INT_CONST", "FLOAT_CONST", "STRING", "PLUS", "MINUS", "STAR",
  "DIV", "MOD", "LT", "LE", "GT", "GE", "EQ", "NE", "AND", "OR", "NOT",
  "ASSIGNOP", "LP", "RP", "LB", "RB", "LC", "RC", "COMMA", "SEMI", "DOT",
  "LOWER_THAN_ELSE", "$accept", "Program", "ExtDefList", "ExtDef",
  "ExtDecList", "Specifier", "StructSpecifier", "OptTag", "Tag", "VarDec",
  "FunDec", "VarList", "ParamDec", "CompSt", "StmtList", "Stmt",
  "MatchedStmt", "UnmatchedStmt", "DefList", "Def", "DecList", "Dec",
  "Exp", "Args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-120)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-30)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      55,   -58,  -120,  -120,   -25,    18,  -120,    55,   -18,  -120,
    -120,   -31,   -12,  -120,  -120,  -120,    -1,  -120,    -4,   -35,
      15,    81,    -3,  -120,    22,    24,    81,  -120,  -120,    24,
      10,    81,  -120,    24,    28,    35,    45,  -120,  -120,    27,
      56,    42,    44,  -120,  -120,    54,  -120,    81,  -120,    57,
      66,   386,    67,    70,  -120,  -120,  -120,   386,   386,   386,
    -120,    63,    27,  -120,  -120,   145,   386,  -120,    24,  -120,
    -120,   386,   169,   386,   378,   -43,   -43,   193,  -120,  -120,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   129,  -120,    90,   312,  -120,   212,  -120,
     231,  -120,   250,    71,  -120,    36,    36,   -43,   -43,   112,
     112,   112,   112,   363,   363,   350,   331,   312,    72,   274,
    -120,    53,    79,   386,  -120,  -120,  -120,   -11,  -120,   116,
      78,  -120,  -120,   105,   105,   386,  -120,  -120,  -120,   293,
      79,   122,    79
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    13,    12,    18,     0,     2,     0,     0,    14,
       9,    19,     0,    16,     1,     3,    20,     6,     0,    10,
       0,    42,     0,     5,     0,     0,    42,     8,     7,     0,
       0,    42,    23,     0,     0,    25,     0,    20,    11,     0,
      46,     0,    44,    15,    41,    26,    22,     0,    21,     0,
       0,     0,     0,    64,    65,    66,    67,     0,     0,     0,
      33,     0,     0,    30,    31,     0,     0,    43,     0,    24,
      37,     0,     0,     0,     0,    62,    63,     0,    27,    28,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,     0,    47,    45,     0,    34,
       0,    69,    74,     0,    61,    57,    58,    59,    60,    51,
      52,    53,    54,    55,    56,    49,    50,    48,     0,     0,
      72,     0,     0,     0,    68,    71,    70,     0,    38,    30,
       0,    35,    73,     0,     0,     0,    40,    36,    39,     0,
       0,     0,     0
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -120,  -120,   128,  -120,   111,     4,  -120,  -120,  -120,   -24,
    -120,    91,  -120,   124,    84,  -119,   -74,    13,    68,  -120,
      80,  -120,   -51,    26
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     5,     6,     7,    18,    29,     9,    12,    13,    19,
      20,    34,    35,    60,    61,    62,    63,    64,    30,    31,
      41,    42,    65,   103
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      72,   133,   128,    10,     8,    40,    75,    76,    77,    45,
      11,     8,     2,    93,   136,    96,     3,    16,    14,    95,
      98,    24,   100,   102,     4,    25,    33,   -17,    49,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   119,    17,    40,    50,    21,   129,   131,    51,
      70,    33,    32,    22,   127,    -4,     1,    23,    36,    37,
     137,    52,    53,    54,    55,    56,   141,    57,   137,    43,
       2,    50,   102,    26,     3,    51,    27,    82,    83,    58,
      49,    59,     4,    46,   139,    26,   -29,    52,    53,    54,
      55,    56,    93,    57,    39,    47,     2,   130,    95,    44,
       3,    51,    48,    67,    68,    58,    49,    59,     4,    66,
      24,    26,    24,    52,    53,    54,    55,    56,    70,    57,
      71,    73,    78,    50,    74,   120,   124,    51,   134,   125,
     118,    58,   135,    59,   142,    15,    38,    26,    69,    52,
      53,    54,    55,    56,    28,    57,    79,   138,    97,   132,
       0,    80,    81,    82,    83,     0,     0,    58,     0,    59,
       0,     0,     0,    26,    53,    54,    55,    56,    93,    57,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,    58,     0,    59,    80,    81,    82,    83,     0,    84,
      85,    86,    87,    88,    89,    90,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,    94,    95,    80,    81,
      82,    83,     0,    84,    85,    86,    87,    88,    89,    90,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
      99,    95,    80,    81,    82,    83,     0,    84,    85,    86,
      87,    88,    89,    90,    91,     0,    92,     0,   104,    93,
       0,    80,    81,    82,    83,    95,    84,    85,    86,    87,
      88,    89,    90,    91,     0,    92,     0,   121,    93,     0,
      80,    81,    82,    83,    95,    84,    85,    86,    87,    88,
      89,    90,    91,     0,    92,     0,   122,    93,     0,    80,
      81,    82,    83,    95,    84,    85,    86,    87,    88,    89,
      90,    91,     0,    92,     0,     0,    93,     0,     0,     0,
     123,     0,    95,    80,    81,    82,    83,     0,    84,    85,
      86,    87,    88,    89,    90,    91,     0,    92,     0,     0,
      93,   126,    80,    81,    82,    83,    95,    84,    85,    86,
      87,    88,    89,    90,    91,     0,    92,     0,   140,    93,
       0,    80,    81,    82,    83,    95,    84,    85,    86,    87,
      88,    89,    90,    91,     0,    92,     0,     0,    93,     0,
      80,    81,    82,    83,    95,    84,    85,    86,    87,    88,
      89,    90,     0,     0,     0,     0,     0,    93,     0,    80,
      81,    82,    83,    95,    84,    85,    86,    87,    88,    89,
       0,     0,    80,    81,    82,    83,    93,    84,    85,    86,
      87,     0,    95,    53,    54,    55,    56,     0,    57,    93,
       0,    53,    54,    55,    56,    95,    57,     0,     0,     0,
      58,     0,    59,   101,     0,     0,     0,     0,    58,     0,
      59
};

static const yytype_int16 yycheck[] =
{
      51,    12,   121,    61,     0,    29,    57,    58,    59,    33,
      35,     7,    15,    56,   133,    66,    19,    35,     0,    62,
      71,    56,    73,    74,    27,    60,    22,    58,     1,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    61,    68,    18,    58,   121,   122,    22,
      61,    47,    55,    54,     1,     0,     1,    61,    36,    35,
     134,    34,    35,    36,    37,    38,   140,    40,   142,    59,
      15,    18,   123,    58,    19,    22,    61,    41,    42,    52,
       1,    54,    27,    55,   135,    58,    59,    34,    35,    36,
      37,    38,    56,    40,    26,    60,    15,    18,    62,    31,
      19,    22,    57,    61,    60,    52,     1,    54,    27,    53,
      56,    58,    56,    34,    35,    36,    37,    38,    61,    40,
      54,    54,    59,    18,    54,    35,    55,    22,    12,    57,
       1,    52,    54,    54,    12,     7,    25,    58,    47,    34,
      35,    36,    37,    38,    20,    40,    62,   134,    68,   123,
      -1,    39,    40,    41,    42,    -1,    -1,    52,    -1,    54,
      -1,    -1,    -1,    58,    35,    36,    37,    38,    56,    40,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    39,    40,    41,    42,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    -1,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    61,    62,    39,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      61,    62,    39,    40,    41,    42,    -1,    44,    45,    46,
      47,    48,    49,    50,    51,    -1,    53,    -1,    55,    56,
      -1,    39,    40,    41,    42,    62,    44,    45,    46,    47,
      48,    49,    50,    51,    -1,    53,    -1,    55,    56,    -1,
      39,    40,    41,    42,    62,    44,    45,    46,    47,    48,
      49,    50,    51,    -1,    53,    -1,    55,    56,    -1,    39,
      40,    41,    42,    62,    44,    45,    46,    47,    48,    49,
      50,    51,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,
      60,    -1,    62,    39,    40,    41,    42,    -1,    44,    45,
      46,    47,    48,    49,    50,    51,    -1,    53,    -1,    -1,
      56,    57,    39,    40,    41,    42,    62,    44,    45,    46,
      47,    48,    49,    50,    51,    -1,    53,    -1,    55,    56,
      -1,    39,    40,    41,    42,    62,    44,    45,    46,    47,
      48,    49,    50,    51,    -1,    53,    -1,    -1,    56,    -1,
      39,    40,    41,    42,    62,    44,    45,    46,    47,    48,
      49,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    39,
      40,    41,    42,    62,    44,    45,    46,    47,    48,    49,
      -1,    -1,    39,    40,    41,    42,    56,    44,    45,    46,
      47,    -1,    62,    35,    36,    37,    38,    -1,    40,    56,
      -1,    35,    36,    37,    38,    62,    40,    -1,    -1,    -1,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    52,    -1,
      54
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    15,    19,    27,    65,    66,    67,    69,    70,
      61,    35,    71,    72,     0,    66,    35,    61,    68,    73,
      74,    58,    54,    61,    56,    60,    58,    61,    77,    69,
      82,    83,    55,    69,    75,    76,    36,    35,    68,    82,
      73,    84,    85,    59,    82,    73,    55,    60,    57,     1,
      18,    22,    34,    35,    36,    37,    38,    40,    52,    54,
      77,    78,    79,    80,    81,    86,    53,    61,    60,    75,
      61,    54,    86,    54,    54,    86,    86,    86,    59,    78,
      39,    40,    41,    42,    44,    45,    46,    47,    48,    49,
      50,    51,    53,    56,    61,    62,    86,    84,    86,    61,
      86,    55,    86,    87,    55,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,     1,    86,
      35,    55,    55,    60,    55,    57,    57,     1,    79,    80,
      18,    80,    87,    12,    12,    54,    79,    80,    81,    86,
      55,    80,    12
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    64,    65,    66,    66,    67,    67,    67,    67,    67,
      68,    68,    69,    69,    69,    70,    70,    71,    71,    72,
      73,    73,    74,    74,    75,    75,    76,    77,    78,    78,
      79,    79,    80,    80,    80,    80,    80,    80,    81,    81,
      81,    82,    82,    83,    84,    84,    85,    85,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    87,    87
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     3,     2,
       1,     3,     1,     1,     1,     5,     2,     1,     0,     1,
       1,     4,     4,     3,     3,     1,     2,     4,     2,     0,
       1,     1,     2,     1,     3,     5,     7,     2,     5,     7,
       7,     2,     0,     3,     1,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     1,     1,     1,     1,     4,     3,
       4,     4,     3,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
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
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif

#ifndef yytnamerr
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
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
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
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
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


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: ExtDefList  */
#line 84 "./syntax.y"
                                             { (yyval.node) = create_node("Program", (yyloc).first_line, 1, (yyvsp[0].node)); root = (yyval.node); }
#line 1739 "./syntax.tab.c"
    break;

  case 3: /* ExtDefList: ExtDef ExtDefList  */
#line 87 "./syntax.y"
                                             { (yyval.node) = create_node("ExtDefList", (yyloc).first_line, 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1745 "./syntax.tab.c"
    break;

  case 4: /* ExtDefList: %empty  */
#line 88 "./syntax.y"
                                             { (yyval.node) = NULL; }
#line 1751 "./syntax.tab.c"
    break;

  case 5: /* ExtDef: Specifier ExtDecList SEMI  */
#line 91 "./syntax.y"
                                             { (yyval.node) = create_node("ExtDef", (yyloc).first_line, 3, (yyvsp[-2].node), (yyvsp[-1].node), create_terminal_node("SEMI", (yylsp[0]).first_line)); }
#line 1757 "./syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 92 "./syntax.y"
                                             { (yyval.node) = create_node("ExtDef", (yyloc).first_line, 2, (yyvsp[-1].node), create_terminal_node("SEMI", (yylsp[0]).first_line)); }
#line 1763 "./syntax.tab.c"
    break;

  case 7: /* ExtDef: Specifier FunDec CompSt  */
#line 93 "./syntax.y"
                                             { (yyval.node) = create_node("ExtDef", (yyloc).first_line, 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1769 "./syntax.tab.c"
    break;

  case 8: /* ExtDef: Specifier FunDec SEMI  */
#line 94 "./syntax.y"
                                             { (yyval.node) = create_node("ExtDef", (yyloc).first_line, 3, (yyvsp[-2].node), (yyvsp[-1].node), create_terminal_node("SEMI", (yylsp[0]).first_line)); }
#line 1775 "./syntax.tab.c"
    break;

  case 9: /* ExtDef: error SEMI  */
#line 95 "./syntax.y"
                                             { yyerrok; (yyval.node) = NULL; }
#line 1781 "./syntax.tab.c"
    break;

  case 10: /* ExtDecList: VarDec  */
#line 98 "./syntax.y"
                                             { (yyval.node) = create_node("ExtDecList", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 1787 "./syntax.tab.c"
    break;

  case 11: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 99 "./syntax.y"
                                             { (yyval.node) = create_node("ExtDecList", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("COMMA", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 1793 "./syntax.tab.c"
    break;

  case 12: /* Specifier: INT  */
#line 102 "./syntax.y"
                                              { Node* t = create_terminal_node("TYPE", (yylsp[0]).first_line); t->u.term.value = strdup("int"); (yyval.node) = create_node("Specifier", (yylsp[0]).first_line, 1, t); }
#line 1799 "./syntax.tab.c"
    break;

  case 13: /* Specifier: FLOAT  */
#line 103 "./syntax.y"
                                              { Node* t = create_terminal_node("TYPE", (yylsp[0]).first_line); t->u.term.value = strdup("float"); (yyval.node) = create_node("Specifier", (yylsp[0]).first_line, 1, t); }
#line 1805 "./syntax.tab.c"
    break;

  case 14: /* Specifier: StructSpecifier  */
#line 104 "./syntax.y"
                                             { (yyval.node) = create_node("Specifier", (yyloc).first_line, 1, (yyvsp[0].node)); }
#line 1811 "./syntax.tab.c"
    break;

  case 15: /* StructSpecifier: STRUCT OptTag LC DefList RC  */
#line 107 "./syntax.y"
                                              { (yyval.node) = create_node("StructSpecifier", (yyloc).first_line, 5, create_terminal_node("STRUCT", (yylsp[-4]).first_line), (yyvsp[-3].node), create_terminal_node("LC", (yylsp[-2]).first_line), (yyvsp[-1].node), create_terminal_node("RC", (yylsp[0]).first_line)); }
#line 1817 "./syntax.tab.c"
    break;

  case 16: /* StructSpecifier: STRUCT Tag  */
#line 108 "./syntax.y"
                                             { (yyval.node) = create_node("StructSpecifier", (yyloc).first_line, 2, create_terminal_node("STRUCT", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 1823 "./syntax.tab.c"
    break;

  case 17: /* OptTag: ID  */
#line 111 "./syntax.y"
                                             { (yyval.node) = create_node("OptTag", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 1829 "./syntax.tab.c"
    break;

  case 18: /* OptTag: %empty  */
#line 112 "./syntax.y"
                                             { (yyval.node) = NULL; }
#line 1835 "./syntax.tab.c"
    break;

  case 19: /* Tag: ID  */
#line 115 "./syntax.y"
                                             { (yyval.node) = create_node("Tag", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 1841 "./syntax.tab.c"
    break;

  case 20: /* VarDec: ID  */
#line 118 "./syntax.y"
                                             { (yyval.node) = create_node("VarDec", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 1847 "./syntax.tab.c"
    break;

  case 21: /* VarDec: VarDec LB INT_CONST RB  */
#line 119 "./syntax.y"
                                             { (yyval.node) = create_node("VarDec", (yyloc).first_line, 4, (yyvsp[-3].node), create_terminal_node("LB", (yylsp[-2]).first_line), (yyvsp[-1].node), create_terminal_node("RB", (yylsp[0]).first_line)); }
#line 1853 "./syntax.tab.c"
    break;

  case 22: /* FunDec: ID LP VarList RP  */
#line 122 "./syntax.y"
                                             { (yyval.node) = create_node("FunDec", (yyloc).first_line, 4, (yyvsp[-3].node), create_terminal_node("LP", (yylsp[-2]).first_line), (yyvsp[-1].node), create_terminal_node("RP", (yylsp[0]).first_line)); }
#line 1859 "./syntax.tab.c"
    break;

  case 23: /* FunDec: ID LP RP  */
#line 123 "./syntax.y"
                                             { (yyval.node) = create_node("FunDec", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("LP", (yylsp[-1]).first_line), create_terminal_node("RP", (yylsp[0]).first_line)); }
#line 1865 "./syntax.tab.c"
    break;

  case 24: /* VarList: ParamDec COMMA VarList  */
#line 126 "./syntax.y"
                                             { (yyval.node) = create_node("VarList", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("COMMA", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 1871 "./syntax.tab.c"
    break;

  case 25: /* VarList: ParamDec  */
#line 127 "./syntax.y"
                                             { (yyval.node) = create_node("VarList", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 1877 "./syntax.tab.c"
    break;

  case 26: /* ParamDec: Specifier VarDec  */
#line 130 "./syntax.y"
                                             { (yyval.node) = create_node("ParamDec", (yyloc).first_line, 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1883 "./syntax.tab.c"
    break;

  case 27: /* CompSt: LC DefList StmtList RC  */
#line 133 "./syntax.y"
                                             { (yyval.node) = create_node("CompSt", (yyloc).first_line, 4, create_terminal_node("LC", (yylsp[-3]).first_line), (yyvsp[-2].node), (yyvsp[-1].node), create_terminal_node("RC", (yylsp[0]).first_line)); }
#line 1889 "./syntax.tab.c"
    break;

  case 28: /* StmtList: Stmt StmtList  */
#line 136 "./syntax.y"
                                             { (yyval.node) = create_node("StmtList", (yyloc).first_line, 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1895 "./syntax.tab.c"
    break;

  case 29: /* StmtList: %empty  */
#line 137 "./syntax.y"
                                             { (yyval.node) = NULL; }
#line 1901 "./syntax.tab.c"
    break;

  case 32: /* MatchedStmt: Exp SEMI  */
#line 145 "./syntax.y"
                                             { (yyval.node) = create_node("Stmt", (yyloc).first_line, 2, (yyvsp[-1].node), create_terminal_node("SEMI", (yylsp[0]).first_line)); }
#line 1907 "./syntax.tab.c"
    break;

  case 33: /* MatchedStmt: CompSt  */
#line 146 "./syntax.y"
                                             { (yyval.node) = create_node("Stmt", (yyloc).first_line, 1, (yyvsp[0].node)); }
#line 1913 "./syntax.tab.c"
    break;

  case 34: /* MatchedStmt: RETURN Exp SEMI  */
#line 147 "./syntax.y"
                                             { (yyval.node) = create_node("Stmt", (yyloc).first_line, 3, create_terminal_node("RETURN", (yylsp[-2]).first_line), (yyvsp[-1].node), create_terminal_node("SEMI", (yylsp[0]).first_line)); }
#line 1919 "./syntax.tab.c"
    break;

  case 35: /* MatchedStmt: WHILE LP Exp RP MatchedStmt  */
#line 148 "./syntax.y"
                                                    { (yyval.node) = create_node("Stmt", (yyloc).first_line, 5, create_terminal_node("WHILE", (yylsp[-4]).first_line), create_terminal_node("LP", (yylsp[-3]).first_line), (yyvsp[-2].node), create_terminal_node("RP", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 1925 "./syntax.tab.c"
    break;

  case 36: /* MatchedStmt: IF LP Exp RP MatchedStmt ELSE MatchedStmt  */
#line 149 "./syntax.y"
                                                            { (yyval.node) = create_node("Stmt", (yyloc).first_line, 7, create_terminal_node("IF", (yylsp[-6]).first_line), create_terminal_node("LP", (yylsp[-5]).first_line), (yyvsp[-4].node), create_terminal_node("RP", (yylsp[-3]).first_line), (yyvsp[-2].node), create_terminal_node("ELSE", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 1931 "./syntax.tab.c"
    break;

  case 37: /* MatchedStmt: error SEMI  */
#line 150 "./syntax.y"
                                             { yyerrok; (yyval.node) = NULL; }
#line 1937 "./syntax.tab.c"
    break;

  case 38: /* UnmatchedStmt: IF LP Exp RP Stmt  */
#line 153 "./syntax.y"
                                                          { (yyval.node) = create_node("Stmt", (yyloc).first_line, 5, create_terminal_node("IF", (yylsp[-4]).first_line), create_terminal_node("LP", (yylsp[-3]).first_line), (yyvsp[-2].node), create_terminal_node("RP", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 1943 "./syntax.tab.c"
    break;

  case 39: /* UnmatchedStmt: IF LP Exp RP MatchedStmt ELSE UnmatchedStmt  */
#line 154 "./syntax.y"
                                                              { (yyval.node) = create_node("Stmt", (yyloc).first_line, 7, create_terminal_node("IF", (yylsp[-6]).first_line), create_terminal_node("LP", (yylsp[-5]).first_line), (yyvsp[-4].node), create_terminal_node("RP", (yylsp[-3]).first_line), (yyvsp[-2].node), create_terminal_node("ELSE", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 1949 "./syntax.tab.c"
    break;

  case 40: /* UnmatchedStmt: IF LP Exp RP error ELSE Stmt  */
#line 156 "./syntax.y"
                                               { has_error = 1; has_syntax_error = 1; printf("Error type B at Line %d: Missing \";\".\n", yylineno); yyerrok; (yyval.node) = NULL; }
#line 1955 "./syntax.tab.c"
    break;

  case 41: /* DefList: Def DefList  */
#line 159 "./syntax.y"
                                             { (yyval.node) = create_node("DefList", (yyloc).first_line, 2, (yyvsp[-1].node), (yyvsp[0].node)); }
#line 1961 "./syntax.tab.c"
    break;

  case 42: /* DefList: %empty  */
#line 160 "./syntax.y"
                                             { (yyval.node) = NULL; }
#line 1967 "./syntax.tab.c"
    break;

  case 43: /* Def: Specifier DecList SEMI  */
#line 163 "./syntax.y"
                                             { (yyval.node) = create_node("Def", (yyloc).first_line, 3, (yyvsp[-2].node), (yyvsp[-1].node), create_terminal_node("SEMI", (yylsp[0]).first_line)); }
#line 1973 "./syntax.tab.c"
    break;

  case 44: /* DecList: Dec  */
#line 166 "./syntax.y"
                                             { (yyval.node) = create_node("DecList", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 1979 "./syntax.tab.c"
    break;

  case 45: /* DecList: Dec COMMA DecList  */
#line 167 "./syntax.y"
                                             { (yyval.node) = create_node("DecList", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("COMMA", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 1985 "./syntax.tab.c"
    break;

  case 46: /* Dec: VarDec  */
#line 170 "./syntax.y"
                                             { (yyval.node) = create_node("Dec", (yyloc).first_line, 1, (yyvsp[0].node)); }
#line 1991 "./syntax.tab.c"
    break;

  case 47: /* Dec: VarDec ASSIGNOP Exp  */
#line 171 "./syntax.y"
                                               { (yyval.node) = create_node("Dec", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("ASSIGNOP", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 1997 "./syntax.tab.c"
    break;

  case 48: /* Exp: Exp ASSIGNOP Exp  */
#line 174 "./syntax.y"
                                               { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("ASSIGNOP", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2003 "./syntax.tab.c"
    break;

  case 49: /* Exp: Exp AND Exp  */
#line 175 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("AND", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2009 "./syntax.tab.c"
    break;

  case 50: /* Exp: Exp OR Exp  */
#line 176 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("OR", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2015 "./syntax.tab.c"
    break;

  case 51: /* Exp: Exp LT Exp  */
#line 177 "./syntax.y"
                                             { Node* op = create_terminal_node("LT", (yylsp[-1]).first_line); (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), op, (yyvsp[0].node)); }
#line 2021 "./syntax.tab.c"
    break;

  case 52: /* Exp: Exp LE Exp  */
#line 178 "./syntax.y"
                                             { Node* op = create_terminal_node("LE", (yylsp[-1]).first_line); (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), op, (yyvsp[0].node)); }
#line 2027 "./syntax.tab.c"
    break;

  case 53: /* Exp: Exp GT Exp  */
#line 179 "./syntax.y"
                                             { Node* op = create_terminal_node("GT", (yylsp[-1]).first_line); (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), op, (yyvsp[0].node)); }
#line 2033 "./syntax.tab.c"
    break;

  case 54: /* Exp: Exp GE Exp  */
#line 180 "./syntax.y"
                                             { Node* op = create_terminal_node("GE", (yylsp[-1]).first_line); (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), op, (yyvsp[0].node)); }
#line 2039 "./syntax.tab.c"
    break;

  case 55: /* Exp: Exp EQ Exp  */
#line 181 "./syntax.y"
                                             { Node* op = create_terminal_node("EQ", (yylsp[-1]).first_line); (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), op, (yyvsp[0].node)); }
#line 2045 "./syntax.tab.c"
    break;

  case 56: /* Exp: Exp NE Exp  */
#line 182 "./syntax.y"
                                             { Node* op = create_terminal_node("NE", (yylsp[-1]).first_line); (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), op, (yyvsp[0].node)); }
#line 2051 "./syntax.tab.c"
    break;

  case 57: /* Exp: Exp PLUS Exp  */
#line 183 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("PLUS", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2057 "./syntax.tab.c"
    break;

  case 58: /* Exp: Exp MINUS Exp  */
#line 184 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("MINUS", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2063 "./syntax.tab.c"
    break;

  case 59: /* Exp: Exp STAR Exp  */
#line 185 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("STAR", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2069 "./syntax.tab.c"
    break;

  case 60: /* Exp: Exp DIV Exp  */
#line 186 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("DIV", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2075 "./syntax.tab.c"
    break;

  case 61: /* Exp: LP Exp RP  */
#line 187 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, create_terminal_node("LP", (yylsp[-2]).first_line), (yyvsp[-1].node), create_terminal_node("RP", (yylsp[0]).first_line)); }
#line 2081 "./syntax.tab.c"
    break;

  case 62: /* Exp: MINUS Exp  */
#line 188 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 2, create_terminal_node("MINUS", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2087 "./syntax.tab.c"
    break;

  case 63: /* Exp: NOT Exp  */
#line 189 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 2, create_terminal_node("NOT", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2093 "./syntax.tab.c"
    break;

  case 64: /* Exp: ID  */
#line 190 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 2099 "./syntax.tab.c"
    break;

  case 65: /* Exp: INT_CONST  */
#line 191 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 2105 "./syntax.tab.c"
    break;

  case 66: /* Exp: FLOAT_CONST  */
#line 192 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 2111 "./syntax.tab.c"
    break;

  case 67: /* Exp: STRING  */
#line 193 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 2117 "./syntax.tab.c"
    break;

  case 68: /* Exp: ID LP Args RP  */
#line 194 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 4, (yyvsp[-3].node), create_terminal_node("LP", (yylsp[-2]).first_line), (yyvsp[-1].node), create_terminal_node("RP", (yylsp[0]).first_line)); }
#line 2123 "./syntax.tab.c"
    break;

  case 69: /* Exp: ID LP RP  */
#line 195 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("LP", (yylsp[-1]).first_line), create_terminal_node("RP", (yylsp[0]).first_line)); }
#line 2129 "./syntax.tab.c"
    break;

  case 70: /* Exp: Exp LB Exp RB  */
#line 196 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 4, (yyvsp[-3].node), create_terminal_node("LB", (yylsp[-2]).first_line), (yyvsp[-1].node), create_terminal_node("RB", (yylsp[0]).first_line)); }
#line 2135 "./syntax.tab.c"
    break;

  case 71: /* Exp: Exp LB error RB  */
#line 198 "./syntax.y"
                                            { has_error = 1; has_syntax_error = 1; printf("Error type B at Line %d: Missing \"]\".\n", yylineno); yyerrok; (yyval.node) = NULL; }
#line 2141 "./syntax.tab.c"
    break;

  case 72: /* Exp: Exp DOT ID  */
#line 199 "./syntax.y"
                                             { (yyval.node) = create_node("Exp", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("DOT", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2147 "./syntax.tab.c"
    break;

  case 73: /* Args: Exp COMMA Args  */
#line 202 "./syntax.y"
                                             { (yyval.node) = create_node("Args", (yyloc).first_line, 3, (yyvsp[-2].node), create_terminal_node("COMMA", (yylsp[-1]).first_line), (yyvsp[0].node)); }
#line 2153 "./syntax.tab.c"
    break;

  case 74: /* Args: Exp  */
#line 203 "./syntax.y"
                                             { (yyval.node) = create_node("Args", (yylsp[0]).first_line, 1, (yyvsp[0].node)); }
#line 2159 "./syntax.tab.c"
    break;


#line 2163 "./syntax.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 206 "./syntax.y"


void yyerror(const char* msg) {
    /* Record and print any bison-reported syntax error. */
    has_error = 1;
    has_syntax_error = 1;
    printf("Error type B at Line %d: %s.\n", yylineno, msg);
}
