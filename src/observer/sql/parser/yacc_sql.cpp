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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <utility>
#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}


#line 115 "yacc_sql.cpp"

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

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_TABLES = 7,                     /* TABLES  */
  YYSYMBOL_UNIQUE = 8,                     /* UNIQUE  */
  YYSYMBOL_INDEX = 9,                      /* INDEX  */
  YYSYMBOL_CALC = 10,                      /* CALC  */
  YYSYMBOL_SELECT = 11,                    /* SELECT  */
  YYSYMBOL_ORDER = 12,                     /* ORDER  */
  YYSYMBOL_BY = 13,                        /* BY  */
  YYSYMBOL_ASC = 14,                       /* ASC  */
  YYSYMBOL_DESC = 15,                      /* DESC  */
  YYSYMBOL_SHOW = 16,                      /* SHOW  */
  YYSYMBOL_SYNC = 17,                      /* SYNC  */
  YYSYMBOL_INSERT = 18,                    /* INSERT  */
  YYSYMBOL_DELETE = 19,                    /* DELETE  */
  YYSYMBOL_UPDATE = 20,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 21,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 22,                    /* RBRACE  */
  YYSYMBOL_COMMA = 23,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 24,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 25,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 26,              /* TRX_ROLLBACK  */
  YYSYMBOL_IS = 27,                        /* IS  */
  YYSYMBOL_NOT = 28,                       /* NOT  */
  YYSYMBOL_NULL_T = 29,                    /* NULL_T  */
  YYSYMBOL_INT_T = 30,                     /* INT_T  */
  YYSYMBOL_STRING_T = 31,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 32,                   /* FLOAT_T  */
  YYSYMBOL_DATE_T = 33,                    /* DATE_T  */
  YYSYMBOL_TEXT_T = 34,                    /* TEXT_T  */
  YYSYMBOL_HELP = 35,                      /* HELP  */
  YYSYMBOL_EXIT = 36,                      /* EXIT  */
  YYSYMBOL_DOT = 37,                       /* DOT  */
  YYSYMBOL_INTO = 38,                      /* INTO  */
  YYSYMBOL_VALUES = 39,                    /* VALUES  */
  YYSYMBOL_FROM = 40,                      /* FROM  */
  YYSYMBOL_INNER = 41,                     /* INNER  */
  YYSYMBOL_JOIN = 42,                      /* JOIN  */
  YYSYMBOL_WHERE = 43,                     /* WHERE  */
  YYSYMBOL_IN = 44,                        /* IN  */
  YYSYMBOL_EXISTS = 45,                    /* EXISTS  */
  YYSYMBOL_AND = 46,                       /* AND  */
  YYSYMBOL_SET = 47,                       /* SET  */
  YYSYMBOL_ON = 48,                        /* ON  */
  YYSYMBOL_LOAD = 49,                      /* LOAD  */
  YYSYMBOL_DATA = 50,                      /* DATA  */
  YYSYMBOL_INFILE = 51,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 52,                   /* EXPLAIN  */
  YYSYMBOL_EQ = 53,                        /* EQ  */
  YYSYMBOL_LT = 54,                        /* LT  */
  YYSYMBOL_GT = 55,                        /* GT  */
  YYSYMBOL_LE = 56,                        /* LE  */
  YYSYMBOL_GE = 57,                        /* GE  */
  YYSYMBOL_NE = 58,                        /* NE  */
  YYSYMBOL_LK = 59,                        /* LK  */
  YYSYMBOL_NK = 60,                        /* NK  */
  YYSYMBOL_NUMBER = 61,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 62,                     /* FLOAT  */
  YYSYMBOL_ID = 63,                        /* ID  */
  YYSYMBOL_DATE_STR = 64,                  /* DATE_STR  */
  YYSYMBOL_SSS = 65,                       /* SSS  */
  YYSYMBOL_COUNT = 66,                     /* COUNT  */
  YYSYMBOL_AGG_FUNC = 67,                  /* AGG_FUNC  */
  YYSYMBOL_68_ = 68,                       /* '+'  */
  YYSYMBOL_69_ = 69,                       /* '-'  */
  YYSYMBOL_70_ = 70,                       /* '*'  */
  YYSYMBOL_71_ = 71,                       /* '/'  */
  YYSYMBOL_UMINUS = 72,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 73,                  /* $accept  */
  YYSYMBOL_commands = 74,                  /* commands  */
  YYSYMBOL_command_wrapper = 75,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 76,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 77,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 78,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 79,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 80,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 81,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 82,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 83,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 84,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 85,         /* create_index_stmt  */
  YYSYMBOL_index_type = 86,                /* index_type  */
  YYSYMBOL_id_list = 87,                   /* id_list  */
  YYSYMBOL_drop_index_stmt = 88,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 89,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 90,             /* attr_def_list  */
  YYSYMBOL_attr_def = 91,                  /* attr_def  */
  YYSYMBOL_number = 92,                    /* number  */
  YYSYMBOL_type = 93,                      /* type  */
  YYSYMBOL_nullable = 94,                  /* nullable  */
  YYSYMBOL_insert_stmt = 95,               /* insert_stmt  */
  YYSYMBOL_value_lists = 96,               /* value_lists  */
  YYSYMBOL_value_list = 97,                /* value_list  */
  YYSYMBOL_value = 98,                     /* value  */
  YYSYMBOL_delete_stmt = 99,               /* delete_stmt  */
  YYSYMBOL_update_stmt = 100,              /* update_stmt  */
  YYSYMBOL_update_list = 101,              /* update_list  */
  YYSYMBOL_update_value = 102,             /* update_value  */
  YYSYMBOL_select_stmt = 103,              /* select_stmt  */
  YYSYMBOL_calc_stmt = 104,                /* calc_stmt  */
  YYSYMBOL_expression_list = 105,          /* expression_list  */
  YYSYMBOL_expression = 106,               /* expression  */
  YYSYMBOL_select_list = 107,              /* select_list  */
  YYSYMBOL_select_attr_list = 108,         /* select_attr_list  */
  YYSYMBOL_all_attr_list = 109,            /* all_attr_list  */
  YYSYMBOL_all_attr = 110,                 /* all_attr  */
  YYSYMBOL_select_attr = 111,              /* select_attr  */
  YYSYMBOL_order_attr = 112,               /* order_attr  */
  YYSYMBOL_order = 113,                    /* order  */
  YYSYMBOL_order_list = 114,               /* order_list  */
  YYSYMBOL_rel_attr = 115,                 /* rel_attr  */
  YYSYMBOL_attr_list = 116,                /* attr_list  */
  YYSYMBOL_rel_list = 117,                 /* rel_list  */
  YYSYMBOL_join_list = 118,                /* join_list  */
  YYSYMBOL_where = 119,                    /* where  */
  YYSYMBOL_condition_list = 120,           /* condition_list  */
  YYSYMBOL_condition = 121,                /* condition  */
  YYSYMBOL_comp_op = 122,                  /* comp_op  */
  YYSYMBOL_load_data_stmt = 123,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 124,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 125,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 126             /* opt_semicolon  */
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
typedef yytype_int16 yy_state_t;

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
#define YYFINAL  73
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   267

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  140
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  264

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   323


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
       2,     2,    70,    68,     2,    69,     2,    71,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    72
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   220,   220,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   251,   257,   262,   268,   274,   280,   286,
     293,   299,   307,   325,   327,   333,   336,   348,   358,   377,
     380,   393,   402,   412,   424,   427,   428,   429,   430,   434,
     437,   440,   445,   466,   469,   487,   490,   501,   505,   509,
     514,   519,   527,   539,   561,   564,   574,   579,   587,   622,
     632,   637,   648,   651,   654,   657,   660,   664,   667,   674,
     682,   686,   700,   702,   714,   719,   735,   742,   757,   768,
     782,   785,   797,   804,   811,   822,   824,   838,   843,   848,
     855,   861,   867,   876,   879,   893,   896,   909,   912,   924,
     927,   933,   936,   941,   948,   960,   972,   984,   996,  1011,
    1026,  1037,  1048,  1055,  1065,  1066,  1067,  1068,  1069,  1070,
    1071,  1072,  1073,  1074,  1075,  1076,  1080,  1093,  1101,  1111,
    1112
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
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "UNIQUE", "INDEX", "CALC", "SELECT", "ORDER",
  "BY", "ASC", "DESC", "SHOW", "SYNC", "INSERT", "DELETE", "UPDATE",
  "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK",
  "IS", "NOT", "NULL_T", "INT_T", "STRING_T", "FLOAT_T", "DATE_T",
  "TEXT_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "INNER",
  "JOIN", "WHERE", "IN", "EXISTS", "AND", "SET", "ON", "LOAD", "DATA",
  "INFILE", "EXPLAIN", "EQ", "LT", "GT", "LE", "GE", "NE", "LK", "NK",
  "NUMBER", "FLOAT", "ID", "DATE_STR", "SSS", "COUNT", "AGG_FUNC", "'+'",
  "'-'", "'*'", "'/'", "UMINUS", "$accept", "commands", "command_wrapper",
  "exit_stmt", "help_stmt", "sync_stmt", "begin_stmt", "commit_stmt",
  "rollback_stmt", "drop_table_stmt", "show_tables_stmt",
  "desc_table_stmt", "create_index_stmt", "index_type", "id_list",
  "drop_index_stmt", "create_table_stmt", "attr_def_list", "attr_def",
  "number", "type", "nullable", "insert_stmt", "value_lists", "value_list",
  "value", "delete_stmt", "update_stmt", "update_list", "update_value",
  "select_stmt", "calc_stmt", "expression_list", "expression",
  "select_list", "select_attr_list", "all_attr_list", "all_attr",
  "select_attr", "order_attr", "order", "order_list", "rel_attr",
  "attr_list", "rel_list", "join_list", "where", "condition_list",
  "condition", "comp_op", "load_data_stmt", "explain_stmt",
  "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-191)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     113,    63,    14,   -16,    24,   -54,     7,  -191,   -12,    10,
     -30,  -191,  -191,  -191,  -191,  -191,    16,    23,   113,    88,
      89,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,
    -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,
    -191,    30,    93,  -191,    37,    42,    49,   -16,  -191,  -191,
    -191,  -191,  -191,   -16,  -191,  -191,    28,    76,    95,   100,
      77,    85,  -191,    99,  -191,  -191,  -191,    79,    94,   103,
     105,   129,  -191,  -191,  -191,  -191,   143,  -191,   133,  -191,
     134,    13,  -191,   -16,   -16,   -16,   -16,   -16,   -53,   -11,
       5,   -51,   120,    24,  -191,   146,   144,   123,   -23,   124,
     125,   127,   128,  -191,  -191,   -40,   -40,  -191,  -191,  -191,
    -191,  -191,   170,   178,  -191,   180,  -191,  -191,   181,    99,
     182,    91,  -191,   152,  -191,   168,   145,   184,   187,  -191,
    -191,   -45,  -191,  -191,   147,   171,  -191,   -23,   164,   190,
     140,   140,  -191,   167,    82,   208,  -191,  -191,  -191,  -191,
       8,   -13,   125,   193,   153,   178,   181,   175,   144,   195,
     198,   209,   194,   177,  -191,  -191,  -191,  -191,  -191,  -191,
    -191,  -191,  -191,     0,    45,    91,   209,  -191,   200,   161,
     196,  -191,  -191,   165,  -191,   184,  -191,   204,  -191,  -191,
     169,   216,   -23,   211,   209,   212,  -191,  -191,    -7,  -191,
    -191,    -7,  -191,  -191,  -191,   213,   173,   144,  -191,  -191,
    -191,   215,  -191,   176,   218,   183,   217,  -191,   195,   219,
     221,  -191,   195,   222,   195,   223,  -191,   185,  -191,     8,
     204,  -191,    91,   -45,  -191,   220,  -191,  -191,   224,  -191,
     225,  -191,    82,  -191,  -191,   171,   227,    29,   -23,  -191,
    -191,   200,  -191,   -45,  -191,  -191,  -191,   195,  -191,   227,
     226,  -191,   219,  -191
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,    80,     0,     0,    25,     0,     0,
       0,    26,    27,    28,    24,    23,     0,     0,     0,     0,
     139,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,    33,     0,     0,     0,     0,    61,    57,
      58,    59,    60,     0,    78,    69,    70,    98,     0,     0,
      97,     0,    79,    82,    84,    31,    30,     0,     0,     0,
       0,     0,   137,     1,   140,     2,     0,    34,     0,    29,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    81,     0,   109,     0,     0,     0,
       0,     0,     0,    76,    71,    72,    73,    74,    75,    99,
     102,    86,     0,   103,    88,     0,   100,   101,   105,    82,
       0,   111,    62,     0,   138,     0,     0,    39,     0,    37,
      85,     0,    89,    87,     0,   107,    83,     0,     0,     0,
       0,     0,   110,   112,     0,     0,    45,    46,    47,    48,
      49,    49,     0,     0,     0,   103,   105,     0,   109,    55,
       0,     0,   133,     0,   134,   124,   125,   126,   127,   128,
     129,   130,   131,     0,     0,   111,     0,    66,    64,     0,
       0,    50,    43,     0,    42,    39,    38,    35,   104,   106,
       0,    90,     0,     0,     0,     0,   132,   135,     0,   115,
     117,     0,   114,   116,   113,     0,     0,   109,   136,    51,
      44,     0,    40,     0,     0,     0,     0,    68,    55,    53,
       0,   122,    55,     0,    55,     0,    67,     0,    63,    49,
      35,    32,   111,     0,    56,     0,    52,   123,     0,   120,
       0,   121,     0,    41,    36,   107,    95,    94,     0,   118,
     119,    64,   108,     0,    91,    92,    93,    55,    65,    95,
       0,    96,    53,    54
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -191,  -191,   233,  -191,  -191,  -191,  -191,  -191,  -191,  -191,
    -191,  -191,  -191,  -191,    22,  -191,  -191,    68,   102,  -191,
    -191,  -149,  -191,    -6,  -190,   -97,  -191,  -191,     4,    15,
     -35,  -191,   179,    87,  -191,  -191,   139,   166,   174,  -191,
      12,     1,    -4,   106,   107,    21,  -151,  -172,  -191,   126,
    -191,  -191,  -191,  -191
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    44,   214,    31,    32,   153,   127,   211,
     151,   182,    33,   236,   193,    54,    34,    35,   207,   178,
      36,    37,    55,    56,    61,    62,    94,    63,   112,   217,
     246,   254,   141,   132,   135,   158,   122,   142,   143,   173,
      38,    39,    40,    75
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      64,   124,   184,   204,     4,    47,    48,   191,   183,    65,
     109,   111,   116,    48,    66,   180,   181,   110,    57,   117,
      45,   198,    48,    46,   140,    60,    67,   114,   234,    48,
      86,    87,   238,    69,   240,   103,   180,   181,    49,    50,
     159,    51,    52,   255,   256,    49,    50,   177,    51,    52,
      68,    83,    57,    53,    49,    50,   228,    51,    52,    60,
     245,    49,    50,    57,    51,    52,   201,   260,    57,    41,
      60,    42,    43,    71,    48,    60,   199,   202,   140,    70,
     243,    84,    85,    86,    87,   113,   113,    57,    73,    64,
      58,    59,    74,    76,    60,   218,    84,    85,    86,    87,
      78,   222,    77,   176,   224,    79,    49,    50,    57,    51,
      52,    48,    80,    88,    91,    60,    89,     1,     2,   138,
      48,    90,    93,     3,     4,    92,   195,   155,     5,     6,
       7,     8,     9,    10,    81,   140,   139,    11,    12,    13,
      82,   205,    95,    49,    50,   177,    51,    52,    14,    15,
      97,   257,    49,    50,    57,    51,    52,    96,    98,   220,
      16,    60,    17,   223,   100,    18,   225,   162,   163,   200,
     203,   105,   106,   107,   108,   146,   147,   148,   149,   150,
      99,   101,   102,   118,   164,   120,   123,   121,   126,   125,
     128,   129,   130,   165,   166,   167,   168,   169,   170,   171,
     172,   131,   133,   137,   134,   144,   145,   152,   154,   160,
     156,   161,   157,   175,   179,   186,   187,   190,   192,   194,
       4,   197,   196,   206,   208,   209,   210,   213,   216,   247,
     233,   232,   215,   219,   221,   226,   227,   229,   242,   230,
     231,   248,   235,   237,   239,   241,   249,   250,   262,   247,
     253,    72,   244,   212,   185,   258,   263,   251,   136,   119,
     261,   188,   104,   189,   115,   259,   252,   174
};

