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
#line 2 "./syntax.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// ------------------ 修复：添加Flex提供的外部声明 ------------------
extern int yylex(void);
extern int yylineno;
extern FILE* yyin;
extern void yyrestart(FILE* input_file);
// ------------------------------------------------------------------

// 错误声明
extern void print_error(char type, int line, const char* msg);

// 语法树节点最大子节点数
#define MAX_CHILDREN 32

// 语法树节点类型
typedef enum {
    NODE_NONTERMINAL, // 非终结符
    NODE_TYPE,         // TYPE终结符
    NODE_ID,           // ID终结符
    NODE_INT,          // INT终结符
    NODE_FLOAT,        // FLOAT终结符
    NODE_TOKEN         // 其他终结符
} NodeType;

// 语法树节点结构体
typedef struct Node {
    NodeType type;
    char name[64];     // 节点名称
    int line;          // 行号
    union {
        int int_val;
        float float_val;
        char* str_val;
    } attr;
    int child_num;
    struct Node* children[MAX_CHILDREN];
} Node;

// 全局根节点
Node* root = NULL;

// 创建语法树节点
Node* create_node(NodeType type, const char* name, int line) {
    Node* node = (Node*)malloc(sizeof(Node));
    memset(node, 0, sizeof(Node));
    node->type = type;
    strncpy(node->name, name, sizeof(node->name)-1);
    node->line = line;
    node->child_num = 0;
    return node;
}

// 为节点添加子节点
void add_child(Node* parent, Node* child) {
    if (!parent || !child) return;
    if (parent->child_num >= MAX_CHILDREN) return;
    parent->children[parent->child_num++] = child;
}

// 递归打印语法树（先序遍历，缩进控制）
void print_tree(Node* node, int indent) {
    if (!node) return;

    // 跳过产生ε的非终结符
    if (node->type == NODE_NONTERMINAL && node->child_num == 0) {
        return;
    }

    // 打印缩进
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    // 按类型打印节点信息
    switch (node->type) {
        case NODE_NONTERMINAL:
            printf("%s (%d)\n", node->name, node->line);
            break;
        case NODE_TYPE:
            printf("TYPE: %s\n", node->attr.str_val);
            break;
        case NODE_ID:
            printf("ID: %s\n", node->attr.str_val);
            break;
        case NODE_INT:
            printf("INT: %d\n", node->attr.int_val);
            break;
        case NODE_FLOAT:
            printf("FLOAT: %.6f\n", node->attr.float_val);
            break;
        case NODE_TOKEN:
            printf("%s\n", node->name);
            break;
    }

    // 递归打印子节点
    for (int i = 0; i < node->child_num; i++) {
        print_tree(node->children[i], indent + 1);
    }
}

// 释放语法树内存
void free_tree(Node* node) {
    if (!node) return;
    for (int i = 0; i < node->child_num; i++) {
        free_tree(node->children[i]);
    }
    if (node->type == NODE_TYPE || node->type == NODE_ID) {
        free(node->attr.str_val);
    }
    free(node);
}

// yyerror函数，语法错误处理
void yyerror(const char* msg) {
    print_error('B', yylineno, msg);
}

