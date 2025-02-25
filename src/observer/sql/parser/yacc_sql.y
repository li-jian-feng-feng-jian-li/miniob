
%{

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

%}

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        TABLE
        TABLES
        UNIQUE
        INDEX
        CALC
        SELECT
        ORDER
        BY
        ASC
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        IS
        NOT
        NULL_T
        INT_T
        STRING_T
        FLOAT_T
        DATE_T
        TEXT_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        INNER
        JOIN
        WHERE
        IN
        EXISTS
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
        EXPLAIN
        EQ
        LT
        GT
        LE
        GE
        NE
        LK
        NK

/** union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据 **/
%union {
  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;
  Value *                           value;
  enum CompOp                       comp;
  RelAttrSqlNode *                  rel_attr;
  RelAttrSqlNode *                  all_attr;
  std::vector<AttrInfoSqlNode> *    attr_infos;
  AttrInfoSqlNode *                 attr_info;
  Expression *                      expression;
  std::vector<Expression *> *       expression_list;
  std::vector<Value> *              value_list;
  std::vector<std::vector<Value> > *     value_lists;
  std::vector<ConditionSqlNode> *   condition_list;
  std::vector<RelAttrSqlNode> *     rel_attr_list;
   std::vector<RelAttrSqlNode> *     select_list;
  std::vector<RelAttrSqlNode> *     all_attr_list;
  std::vector<RelAttrSqlNode> *     select_attr_list;
  std::vector<std::string> *        relation_list;
  std::vector<std::string> *        id_list;
  std::pair<std::vector<std::string> , std::vector<ConditionSqlNode> > * join_list;

  UpdateValueSqlNode *              update_value;

  std::vector<std::pair<std::string,UpdateValueSqlNode> >        *update_list;
  RelAttrOrderNode *                order;
  std::vector<RelAttrOrderNode> *   order_list;
  int                               nullable;    //0 ->not null,1 -> null
  char *                            string;
  int                               number;
  int                               index_type;
  float                             floats;
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
%token <string> DATE_STR
%token <string> SSS
%token <string> COUNT 
%token <string> AGG_FUNC
//非终结符

/** type 定义了各种解析后的结果输出的是什么类型。类型对应了 union 中的定义的成员变量名称 **/
%type <number>              type
%type <condition>           condition
%type <value>               value
%type <number>              number
%type <comp>                comp_op
%type <rel_attr>            rel_attr
%type <all_attr>            all_attr
%type <attr_infos>          attr_def_list
%type <attr_info>           attr_def
%type <join_list>           join_list
%type <update_value>        update_value;
%type <update_list>         update_list
%type <index_type>          index_type
%type <value_list>          value_list
%type <value_lists>         value_lists
%type <condition_list>      where
%type <condition_list>      condition_list
%type <rel_attr_list>       select_attr
%type <all_attr_list>       all_attr_list
%type <select_attr_list>    select_attr_list
%type <select_list>         select_list
%type <relation_list>       rel_list
%type <rel_attr_list>       attr_list
%type <nullable>            nullable
%type <order>               order
%type <order_list>          order_list
%type <order_list>          order_attr
%type <id_list>             id_list
%type <expression>          expression
%type <expression_list>     expression_list
%type <sql_node>            calc_stmt
%type <sql_node>            select_stmt
%type <sql_node>            insert_stmt
%type <sql_node>            update_stmt
%type <sql_node>            delete_stmt
%type <sql_node>            create_table_stmt
%type <sql_node>            drop_table_stmt
%type <sql_node>            show_tables_stmt
%type <sql_node>            desc_table_stmt
%type <sql_node>            create_index_stmt
%type <sql_node>            drop_index_stmt
%type <sql_node>            sync_stmt
%type <sql_node>            begin_stmt
%type <sql_node>            commit_stmt
%type <sql_node>            rollback_stmt
%type <sql_node>            load_data_stmt
%type <sql_node>            explain_stmt
%type <sql_node>            set_variable_stmt
%type <sql_node>            help_stmt
%type <sql_node>            exit_stmt
%type <sql_node>            command_wrapper
// commands should be a list but I use a single command instead
%type <sql_node>            commands

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon  //commands or sqls. parser starts here.
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calc_stmt
  | select_stmt
  | insert_stmt
  | update_stmt
  | delete_stmt
  | create_table_stmt
  | drop_table_stmt
  | show_tables_stmt
  | desc_table_stmt
  | create_index_stmt
  | drop_index_stmt
  | sync_stmt
  | begin_stmt
  | commit_stmt
  | rollback_stmt
  | load_data_stmt
  | explain_stmt
  | set_variable_stmt
  | help_stmt
  | exit_stmt
    ;

exit_stmt:      
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SCF_EXIT);
    };

