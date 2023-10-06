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

#include "sql/operator/insert_physical_operator.h"
#include "sql/stmt/insert_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

using namespace std;

InsertPhysicalOperator::InsertPhysicalOperator(Table *table, std::vector<std::vector<Value> > &&values_vecs)
    : table_(table), values_vecs_(std::move(values_vecs))
{}

RC InsertPhysicalOperator::open(Trx *trx)
{
  bool can_insert = true;
  std::vector<Record *> inserted_records;
  for (int i = 0; i < values_vecs_.size(); i++) {
    Record record;
    RC     rc = table_->make_record(static_cast<int>(values_vecs_[i].size()), values_vecs_[i].data(), record);
    if (rc != RC::SUCCESS) {
      can_insert = false;
      LOG_WARN("failed to make record. rc=%s", strrc(rc));
      continue;
    }

    rc = trx->insert_record(table_, record);
    if (rc != RC::SUCCESS) {
      can_insert = false;
      LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
      continue;
    } else {
      inserted_records.emplace_back(&record);
    }
  }
  if(!can_insert){
    for(int i=0;i<inserted_records.size();i++){
      trx->delete_record(table_,*(inserted_records[i]));
    }
    return RC::INVALID_ARGUMENT;
  }
  return RC::SUCCESS;
}

RC InsertPhysicalOperator::next() { return RC::RECORD_EOF; }

RC InsertPhysicalOperator::close() { return RC::SUCCESS; }