#line 195 "./syntax.tab.c"

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
  YYSYMBOL_TYPE = 3,                       /* TYPE  */
  YYSYMBOL_ID = 4,                         /* ID  */
  YYSYMBOL_RELOP = 5,                      /* RELOP  */
  YYSYMBOL_INT = 6,                        /* INT  */
  YYSYMBOL_FLOAT = 7,                      /* FLOAT  */
  YYSYMBOL_STRUCT = 8,                     /* STRUCT  */
  YYSYMBOL_IF = 9,                         /* IF  */
  YYSYMBOL_ELSE = 10,                      /* ELSE  */
  YYSYMBOL_WHILE = 11,                     /* WHILE  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_VOID = 13,                      /* VOID  */
  YYSYMBOL_LP = 14,                        /* LP  */
  YYSYMBOL_RP = 15,                        /* RP  */
  YYSYMBOL_LC = 16,                        /* LC  */
  YYSYMBOL_RC = 17,                        /* RC  */
  YYSYMBOL_LB = 18,                        /* LB  */
  YYSYMBOL_RB = 19,                        /* RB  */
  YYSYMBOL_SEMI = 20,                      /* SEMI  */
  YYSYMBOL_COMMA = 21,                     /* COMMA  */
  YYSYMBOL_DOT = 22,                       /* DOT  */
  YYSYMBOL_ASSIGNOP = 23,                  /* ASSIGNOP  */
  YYSYMBOL_AND = 24,                       /* AND  */
  YYSYMBOL_OR = 25,                        /* OR  */
  YYSYMBOL_NOT = 26,                       /* NOT  */
  YYSYMBOL_PLUS = 27,                      /* PLUS  */
  YYSYMBOL_MINUS = 28,                     /* MINUS  */
  YYSYMBOL_STAR = 29,                      /* STAR  */
  YYSYMBOL_DIV = 30,                       /* DIV  */
  YYSYMBOL_UMINUS = 31,                    /* UMINUS  */
  YYSYMBOL_LOWER_THAN_ELSE = 32,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 33,                  /* $accept  */
  YYSYMBOL_Program = 34,                   /* Program  */
  YYSYMBOL_ExtDefList = 35,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 36,                    /* ExtDef  */
  YYSYMBOL_Specifier = 37,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 38,           /* StructSpecifier  */
  YYSYMBOL_OptTag = 39,                    /* OptTag  */
  YYSYMBOL_Tag = 40,                       /* Tag  */
  YYSYMBOL_FunDec = 41,                    /* FunDec  */
  YYSYMBOL_VarList = 42,                   /* VarList  */
  YYSYMBOL_ParamDec = 43,                  /* ParamDec  */
  YYSYMBOL_CompSt = 44,                    /* CompSt  */
  YYSYMBOL_DefList = 45,                   /* DefList  */
  YYSYMBOL_Def = 46,                       /* Def  */
  YYSYMBOL_DecList = 47,                   /* DecList  */
  YYSYMBOL_Dec = 48,                       /* Dec  */
  YYSYMBOL_VarDec = 49,                    /* VarDec  */
  YYSYMBOL_StmtList = 50,                  /* StmtList  */
  YYSYMBOL_Stmt = 51,                      /* Stmt  */
  YYSYMBOL_Exp = 52,                       /* Exp  */
  YYSYMBOL_Args = 53                       /* Args  */
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
typedef yytype_int8 yy_state_t;

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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   287

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  124

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   287


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
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   161,   161,   173,   178,   183,   189,   194,   200,   203,
     209,   215,   223,   230,   236,   241,   249,   258,   266,   269,
     275,   281,   288,   295,   298,   303,   308,   314,   317,   323,
     329,   333,   341,   347,   356,   359,   364,   369,   374,   378,
     386,   396,   404,   410,   414,   417,   423,   429,   435,   443,
     449,   455,   461,   467,   473,   478,   483,   491,   500,   508,
     515,   521,   527,   533,   534,   537,   543
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TYPE", "ID", "RELOP",
  "INT", "FLOAT", "STRUCT", "IF", "ELSE", "WHILE", "RETURN", "VOID", "LP",
  "RP", "LC", "RC", "LB", "RB", "SEMI", "COMMA", "DOT", "ASSIGNOP", "AND",
  "OR", "NOT", "PLUS", "MINUS", "STAR", "DIV", "UMINUS", "LOWER_THAN_ELSE",
  "$accept", "Program", "ExtDefList", "ExtDef", "Specifier",
  "StructSpecifier", "OptTag", "Tag", "FunDec", "VarList", "ParamDec",
  "CompSt", "DefList", "Def", "DecList", "Dec", "VarDec", "StmtList",
  "Stmt", "Exp", "Args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-33)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-37)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     115,   -18,   -33,     0,     5,   -33,   115,    23,   -33,   -33,
      -1,    13,   -33,   -33,   -33,    64,    25,   -33,     9,    28,
      36,    -6,   137,   -33,   -33,    70,    48,    60,   -33,   -33,
      37,    69,   100,    50,    37,    74,    60,   -33,    37,    66,
      72,   -33,    83,    77,   -33,   -33,    86,   105,    94,   -33,
     -33,   100,   100,   100,   216,   -33,    90,   -33,   -33,    95,
     -33,    32,   102,   125,   127,   100,   -33,   -33,   132,    83,
     107,   -33,   -33,   -33,    30,   128,    80,    80,   100,   100,
     138,   100,   100,   100,   100,   100,   100,   100,   -33,   -33,
     -33,   100,   100,   142,   -33,   -33,   -33,   -33,   156,   144,
     -33,   257,   170,   -33,   216,   244,   230,   247,   247,    80,
      80,   186,   202,   -33,   100,   -33,   -33,     2,     2,   -33,
     153,   -33,     2,   -33
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     9,    14,     0,     2,     0,     0,    10,     8,
      15,     0,    12,     1,     3,     0,    32,     6,     0,     0,
      29,    30,     0,    18,    34,     0,     0,     0,     5,     7,
       0,     0,     0,     0,     0,     0,     0,    17,     0,     0,
      20,    23,     0,     0,    32,    28,     0,     0,    60,    61,
      62,     0,     0,     0,    31,    27,     0,    11,    24,    21,
      16,     0,     0,     0,     0,     0,    43,    38,     0,     0,
       0,    33,    63,    64,     0,     0,    55,    54,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26,    19,
      44,     0,     0,     0,    22,    35,    37,    58,    66,     0,
      53,    48,     0,    56,    45,    47,    46,    49,    50,    51,
      52,     0,     0,    42,     0,    57,    59,     0,     0,    65,
      39,    41,     0,    40
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -33,   -33,   162,   -33,     1,   -33,   -33,   -33,   -33,   112,
     -33,   158,    -4,   -33,   114,   -33,   149,   113,   101,   -32,
      76
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,    34,     8,    11,    12,    18,    39,
      40,    67,    35,    36,    19,    20,    21,    68,    69,    70,
      99
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      54,     7,     9,    62,    10,    13,    48,     7,    49,    50,
      26,    63,    31,    64,    65,   -13,    51,    32,    27,    75,
      76,    77,    66,    42,    15,    27,    38,    16,    52,    22,
      53,    47,    58,    93,    48,     2,    49,    50,    43,    25,
       3,    44,    98,    17,    51,    97,   101,   102,    29,   104,
     105,   106,   107,   108,   109,   110,    52,    30,    53,   111,
     112,    33,    38,     2,   -25,    41,   -25,   -25,     3,   -25,
      55,   -25,   -25,     2,   -25,    46,   -25,   -25,     3,    23,
     -25,    60,    98,    24,    62,    37,   -25,    48,   -25,    49,
      50,    57,    63,    61,    64,    65,    24,    51,    79,    27,
     -36,    47,    80,    66,    48,    71,    49,    50,    74,    52,
      88,    53,    78,    31,    51,    -4,     1,    72,     2,    41,
      72,    73,    90,     3,    73,    79,    52,    96,    53,    80,
      81,    82,    83,    78,    84,    85,    86,    87,    33,    91,
       2,    92,   103,   100,    45,     3,    79,    78,    56,    94,
      80,    81,    82,    83,   -25,    84,    85,    86,    87,   115,
      79,    78,   113,   122,    80,    81,    82,    83,    14,    84,
      85,    86,    87,    89,    79,    78,    28,   114,    80,    81,
      82,    83,    95,    84,    85,    86,    87,    59,    79,   116,
     119,    78,    80,    81,    82,    83,     0,    84,    85,    86,
      87,   117,     0,     0,    79,     0,     0,    78,    80,    81,
      82,    83,     0,    84,    85,    86,    87,   118,   120,   121,
      79,    78,     0,   123,    80,    81,    82,    83,     0,    84,
      85,    86,    87,     0,    79,    78,     0,     0,    80,    81,
      82,    83,     0,    84,    85,    86,    87,     0,    79,    78,
       0,     0,    80,     0,    82,     0,     0,    84,    85,    86,
      87,     0,    79,     0,     0,    79,    80,     0,     0,    80,
       0,    84,    85,    86,    87,    79,    86,    87,     0,    80,
       0,     0,     0,     0,    84,    85,    86,    87
};