help_stmt:
    HELP {
      $$ = new ParsedSqlNode(SCF_HELP);
    };

sync_stmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

begin_stmt:
    TRX_BEGIN  {
      $$ = new ParsedSqlNode(SCF_BEGIN);
    }
    ;

commit_stmt:
    TRX_COMMIT {
      $$ = new ParsedSqlNode(SCF_COMMIT);
    }
    ;

rollback_stmt:
    TRX_ROLLBACK  {
      $$ = new ParsedSqlNode(SCF_ROLLBACK);
    }
    ;

drop_table_stmt:    /*drop table 语句的语法解析树*/
    DROP TABLE ID {
      $$ = new ParsedSqlNode(SCF_DROP_TABLE);
      $$->drop_table.relation_name = $3;
      free($3);
    };

show_tables_stmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
    ;

desc_table_stmt:
    DESC ID  {
      $$ = new ParsedSqlNode(SCF_DESC_TABLE);
      $$->desc_table.relation_name = $2;
      free($2);
    }
    ;

create_index_stmt:    /*create index 语句的语法解析树*/
    CREATE index_type ID ON ID LBRACE ID id_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      create_index.index_name = $3;
      create_index.relation_name = $5;
      create_index.is_unique = $2;
      if( $8 != nullptr){
        create_index.attribute_name.swap( *$8);
      }
      create_index.attribute_name.emplace_back($7);
      std::reverse(create_index.attribute_name.begin(),create_index.attribute_name.end());
      free($3);
      free($5);
    }
    ;
  
  index_type:
  INDEX {
    $$ = 0;
  } | UNIQUE INDEX {
    $$ = 1;
  }

  id_list:
  /* empty */
    {
      $$ = nullptr;
    }
    | COMMA ID id_list {
      if($3 != nullptr){
        $$ = $3;
      } else {
        $$ = new std::vector<std::string>;
      }
      $$->emplace_back($2);
      std::reverse($$->begin(),$$->end());
    }
    ;

drop_index_stmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(SCF_DROP_INDEX);
      $$->drop_index.index_name = $3;
      $$->drop_index.relation_name = $5;
      free($3);
      free($5);
    }
    ;
create_table_stmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      free($3);

      std::vector<AttrInfoSqlNode> *src_attrs = $6;

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*$5);
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete $5;
    }
    ;
attr_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA attr_def attr_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<AttrInfoSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
    
attr_def:
    ID type LBRACE number RBRACE nullable
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->nullable = $6;
      free($1);
    }
    | ID type nullable
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      /*change the length of type,default*/
      $$->length = 4;
      $$->nullable = $3;
      free($1);
    }
    | ID TEXT_T nullable
    {
      $$ = new AttrInfoSqlNode;
      $$->type = CHARS;
      $$->name = $1;
      /*change the length of type,default*/
      $$->length = 4096;
      $$->nullable = $3;
      free($1);
    }
    ;
number:
    NUMBER {$$ = $1;}
    ;
type:
    INT_T      { $$=INTS; }
    | STRING_T { $$=CHARS; }
    | FLOAT_T  { $$=FLOATS; }
    | DATE_T   { $$=DATES; }    
    ;
nullable:
    /* empty */
    {
      $$ = 0;
    }
    | NULL_T {
      $$ = 1;
    }
    | NOT NULL_T {
      $$ = 0;
    }
    ;
insert_stmt:        /*insert   语句的语法解析树*/
    INSERT INTO ID VALUES LBRACE value value_list RBRACE value_lists 
    {
      $$ = new ParsedSqlNode(SCF_INSERT);
      $$->insertion.relation_name = $3;
      std::vector<Value> vec;
      if ($9 != nullptr){
        $$->insertion.values.swap(*$9);
      }
      if ($7 != nullptr ) {
        vec.swap(*$7);
      }
      vec.emplace_back(*$6);
      std::reverse(vec.begin(), vec.end());
      $$->insertion.values.emplace_back(vec);
      std::reverse($$->insertion.values.begin(), $$->insertion.values.end());
      delete $6;
      free($3);
    }
    ;
value_lists:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA LBRACE value value_list RBRACE value_lists {
      if($6 != nullptr){
        $$ = $6;
      } else {
        $$ = new std::vector<std::vector<Value> >;
      }
      std::vector<Value> vec1;
      if( $4 != nullptr){
        vec1.swap(*$4);
      }
      vec1.emplace_back(*$3);
      std::reverse(vec1.begin(), vec1.end());
      $$->emplace_back(vec1);
      delete $3;
    }
    ;