static const yytype_int16 yycheck[] =
{
       4,    98,   151,   175,    11,    21,    29,   158,    21,    63,
      63,    22,    63,    29,     7,    28,    29,    70,    63,    70,
       6,    21,    29,     9,   121,    70,    38,    22,   218,    29,
      70,    71,   222,    63,   224,    22,    28,    29,    61,    62,
     137,    64,    65,    14,    15,    61,    62,   144,    64,    65,
      40,    23,    63,    69,    61,    62,   207,    64,    65,    70,
     232,    61,    62,    63,    64,    65,    21,   257,    63,     6,
      70,     8,     9,    50,    29,    70,   173,   174,   175,    63,
     229,    68,    69,    70,    71,    89,    90,    63,     0,    93,
      66,    67,     3,    63,    70,   192,    68,    69,    70,    71,
      63,   198,     9,    21,   201,    63,    61,    62,    63,    64,
      65,    29,    63,    37,    37,    70,    21,     4,     5,    28,
      29,    21,    23,    10,    11,    40,   161,   131,    15,    16,
      17,    18,    19,    20,    47,   232,    45,    24,    25,    26,
      53,   176,    63,    61,    62,   242,    64,    65,    35,    36,
      47,   248,    61,    62,    63,    64,    65,    63,    53,   194,
      47,    70,    49,   198,    21,    52,   201,    27,    28,   173,
     174,    84,    85,    86,    87,    30,    31,    32,    33,    34,
      51,    48,    48,    63,    44,    39,    63,    43,    63,    65,
      63,    63,    22,    53,    54,    55,    56,    57,    58,    59,
      60,    23,    22,    21,    23,    53,    38,    23,    21,    45,
      63,    21,    41,    46,     6,    22,    63,    42,    23,    21,
      11,    44,    28,    23,    63,    29,    61,    23,    12,   233,
      13,    48,    63,    22,    22,    22,    63,    22,    53,    63,
      22,    21,    23,    22,    22,    22,    22,    22,    22,   253,
      23,    18,   230,   185,   152,   251,   262,   242,   119,    93,
     259,   155,    83,   156,    90,   253,   245,   141
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,    10,    11,    15,    16,    17,    18,    19,
      20,    24,    25,    26,    35,    36,    47,    49,    52,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    88,    89,    95,    99,   100,   103,   104,   123,   124,
     125,     6,     8,     9,    86,     6,     9,    21,    29,    61,
      62,    64,    65,    69,    98,   105,   106,    63,    66,    67,
      70,   107,   108,   110,   115,    63,     7,    38,    40,    63,
      63,    50,    75,     0,     3,   126,    63,     9,    63,    63,
      63,   106,   106,    23,    68,    69,    70,    71,    37,    21,
      21,    37,    40,    23,   109,    63,    63,    47,    53,    51,
      21,    48,    48,    22,   105,   106,   106,   106,   106,    63,
      70,    22,   111,   115,    22,   111,    63,    70,    63,   110,
      39,    43,   119,    63,    98,    65,    63,    91,    63,    63,
      22,    23,   116,    22,    23,   117,   109,    21,    28,    45,
      98,   115,   120,   121,    53,    38,    30,    31,    32,    33,
      34,    93,    23,    90,    21,   115,    63,    41,   118,    98,
      45,    21,    27,    28,    44,    53,    54,    55,    56,    57,
      58,    59,    60,   122,   122,    46,    21,    98,   102,     6,
      28,    29,    94,    21,    94,    91,    22,    63,   116,   117,
      42,   119,    23,    97,    21,   103,    28,    44,    21,    98,
     115,    21,    98,   115,   120,   103,    23,   101,    63,    29,
      61,    92,    90,    23,    87,    63,    12,   112,    98,    22,
     103,    22,    98,   103,    98,   103,    22,    63,   119,    22,
      63,    22,    48,    13,    97,    23,    96,    22,    97,    22,
      97,    22,    53,    94,    87,   120,   113,   115,    21,    22,
      22,   102,   118,    23,   114,    14,    15,    98,   101,   113,
      97,   114,    22,    96
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    73,    74,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    86,    87,    87,    88,    89,    90,
      90,    91,    91,    91,    92,    93,    93,    93,    93,    94,
      94,    94,    95,    96,    96,    97,    97,    98,    98,    98,
      98,    98,    99,   100,   101,   101,   102,   102,   103,   104,
     105,   105,   106,   106,   106,   106,   106,   106,   106,   107,
     108,   108,   109,   109,   110,   110,   110,   110,   110,   111,
     112,   112,   113,   113,   113,   114,   114,   115,   115,   115,
     115,   115,   115,   116,   116,   117,   117,   118,   118,   119,
     119,   120,   120,   120,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   123,   124,   125,   126,
     126
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     9,     1,     2,     0,     3,     5,     7,     0,
       3,     6,     3,     3,     1,     1,     1,     1,     1,     0,
       1,     2,     9,     0,     6,     0,     3,     1,     1,     1,
       1,     1,     4,     8,     0,     5,     1,     3,     8,     2,
       1,     3,     3,     3,     3,     3,     3,     2,     1,     1,
       0,     2,     0,     3,     1,     4,     3,     4,     3,     2,
       0,     4,     2,     2,     1,     0,     3,     1,     1,     3,
       3,     3,     3,     0,     3,     0,     3,     0,     6,     0,
       2,     0,     1,     3,     3,     3,     3,     3,     6,     6,
       5,     5,     4,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     2,     7,     2,     4,     0,
       1
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
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
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
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
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
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
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
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

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
      yychar = yylex (&yylval, &yylloc, scanner);
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
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 221 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1827 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 251 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1836 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 257 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1844 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 262 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1852 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 268 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1860 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 274 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1868 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 280 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1876 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 286 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1886 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 293 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1894 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 299 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1904 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE index_type ID ON ID LBRACE ID id_list RBRACE  */
#line 308 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);
      create_index.is_unique = (yyvsp[-7].index_type);
      if( (yyvsp[-1].id_list) != nullptr){
        create_index.attribute_name.swap( *(yyvsp[-1].id_list));
      }
      create_index.attribute_name.emplace_back((yyvsp[-2].string));
      std::reverse(create_index.attribute_name.begin(),create_index.attribute_name.end());
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
    }
