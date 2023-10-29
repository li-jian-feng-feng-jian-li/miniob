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
// Created by WangYunlai on 2022/6/27.
//

#include "common/log/log.h"
#include "sql/operator/predicate_physical_operator.h"
#include "storage/record/record.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/field/field.h"

PredicatePhysicalOperator::PredicatePhysicalOperator(
    std::unique_ptr<Expression> expr, std::vector<std::pair<Field, CompOp> > other_exprs)
    : expression_(std::move(expr))
{
  other_exprs_.swap(other_exprs);
  return_more_than_one_field.resize(other_exprs_.size(), false);
  ASSERT(expression_->value_type() == BOOLEANS, "predicate's expression should be BOOLEAN type");
}

RC PredicatePhysicalOperator::open(Trx *trx)
{
  RC rc = RC::SUCCESS;
  LOG_DEBUG("childrensize = %d",children_.size());
  for (auto &p : children_) {
    rc = p->open(trx);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to open child operator: %s", strrc(rc));
      return rc;
    }
  }
  return rc;
}

RC PredicatePhysicalOperator::next()
{
  RC rc = RC::SUCCESS;
  // handle sub_query
  if (!finish_sub_select_) {
    for (int i = 0; i < other_exprs_.size(); i++) {
      PhysicalOperator  *child = children_[child_oper_index++].get();
      std::vector<Value> select_value;
      while (RC::SUCCESS == (rc = child->next())) {
        Tuple *ctuple = child->current_tuple();
        if (nullptr == ctuple) {
          LOG_WARN("failed to get current record: %s", strrc(rc));
          return rc;
        }

        ProjectTuple *project_tuple = static_cast<ProjectTuple *>(ctuple);
        if (project_tuple->cell_num() != 1) {
          return_more_than_one_field[child_oper_index - 1] = true;
        }

        Value value;
        rc = project_tuple->cell_at(0, value);
        if (rc != RC::SUCCESS) {
          LOG_WARN("failed to find cell at 0: %s",strrc(rc));
          return rc;
        }
        select_value.emplace_back(value);
      }
      all_values_in_subq.emplace_back(select_value);
    }

    finish_sub_select_ = true;
  }

  PhysicalOperator *oper = children_.back().get();

  while (RC::SUCCESS == (rc = oper->next())) {
    Tuple *tuple = oper->current_tuple();
    if (nullptr == tuple) {
      rc = RC::INTERNAL;
      LOG_WARN("failed to get tuple from operator");
      break;
    }

    Value value;
    rc = expression_->get_value(*tuple, value);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    bool is_correct = value.get_boolean();

    int filter_index = 0;
    // handle sub_q
    for (auto &p : other_exprs_) {
      CompOp op    = p.second;
      Field  field = p.first;
      Value  fvalue;
      if (op != EX && op != NOT_EX) {
        TupleCellSpec spec(field.table_name(), field.field_name());
        tuple->find_cell(spec, fvalue);
      }

      std::vector<Value> &select_value = all_values_in_subq[filter_index];

      switch (op) {
        case EQUAL_TO: {
          if (return_more_than_one_field[filter_index]) {
            LOG_WARN("select operation returns with more than one field!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          }
          if (select_value.size() > 1) {
            LOG_WARN("select operation returns more than one row!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          } else if (select_value.size() == 0) {
            is_correct = false;
          } else {
            is_correct &= (fvalue.compare(select_value[0]) == 0 ? true : false);
          }
        } break;
        case LESS_EQUAL: {
          if (return_more_than_one_field[filter_index]) {
            LOG_WARN("select operation returns with more than one field!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          }
          if (select_value.size() > 1) {
            LOG_WARN("select operation returns more than one row!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          } else if (select_value.size() == 0) {
            is_correct = false;
          } else {
            is_correct &= (fvalue.compare(select_value[0]) <= 0 ? true : false);
          }
        } break;
        case NOT_EQUAL: {
          if (return_more_than_one_field[filter_index]) {
            LOG_WARN("select operation returns with more than one field!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          }
          if (select_value.size() > 1) {
            LOG_WARN("select operation returns more than one row!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          } else if (select_value.size() == 0) {
            is_correct = false;
          } else {
            is_correct &= (fvalue.compare(select_value[0]) != 0 ? true : false);
          }
        } break;
        case LESS_THAN: {
          if (return_more_than_one_field[filter_index]) {
            LOG_WARN("select operation returns with more than one field!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          }
          if (select_value.size() > 1) {
            LOG_WARN("select operation returns more than one row!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          } else if (select_value.size() == 0) {
            is_correct = false;
          } else {
            is_correct &= (fvalue.compare(select_value[0]) < 0 ? true : false);
          }
        } break;
        case GREAT_EQUAL: {
          if (return_more_than_one_field[filter_index]) {
            LOG_WARN("select operation returns with more than one field!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          }
          if (select_value.size() > 1) {
            LOG_WARN("select operation returns more than one row!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          } else if (select_value.size() == 0) {
            is_correct = false;
          } else {
            is_correct &= (fvalue.compare(select_value[0]) >= 0 ? true : false);
          }
        } break;
        case GREAT_THAN: {
          if (return_more_than_one_field[filter_index]) {
            LOG_WARN("select operation returns with more than one field!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          }
          if (select_value.size() > 1) {
            LOG_WARN("select operation returns more than one row!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          } else if (select_value.size() == 0) {
            is_correct = false;
          } else {
            is_correct &= (fvalue.compare(select_value[0]) > 0 ? true : false);
          }
        } break;
        case IN_: {
          if (return_more_than_one_field[filter_index]) {
            LOG_WARN("select operation returns with more than one field!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          }
          bool find = false;
          for (int i = 0; i < select_value.size(); i++) {
            if (select_value[i].is_null())
              continue;
            if (fvalue.compare(select_value[i]) == 0) {
              find = true;
              break;
            }
          }
          is_correct &= find;
        } break;
        case NOT_IN: {
          if (return_more_than_one_field[filter_index]) {
            LOG_WARN("select operation returns with more than one field!");
            rc = RC::INVALID_ARGUMENT;
            return rc;
          }
          bool find = false;
          for (int i = 0; i < select_value.size(); i++) {
            if (select_value[i].is_null()) {
              find = true;
              break;
            }
            if (fvalue.compare(select_value[i]) == 0) {
              find = true;
              break;
            }
          }
          is_correct &= (!find);
        } break;
        case EX: {
          is_correct &= (select_value.size() > 0);
        } break;
        case NOT_EX: {
          is_correct &= (select_value.size() == 0);
        } break;
        default: {
          LOG_WARN("invalid op: %d",op);
          rc = RC::INTERNAL;
          return rc;
        } break;
      }

      if (!is_correct)
        break;
      filter_index++;
    }
    if (is_correct) {
      correct_tuple_.emplace_back(tuple);
      return rc;
    }
  }

  return rc;
}

RC PredicatePhysicalOperator::close()
{
  for (auto &p : children_) {
    p->close();
  }
  return RC::SUCCESS;
}

Tuple *PredicatePhysicalOperator::current_tuple() { return correct_tuple_.back(); }