value_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA value value_list  { 
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Value>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
value:
    NUMBER {
      $$ = new Value((int)$1);
      @$ = @1;
    }
    |FLOAT {
      $$ = new Value((float)$1);
      @$ = @1;
    }
    |DATE_STR {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(tmp,true);
      free(tmp);
    }
    |SSS {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(tmp,false);
      free(tmp);
    }
    |NULL_T {
      const char *tmp = "null";
      $$ = new Value(tmp,false);
      $$->set_null();
    }
    ;
    
delete_stmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID where 
    {
      $$ = new ParsedSqlNode(SCF_DELETE);
      $$->deletion.relation_name = $3;
      if ($4 != nullptr) {
        $$->deletion.conditions.swap(*$4);
        delete $4;
      }
      free($3);
    }
    ;
update_stmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET ID EQ update_value update_list where 
    {
      $$ = new ParsedSqlNode(SCF_UPDATE);
      if($7 != nullptr){
        for(auto p = $7->begin();p != $7->end();p++){
          $$->update.attribute_name.emplace_back((*p).first);
          $$->update.value.emplace_back((*p).second);
        }
      }
      $$->update.relation_name = $2;
      $$->update.attribute_name.emplace_back($4);
      $$->update.value.emplace_back(*$6);
      if ($8 != nullptr) {
        $$->update.conditions.swap(*$8);
        delete $8;
      }
      free($2);
      free($4);
    }
    ;
update_list:
  /* empty */
  {
    $$ = nullptr;
  } 
  | COMMA ID EQ update_value update_list {
    if($5 != nullptr){
      $$ = $5;
    } else {
      $$ = new std::vector<std::pair<std::string,UpdateValueSqlNode> >;
    }
    $$->emplace_back(std::make_pair($2,*$4));
  }
  ;
update_value:
  value
  {
    $$ = new UpdateValueSqlNode(true,*$1);
    delete $1;
  }
  | LBRACE select_stmt RBRACE
  {
    $$ = new UpdateValueSqlNode(false,$2->selection);
    delete $2;
  }
  ;
  
select_stmt:        /*  select 语句的语法解析树*/
    SELECT select_list FROM ID rel_list join_list where order_attr
    {
      $$ = new ParsedSqlNode(SCF_SELECT);
      if ($2 != nullptr) {
        $$->selection.attributes.swap(*$2);
        delete $2;
      }

      if($8 != nullptr) {
        std::reverse($8->begin(),$8->end());
        $$->selection.orders.swap(*$8);
        delete $8;
      }

      if ($5 != nullptr) {
        $$->selection.relations.swap(*$5);
        delete $5;
      }
      $$->selection.relations.push_back($4);
      std::reverse($$->selection.relations.begin(), $$->selection.relations.end());

      if ($7 != nullptr) {
        $$->selection.conditions.swap(*$7);
        delete $7;
      }

      if($6 != nullptr){
        $$->selection.relations.insert($$->selection.relations.end(),($6->first).begin(),($6->first).end());
        $$->selection.conditions.insert($$->selection.conditions.end(),($6->second).begin(),($6->second).end());
      }
      free($4);
    }
    ;
  
calc_stmt:
    CALC expression_list
    {
      $$ = new ParsedSqlNode(SCF_CALC);
      std::reverse($2->begin(), $2->end());
      $$->calc.expressions.swap(*$2);
      delete $2;
    }
    ;

expression_list:
    expression
    {
      $$ = new std::vector<Expression*>;
      $$->emplace_back($1);
    }
    | expression COMMA expression_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Expression *>;
      }
      $$->emplace_back($1);
    }
    ;