#line 1923 "yacc_sql.cpp"
    break;

  case 33: /* index_type: INDEX  */
#line 325 "yacc_sql.y"
        {
    (yyval.index_type) = 0;
  }
#line 1931 "yacc_sql.cpp"
    break;

  case 34: /* index_type: UNIQUE INDEX  */
#line 327 "yacc_sql.y"
                   {
    (yyval.index_type) = 1;
  }
#line 1939 "yacc_sql.cpp"
    break;

  case 35: /* id_list: %empty  */
#line 333 "yacc_sql.y"
    {
      (yyval.id_list) = nullptr;
    }
#line 1947 "yacc_sql.cpp"
    break;

  case 36: /* id_list: COMMA ID id_list  */
#line 336 "yacc_sql.y"
                       {
      if((yyvsp[0].id_list) != nullptr){
        (yyval.id_list) = (yyvsp[0].id_list);
      } else {
        (yyval.id_list) = new std::vector<std::string>;
      }
      (yyval.id_list)->emplace_back((yyvsp[-1].string));
      std::reverse((yyval.id_list)->begin(),(yyval.id_list)->end());
    }
#line 1961 "yacc_sql.cpp"
    break;

  case 37: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 349 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 1973 "yacc_sql.cpp"
    break;

  case 38: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE  */
