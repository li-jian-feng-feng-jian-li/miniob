/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/08/16.
//

#include "sql/optimizer/logical_plan_generator.h"

#include "sql/operator/logical_operator.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/sort_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"

#include "sql/stmt/stmt.h"
#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/update_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/explain_stmt.h"

using namespace std;

RC LogicalPlanGenerator::create(Stmt *stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  RC rc = RC::SUCCESS;
  switch (stmt->type()) {
    case StmtType::CALC: {
      CalcStmt *calc_stmt = static_cast<CalcStmt *>(stmt);
      rc                  = create_plan(calc_stmt, logical_operator);
    } break;

    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);
      rc                      = create_plan(select_stmt, logical_operator);
    } break;

    case StmtType::INSERT: {
      InsertStmt *insert_stmt = static_cast<InsertStmt *>(stmt);
      rc                      = create_plan(insert_stmt, logical_operator);
    } break;

    case StmtType::DELETE: {
      DeleteStmt *delete_stmt = static_cast<DeleteStmt *>(stmt);
      rc                      = create_plan(delete_stmt, logical_operator);
    } break;

    case StmtType::UPDATE: {
      UpdateStmt *update_stmt = static_cast<UpdateStmt *>(stmt);
      rc                      = create_plan(update_stmt, logical_operator);
    } break;

    case StmtType::EXPLAIN: {
      ExplainStmt *explain_stmt = static_cast<ExplainStmt *>(stmt);
      rc                        = create_plan(explain_stmt, logical_operator);
    } break;
    default: {
      rc = RC::UNIMPLENMENT;
    }
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(CalcStmt *calc_stmt, std::unique_ptr<LogicalOperator> &logical_operator)
{
  logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  // 此处是查询的逻辑字段
  unique_ptr<LogicalOperator> table_oper(nullptr);

  const std::vector<Table *> &tables     = select_stmt->tables();
  const std::vector<Field>   &all_fields = select_stmt->query_fields();
  for (Table *table : tables) {
    std::vector<Field> fields;
    for (const Field &field : all_fields) {
      if (0 == strcmp(field.table_name(), table->name())) {
        fields.push_back(field);
      }
    }

    unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, true /*readonly*/));
    if (table_oper == nullptr) {
      table_oper = std::move(table_get_oper);
    } else {
      JoinLogicalOperator *join_oper = new JoinLogicalOperator;
      join_oper->add_child(std::move(table_oper));
      join_oper->add_child(std::move(table_get_oper));
      table_oper = unique_ptr<LogicalOperator>(join_oper);
    }
  }

  unique_ptr<LogicalOperator> predicate_oper;
  RC                          rc = create_plan(select_stmt->filter_stmt(), predicate_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
    return rc;
  }
  // 获取是否为count_star 用来操作逻辑运算符
  bool                        is_count_star = select_stmt->get_is_count_star();
  bool                        need_to_agg   = !select_stmt->agg_fields().empty();
  bool                        need_to_order = !select_stmt->order_fields().empty();
  unique_ptr<LogicalOperator> order_oper(nullptr);
  if (need_to_agg | need_to_order) {
    // 此处创建自运算符
    if (is_count_star) {
      unique_ptr<LogicalOperator> sort_oper(
          new SortLogicalOperator(select_stmt->order_fields(), select_stmt->agg_fields(), true));
      order_oper = std::move(sort_oper);
    } else {
      unique_ptr<LogicalOperator> sort_oper(
          new SortLogicalOperator(select_stmt->order_fields(), select_stmt->agg_fields()));
      order_oper = std::move(sort_oper);
    }
  }

  unique_ptr<LogicalOperator> project_oper(new ProjectLogicalOperator(all_fields, need_to_agg));
  // if (predicate_oper) {
  //   if (table_oper) {
  //     predicate_oper->add_child(std::move(table_oper));
  //   }
  //   project_oper->add_child(std::move(predicate_oper));
  // } else {
  //   if (table_oper) {
  //     project_oper->add_child(std::move(table_oper));
  //   }
  // }
  if (order_oper) {
    if (predicate_oper) {
      if (table_oper) {
        predicate_oper->add_child(std::move(table_oper));
      }
      order_oper->add_child(std::move(predicate_oper));
    } else {
      if (table_oper) {
        order_oper->add_child(std::move(table_oper));
      }
    }
    project_oper->add_child(std::move(order_oper));
  } else {
    if (predicate_oper) {
      if (table_oper) {
        predicate_oper->add_child(std::move(table_oper));
      }
      project_oper->add_child(std::move(predicate_oper));
    } else {
      if (table_oper) {
        project_oper->add_child(std::move(table_oper));
      }
    }
  }

  logical_operator.swap(project_oper);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(FilterStmt *filter_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  std::vector<unique_ptr<Expression> > cmp_exprs;
  const std::vector<FilterUnit *>     &filter_units = filter_stmt->filter_units();
  // in/not int/exists/not exists
  std::vector<std::pair<Field, CompOp> > other_exprs;
  std::vector<SelectStmt *>              subqs;
  for (const FilterUnit *filter_unit : filter_units) {
    CompOp           op               = filter_unit->comp();
    const FilterObj &filter_obj_left  = filter_unit->left();
    const FilterObj &filter_obj_right = filter_unit->right();
    if (!filter_obj_right.is_subq) {
      unique_ptr<Expression> left(filter_obj_left.is_attr
                                      ? static_cast<Expression *>(new FieldExpr(filter_obj_left.field))
                                      : static_cast<Expression *>(new ValueExpr(filter_obj_left.value)));

      unique_ptr<Expression> right(
          filter_obj_right.is_attr            ? static_cast<Expression *>(new FieldExpr(filter_obj_right.field))
          : (filter_obj_right.values.empty()) ? static_cast<Expression *>(new ValueExpr(filter_obj_right.value))
                                              : static_cast<Expression *>(new ValuesExpr(filter_obj_right.values)));

      ComparisonExpr *cmp_expr = new ComparisonExpr(filter_unit->comp(), std::move(left), std::move(right));
      cmp_exprs.emplace_back(cmp_expr);
    } else {
      other_exprs.emplace_back(make_pair(filter_obj_left.field, op));
      subqs.emplace_back(filter_obj_right.sub_select);
    }
  }

  unique_ptr<PredicateLogicalOperator> predicate_oper;

  unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(ConjunctionExpr::Type::AND, cmp_exprs));
  predicate_oper =
      unique_ptr<PredicateLogicalOperator>(new PredicateLogicalOperator(std::move(conjunction_expr), other_exprs));
  RC rc;
  for (auto &p : subqs) {
    unique_ptr<LogicalOperator> select_oper;
    rc = create_plan(p, select_oper);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    predicate_oper->add_child(std::move(select_oper));
  }

  logical_operator = std::move(predicate_oper);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(InsertStmt *insert_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table                 *table           = insert_stmt->table();
  InsertLogicalOperator *insert_operator = new InsertLogicalOperator(table, *insert_stmt->values());
  logical_operator.reset(insert_operator);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(DeleteStmt *delete_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table             *table       = delete_stmt->table();
  FilterStmt        *filter_stmt = delete_stmt->filter_stmt();
  std::vector<Field> fields;
  for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(i);
    fields.push_back(Field(table, field_meta));
  }
  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, false /*readonly*/));

  unique_ptr<LogicalOperator> predicate_oper;
  RC                          rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalOperator> delete_oper(new DeleteLogicalOperator(table));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    delete_oper->add_child(std::move(predicate_oper));
  } else {
    delete_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(delete_oper);
  return rc;
}