expression:
    expression '+' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::ADD, $1, $3, sql_string, &@$);
    }
    | expression '-' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::SUB, $1, $3, sql_string, &@$);
    }
    | expression '*' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::MUL, $1, $3, sql_string, &@$);
    }
    | expression '/' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::DIV, $1, $3, sql_string, &@$);
    }
    | LBRACE expression RBRACE {
      $$ = $2;
      $$->set_name(token_name(sql_string, &@$));
    }
    | '-' expression %prec UMINUS {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, $2, nullptr, sql_string, &@$);
    }
    | value {
      $$ = new ValueExpr(*$1);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    ;
select_list:
  select_attr_list {
      // 这个处理就是直接放入
      $$ = $1;
    }
    ;
//接下来就是列表的修改
select_attr_list:
   /* empty */
    {
      $$ = nullptr;
    }
  // 此处还需要再处理
  | all_attr all_attr_list{
    if ($2 != nullptr){
        $$ = $2;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $$ -> emplace_back(*$1);
      delete $1;
  } 
  ;

// 此处就是加上列表
all_attr_list:
    /* empty */
    {
      $$ = nullptr;
    } | COMMA all_attr all_attr_list {
      if($3 != nullptr){
        $$ = $3;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
// 匹配单个 
all_attr:
   rel_attr
   {
      // 如果直接是引用字段就之间加入
      $$ = $1;// 表示是rel_attr的字段
   }
  | COUNT LBRACE select_attr RBRACE 
	 {
    if($3->size() > 1){
      $$ = new RelAttrSqlNode;
      $$->agg_name = strdup($1);
      $$->isOK = false;
      free($3);
    }
    else{
      // 只有COUNT允许COUNT(*)
		  $$ = new RelAttrSqlNode;
      $$->attribute_name = $3->back().attribute_name;
      $$->agg_name = strdup($1);
      free($1);
    }
	}
  | COUNT LBRACE RBRACE 
	 {
   
      $$ = new RelAttrSqlNode;
      $$->agg_name = strdup($1);
      $$->isOK = false;   
	}
  | AGG_FUNC LBRACE select_attr RBRACE 
  {
    if($3->size() > 1){
      $$ = new RelAttrSqlNode;
      $$->agg_name = strdup($1);
      $$->isOK = false;
    }
    else{
      // 只有COUNT允许COUNT(*)
		  $$ = new RelAttrSqlNode;
      $$->attribute_name = $3->back().attribute_name;
      $$->agg_name = strdup($1);
      free($1);
    }
  }
  | AGG_FUNC LBRACE RBRACE 
	 {
   
      $$ = new RelAttrSqlNode;
      $$->agg_name = strdup($1);
      $$->isOK = false;   
	}
  ;

select_attr:
    // 此处就是直接的一个列表
    rel_attr attr_list {
      if ($2 != nullptr) {
        $$ = $2;
      } else {
        /* 这里创建的是新数组 */
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $$->emplace_back(*$1);
      delete $1;
    }
    ;

order_attr:
    /* empty */
    {
      $$ = nullptr;
    }
    | ORDER BY order order_list {
      if ($4 != nullptr){
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrOrderNode>;
      }
      $$ -> emplace_back(*$3);
      delete $3;
    }
    ;

order:
    rel_attr ASC {
      $$ = new RelAttrOrderNode;
      $$->attribute_name = $1->attribute_name;
      $$->relation_name = $1->relation_name;
      $$->order_by_desc = false;
      delete $1;
    }
    | rel_attr DESC {
      $$ = new RelAttrOrderNode;
      $$->attribute_name = $1->attribute_name;
      $$->relation_name = $1->relation_name;
      $$->order_by_desc = true;
      delete $1;
    }
    | rel_attr {
      $$ = new RelAttrOrderNode;
      $$->attribute_name = $1->attribute_name;
      $$->relation_name = $1->relation_name;
      $$->order_by_desc = false;
      delete $1;
    }
    ;

order_list:
    /* empty */
    {
      $$ = nullptr;
    } | COMMA order order_list {
      if($3 != nullptr){
        $$ = $3;
      } else {
        $$ = new std::vector<RelAttrOrderNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;


// 这里加一个聚合函数的list，用list存储
rel_attr:
   '*'{
      // 此处就是直接获取，增加一个该判断请求
      $$ = new RelAttrSqlNode;
      $$->attribute_name='*';
    }
    | ID {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      free($1);
    }
    | ID DOT ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      free($1);
      free($3);
    }
    | '*' DOT ID{
      $$ = new RelAttrSqlNode;
      $$->relation_name  = "*";
      $$->attribute_name = $3;
      free($3);
    }
    | '*' DOT '*'{
      $$ = new RelAttrSqlNode;
      $$->relation_name  = "*";
      $$->attribute_name = "*";
      
    }
    | ID DOT '*'{
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = "*";    
    }
  ;

attr_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA rel_attr attr_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }

      $$->emplace_back(*$2);
      delete $2;
    }
    ;

rel_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA ID rel_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<std::string>;
      }

      $$->push_back($2);
      free($2);
    }
    ;
join_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | INNER JOIN ID ON condition_list join_list {
      if($6 != nullptr){
        $$ = $6;
      } else {
        $$ = new std::pair<std::vector<std::string>,std::vector<ConditionSqlNode> >;
      }
      $$->first.emplace_back($3);
      std::reverse($$->first.begin(),$$->first.end());
      $$->second.insert($$->second.end(),$5->begin(),$5->end());                                          
    }
where:
    /* empty */
    {
      $$ = nullptr;
    }
    | WHERE condition_list {
      $$ = $2;  
    }
    ;
condition_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | condition {
      $$ = new std::vector<ConditionSqlNode>;
      $$->emplace_back(*$1);
      delete $1;
    }
    | condition AND condition_list {
      $$ = $3;
      $$->emplace_back(*$1);
      delete $1;
    }
    ;
condition:
    rel_attr comp_op value
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      $$->right_is_attr = 0;
      $$->right_value = *$3;
      $$->comp = $2;

      delete $1;
      delete $3;
    }
    | value comp_op value 
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->left_value = *$1;
      $$->right_is_attr = 0;
      $$->right_value = *$3;
      $$->comp = $2;

      delete $1;
      delete $3;
    }
    | rel_attr comp_op rel_attr
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      $$->right_is_attr = 1;
      $$->right_attr = *$3;
      $$->comp = $2;

      delete $1;
      delete $3;
    }
    | value comp_op rel_attr
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->left_value = *$1;
      $$->right_is_attr = 1;
      $$->right_attr = *$3;
      $$->comp = $2;

      delete $1;
      delete $3;
    }
    | value comp_op LBRACE value value_list RBRACE
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->left_value = *$1;
      if($5 != nullptr){
        $$->in_values.swap(*$5);
      }
      $$->in_values.emplace_back(*$4);
      $$->comp = $2;

      delete $1;
      delete $4;
      delete $5;
    }
    | rel_attr comp_op LBRACE value value_list RBRACE
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      if($5 != nullptr){
        $$->in_values.swap(*$5);
      }
      $$->in_values.emplace_back(*$4);
      $$->comp = $2;

      delete $1;
      delete $4;
      delete $5;
    }
    | value comp_op LBRACE select_stmt RBRACE
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->left_value = *$1;
      $$->need_sub_query = true;
      $$->sub_select = &($4->selection);
      $$->comp = $2;

      delete $1;
    }
    | rel_attr comp_op LBRACE select_stmt RBRACE
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      $$->need_sub_query = true;
      $$->sub_select = &($4->selection);
      $$->comp = $2;

      delete $1;
    }
    | EXISTS LBRACE select_stmt RBRACE
    {
      $$ = new ConditionSqlNode;
      $$->need_sub_query = true;
      $$->sub_select = &($3->selection);
      $$->comp = EX;
    }
    | NOT EXISTS LBRACE select_stmt RBRACE
    {
      $$ = new ConditionSqlNode;
      $$->need_sub_query = true;
      $$->sub_select = &($4->selection);
      $$->comp = NOT_EX;
    }
    ;