#line 359 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-4].string);
      free((yyvsp[-4].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-1].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-2].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-2].attr_info);
    }
#line 1993 "yacc_sql.cpp"
    break;

  case 39: /* attr_def_list: %empty  */
#line 377 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2001 "yacc_sql.cpp"
    break;

  case 40: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 381 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2015 "yacc_sql.cpp"
    break;

  case 41: /* attr_def: ID type LBRACE number RBRACE nullable  */
#line 394 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = (yyvsp[0].nullable);
      free((yyvsp[-5].string));
    }
#line 2028 "yacc_sql.cpp"
    break;

  case 42: /* attr_def: ID type nullable  */
#line 403 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      /*change the length of type,default*/
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = (yyvsp[0].nullable);
      free((yyvsp[-2].string));
    }
#line 2042 "yacc_sql.cpp"
    break;

  case 43: /* attr_def: ID TEXT_T nullable  */
#line 413 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = CHARS;
      (yyval.attr_info)->name = (yyvsp[-2].string);
      /*change the length of type,default*/
      (yyval.attr_info)->length = 4096;
      (yyval.attr_info)->nullable = (yyvsp[0].nullable);
      free((yyvsp[-2].string));
    }
#line 2056 "yacc_sql.cpp"
    break;

  case 44: /* number: NUMBER  */
#line 424 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2062 "yacc_sql.cpp"
    break;

  case 45: /* type: INT_T  */
#line 427 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 2068 "yacc_sql.cpp"
    break;

  case 46: /* type: STRING_T  */
#line 428 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 2074 "yacc_sql.cpp"
    break;

  case 47: /* type: FLOAT_T  */
#line 429 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 2080 "yacc_sql.cpp"
    break;

  case 48: /* type: DATE_T  */