static const yytype_int8 yycheck[] =
{
      32,     0,    20,     1,     4,     0,     4,     6,     6,     7,
       1,     9,    18,    11,    12,    16,    14,    23,    16,    51,
      52,    53,    20,    27,     1,    16,    25,     4,    26,    16,
      28,     1,    36,    65,     4,     3,     6,     7,     1,    14,
       8,     4,    74,    20,    14,    15,    78,    79,    20,    81,
      82,    83,    84,    85,    86,    87,    26,    21,    28,    91,
      92,     1,    61,     3,     4,    17,     6,     7,     8,     9,
      20,    11,    12,     3,    14,     6,    16,    17,     8,    15,
      20,    15,   114,    19,     1,    15,    26,     4,    28,     6,
       7,    17,     9,    21,    11,    12,    19,    14,    18,    16,
      17,     1,    22,    20,     4,    19,     6,     7,    14,    26,
      20,    28,     5,    18,    14,     0,     1,    15,     3,    17,
      15,    19,    20,     8,    19,    18,    26,    20,    28,    22,
      23,    24,    25,     5,    27,    28,    29,    30,     1,    14,
       3,    14,     4,    15,    30,     8,    18,     5,    34,    17,
      22,    23,    24,    25,    17,    27,    28,    29,    30,    15,
      18,     5,    20,    10,    22,    23,    24,    25,     6,    27,
      28,    29,    30,    61,    18,     5,    18,    21,    22,    23,
      24,    25,    69,    27,    28,    29,    30,    38,    18,    19,
     114,     5,    22,    23,    24,    25,    -1,    27,    28,    29,
      30,    15,    -1,    -1,    18,    -1,    -1,     5,    22,    23,
      24,    25,    -1,    27,    28,    29,    30,    15,   117,   118,
      18,     5,    -1,   122,    22,    23,    24,    25,    -1,    27,
      28,    29,    30,    -1,    18,     5,    -1,    -1,    22,    23,
      24,    25,    -1,    27,    28,    29,    30,    -1,    18,     5,
      -1,    -1,    22,    -1,    24,    -1,    -1,    27,    28,    29,
      30,    -1,    18,    -1,    -1,    18,    22,    -1,    -1,    22,
      -1,    27,    28,    29,    30,    18,    29,    30,    -1,    22,
      -1,    -1,    -1,    -1,    27,    28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,     8,    34,    35,    36,    37,    38,    20,
       4,    39,    40,     0,    35,     1,     4,    20,    41,    47,
      48,    49,    16,    15,    19,    14,     1,    16,    44,    20,
      21,    18,    23,     1,    37,    45,    46,    15,    37,    42,
      43,    17,    45,     1,     4,    47,     6,     1,     4,     6,
       7,    14,    26,    28,    52,    20,    47,    17,    45,    49,
      15,    21,     1,     9,    11,    12,    20,    44,    50,    51,
      52,    19,    15,    19,    14,    52,    52,    52,     5,    18,
      22,    23,    24,    25,    27,    28,    29,    30,    20,    42,
      20,    14,    14,    52,    17,    50,    20,    15,    52,    53,
      15,    52,    52,     4,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    20,    21,    15,    19,    15,    15,    53,
      51,    51,    10,    51
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    33,    34,    35,    35,    36,    36,    36,    36,    37,
      37,    38,    38,    39,    39,    40,    41,    41,    41,    42,
      42,    43,    44,    44,    45,    45,    46,    46,    47,    47,
      48,    48,    49,    49,    49,    50,    50,    51,    51,    51,
      51,    51,    51,    51,    51,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    53,    53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     2,     1,
       1,     5,     2,     1,     0,     1,     4,     3,     2,     3,
       1,     2,     4,     2,     2,     0,     3,     2,     3,     1,
       1,     3,     1,     4,     2,     2,     0,     2,     1,     5,
       7,     5,     3,     1,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     3,     4,     3,     4,
       1,     1,     1,     2,     2,     3,     1
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
#line 161 "./syntax.y"
                     {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "Program", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[0].node_val));
            root = (yyval.node_val);
            // 无错误时打印语法树
            if (yynerrs == 0) {
                print_tree(root, 0);
            }
            free_tree(root);
        }