comp_op:
      EQ { $$ = EQUAL_TO; }
    | LT { $$ = LESS_THAN; }
    | GT { $$ = GREAT_THAN; }
    | LE { $$ = LESS_EQUAL; }
    | GE { $$ = GREAT_EQUAL; }
    | NE { $$ = NOT_EQUAL; }
    | LK { $$ = LIKE;}
    | NK { $$ = NLIKE;}
    | IS NOT { $$ = IS_NOT_NULL;}
    | IS { $$ = IS_NULL;}
    | IN { $$ = IN_;}
    | NOT IN { $$ = NOT_IN;}
    ;

load_data_stmt:
    LOAD DATA INFILE SSS INTO TABLE ID 
    {
      char *tmp_file_name = common::substr($4, 1, strlen($4) - 2);
      
      $$ = new ParsedSqlNode(SCF_LOAD_DATA);
      $$->load_data.relation_name = $7;
      $$->load_data.file_name = tmp_file_name;
      free($7);
      free(tmp_file_name);
    }
    ;

explain_stmt:
    EXPLAIN command_wrapper
    {
      $$ = new ParsedSqlNode(SCF_EXPLAIN);
      $$->explain.sql_node = std::unique_ptr<ParsedSqlNode>($2);
    }
    ;

set_variable_stmt:
    SET ID EQ value
    {
      $$ = new ParsedSqlNode(SCF_SET_VARIABLE);
      $$->set_variable.name  = $2;
      $$->set_variable.value = *$4;
      free($2);
      delete $4;
    }
    ;

opt_semicolon: /*empty*/
    | SEMICOLON
    ;
%%
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