#line 430 "yacc_sql.y"
               { (yyval.number)=DATES; }
#line 2086 "yacc_sql.cpp"
    break;

  case 49: /* nullable: %empty  */
#line 434 "yacc_sql.y"
    {
      (yyval.nullable) = 0;
    }
#line 2094 "yacc_sql.cpp"
    break;

  case 50: /* nullable: NULL_T  */
#line 437 "yacc_sql.y"
             {
      (yyval.nullable) = 1;
    }
#line 2102 "yacc_sql.cpp"
    break;

  case 51: /* nullable: NOT NULL_T  */
#line 440 "yacc_sql.y"
                 {
      (yyval.nullable) = 0;
    }
#line 2110 "yacc_sql.cpp"
    break;

  case 52: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE value_lists  */
#line 446 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-6].string);
      std::vector<Value> vec;
      if ((yyvsp[0].value_lists) != nullptr){
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[0].value_lists));
      }
      if ((yyvsp[-2].value_list) != nullptr ) {
        vec.swap(*(yyvsp[-2].value_list));
      }
      vec.emplace_back(*(yyvsp[-3].value));
      std::reverse(vec.begin(), vec.end());
      (yyval.sql_node)->insertion.values.emplace_back(vec);
      std::reverse((yyval.sql_node)->insertion.values.begin(), (yyval.sql_node)->insertion.values.end());
      delete (yyvsp[-3].value);
      free((yyvsp[-6].string));
    }
#line 2132 "yacc_sql.cpp"
    break;

  case 53: /* value_lists: %empty  */
#line 466 "yacc_sql.y"
    {
      (yyval.value_lists) = nullptr;
    }
#line 2140 "yacc_sql.cpp"
    break;

  case 54: /* value_lists: COMMA LBRACE value value_list RBRACE value_lists  */
#line 469 "yacc_sql.y"
                                                       {
      if((yyvsp[0].value_lists) != nullptr){
        (yyval.value_lists) = (yyvsp[0].value_lists);
      } else {
        (yyval.value_lists) = new std::vector<std::vector<Value> >;
      }
      std::vector<Value> vec1;
      if( (yyvsp[-2].value_list) != nullptr){
        vec1.swap(*(yyvsp[-2].value_list));
      }
      vec1.emplace_back(*(yyvsp[-3].value));
      std::reverse(vec1.begin(), vec1.end());
      (yyval.value_lists)->emplace_back(vec1);
      delete (yyvsp[-3].value);
    }
#line 2160 "yacc_sql.cpp"
    break;

  case 55: /* value_list: %empty  */
#line 487 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 2168 "yacc_sql.cpp"
    break;

  case 56: /* value_list: COMMA value value_list  */
#line 490 "yacc_sql.y"
                              { 
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2182 "yacc_sql.cpp"
    break;

  case 57: /* value: NUMBER  */
#line 501 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2191 "yacc_sql.cpp"
    break;

  case 58: /* value: FLOAT  */
#line 505 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2200 "yacc_sql.cpp"
    break;

  case 59: /* value: DATE_STR  */
#line 509 "yacc_sql.y"
              {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp,true);
      free(tmp);
    }
#line 2210 "yacc_sql.cpp"
    break;

  case 60: /* value: SSS  */
#line 514 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp,false);
      free(tmp);
    }
#line 2220 "yacc_sql.cpp"
    break;

  case 61: /* value: NULL_T  */
#line 519 "yacc_sql.y"
            {
      const char *tmp = "null";
      (yyval.value) = new Value(tmp,false);
      (yyval.value)->set_null();
    }
#line 2230 "yacc_sql.cpp"
    break;

  case 62: /* delete_stmt: DELETE FROM ID where  */
#line 528 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2244 "yacc_sql.cpp"
    break;

  case 63: /* update_stmt: UPDATE ID SET ID EQ update_value update_list where  */
#line 540 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      if((yyvsp[-1].update_list) != nullptr){
        for(auto p = (yyvsp[-1].update_list)->begin();p != (yyvsp[-1].update_list)->end();p++){
          (yyval.sql_node)->update.attribute_name.emplace_back((*p).first);
          (yyval.sql_node)->update.value.emplace_back((*p).second);
        }
      }
      (yyval.sql_node)->update.relation_name = (yyvsp[-6].string);
      (yyval.sql_node)->update.attribute_name.emplace_back((yyvsp[-4].string));
      (yyval.sql_node)->update.value.emplace_back(*(yyvsp[-2].update_value));
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->update.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
    }
#line 2267 "yacc_sql.cpp"
    break;

  case 64: /* update_list: %empty  */
#line 561 "yacc_sql.y"
  {
    (yyval.update_list) = nullptr;
  }
#line 2275 "yacc_sql.cpp"
    break;

  case 65: /* update_list: COMMA ID EQ update_value update_list  */
#line 564 "yacc_sql.y"
                                         {
    if((yyvsp[0].update_list) != nullptr){
      (yyval.update_list) = (yyvsp[0].update_list);
    } else {
      (yyval.update_list) = new std::vector<std::pair<std::string,UpdateValueSqlNode> >;
    }
    (yyval.update_list)->emplace_back(std::make_pair((yyvsp[-3].string),*(yyvsp[-1].update_value)));
  }
#line 2288 "yacc_sql.cpp"
    break;

  case 66: /* update_value: value  */
#line 575 "yacc_sql.y"
  {
    (yyval.update_value) = new UpdateValueSqlNode(true,*(yyvsp[0].value));
    delete (yyvsp[0].value);
  }
#line 2297 "yacc_sql.cpp"
    break;

  case 67: /* update_value: LBRACE select_stmt RBRACE  */
#line 580 "yacc_sql.y"
  {
    (yyval.update_value) = new UpdateValueSqlNode(false,(yyvsp[-1].sql_node)->selection);
    delete (yyvsp[-1].sql_node);
  }
#line 2306 "yacc_sql.cpp"
    break;

  case 68: /* select_stmt: SELECT select_list FROM ID rel_list join_list where order_attr  */
