/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
# define YY_YY_YACC_SQL_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SEMICOLON = 258,               /* SEMICOLON  */
    CREATE = 259,                  /* CREATE  */
    DROP = 260,                    /* DROP  */
    TABLE = 261,                   /* TABLE  */
    TABLES = 262,                  /* TABLES  */
    UNIQUE = 263,                  /* UNIQUE  */
    INDEX = 264,                   /* INDEX  */
    CALC = 265,                    /* CALC  */
    SELECT = 266,                  /* SELECT  */
    ORDER = 267,                   /* ORDER  */
    BY = 268,                      /* BY  */
    ASC = 269,                     /* ASC  */
    DESC = 270,                    /* DESC  */
    SHOW = 271,                    /* SHOW  */
    SYNC = 272,                    /* SYNC  */
    INSERT = 273,                  /* INSERT  */
    DELETE = 274,                  /* DELETE  */
    UPDATE = 275,                  /* UPDATE  */
    LBRACE = 276,                  /* LBRACE  */
    RBRACE = 277,                  /* RBRACE  */
    COMMA = 278,                   /* COMMA  */
    TRX_BEGIN = 279,               /* TRX_BEGIN  */
    TRX_COMMIT = 280,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 281,            /* TRX_ROLLBACK  */
    INT_T = 282,                   /* INT_T  */
    STRING_T = 283,                /* STRING_T  */
    FLOAT_T = 284,                 /* FLOAT_T  */
    DATE_T = 285,                  /* DATE_T  */
    TEXT_T = 286,                  /* TEXT_T  */
    HELP = 287,                    /* HELP  */
    EXIT = 288,                    /* EXIT  */
    DOT = 289,                     /* DOT  */
    INTO = 290,                    /* INTO  */
    VALUES = 291,                  /* VALUES  */
    FROM = 292,                    /* FROM  */
    INNER = 293,                   /* INNER  */
    JOIN = 294,                    /* JOIN  */
    WHERE = 295,                   /* WHERE  */
    AND = 296,                     /* AND  */
    SET = 297,                     /* SET  */
    ON = 298,                      /* ON  */
    LOAD = 299,                    /* LOAD  */
    DATA = 300,                    /* DATA  */
    INFILE = 301,                  /* INFILE  */
    EXPLAIN = 302,                 /* EXPLAIN  */
    EQ = 303,                      /* EQ  */
    LT = 304,                      /* LT  */
    GT = 305,                      /* GT  */
    LE = 306,                      /* LE  */
    GE = 307,                      /* GE  */
    NE = 308,                      /* NE  */
    NUMBER = 309,                  /* NUMBER  */
    FLOAT = 310,                   /* FLOAT  */
    ID = 311,                      /* ID  */
    DATE_STR = 312,                /* DATE_STR  */
    SSS = 313,                     /* SSS  */
    UMINUS = 314                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 110 "yacc_sql.y"

  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;
  Value *                           value;
  enum CompOp                       comp;
  RelAttrSqlNode *                  rel_attr;
  std::vector<AttrInfoSqlNode> *    attr_infos;
  AttrInfoSqlNode *                 attr_info;
  Expression *                      expression;
  std::vector<Expression *> *       expression_list;
  std::vector<Value> *              value_list;
  std::vector<std::vector<Value> > *     value_lists;
  std::vector<ConditionSqlNode> *   condition_list;
  std::vector<RelAttrSqlNode> *     rel_attr_list;
  std::vector<std::string> *        relation_list;
  std::vector<std::string> *        id_list;
  std::pair<std::vector<std::string> , std::vector<ConditionSqlNode> > * join_list;
  std::vector<std::pair<std::string,Value> >        *update_list;
  RelAttrOrderNode *                order;
  std::vector<RelAttrOrderNode> *   order_list;
  char *                            string;
  int                               number;
  int                               index_type;
  float                             floats;

#line 149 "yacc_sql.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner);


#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