/**
 * @author ljf
 */
RC LogicalPlanGenerator::create_plan(UpdateStmt *update_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  LOG_INFO("update logical plan created!");
  Table                          *table       = update_stmt->table();
  FilterStmt                     *filter_stmt = update_stmt->filter_stmt();
  std::vector<const char *>       field_name  = update_stmt->field_name();
  std::vector<UpdateValueSqlNode> value       = update_stmt->value();
  std::vector<Field>              fields;
  for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(i);
    fields.push_back(Field(table, field_meta));
  }

  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, false /*readonly*/));

  unique_ptr<LogicalOperator> predicate_oper;
  RC                          rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalOperator> update_oper(new UpdateLogicalOperator(table, value, field_name));

  // add select_oper
  for (int i = 0; i < update_stmt->select_stmt().size(); i++) {
    unique_ptr<LogicalOperator> select_oper;
    RC                          rc = create_plan((update_stmt->select_stmt())[i], select_oper);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    update_oper->add_child(std::move(select_oper));
  }

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    update_oper->add_child(std::move(predicate_oper));
  } else {
    update_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(update_oper);

  return rc;
}

RC LogicalPlanGenerator::create_plan(ExplainStmt *explain_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Stmt                       *child_stmt = explain_stmt->child();
  unique_ptr<LogicalOperator> child_oper;
  RC                          rc = create(child_stmt, child_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child operator. rc=%s", strrc(rc));
    return rc;
  }

  logical_operator = unique_ptr<LogicalOperator>(new ExplainLogicalOperator);
  logical_operator->add_child(std::move(child_oper));
  return rc;
}
