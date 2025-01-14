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

#include "common/rc.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

FilterStmt::~FilterStmt()
{
  for (FilterUnit *unit : filter_units_) {
    delete unit;
  }
  filter_units_.clear();
}

RC FilterStmt::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const ConditionSqlNode *conditions, int condition_num, FilterStmt *&stmt)
{
  RC rc = RC::SUCCESS;
  stmt  = nullptr;

  FilterStmt *tmp_stmt = new FilterStmt();
  for (int i = 0; i < condition_num; i++) {
    FilterUnit *filter_unit = nullptr;
    rc                      = create_filter_unit(db, default_table, tables, conditions[i], filter_unit);
    if (rc != RC::SUCCESS) {
      delete tmp_stmt;
      LOG_WARN("failed to create filter unit. condition index=%d", i);
      return rc;
    }
    tmp_stmt->filter_units_.push_back(filter_unit);
  }

  stmt = tmp_stmt;
  return rc;
}

RC get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const RelAttrSqlNode &attr, Table *&table, const FieldMeta *&field)
{
  if (common::is_blank(attr.relation_name.c_str())) {
    table = default_table;
  } else if (nullptr != tables) {
    auto iter = tables->find(attr.relation_name);
    if (iter != tables->end()) {
      table = iter->second;
    }
  } else {
    table = db->find_table(attr.relation_name.c_str());
  }
  if (nullptr == table) {
    LOG_WARN("No such table: attr.relation_name: %s", attr.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  field = table->table_meta().field(attr.attribute_name.c_str());
  if (nullptr == field) {
    LOG_WARN("no such field in table: table %s, field %s", table->name(), attr.attribute_name.c_str());
    table = nullptr;
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  return RC::SUCCESS;
}

RC FilterStmt::create_filter_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const ConditionSqlNode &condition, FilterUnit *&filter_unit)
{
  RC rc = RC::SUCCESS;

  CompOp comp = condition.comp;
  if (comp < EQUAL_TO || comp >= NO_OP) {
    LOG_WARN("invalid compare operator : %d", comp);
    return RC::INVALID_ARGUMENT;
  }

  filter_unit = new FilterUnit;

  // store attr's type
  AttrType left_attr_type;
  AttrType right_attr_type;

  if (condition.comp != EX && condition.comp != NOT_EX) {
    if (condition.left_is_attr) {
      Table           *table = nullptr;
      const FieldMeta *field = nullptr;
      rc                     = get_table_and_field(db, default_table, tables, condition.left_attr, table, field);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot find attr");
        return rc;
      }
      FilterObj filter_obj;
      Field     left_field = Field(table, field);
      filter_obj.init_attr(left_field);
      filter_unit->set_left(filter_obj);
      left_attr_type = left_field.attr_type();
    } else {
      FilterObj filter_obj;
      filter_obj.init_value(condition.left_value);
      filter_unit->set_left(filter_obj);
    }
  }

  if (condition.need_sub_query) {
    FilterObj filter_obj;
    Stmt     *select_stmt = nullptr;
    RC        rc          = SelectStmt::create(db, *condition.sub_select, select_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("falied to create select stmt in filter stmt: %s",strrc(rc));
      return rc;
    }
    filter_obj.init_subq(static_cast<SelectStmt *>(select_stmt));
    filter_unit->set_right(filter_obj);
  } else {
    if (condition.right_is_attr) {
      Table           *table = nullptr;
      const FieldMeta *field = nullptr;
      rc                     = get_table_and_field(db, default_table, tables, condition.right_attr, table, field);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot find attr");
        return rc;
      }
      FilterObj filter_obj;
      Field     right_field = Field(table, field);
      filter_obj.init_attr(right_field);
      filter_unit->set_right(filter_obj);
      right_attr_type = right_field.attr_type();
    } else {
      if (condition.in_values.empty()) {
        FilterObj filter_obj;
        filter_obj.init_value(condition.right_value);
        filter_unit->set_right(filter_obj);
      } else {
        FilterObj filter_obj;
        filter_obj.init_values(condition.in_values);
        filter_unit->set_right(filter_obj);
      }
    }
  }

  filter_unit->set_comp(comp);

  // check date in conditions,consider about leap year,and days in different months
  if (!condition.left_is_attr && condition.left_value.attr_type() == DATES) {
    int date_value = condition.left_value.get_date();
    if (date_value == 0) {
      rc = RC::INVALID_ARGUMENT;
    }
  }
  if (!condition.right_is_attr && condition.right_value.attr_type() == DATES) {
    int date_value = condition.right_value.get_date();
    if (date_value == 0) {
      rc = RC::INVALID_ARGUMENT;
    }
  }

  // check date comp char(incorrect date format will be cast to char type
  //,so i use this comp to find invalid date value,consider about more than 12 months etc)
  bool can_be_compared = true;

  if (condition.left_is_attr && left_attr_type == DATES && !condition.right_is_attr &&
      !condition.right_value.is_null() && condition.right_value.attr_type() == CHARS) {
    can_be_compared = false;
  }

  if (condition.right_is_attr && right_attr_type == DATES && !condition.left_is_attr &&
      !condition.left_value.is_null() && condition.left_value.attr_type() == CHARS) {
    can_be_compared = false;
  }

  if (!can_be_compared) {
    rc = RC::INVALID_ARGUMENT;
  }
  return rc;
}