#line 588 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-6].select_list) != nullptr) {
        (yyval.sql_node)->selection.attributes.swap(*(yyvsp[-6].select_list));
        delete (yyvsp[-6].select_list);
      }

      if((yyvsp[0].order_list) != nullptr) {
        std::reverse((yyvsp[0].order_list)->begin(),(yyvsp[0].order_list)->end());
        (yyval.sql_node)->selection.orders.swap(*(yyvsp[0].order_list));
        delete (yyvsp[0].order_list);
      }

      if ((yyvsp[-3].relation_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-3].relation_list));
        delete (yyvsp[-3].relation_list);
      }
      (yyval.sql_node)->selection.relations.push_back((yyvsp[-4].string));
      std::reverse((yyval.sql_node)->selection.relations.begin(), (yyval.sql_node)->selection.relations.end());

      if ((yyvsp[-1].condition_list) != nullptr) {
        (yyval.sql_node)->selection.conditions.swap(*(yyvsp[-1].condition_list));
        delete (yyvsp[-1].condition_list);
      }

      if((yyvsp[-2].join_list) != nullptr){
        (yyval.sql_node)->selection.relations.insert((yyval.sql_node)->selection.relations.end(),((yyvsp[-2].join_list)->first).begin(),((yyvsp[-2].join_list)->first).end());
        (yyval.sql_node)->selection.conditions.insert((yyval.sql_node)->selection.conditions.end(),((yyvsp[-2].join_list)->second).begin(),((yyvsp[-2].join_list)->second).end());
      }
      free((yyvsp[-4].string));
    }
#line 2342 "yacc_sql.cpp"
    break;

  case 69: /* calc_stmt: CALC expression_list  */
#line 623 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2353 "yacc_sql.cpp"
    break;

  case 70: /* expression_list: expression  */
#line 633 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<Expression*>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2362 "yacc_sql.cpp"
    break;

  case 71: /* expression_list: expression COMMA expression_list  */
#line 638 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-2].expression));
    }
#line 2375 "yacc_sql.cpp"
    break;

  case 72: /* expression: expression '+' expression  */
#line 648 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2383 "yacc_sql.cpp"
    break;

  case 73: /* expression: expression '-' expression  */
#line 651 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2391 "yacc_sql.cpp"
    break;

  case 74: /* expression: expression '*' expression  */
#line 654 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2399 "yacc_sql.cpp"
    break;

  case 75: /* expression: expression '/' expression  */
#line 657 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2407 "yacc_sql.cpp"
    break;

  case 76: /* expression: LBRACE expression RBRACE  */
#line 660 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2416 "yacc_sql.cpp"
    break;

  case 77: /* expression: '-' expression  */
#line 664 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2424 "yacc_sql.cpp"
    break;

  case 78: /* expression: value  */
#line 667 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2434 "yacc_sql.cpp"
    break;

  case 79: /* select_list: select_attr_list  */
#line 674 "yacc_sql.y"
                   {
      // 这个处理就是直接放入
      (yyval.select_list) = (yyvsp[0].select_attr_list);
    }
#line 2443 "yacc_sql.cpp"
    break;

  case 80: /* select_attr_list: %empty  */
#line 682 "yacc_sql.y"
    {
      (yyval.select_attr_list) = nullptr;
    }
#line 2451 "yacc_sql.cpp"
    break;

  case 81: /* select_attr_list: all_attr all_attr_list  */
