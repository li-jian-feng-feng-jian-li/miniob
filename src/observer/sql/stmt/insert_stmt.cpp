/* Copyright (c) 2021OceanBase and/or its affiliates. All rights reserved.
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

#include "sql/stmt/insert_stmt.h"
#include "common/log/log.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "cmath"
#include "sstream"

InsertStmt::InsertStmt(Table *table, std::vector<std::vector<Value> > *values, int value_amount)
    : table_(table), values_(values), value_amount_(value_amount)
{}

RC InsertStmt::create(Db *db, InsertSqlNode &inserts, Stmt *&stmt)
{
  LOG_DEBUG("start create insertstmt!");
  const char *table_name = inserts.relation_name.c_str();
  bool        has_empty  = false;
  for (int i = 0; i < inserts.values.size(); i++) {
    if (inserts.values[i].empty()) {
      has_empty = true;
      break;
    }
  }

  if (nullptr == db || nullptr == table_name || inserts.values.empty() || has_empty) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, value_num=%d",
        db, table_name, static_cast<int>(inserts.values.size()));
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // check the fields number
  const TableMeta                 &table_meta    = table->table_meta();
  const int                        field_num     = table_meta.field_num() - table_meta.sys_field_num();
  const int                        sys_field_num = table_meta.sys_field_num();
  std::vector<std::vector<Value> > values        = inserts.values;
  bool                             args_invalid  = false;

  for (int i = 0; i < values.size(); i++) {
    const int value_num = static_cast<int>(values[i].size());
    if (field_num != value_num) {
      LOG_WARN("schema mismatch. value num=%d, field num in schema=%d", value_num, field_num);
      return RC::SCHEMA_FIELD_MISSING;
    }

    // check fields type
    for (int j = 0; j < value_num; j++) {
      const FieldMeta *field_meta = table_meta.field(j + sys_field_num);
      const AttrType   field_type = field_meta->type();
      const bool       nullable   = field_meta->nullable();
      const AttrType   value_type = values[i][j].attr_type();
      const bool       is_null    = values[i][j].is_null();
      if (field_type != value_type) {
        if (value_type == CHARS) {
          if (!is_null) {
            std::string s = values[i][j].get_string();
            LOG_DEBUG("value =%s",s);
            if (field_type == INTS) {
              int num;
              try {
                num = std::stof(s);
                LOG_DEBUG("num =%d",num);
              } catch (std::invalid_argument &) {
                num = 0;
              }
              values[i][j].set_value(Value(num));
            } else if (field_type == FLOATS) {
              float num;
              try {
                num = std::stof(s);
              } catch (std::invalid_argument &) {
                num = 0.0;
              }
              values[i][j].set_value(Value(num));
            } else {
              LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
          table_name, field_meta->name(), field_type, value_type);
              args_invalid = true;
              break;
            }
          } else {
            if (!nullable) {
              LOG_WARN("can not insert a null value into a non nullable field, table=%s, field=%s, field type=%d",table_name, field_meta->name(), field_type);
              args_invalid = true;
              break;
            }
          }
        }

        if (value_type == INTS) {
          int num = values[i][j].get_int();
          if (field_type == FLOATS) {
            values[i][j].set_value(Value((float)num));
          } else if (field_type == CHARS) {
            std::string s = std::to_string(num);
            values[i][j].set_value(Value(s.c_str(), false));
          } else {
            LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
          table_name, field_meta->name(), field_type, value_type);
            args_invalid = true;
            break;
          }
        }

        if (value_type == FLOATS) {
          float num = values[i][j].get_float();
          if (field_type == INTS) {
            values[i][j].set_value(Value((int)round(num)));
          } else if (field_type == CHARS) {
            std::ostringstream oss;
            oss << num;
            values[i][j].set_value(Value(oss.str().c_str(), false));
          } else {
            LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
          table_name, field_meta->name(), field_type, value_type);
            args_invalid = true;
          }
        }
      } else if (field_type == DATES && values[i][j].get_date() == 0) {
        args_invalid = true;
      }
    }
  }

  if (args_invalid)
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;

  // everything alright

  inserts.values.swap(values);

  stmt = new InsertStmt(table, &inserts.values, static_cast<int>(inserts.values[0].size()));
  LOG_DEBUG("insert stmt finished!");
  return RC::SUCCESS;
}