#line 1469 "./syntax.tab.c"
    break;

  case 3: /* ExtDefList: ExtDef ExtDefList  */
#line 173 "./syntax.y"
                               {
                (yyval.node_val) = create_node(NODE_NONTERMINAL, "ExtDefList", (yyloc).first_line);
                add_child((yyval.node_val), (yyvsp[-1].node_val));
                add_child((yyval.node_val), (yyvsp[0].node_val));
            }
#line 1479 "./syntax.tab.c"
    break;

  case 4: /* ExtDefList: %empty  */
#line 178 "./syntax.y"
                       {
                (yyval.node_val) = create_node(NODE_NONTERMINAL, "ExtDefList", (yyloc).first_line);
            }
#line 1487 "./syntax.tab.c"
    break;

  case 5: /* ExtDef: Specifier FunDec CompSt  */
#line 183 "./syntax.y"
                                 {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "ExtDef", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[-2].node_val));
            add_child((yyval.node_val), (yyvsp[-1].node_val));
            add_child((yyval.node_val), (yyvsp[0].node_val));
        }
#line 1498 "./syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 189 "./syntax.y"
                         {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "ExtDef", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[-1].node_val));
            add_child((yyval.node_val), create_node(NODE_TOKEN, "SEMI", (yylsp[0]).first_line));
        }
#line 1508 "./syntax.tab.c"
    break;

  case 7: /* ExtDef: Specifier DecList SEMI  */
#line 194 "./syntax.y"
                                 {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "ExtDef", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[-2].node_val));
            add_child((yyval.node_val), (yyvsp[-1].node_val));
            add_child((yyval.node_val), create_node(NODE_TOKEN, "SEMI", (yylsp[0]).first_line));
        }