#line 686 "yacc_sql.y"
                          {
    if ((yyvsp[0].all_attr_list) != nullptr){
        (yyval.select_attr_list) = (yyvsp[0].all_attr_list);
      } else {
        (yyval.select_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.select_attr_list) -> emplace_back(*(yyvsp[-1].all_attr));
      delete (yyvsp[-1].all_attr);
  }
#line 2465 "yacc_sql.cpp"
    break;

  case 82: /* all_attr_list: %empty  */
#line 700 "yacc_sql.y"
    {
      (yyval.all_attr_list) = nullptr;
    }
#line 2473 "yacc_sql.cpp"
    break;

  case 83: /* all_attr_list: COMMA all_attr all_attr_list  */
#line 702 "yacc_sql.y"
                                     {
      if((yyvsp[0].all_attr_list) != nullptr){
        (yyval.all_attr_list) = (yyvsp[0].all_attr_list);
      } else {
        (yyval.all_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.all_attr_list)->emplace_back(*(yyvsp[-1].all_attr));
      delete (yyvsp[-1].all_attr);
    }
#line 2487 "yacc_sql.cpp"
    break;

  case 84: /* all_attr: rel_attr  */
#line 715 "yacc_sql.y"
   {
      // 如果直接是引用字段就之间加入
      (yyval.all_attr) = (yyvsp[0].rel_attr);// 表示是rel_attr的字段
   }
#line 2496 "yacc_sql.cpp"
    break;

  case 85: /* all_attr: COUNT LBRACE select_attr RBRACE  */
#line 720 "yacc_sql.y"
         {
    if((yyvsp[-1].rel_attr_list)->size() > 1){
      (yyval.all_attr) = new RelAttrSqlNode;
      (yyval.all_attr)->agg_name = strdup((yyvsp[-3].string));
      (yyval.all_attr)->isOK = false;
      free((yyvsp[-1].rel_attr_list));
    }
    else{
      // 只有COUNT允许COUNT(*)
		  (yyval.all_attr) = new RelAttrSqlNode;
      (yyval.all_attr)->attribute_name = (yyvsp[-1].rel_attr_list)->back().attribute_name;
      (yyval.all_attr)->agg_name = strdup((yyvsp[-3].string));
      free((yyvsp[-3].string));
    }
	}
#line 2516 "yacc_sql.cpp"
    break;

  case 86: /* all_attr: COUNT LBRACE RBRACE  */
#line 736 "yacc_sql.y"
         {
   
      (yyval.all_attr) = new RelAttrSqlNode;
      (yyval.all_attr)->agg_name = strdup((yyvsp[-2].string));
      (yyval.all_attr)->isOK = false;   
	}
#line 2527 "yacc_sql.cpp"
    break;

  case 87: /* all_attr: AGG_FUNC LBRACE select_attr RBRACE  */
#line 743 "yacc_sql.y"
  {
    if((yyvsp[-1].rel_attr_list)->size() > 1){
      (yyval.all_attr) = new RelAttrSqlNode;
      (yyval.all_attr)->agg_name = strdup((yyvsp[-3].string));
      (yyval.all_attr)->isOK = false;
    }
    else{
      // 只有COUNT允许COUNT(*)
		  (yyval.all_attr) = new RelAttrSqlNode;
      (yyval.all_attr)->attribute_name = (yyvsp[-1].rel_attr_list)->back().attribute_name;
      (yyval.all_attr)->agg_name = strdup((yyvsp[-3].string));
      free((yyvsp[-3].string));
    }
  }
#line 2546 "yacc_sql.cpp"
    break;

  case 88: /* all_attr: AGG_FUNC LBRACE RBRACE  */
#line 758 "yacc_sql.y"
         {
   
      (yyval.all_attr) = new RelAttrSqlNode;
      (yyval.all_attr)->agg_name = strdup((yyvsp[-2].string));
      (yyval.all_attr)->isOK = false;   
	}
#line 2557 "yacc_sql.cpp"
    break;

  case 89: /* select_attr: rel_attr attr_list  */
#line 768 "yacc_sql.y"
                       {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        /* 这里创建的是新数组 */
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2572 "yacc_sql.cpp"
    break;

  case 90: /* order_attr: %empty  */
#line 782 "yacc_sql.y"
    {
      (yyval.order_list) = nullptr;
    }
#line 2580 "yacc_sql.cpp"
    break;

  case 91: /* order_attr: ORDER BY order order_list  */
#line 785 "yacc_sql.y"
                                {
      if ((yyvsp[0].order_list) != nullptr){
        (yyval.order_list) = (yyvsp[0].order_list);
      } else {
        (yyval.order_list) = new std::vector<RelAttrOrderNode>;
      }
      (yyval.order_list) -> emplace_back(*(yyvsp[-1].order));
      delete (yyvsp[-1].order);
    }
#line 2594 "yacc_sql.cpp"
    break;

  case 92: /* order: rel_attr ASC  */
#line 797 "yacc_sql.y"
                 {
      (yyval.order) = new RelAttrOrderNode;
      (yyval.order)->attribute_name = (yyvsp[-1].rel_attr)->attribute_name;
      (yyval.order)->relation_name = (yyvsp[-1].rel_attr)->relation_name;
      (yyval.order)->order_by_desc = false;
      delete (yyvsp[-1].rel_attr);
    }
#line 2606 "yacc_sql.cpp"
    break;

  case 93: /* order: rel_attr DESC  */
#line 804 "yacc_sql.y"
                    {
      (yyval.order) = new RelAttrOrderNode;
      (yyval.order)->attribute_name = (yyvsp[-1].rel_attr)->attribute_name;
      (yyval.order)->relation_name = (yyvsp[-1].rel_attr)->relation_name;
      (yyval.order)->order_by_desc = true;
      delete (yyvsp[-1].rel_attr);
    }
#line 2618 "yacc_sql.cpp"
    break;

  case 94: /* order: rel_attr  */
#line 811 "yacc_sql.y"
               {
      (yyval.order) = new RelAttrOrderNode;
      (yyval.order)->attribute_name = (yyvsp[0].rel_attr)->attribute_name;
      (yyval.order)->relation_name = (yyvsp[0].rel_attr)->relation_name;
      (yyval.order)->order_by_desc = false;
      delete (yyvsp[0].rel_attr);
    }
#line 2630 "yacc_sql.cpp"
    break;

  case 95: /* order_list: %empty  */
#line 822 "yacc_sql.y"
    {
      (yyval.order_list) = nullptr;
    }
#line 2638 "yacc_sql.cpp"
    break;

  case 96: /* order_list: COMMA order order_list  */
#line 824 "yacc_sql.y"
                               {
      if((yyvsp[0].order_list) != nullptr){
        (yyval.order_list) = (yyvsp[0].order_list);
      } else {
        (yyval.order_list) = new std::vector<RelAttrOrderNode>;
      }
      (yyval.order_list)->emplace_back(*(yyvsp[-1].order));
      delete (yyvsp[-1].order);
    }
#line 2652 "yacc_sql.cpp"
    break;

  case 97: /* rel_attr: '*'  */
#line 838 "yacc_sql.y"
      {
      // 此处就是直接获取，增加一个该判断请求
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name='*';
    }
#line 2662 "yacc_sql.cpp"
    break;

  case 98: /* rel_attr: ID  */
#line 843 "yacc_sql.y"
         {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2672 "yacc_sql.cpp"
    break;

  case 99: /* rel_attr: ID DOT ID  */
#line 848 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2684 "yacc_sql.cpp"
    break;

  case 100: /* rel_attr: '*' DOT ID  */
#line 855 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = "*";
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2695 "yacc_sql.cpp"
    break;

  case 101: /* rel_attr: '*' DOT '*'  */
#line 861 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = "*";
      (yyval.rel_attr)->attribute_name = "*";
      
    }
#line 2706 "yacc_sql.cpp"
    break;

  case 102: /* rel_attr: ID DOT '*'  */
#line 867 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = "*";    
    }
#line 2716 "yacc_sql.cpp"
    break;

  case 103: /* attr_list: %empty  */
#line 876 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2724 "yacc_sql.cpp"
    break;

  case 104: /* attr_list: COMMA rel_attr attr_list  */
#line 879 "yacc_sql.y"
                               {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }

      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2739 "yacc_sql.cpp"
    break;

  case 105: /* rel_list: %empty  */
#line 893 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2747 "yacc_sql.cpp"
    break;

  case 106: /* rel_list: COMMA ID rel_list  */
#line 896 "yacc_sql.y"
                        {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }

      (yyval.relation_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 2762 "yacc_sql.cpp"
    break;

  case 107: /* join_list: %empty  */
#line 909 "yacc_sql.y"
    {
      (yyval.join_list) = nullptr;
    }
#line 2770 "yacc_sql.cpp"
    break;

  case 108: /* join_list: INNER JOIN ID ON condition_list join_list  */
#line 912 "yacc_sql.y"
                                                {
      if((yyvsp[0].join_list) != nullptr){
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new std::pair<std::vector<std::string>,std::vector<ConditionSqlNode> >;
      }
      (yyval.join_list)->first.emplace_back((yyvsp[-3].string));
      std::reverse((yyval.join_list)->first.begin(),(yyval.join_list)->first.end());
      (yyval.join_list)->second.insert((yyval.join_list)->second.end(),(yyvsp[-1].condition_list)->begin(),(yyvsp[-1].condition_list)->end());                                          
    }
#line 2785 "yacc_sql.cpp"
    break;

  case 109: /* where: %empty  */
#line 924 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2793 "yacc_sql.cpp"
    break;

  case 110: /* where: WHERE condition_list  */
#line 927 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 2801 "yacc_sql.cpp"
    break;

  case 111: /* condition_list: %empty  */
#line 933 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2809 "yacc_sql.cpp"
    break;

  case 112: /* condition_list: condition  */
#line 936 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 2819 "yacc_sql.cpp"
    break;

  case 113: /* condition_list: condition AND condition_list  */
#line 941 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 2829 "yacc_sql.cpp"
    break;

  case 114: /* condition: rel_attr comp_op value  */
#line 949 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->left_attr = *(yyvsp[-2].rel_attr);
      (yyval.condition)->right_is_attr = 0;
      (yyval.condition)->right_value = *(yyvsp[0].value);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].rel_attr);
      delete (yyvsp[0].value);
    }
