/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/12/14.
//

#include <utility>
#include "sql/optimizer/physical_plan_generator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/table_scan_physical_operator.h"
#include "sql/operator/index_scan_physical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/predicate_physical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/insert_physical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/update_physical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/delete_physical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/explain_physical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/join_physical_operator.h"
#include "sql/operator/sort_logical_operator.h"
#include "sql/operator/sort_physical_operator.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/calc_physical_operator.h"
#include "sql/expr/expression.h"
#include "storage/index/index.h"
#include "common/log/log.h"

using namespace std;

RC PhysicalPlanGenerator::create(LogicalOperator &logical_operator, unique_ptr<PhysicalOperator> &oper)
{
  RC rc = RC::SUCCESS;

  switch (logical_operator.type()) {
    case LogicalOperatorType::CALC: {
      return create_plan(static_cast<CalcLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::TABLE_GET: {
      return create_plan(static_cast<TableGetLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::PREDICATE: {
      return create_plan(static_cast<PredicateLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::PROJECTION: {
      return create_plan(static_cast<ProjectLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::INSERT: {
      return create_plan(static_cast<InsertLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::UPDATE: {
      return create_plan(static_cast<UpdateLogicalOperator &>(logical_operator), oper);
    }

    case LogicalOperatorType::DELETE: {
      return create_plan(static_cast<DeleteLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::EXPLAIN: {
      return create_plan(static_cast<ExplainLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::JOIN: {
      return create_plan(static_cast<JoinLogicalOperator &>(logical_operator), oper);
    } break;
    case LogicalOperatorType::SORT: {
      return create_plan(static_cast<SortLogicalOperator &>(logical_operator), oper);
    }
    default: {
      return RC::INVALID_ARGUMENT;
    }
  }
  return rc;
}

RC PhysicalPlanGenerator::create_plan(TableGetLogicalOperator &table_get_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<Expression>> &predicates = table_get_oper.predicates();
  std::reverse(predicates.begin(), predicates.end());
  // 看看是否有可以用于索引查找的表达式
  Table *table = table_get_oper.table();

  Index             *index       = nullptr;
  int                index_index = 0;
  int                total_len   = 0;
  std::vector<Value> index_value;
  for (int i = 0; i < predicates.size(); i++) {
    ValueExpr *value_expr = nullptr;
    auto      &expr       = predicates[i];
    if (expr->type() == ExprType::COMPARISON) {
      auto comparison_expr = static_cast<ComparisonExpr *>(expr.get());
      // 简单处理，就找等值查询
      if (comparison_expr->comp() != EQUAL_TO) {
        continue;
      }

      unique_ptr<Expression> &left_expr  = comparison_expr->left();
      unique_ptr<Expression> &right_expr = comparison_expr->right();
      // 左右比较的一边最少是一个值
      if (left_expr->type() != ExprType::VALUE && right_expr->type() != ExprType::VALUE) {
        continue;
      }

      FieldExpr *field_expr = nullptr;
      if (left_expr->type() == ExprType::FIELD) {
        ASSERT(right_expr->type() == ExprType::VALUE, "right expr should be a value expr while left is field expr");
        field_expr = static_cast<FieldExpr *>(left_expr.get());
        value_expr = static_cast<ValueExpr *>(right_expr.get());
      } else if (right_expr->type() == ExprType::FIELD) {
        ASSERT(left_expr->type() == ExprType::VALUE, "left expr should be a value expr while right is a field expr");
        field_expr = static_cast<FieldExpr *>(right_expr.get());
        value_expr = static_cast<ValueExpr *>(left_expr.get());
      }

      if (field_expr == nullptr) {
        continue;
      }

      const Field &field = field_expr->field();
      index              = table->find_index_by_field(field.field_name());

      if (nullptr != index) {
        index_index = i;
        index_value.emplace_back(value_expr->get_value());
        // LOG_DEBUG("index key add a value %d",value_expr->get_value().get_int());
        total_len += value_expr->get_value().length();
        break;
      }
    }
  }

  if (index != nullptr) {
    // ASSERT(index_value[0], "got an index but value expr is null ?");
    const std::vector<std::string> fields = index->index_meta().fields();
    // if (fields.size() == 1) {
    //   // single index
    //   const Value               &value           = index_value[0];
    //   IndexScanPhysicalOperator *index_scan_oper = new IndexScanPhysicalOperator(
    //       table, index, table_get_oper.readonly(), &value, true /*left_inclusive*/, &value, true
    //       /*right_inclusive*/);

    //   index_scan_oper->set_predicates(std::move(predicates));
    //   oper = unique_ptr<PhysicalOperator>(index_scan_oper);
    //   LOG_TRACE("use single index scan");
    // } else {
    // mutiple index
    // do not optimize the order
    int comp_num    = 1;
    int index_order = 1;
    LOG_DEBUG("find index start from %d,predicates size = %d",index_index,predicates.size());
    for (int i = index_index + 1; i < predicates.size(); i++) {
      auto &expr = predicates[i];

      if (expr->type() == ExprType::COMPARISON) {
        auto comparison_expr = static_cast<ComparisonExpr *>(expr.get());
        // 简单处理，就找等值查询
        if (comparison_expr->comp() != EQUAL_TO) {
          continue;
        }

        unique_ptr<Expression> &left_expr  = comparison_expr->left();
        unique_ptr<Expression> &right_expr = comparison_expr->right();
        // 左右比较的一边最少是一个值
        if (left_expr->type() != ExprType::VALUE && right_expr->type() != ExprType::VALUE) {
          continue;
        }

        ValueExpr *value_expr = nullptr;
        FieldExpr *field_expr = nullptr;
        if (left_expr->type() == ExprType::FIELD) {
          ASSERT(right_expr->type() == ExprType::VALUE, "right expr should be a value expr while left is field expr");
          field_expr = static_cast<FieldExpr *>(left_expr.get());
          value_expr = static_cast<ValueExpr *>(right_expr.get());
        } else if (right_expr->type() == ExprType::FIELD) {
          ASSERT(left_expr->type() == ExprType::VALUE, "left expr should be a value expr while right is a field expr");
          field_expr = static_cast<FieldExpr *>(right_expr.get());
          value_expr = static_cast<ValueExpr *>(left_expr.get());
        }

        if (field_expr == nullptr) {
          continue;
        }

        const Field &field = field_expr->field();

        if (0 == strcmp(field.field_name(), fields[index_order].c_str())) {
          comp_num++;
          index_order++;
          index_value.emplace_back(value_expr->get_value());
          // LOG_DEBUG("index key add a value %d",value_expr->get_value().get_int());
          total_len += value_expr->get_value().length();
        } else {
          LOG_INFO("next mutiple index field should be:%s,now is %s",fields[index_order],field.field_name());
          break;
        }
      }
    }

    // char *values     = (char *)malloc(total_len);
    // int   key_offset = 0;
    // LOG_DEBUG("index key contains %d fields.",index_value.size());
    // for (int i = 0; i < index_value.size(); i++) {
    //   memcpy(values + key_offset, index_value[i].data(), index_value[i].length());
    //   // LOG_DEBUG("add value %d", *(int *)((void *)(values+key_offset)));
    //   key_offset += index_value[i].length();
    // }
    // LOG_DEBUG("values size %d",sizeof(values) );
    std::vector<bool> left_include;
    std::vector<bool> rigth_include;
    for (int i = 0; i < index_value.size(); i++) {
      left_include.emplace_back(true);
      rigth_include.emplace_back(true);
    }

    IndexScanPhysicalOperator *index_scan_oper = new IndexScanPhysicalOperator(
        table, index, table_get_oper.readonly(), index_value, left_include, index_value, rigth_include, comp_num);
    index_scan_oper->set_predicates(std::move(predicates));
    oper = unique_ptr<PhysicalOperator>(index_scan_oper);
    LOG_TRACE("use index scan");
  }
  //}
  else {
    auto table_scan_oper = new TableScanPhysicalOperator(table, table_get_oper.readonly());
    table_scan_oper->set_predicates(std::move(predicates));
    oper = unique_ptr<PhysicalOperator>(table_scan_oper);
    LOG_TRACE("use table scan");
  }

  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(PredicateLogicalOperator &pred_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &children_opers = pred_oper.children();
  // ASSERT(children_opers.size() == 1, "predicate logical operator's sub oper number should be 1");

  vector<unique_ptr<Expression>> &expressions = pred_oper.expressions();
  ASSERT(expressions.size() == 1, "predicate logical operator's expression should be 1");

  std::vector<std::pair<Field, CompOp>> other_exprs = pred_oper.other_exprs();
  unique_ptr<Expression>                expression  = std::move(expressions.front());
  oper  = unique_ptr<PhysicalOperator>(new PredicatePhysicalOperator(std::move(expression), other_exprs));
  RC rc = RC::SUCCESS;
  
  for (auto &p : children_opers) {
    LogicalOperator &child_oper = *p;
    unique_ptr<PhysicalOperator> child_phy_oper;
    rc = create(child_oper, child_phy_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create child operator of predicate operator. rc=%s", strrc(rc));
      return rc;
    }
    oper->add_child(std::move(child_phy_oper));
  }
  LOG_TRACE("create a predicate physical operator");
  return rc;
}

RC PhysicalPlanGenerator::create_plan(SortLogicalOperator &sort_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &child_opers = sort_oper.children();
  unique_ptr<PhysicalOperator>         child_phy_oper;
  // 此处定义物理算子的情况
  bool is_count_star = sort_oper.get_count_star();

  RC rc = RC::SUCCESS;
  if (!child_opers.empty()) {
    LogicalOperator *child_oper = child_opers.front().get();
    rc                          = create(*child_oper, child_phy_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create Sort logical operator's child physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  SortPhysicalOperator *sort_operator = new SortPhysicalOperator(sort_oper.order_fields(),sort_oper.agg_fields(),is_count_star);


  if (child_phy_oper) {
    sort_operator->add_child(std::move(child_phy_oper));
  }

  oper = unique_ptr<PhysicalOperator>(sort_operator);

  LOG_TRACE("create a sort physical operator");
  return rc;
}

RC PhysicalPlanGenerator::create_plan(ProjectLogicalOperator &project_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &child_opers = project_oper.children();

  unique_ptr<PhysicalOperator> child_phy_oper;

  RC rc = RC::SUCCESS;
  if (!child_opers.empty()) {
    LogicalOperator *child_oper = child_opers.front().get();
    rc                          = create(*child_oper, child_phy_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create project logical operator's child physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  ProjectPhysicalOperator *project_operator = new ProjectPhysicalOperator;
  const vector<Field>     &project_fields   = project_oper.fields();
  for (const Field &field : project_fields) {
    project_operator->add_projection(field.table(), field.meta());
  }

  if (child_phy_oper) {
    project_operator->add_child(std::move(child_phy_oper));
  }
  if(project_oper.getAgg()){
    // 此处表示需要聚合
    project_operator->set_agg();
  }

  oper = unique_ptr<PhysicalOperator>(project_operator);

  LOG_TRACE("create a project physical operator");
  return rc;
}

RC PhysicalPlanGenerator::create_plan(InsertLogicalOperator &insert_oper, unique_ptr<PhysicalOperator> &oper)
{
  Table                  *table           = insert_oper.table();
  vector<vector<Value>>  &values          = insert_oper.values_vecs();
  InsertPhysicalOperator *insert_phy_oper = new InsertPhysicalOperator(table, std::move(values));
  oper.reset(insert_phy_oper);
  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(DeleteLogicalOperator &delete_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &child_opers = delete_oper.children();

  unique_ptr<PhysicalOperator> child_physical_oper;

  RC rc = RC::SUCCESS;
  if (!child_opers.empty()) {
    LogicalOperator *child_oper = child_opers.front().get();
    rc                          = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  oper = unique_ptr<PhysicalOperator>(new DeletePhysicalOperator(delete_oper.table()));

  if (child_physical_oper) {
    oper->add_child(std::move(child_physical_oper));
  }
  return rc;
}

/**
 * @author ljf
 */
RC PhysicalPlanGenerator::create_plan(UpdateLogicalOperator &update_oper, unique_ptr<PhysicalOperator> &oper)
{
  LOG_DEBUG("update physical plan created!");
  vector<unique_ptr<LogicalOperator>> &child_opers = update_oper.children();

  oper = unique_ptr<PhysicalOperator>(
      new UpdatePhysicalOperator(update_oper.table(), update_oper.value(), update_oper.field_name()));

  RC rc = RC::SUCCESS;
  if (!child_opers.empty()) {
    for (auto &p : child_opers) {
      unique_ptr<PhysicalOperator> child_physical_oper;
      LogicalOperator             *child_oper = p.get();
      rc                                      = create(*child_oper, child_physical_oper);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to create physical operator. rc=%s", strrc(rc));
        return rc;
      }
      if (child_physical_oper) {
        oper->add_child(std::move(child_physical_oper));
      }
    }
  }
  return rc;
}

RC PhysicalPlanGenerator::create_plan(ExplainLogicalOperator &explain_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &child_opers = explain_oper.children();

  RC                           rc = RC::SUCCESS;
  unique_ptr<PhysicalOperator> explain_physical_oper(new ExplainPhysicalOperator);
  for (unique_ptr<LogicalOperator> &child_oper : child_opers) {
    unique_ptr<PhysicalOperator> child_physical_oper;
    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create child physical operator. rc=%s", strrc(rc));
      return rc;
    }

    explain_physical_oper->add_child(std::move(child_physical_oper));
  }

  oper = std::move(explain_physical_oper);
  return rc;
}

RC PhysicalPlanGenerator::create_plan(JoinLogicalOperator &join_oper, unique_ptr<PhysicalOperator> &oper)
{
  RC rc = RC::SUCCESS;

  vector<unique_ptr<LogicalOperator>> &child_opers = join_oper.children();
  if (child_opers.size() != 2) {
    LOG_WARN("join operator should have 2 children, but have %d", child_opers.size());
    return RC::INTERNAL;
  }

  unique_ptr<PhysicalOperator> join_physical_oper(new NestedLoopJoinPhysicalOperator);
  for (auto &child_oper : child_opers) {
    unique_ptr<PhysicalOperator> child_physical_oper;
    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create physical child oper. rc=%s", strrc(rc));
      return rc;
    }

    join_physical_oper->add_child(std::move(child_physical_oper));
  }

  oper = std::move(join_physical_oper);
  return rc;
}

RC PhysicalPlanGenerator::create_plan(CalcLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper)
{
  RC                    rc        = RC::SUCCESS;
  CalcPhysicalOperator *calc_oper = new CalcPhysicalOperator(std::move(logical_oper.expressions()));
  oper.reset(calc_oper);
  return rc;
}