#line 1519 "./syntax.tab.c"
    break;

  case 8: /* ExtDef: error SEMI  */
#line 200 "./syntax.y"
                     { yyerrok; }
#line 1525 "./syntax.tab.c"
    break;

  case 9: /* Specifier: TYPE  */
#line 203 "./syntax.y"
                 {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "Specifier", (yyloc).first_line);
            Node* type_node = create_node(NODE_TYPE, "TYPE", (yylsp[0]).first_line);
            type_node->attr.str_val = (yyvsp[0].str_val);
            add_child((yyval.node_val), type_node);
        }
#line 1536 "./syntax.tab.c"
    break;

  case 10: /* Specifier: StructSpecifier  */
#line 209 "./syntax.y"
                          {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "Specifier", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[0].node_val));
        }
#line 1545 "./syntax.tab.c"
    break;

  case 11: /* StructSpecifier: STRUCT OptTag LC DefList RC  */
#line 215 "./syntax.y"
                                              {
                    (yyval.node_val) = create_node(NODE_NONTERMINAL, "StructSpecifier", (yyloc).first_line);
                    add_child((yyval.node_val), create_node(NODE_TOKEN, "STRUCT", (yylsp[-4]).first_line));
                    add_child((yyval.node_val), (yyvsp[-3].node_val));
                    add_child((yyval.node_val), create_node(NODE_TOKEN, "LC", (yylsp[-2]).first_line));
                    add_child((yyval.node_val), (yyvsp[-1].node_val));
                    add_child((yyval.node_val), create_node(NODE_TOKEN, "RC", (yylsp[0]).first_line));
                }
#line 1558 "./syntax.tab.c"
    break;

  case 12: /* StructSpecifier: STRUCT Tag  */
#line 223 "./syntax.y"
                             {
                    (yyval.node_val) = create_node(NODE_NONTERMINAL, "StructSpecifier", (yyloc).first_line);
                    add_child((yyval.node_val), create_node(NODE_TOKEN, "STRUCT", (yylsp[-1]).first_line));
                    add_child((yyval.node_val), (yyvsp[0].node_val));
                }
#line 1568 "./syntax.tab.c"
    break;

  case 13: /* OptTag: ID  */
#line 230 "./syntax.y"
            {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "OptTag", (yyloc).first_line);
            Node* id_node = create_node(NODE_ID, "ID", (yylsp[0]).first_line);
            id_node->attr.str_val = (yyvsp[0].str_val);
            add_child((yyval.node_val), id_node);
        }
#line 1579 "./syntax.tab.c"
    break;

  case 14: /* OptTag: %empty  */
#line 236 "./syntax.y"
                   {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "OptTag", (yyloc).first_line);
        }
#line 1587 "./syntax.tab.c"
    break;

  case 15: /* Tag: ID  */
#line 241 "./syntax.y"
         {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Tag", (yyloc).first_line);
        Node* id_node = create_node(NODE_ID, "ID", (yylsp[0]).first_line);
        id_node->attr.str_val = (yyvsp[0].str_val);
        add_child((yyval.node_val), id_node);
    }
#line 1598 "./syntax.tab.c"
    break;

  case 16: /* FunDec: ID LP VarList RP  */
#line 249 "./syntax.y"
                          {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "FunDec", (yyloc).first_line);
            Node* id_node = create_node(NODE_ID, "ID", (yylsp[-3]).first_line);
            id_node->attr.str_val = (yyvsp[-3].str_val);
            add_child((yyval.node_val), id_node);
            add_child((yyval.node_val), create_node(NODE_TOKEN, "LP", (yylsp[-2]).first_line));
            add_child((yyval.node_val), (yyvsp[-1].node_val));
            add_child((yyval.node_val), create_node(NODE_TOKEN, "RP", (yylsp[0]).first_line));
        }
#line 1612 "./syntax.tab.c"
    break;

  case 17: /* FunDec: ID LP RP  */
#line 258 "./syntax.y"
                   {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "FunDec", (yyloc).first_line);
            Node* id_node = create_node(NODE_ID, "ID", (yylsp[-2]).first_line);
            id_node->attr.str_val = (yyvsp[-2].str_val);
            add_child((yyval.node_val), id_node);
            add_child((yyval.node_val), create_node(NODE_TOKEN, "LP", (yylsp[-1]).first_line));
            add_child((yyval.node_val), create_node(NODE_TOKEN, "RP", (yylsp[0]).first_line));
        }
#line 1625 "./syntax.tab.c"
    break;

  case 18: /* FunDec: error RP  */
