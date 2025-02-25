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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

UpdateStmt::UpdateStmt(Table *table, std::vector<UpdateValueSqlNode> value, FilterStmt *filter_stmt,
    std::vector<SelectStmt *> select_stmt, std::vector<const char *> field_name)
    : table_(table), value_(value), filter_stmt_(filter_stmt), select_stmt_(select_stmt), field_name_(field_name)
{}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{

  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  const char *table_name = update.relation_name.c_str();
  if (nullptr == table_name) {
    LOG_WARN("invalid argument. relation name is null.");
    return RC::INVALID_ARGUMENT;
  }

  Table                                   *table = db->find_table(table_name);
  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(table_name, table));

  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  std::vector<const char *> field_names;
  for (int i = 0; i < update.attribute_name.size(); i++) {
    const char      *field_name = update.attribute_name[i].c_str();
    const FieldMeta *field_meta = table->table_meta().field(field_name);

    if (nullptr == field_meta) {
      LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    field_names.emplace_back(field_name);
  }

  // AttrType field_type = field_meta->type();
  // AttrType value_type = value->attr_type();
  // // check if field type matches value type
  // if (field_type != value_type) {
  //   LOG_WARN("field type mismatch. table=%s, field=%s, field_type=%d, value_type=%d",
  //         table_name, field_meta->name(), field_type, value_type);
  //   return RC::INVALID_ARGUMENT;
  // }

  FilterStmt *filter_stmt = nullptr;
  RC          rc          = FilterStmt::create(
      db, table, &table_map, update.conditions.data(), static_cast<int>(update.conditions.size()), filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  std::vector<SelectStmt *> select_stmts;
  for (auto &p : update.value) {
    if (!p.is_value) {
      Stmt *select_stmt = nullptr;
      RC    rc          = SelectStmt::create(db, std::get<0>(p.update_value), select_stmt);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot construct select stmt");
        return rc;
      }
      select_stmts.emplace_back(static_cast<SelectStmt *>(select_stmt));
    }
  }

  stmt = new UpdateStmt(table, update.value, filter_stmt, select_stmts, field_names);
  LOG_DEBUG("update stmt created!");
  return RC::SUCCESS;
}