#line 2845 "yacc_sql.cpp"
    break;

  case 115: /* condition: value comp_op value  */
#line 961 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 0;
      (yyval.condition)->left_value = *(yyvsp[-2].value);
      (yyval.condition)->right_is_attr = 0;
      (yyval.condition)->right_value = *(yyvsp[0].value);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].value);
      delete (yyvsp[0].value);
    }
#line 2861 "yacc_sql.cpp"
    break;

  case 116: /* condition: rel_attr comp_op rel_attr  */
#line 973 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->left_attr = *(yyvsp[-2].rel_attr);
      (yyval.condition)->right_is_attr = 1;
      (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].rel_attr);
      delete (yyvsp[0].rel_attr);
    }
#line 2877 "yacc_sql.cpp"
    break;

  case 117: /* condition: value comp_op rel_attr  */
#line 985 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 0;
      (yyval.condition)->left_value = *(yyvsp[-2].value);
      (yyval.condition)->right_is_attr = 1;
      (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].value);
      delete (yyvsp[0].rel_attr);
    }
#line 2893 "yacc_sql.cpp"
    break;

  case 118: /* condition: value comp_op LBRACE value value_list RBRACE  */
#line 997 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 0;
      (yyval.condition)->left_value = *(yyvsp[-5].value);
      if((yyvsp[-1].value_list) != nullptr){
        (yyval.condition)->in_values.swap(*(yyvsp[-1].value_list));
      }
      (yyval.condition)->in_values.emplace_back(*(yyvsp[-2].value));
      (yyval.condition)->comp = (yyvsp[-4].comp);

      delete (yyvsp[-5].value);
      delete (yyvsp[-2].value);
      delete (yyvsp[-1].value_list);
    }
#line 2912 "yacc_sql.cpp"
    break;

  case 119: /* condition: rel_attr comp_op LBRACE value value_list RBRACE  */
#line 1012 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->left_attr = *(yyvsp[-5].rel_attr);
      if((yyvsp[-1].value_list) != nullptr){
        (yyval.condition)->in_values.swap(*(yyvsp[-1].value_list));
      }
      (yyval.condition)->in_values.emplace_back(*(yyvsp[-2].value));
      (yyval.condition)->comp = (yyvsp[-4].comp);

      delete (yyvsp[-5].rel_attr);
      delete (yyvsp[-2].value);
      delete (yyvsp[-1].value_list);
    }
#line 2931 "yacc_sql.cpp"
    break;

  case 120: /* condition: value comp_op LBRACE select_stmt RBRACE  */
#line 1027 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 0;
      (yyval.condition)->left_value = *(yyvsp[-4].value);
      (yyval.condition)->need_sub_query = true;
      (yyval.condition)->sub_select = &((yyvsp[-1].sql_node)->selection);
      (yyval.condition)->comp = (yyvsp[-3].comp);

      delete (yyvsp[-4].value);
    }
#line 2946 "yacc_sql.cpp"
    break;

  case 121: /* condition: rel_attr comp_op LBRACE select_stmt RBRACE  */
#line 1038 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->left_attr = *(yyvsp[-4].rel_attr);
      (yyval.condition)->need_sub_query = true;
      (yyval.condition)->sub_select = &((yyvsp[-1].sql_node)->selection);
      (yyval.condition)->comp = (yyvsp[-3].comp);

      delete (yyvsp[-4].rel_attr);
    }
#line 2961 "yacc_sql.cpp"
    break;

  case 122: /* condition: EXISTS LBRACE select_stmt RBRACE  */
#line 1049 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->need_sub_query = true;
      (yyval.condition)->sub_select = &((yyvsp[-1].sql_node)->selection);
      (yyval.condition)->comp = EX;
    }
#line 2972 "yacc_sql.cpp"
    break;

  case 123: /* condition: NOT EXISTS LBRACE select_stmt RBRACE  */
#line 1056 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->need_sub_query = true;
      (yyval.condition)->sub_select = &((yyvsp[-1].sql_node)->selection);
      (yyval.condition)->comp = NOT_EX;
    }
#line 2983 "yacc_sql.cpp"
    break;

  case 124: /* comp_op: EQ  */
#line 1065 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 2989 "yacc_sql.cpp"
    break;

  case 125: /* comp_op: LT  */
#line 1066 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 2995 "yacc_sql.cpp"
    break;

  case 126: /* comp_op: GT  */
#line 1067 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3001 "yacc_sql.cpp"
    break;

  case 127: /* comp_op: LE  */
#line 1068 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3007 "yacc_sql.cpp"
    break;

  case 128: /* comp_op: GE  */
#line 1069 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3013 "yacc_sql.cpp"
    break;

  case 129: /* comp_op: NE  */
#line 1070 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3019 "yacc_sql.cpp"
    break;

  case 130: /* comp_op: LK  */
#line 1071 "yacc_sql.y"
         { (yyval.comp) = LIKE;}
#line 3025 "yacc_sql.cpp"
    break;

  case 131: /* comp_op: NK  */
#line 1072 "yacc_sql.y"
         { (yyval.comp) = NLIKE;}
#line 3031 "yacc_sql.cpp"
    break;

  case 132: /* comp_op: IS NOT  */
#line 1073 "yacc_sql.y"
             { (yyval.comp) = IS_NOT_NULL;}
#line 3037 "yacc_sql.cpp"
    break;

  case 133: /* comp_op: IS  */
#line 1074 "yacc_sql.y"
         { (yyval.comp) = IS_NULL;}
#line 3043 "yacc_sql.cpp"
    break;

  case 134: /* comp_op: IN  */
#line 1075 "yacc_sql.y"
         { (yyval.comp) = IN_;}
#line 3049 "yacc_sql.cpp"
    break;

  case 135: /* comp_op: NOT IN  */
#line 1076 "yacc_sql.y"
             { (yyval.comp) = NOT_IN;}
#line 3055 "yacc_sql.cpp"
    break;

  case 136: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1081 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3069 "yacc_sql.cpp"
    break;

  case 137: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1094 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3078 "yacc_sql.cpp"
    break;

  case 138: /* set_variable_stmt: SET ID EQ value  */
#line 1102 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3090 "yacc_sql.cpp"
    break;


#line 3094 "yacc_sql.cpp"

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
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
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
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
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
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
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

#line 1114 "yacc_sql.y"

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