#line 266 "./syntax.y"
                   { yyerrok; }
#line 1631 "./syntax.tab.c"
    break;

  case 19: /* VarList: ParamDec COMMA VarList  */
#line 269 "./syntax.y"
                                 {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "VarList", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[-2].node_val));
            add_child((yyval.node_val), create_node(NODE_TOKEN, "COMMA", (yylsp[-1]).first_line));
            add_child((yyval.node_val), (yyvsp[0].node_val));
        }
#line 1642 "./syntax.tab.c"
    break;

  case 20: /* VarList: ParamDec  */
#line 275 "./syntax.y"
                   {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "VarList", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[0].node_val));
        }
#line 1651 "./syntax.tab.c"
    break;

  case 21: /* ParamDec: Specifier VarDec  */
#line 281 "./syntax.y"
                            {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "ParamDec", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[-1].node_val));
            add_child((yyval.node_val), (yyvsp[0].node_val));
        }
#line 1661 "./syntax.tab.c"
    break;

  case 22: /* CompSt: LC DefList StmtList RC  */
#line 288 "./syntax.y"
                                {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "CompSt", (yyloc).first_line);
            add_child((yyval.node_val), create_node(NODE_TOKEN, "LC", (yylsp[-3]).first_line));
            add_child((yyval.node_val), (yyvsp[-2].node_val));
            add_child((yyval.node_val), (yyvsp[-1].node_val));
            add_child((yyval.node_val), create_node(NODE_TOKEN, "RC", (yylsp[0]).first_line));
        }
#line 1673 "./syntax.tab.c"
    break;

  case 23: /* CompSt: error RC  */
#line 295 "./syntax.y"
                   { yyerrok; }
#line 1679 "./syntax.tab.c"
    break;

  case 24: /* DefList: Def DefList  */
#line 298 "./syntax.y"
                      {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "DefList", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[-1].node_val));
            add_child((yyval.node_val), (yyvsp[0].node_val));
        }
#line 1689 "./syntax.tab.c"
    break;

  case 25: /* DefList: %empty  */
#line 303 "./syntax.y"
                   {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "DefList", (yyloc).first_line);
        }
#line 1697 "./syntax.tab.c"
    break;

  case 26: /* Def: Specifier DecList SEMI  */
#line 308 "./syntax.y"
                             {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Def", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), (yyvsp[-1].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "SEMI", (yylsp[0]).first_line));
    }
#line 1708 "./syntax.tab.c"
    break;

  case 27: /* Def: error SEMI  */
#line 314 "./syntax.y"
                 { yyerrok; }
#line 1714 "./syntax.tab.c"
    break;

  case 28: /* DecList: Dec COMMA DecList  */
#line 317 "./syntax.y"
                            {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "DecList", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[-2].node_val));
            add_child((yyval.node_val), create_node(NODE_TOKEN, "COMMA", (yylsp[-1]).first_line));
            add_child((yyval.node_val), (yyvsp[0].node_val));
        }
#line 1725 "./syntax.tab.c"
    break;

  case 29: /* DecList: Dec  */
#line 323 "./syntax.y"
              {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "DecList", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[0].node_val));
        }
#line 1734 "./syntax.tab.c"
    break;

  case 30: /* Dec: VarDec  */
#line 329 "./syntax.y"
             {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Dec", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1743 "./syntax.tab.c"
    break;

  case 31: /* Dec: VarDec ASSIGNOP Exp  */
#line 333 "./syntax.y"
                          {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Dec", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "ASSIGNOP", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1754 "./syntax.tab.c"
    break;

  case 32: /* VarDec: ID  */
#line 341 "./syntax.y"
            {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "VarDec", (yyloc).first_line);
            Node* id_node = create_node(NODE_ID, "ID", (yylsp[0]).first_line);
            id_node->attr.str_val = (yyvsp[0].str_val);
            add_child((yyval.node_val), id_node);
        }
#line 1765 "./syntax.tab.c"
    break;

  case 33: /* VarDec: VarDec LB INT RB  */
#line 347 "./syntax.y"
                           {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "VarDec", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[-3].node_val));
            add_child((yyval.node_val), create_node(NODE_TOKEN, "LB", (yylsp[-2]).first_line));
            Node* int_node = create_node(NODE_INT, "INT", (yylsp[-1]).first_line);
            int_node->attr.int_val = (yyvsp[-1].int_val);
            add_child((yyval.node_val), int_node);
            add_child((yyval.node_val), create_node(NODE_TOKEN, "RB", (yylsp[0]).first_line));
        }
#line 1779 "./syntax.tab.c"
    break;

  case 34: /* VarDec: error RB  */
