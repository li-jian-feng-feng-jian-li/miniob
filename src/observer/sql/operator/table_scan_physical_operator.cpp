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
// Created by WangYunlai on 2021/6/9.
//

#include "sql/operator/table_scan_physical_operator.h"
#include "storage/table/table.h"
#include "event/sql_debug.h"

using namespace std;

RC TableScanPhysicalOperator::open(Trx *trx)
{
  RC rc = table_->get_record_scanner(record_scanner_, trx, readonly_);
  if (rc == RC::SUCCESS) {
    // for (int i = 0; i < 2000; i++) {
    //   tuple_[i].set_schema(table_, table_->table_meta().field_metas());
    // }
  }
  trx_ = trx;
  return rc;
}

RC TableScanPhysicalOperator::next()
{
  if (!record_scanner_.has_next()) {
    return RC::RECORD_EOF;
  }

  RC   rc            = RC::SUCCESS;
  bool filter_result = false;
  while (record_scanner_.has_next()) {
    rc = record_scanner_.next(current_record_[index]);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    tuple_[index].set_schema(table_, table_->table_meta().field_metas());
    tuple_[index].set_record(&(current_record_[index]));

    rc = filter(tuple_[index], filter_result);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (filter_result) {
      sql_debug("get a tuple: %s", tuple_[index].to_string().c_str());
      correct_tuple_[correct_index_] = (tuple_[index]);
      index++;
      correct_index_++;
      break;
    } else {
      sql_debug("a tuple is filtered: %s", tuple_[index].to_string().c_str());
      rc = RC::RECORD_EOF;
      index++;
    }
  }
  return rc;
}

RC TableScanPhysicalOperator::close() { return record_scanner_.close_scan(); }

Tuple *TableScanPhysicalOperator::current_tuple()
{ 
  //first i use vector instead of array
  //question?  why the address in this vector may change???
  //for example, in the sort_physical_operator,i called this function 
  //and store the address of the last content in this vector,
  //but when i add a content to this vector,add call the method again to store it
  //the address of the content added before may changed.

  // if (!correct_tuple_.empty()) {
  //   return &(correct_tuple_.back());
  // } else {
  //   return nullptr;
  // }
  return &(correct_tuple_[correct_index_-1]);
}

string TableScanPhysicalOperator::param() const { return table_->name(); }

void TableScanPhysicalOperator::set_predicates(vector<unique_ptr<Expression>> &&exprs)
{
  predicates_ = std::move(exprs);
}

RC TableScanPhysicalOperator::filter(RowTuple &tuple, bool &result)
{
  RC    rc = RC::SUCCESS;
  Value value;
  LOG_DEBUG("start filter!");
  for (unique_ptr<Expression> &expr : predicates_) {
    rc = expr->get_value(tuple, value);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    bool tmp_result = value.get_boolean();
    if (!tmp_result) {
      result = false;
      return rc;
    }
  }

  result = true;
  return rc;
}
