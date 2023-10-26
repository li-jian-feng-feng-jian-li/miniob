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
// Created by Wangyunlai on 2022/07/08.
//

#include "sql/operator/index_scan_physical_operator.h"
#include "storage/index/index.h"
#include "storage/trx/trx.h"

IndexScanPhysicalOperator::IndexScanPhysicalOperator(Table *table, Index *index, bool readonly,
    std::vector<Value> left_value, std::vector<bool> left_inclusive, std::vector<Value> right_value,
    std::vector<bool> right_inclusive, int comp_num)
    : table_(table),
      index_(index),
      readonly_(readonly),
      left_inclusive_(left_inclusive),
      right_inclusive_(right_inclusive),
      left_value_(left_value),
      right_value_(right_value),
      comp_num_(comp_num)
{}

RC IndexScanPhysicalOperator::open(Trx *trx)
{
  if (nullptr == table_ || nullptr == index_) {
    return RC::INTERNAL;
  }

  LOG_DEBUG("start create index scanner!");
  IndexScanner *index_scanner = nullptr;

  index_scanner = index_->create_scanner(&left_value_, &left_inclusive_, &right_value_, &right_inclusive_, comp_num_);
  LOG_DEBUG("index scanner created!");

  if (nullptr == index_scanner) {
    LOG_WARN("failed to create index scanner");
    return RC::INTERNAL;
  }
  LOG_DEBUG("finish create index scanner!");

  record_handler_ = table_->record_handler();
  if (nullptr == record_handler_) {
    LOG_WARN("invalid record handler");
    index_scanner->destroy();
    return RC::INTERNAL;
  }
  index_scanner_ = index_scanner;
  trx_           = trx;
  LOG_DEBUG("trx open successfully!");
  return RC::SUCCESS;
}

RC IndexScanPhysicalOperator::next()
{
  RID rid;
  RC  rc = RC::SUCCESS;

  record_page_handler_.cleanup();

  bool filter_result = false;
  while (RC::SUCCESS == (rc = index_scanner_->next_entry(&rid))) {
    LOG_DEBUG("index get a entry!");
    rc = record_handler_->get_record(record_page_handler_, &rid, readonly_, &current_record_[record_index_]);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    tuple_[record_index_].set_schema(table_, table_->table_meta().field_metas());
    tuple_[record_index_].set_record(&current_record_[record_index_]);
    LOG_DEBUG("start filter!");
    rc = filter(tuple_[record_index_], filter_result);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (!filter_result) {
      record_index_++;
      continue;
    }

    rc = trx_->visit_record(table_, current_record_[record_index_], readonly_);
    if (rc == RC::RECORD_INVISIBLE) {
      record_index_++;
      continue;
    } else {
      record_index_++;
      return rc;
    }
  }

  return rc;
}

RC IndexScanPhysicalOperator::close()
{
  index_scanner_->destroy();
  index_scanner_ = nullptr;
  return RC::SUCCESS;
}

Tuple *IndexScanPhysicalOperator::current_tuple()
{
  //tuple_.set_record(&current_record_);
  return &tuple_[record_index_-1];
}

void IndexScanPhysicalOperator::set_predicates(std::vector<std::unique_ptr<Expression>> &&exprs)
{
  predicates_ = std::move(exprs);
}

RC IndexScanPhysicalOperator::filter(RowTuple &tuple, bool &result)
{
  RC    rc = RC::SUCCESS;
  Value value;
  for (std::unique_ptr<Expression> &expr : predicates_) {
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

std::string IndexScanPhysicalOperator::param() const
{
  return std::string(index_->index_meta().name()) + " ON " + table_->name();
}