#line 356 "./syntax.y"
                   { yyerrok; }
#line 1785 "./syntax.tab.c"
    break;

  case 35: /* StmtList: Stmt StmtList  */
#line 359 "./syntax.y"
                         {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "StmtList", (yyloc).first_line);
            add_child((yyval.node_val), (yyvsp[-1].node_val));
            add_child((yyval.node_val), (yyvsp[0].node_val));
        }
#line 1795 "./syntax.tab.c"
    break;

  case 36: /* StmtList: %empty  */
#line 364 "./syntax.y"
                   {
            (yyval.node_val) = create_node(NODE_NONTERMINAL, "StmtList", (yyloc).first_line);
        }
#line 1803 "./syntax.tab.c"
    break;

  case 37: /* Stmt: Exp SEMI  */
#line 369 "./syntax.y"
                {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Stmt", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-1].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "SEMI", (yylsp[0]).first_line));
    }
#line 1813 "./syntax.tab.c"
    break;

  case 38: /* Stmt: CompSt  */
#line 374 "./syntax.y"
             {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Stmt", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1822 "./syntax.tab.c"
    break;

  case 39: /* Stmt: IF LP Exp RP Stmt  */
#line 378 "./syntax.y"
                                              {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Stmt", (yyloc).first_line);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "IF", (yylsp[-4]).first_line));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "LP", (yylsp[-3]).first_line));
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "RP", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1835 "./syntax.tab.c"
    break;

  case 40: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 386 "./syntax.y"
                                  {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Stmt", (yyloc).first_line);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "IF", (yylsp[-6]).first_line));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "LP", (yylsp[-5]).first_line));
        add_child((yyval.node_val), (yyvsp[-4].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "RP", (yylsp[-3]).first_line));
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "ELSE", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1850 "./syntax.tab.c"
    break;

  case 41: /* Stmt: WHILE LP Exp RP Stmt  */
#line 396 "./syntax.y"
                           {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Stmt", (yyloc).first_line);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "WHILE", (yylsp[-4]).first_line));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "LP", (yylsp[-3]).first_line));
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "RP", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1863 "./syntax.tab.c"
    break;

  case 42: /* Stmt: RETURN Exp SEMI  */
#line 404 "./syntax.y"
                      {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Stmt", (yyloc).first_line);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "RETURN", (yylsp[-2]).first_line));
        add_child((yyval.node_val), (yyvsp[-1].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "SEMI", (yylsp[0]).first_line));
    }
#line 1874 "./syntax.tab.c"
    break;

  case 43: /* Stmt: SEMI  */
#line 410 "./syntax.y"
           {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Stmt", (yyloc).first_line);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "SEMI", (yylsp[0]).first_line));
    }
#line 1883 "./syntax.tab.c"
    break;

  case 44: /* Stmt: error SEMI  */
#line 414 "./syntax.y"
                 { yyerrok; }
#line 1889 "./syntax.tab.c"
    break;

  case 45: /* Exp: Exp ASSIGNOP Exp  */
#line 417 "./syntax.y"
                       {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "ASSIGNOP", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1900 "./syntax.tab.c"
    break;

  case 46: /* Exp: Exp OR Exp  */
#line 423 "./syntax.y"
                 {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "OR", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1911 "./syntax.tab.c"
    break;

  case 47: /* Exp: Exp AND Exp  */
#line 429 "./syntax.y"
                  {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "AND", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1922 "./syntax.tab.c"
    break;

  case 48: /* Exp: Exp RELOP Exp  */
#line 435 "./syntax.y"
                    {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        Node* relop_node = create_node(NODE_TOKEN, (yyvsp[-1].str_val), (yylsp[-1]).first_line);
        relop_node->attr.str_val = (yyvsp[-1].str_val);
        add_child((yyval.node_val), relop_node);
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1935 "./syntax.tab.c"
    break;

  case 49: /* Exp: Exp PLUS Exp  */
#line 443 "./syntax.y"
                   {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "PLUS", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1946 "./syntax.tab.c"
    break;

  case 50: /* Exp: Exp MINUS Exp  */
#line 449 "./syntax.y"
                    {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "MINUS", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1957 "./syntax.tab.c"
    break;

  case 51: /* Exp: Exp STAR Exp  */
#line 455 "./syntax.y"
                   {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "STAR", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1968 "./syntax.tab.c"
    break;

  case 52: /* Exp: Exp DIV Exp  */
#line 461 "./syntax.y"
                  {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "DIV", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 1979 "./syntax.tab.c"
    break;

  case 53: /* Exp: LP Exp RP  */
#line 467 "./syntax.y"
                {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "LP", (yylsp[-2]).first_line));
        add_child((yyval.node_val), (yyvsp[-1].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "RP", (yylsp[0]).first_line));
    }
#line 1990 "./syntax.tab.c"
    break;

  case 54: /* Exp: MINUS Exp  */
#line 473 "./syntax.y"
                             {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "MINUS", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 2000 "./syntax.tab.c"
    break;

  case 55: /* Exp: NOT Exp  */
#line 478 "./syntax.y"
              {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "NOT", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 2010 "./syntax.tab.c"
    break;

  case 56: /* Exp: Exp DOT ID  */
#line 483 "./syntax.y"
                 {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "DOT", (yylsp[-1]).first_line));
        Node* id_node = create_node(NODE_ID, "ID", (yylsp[0]).first_line);
        id_node->attr.str_val = (yyvsp[0].str_val);
        add_child((yyval.node_val), id_node);
    }
#line 2023 "./syntax.tab.c"
    break;

  case 57: /* Exp: ID LP Args RP  */
#line 491 "./syntax.y"
                    {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        Node* id_node = create_node(NODE_ID, "ID", (yylsp[-3]).first_line);
        id_node->attr.str_val = (yyvsp[-3].str_val);
        add_child((yyval.node_val), id_node);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "LP", (yylsp[-2]).first_line));
        add_child((yyval.node_val), (yyvsp[-1].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "RP", (yylsp[0]).first_line));
    }
#line 2037 "./syntax.tab.c"
    break;

  case 58: /* Exp: ID LP RP  */
#line 500 "./syntax.y"
               {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        Node* id_node = create_node(NODE_ID, "ID", (yylsp[-2]).first_line);
        id_node->attr.str_val = (yyvsp[-2].str_val);
        add_child((yyval.node_val), id_node);
        add_child((yyval.node_val), create_node(NODE_TOKEN, "LP", (yylsp[-1]).first_line));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "RP", (yylsp[0]).first_line));
    }
#line 2050 "./syntax.tab.c"
    break;

  case 59: /* Exp: Exp LB Exp RB  */
#line 508 "./syntax.y"
                    {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-3].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "LB", (yylsp[-2]).first_line));
        add_child((yyval.node_val), (yyvsp[-1].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "RB", (yylsp[0]).first_line));
    }
#line 2062 "./syntax.tab.c"
    break;

  case 60: /* Exp: ID  */
#line 515 "./syntax.y"
         {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        Node* id_node = create_node(NODE_ID, "ID", (yylsp[0]).first_line);
        id_node->attr.str_val = (yyvsp[0].str_val);
        add_child((yyval.node_val), id_node);
    }
#line 2073 "./syntax.tab.c"
    break;

  case 61: /* Exp: INT  */
#line 521 "./syntax.y"
          {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        Node* int_node = create_node(NODE_INT, "INT", (yylsp[0]).first_line);
        int_node->attr.int_val = (yyvsp[0].int_val);
        add_child((yyval.node_val), int_node);
    }
#line 2084 "./syntax.tab.c"
    break;

  case 62: /* Exp: FLOAT  */
#line 527 "./syntax.y"
            {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Exp", (yyloc).first_line);
        Node* float_node = create_node(NODE_FLOAT, "FLOAT", (yylsp[0]).first_line);
        float_node->attr.float_val = (yyvsp[0].float_val);
        add_child((yyval.node_val), float_node);
    }
#line 2095 "./syntax.tab.c"
    break;

  case 63: /* Exp: error RP  */
#line 533 "./syntax.y"
               { yyerrok; }
#line 2101 "./syntax.tab.c"
    break;

  case 64: /* Exp: error RB  */
#line 534 "./syntax.y"
               { yyerrok; }
#line 2107 "./syntax.tab.c"
    break;

  case 65: /* Args: Exp COMMA Args  */
#line 537 "./syntax.y"
                      {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Args", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[-2].node_val));
        add_child((yyval.node_val), create_node(NODE_TOKEN, "COMMA", (yylsp[-1]).first_line));
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 2118 "./syntax.tab.c"
    break;

  case 66: /* Args: Exp  */
#line 543 "./syntax.y"
          {
        (yyval.node_val) = create_node(NODE_NONTERMINAL, "Args", (yyloc).first_line);
        add_child((yyval.node_val), (yyvsp[0].node_val));
    }
#line 2127 "./syntax.tab.c"
    break;


#line 2131 "./syntax.tab.c"

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
      yyerror (YY_("syntax error"));
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

  return yyresult;
}

#line 548 "./syntax.y"
