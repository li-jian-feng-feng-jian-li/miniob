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

#pragma once

#include "sql/stmt/stmt.h"
#include "common/rc.h"

class Table;
class FilterStmt;
class SelectStmt;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt() = default;
  UpdateStmt(Table *table, std::vector<UpdateValueSqlNode> value, FilterStmt *filter_stmt,
      std::vector<SelectStmt *> select_stmt, std::vector<const char *> field_name);
  StmtType type() const override { return StmtType::UPDATE; }

public:
  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);

public:
  Table                          *table() const { return table_; }
  std::vector<UpdateValueSqlNode> value() const { return value_; }
  FilterStmt                     *filter_stmt() const { return filter_stmt_; }
  std::vector<const char *>       field_name() const { return field_name_; }
  std::vector<SelectStmt *>       select_stmt() const { return select_stmt_; }

private:
  Table                          *table_ = nullptr;
  std::vector<UpdateValueSqlNode> value_;
  FilterStmt                     *filter_stmt_ = nullptr;
  std::vector<const char *>       field_name_;
  std::vector<SelectStmt *>       select_stmt_;
};
