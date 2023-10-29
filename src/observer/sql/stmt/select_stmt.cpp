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
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

SelectStmt::~SelectStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

static void wildcard_fields(Table *table, std::vector<Field> &field_metas)
{
  const TableMeta &table_meta = table->table_meta();
  const int        field_num  = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    field_metas.push_back(Field(table, table_meta.field(i)));
  }
}

RC SelectStmt::create(Db *db, const SelectSqlNode &select_sql, Stmt *&stmt)
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  // collect tables in `from` statement
  std::vector<Table *>                     tables;
  std::unordered_map<std::string, Table *> table_map;
  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const char *table_name = select_sql.relations[i].c_str();
    if (nullptr == table_name) {
      LOG_WARN("invalid argument. relation name is null. index=%d", i);
      return RC::INVALID_ARGUMENT;
    }

    Table *table = db->find_table(table_name);
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    tables.push_back(table);
    table_map.insert(std::pair<std::string, Table *>(table_name, table));
  }

  // collect query fields in `select` statement
  std::vector<Field> query_fields;
  for (int i = static_cast<int>(select_sql.attributes.size()) - 1; i >= 0; i--) {
    const RelAttrSqlNode &relation_attr = select_sql.attributes[i];

    // e.g. select * from t or select id from t
    if (common::is_blank(relation_attr.relation_name.c_str()) &&
        0 == strcmp(relation_attr.attribute_name.c_str(), "*")) {
      for (Table *table : tables) {
        wildcard_fields(table, query_fields);
      }

      // e.g. select c.xx from c ,xx can be * or any attr
    } else if (!common::is_blank(relation_attr.relation_name.c_str())) {
      const char *table_name = relation_attr.relation_name.c_str();
      const char *field_name = relation_attr.attribute_name.c_str();

      // select *.xx
      if (0 == strcmp(table_name, "*")) {
        // if not select *.* ,return error
        if (0 != strcmp(field_name, "*")) {
          LOG_WARN("invalid field name while table is *. attr=%s", field_name);
          return RC::SCHEMA_FIELD_MISSING;
        }
        // select *.*
        for (Table *table : tables) {
          wildcard_fields(table, query_fields);
        }
      } else {
        // select t.xx
        auto iter = table_map.find(table_name);
        if (iter == table_map.end()) {
          LOG_WARN("no such table in from list: %s", table_name);
          return RC::SCHEMA_FIELD_MISSING;
        }

        Table *table = iter->second;
        // select t.*
        if (0 == strcmp(field_name, "*")) {
          wildcard_fields(table, query_fields);
        } else {
          // select t.xx
          const FieldMeta *field_meta = table->table_meta().field(field_name);
          if (nullptr == field_meta) {
            LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name);
            return RC::SCHEMA_FIELD_MISSING;
          }

          query_fields.push_back(Field(table, field_meta));
        }
      }
    } else {
      // select id,name from t1.t2,invaild
      if (tables.size() != 1) {
        LOG_WARN("invalid. I do not know the attr's table. attr=%s", relation_attr.attribute_name.c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }
      // select id,name from t
      Table           *table      = tables[0];
      const FieldMeta *field_meta = table->table_meta().field(relation_attr.attribute_name.c_str());
      if (nullptr == field_meta) {
        LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), relation_attr.attribute_name.c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }

      query_fields.push_back(Field(table, field_meta));
    }
  }

  LOG_INFO("got %d tables in from stmt and %d fields in query stmt", tables.size(), query_fields.size());


 // 此处加入聚合查询字段
  std::vector<std::pair<Field, std::string> > agg_fields;
  bool isAgg = false;
  bool is_count_star = false;
  // 先遍历一遍置位

  // 此处处理查询字段多于1个返回错误
  //std::string agg_func1 = select_sql.attributes[0].agg_name;
  // if(0 != strcmp(agg_func1.c_str(), "count") && query_fields.size() > 1){
  //   return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  // }
  // std::string agg_func2 = select_sql.attributes[1].agg_name;
  // if(0 == strcmp(agg_func1.c_str(), "count") && common::is_blank(agg_func2.c_str())){
  //   return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  // }
  // 最开始就要做一个判断
   for (int i = static_cast<int>(select_sql.attributes.size()) - 1; i >= 0; i--) {
    const RelAttrSqlNode &relation_attr = select_sql.attributes[i];
    if(!relation_attr.isOK){
      // 此处直接返回即可
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
    if(!common::is_blank(relation_attr.agg_name.c_str())){
      // 此处就来置位
      isAgg = true;
    }
   }
  for (int i = static_cast<int>(select_sql.attributes.size()) - 1; i >= 0; i--) {
    const RelAttrSqlNode &relation_attr = select_sql.attributes[i];
    // if(!common::is_blank(relation_attr.agg_name.c_str()) &&
    //  static_cast<int>(select_sql.attributes.size()) > 1
    // ){
    //     return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    // }
    // 这在可以最开始来一个判断
    //std::cout << relation_attr.agg_name.c_str();
    if(common::is_blank(relation_attr.agg_name.c_str())){
      // 此处表示是聚合是空值
      // 直接跳到下一个即可
      //std::cout << "空置";
      if(isAgg){
        // 表示有了agg ，但这个却不是
        return RC::SCHEMA_FIELD_MISSING;
      }
      continue;
    } 
    // 否则检查
    
    // e.g. 只有属性，没有表关系
    if (common::is_blank(relation_attr.relation_name.c_str())) {
       if (tables.size() != 1) {
        // 此处暂不处理多表无约束的情况
        return RC::SCHEMA_FIELD_MISSING;
      }
      

      Table           *table      = tables[0];
      const FieldMeta *field_meta = table->table_meta().field(relation_attr.attribute_name.c_str());
      const FieldMeta *field_meta2 = table->table_meta().field(table->table_meta().sys_field_num());
      // 此处要做一个单独判断
      bool isCount = false;
      if (common::is_blank(relation_attr.relation_name.c_str()) &&
        0 == strcmp(relation_attr.attribute_name.c_str(), "*")) {
          if(0 == strcmp(relation_attr.agg_name.c_str(), "count") ||
          0 == strcmp(relation_attr.agg_name.c_str(), "COUNT"
          )){
            // 不能中断
            isCount = true;
            is_count_star = true;
            agg_fields.push_back(std::make_pair(Field(table, field_meta2),"count"));
            //field_meta = table->table_meta().field(table->table_meta().sys_field_num());
          }
          else{
            return RC::SCHEMA_FIELD_TYPE_MISMATCH;
          }
        }
      if (nullptr == field_meta & nullptr == field_meta2) {
        //std::cout << "ji" <<std::endl;
        return RC::SCHEMA_FIELD_MISSING;
      }
      // 这里才能加，不然就是重复加两个了
      if(!isCount){
        agg_fields.emplace_back(std::make_pair(Field(table, field_meta),relation_attr.agg_name));
      }
      // 此处是单表操作，如果我保存的
      //saveStringToFile(relation_attr.agg_name,"1.txt");
      // e.g. 如果有多表的操作
    } else if (!common::is_blank(relation_attr.relation_name.c_str())) {
      const char *table_name = relation_attr.relation_name.c_str();
      const char *field_name = relation_attr.attribute_name.c_str();

      // select *.xx
      if (0 == strcmp(table_name, "*")) {
        // if not select *.* ,return error
        if (0 != strcmp(field_name, "*")) {
          LOG_WARN("invalid field name while table is *. attr=%s", field_name);
          return RC::SCHEMA_FIELD_MISSING;
        }
        // 此处* 点 *也是错误的
        else{
          return RC::SCHEMA_FIELD_MISSING;
        }
      } else {
        // select t.xx
        auto iter = table_map.find(table_name);
        if (iter == table_map.end()) {
          LOG_WARN("no such table in from list: %s", table_name);
          return RC::SCHEMA_FIELD_MISSING;
        }

        Table *table = iter->second;
        // select t.*
        if (0 == strcmp(field_name, "*")) {
          // 如果是t.*则只可能是count
          if((relation_attr.agg_name.compare("count") == 0) |
          (relation_attr.agg_name.compare("COUNT") == 0)) {
            // 此处表示这种是可以处理的
            // 由于是count 其实加入一个字段即可进行
            // 此处就是仅仅获取一个字段
            const FieldMeta *field_meta = table->table_meta().field(table->table_meta().sys_field_num());
            agg_fields.push_back(std::make_pair(Field(table, field_meta),"count"));
        }
          else{
              return RC::SCHEMA_FIELD_MISSING;
          } 
        }
        else {
          // select t.xx
          const FieldMeta *field_meta = table->table_meta().field(field_name);
          if (nullptr == field_meta) {
            LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name);
            return RC::SCHEMA_FIELD_MISSING;
          }

          agg_fields.push_back(std::make_pair(Field(table, field_meta),relation_attr.agg_name));
        }
      }
    } else {
     return RC::SCHEMA_FIELD_MISSING;
    }
  }



  // get from order by
  std::vector<std::pair<Field, bool> > order_fields;
  for (int i = 0; i < select_sql.orders.size(); i++) {
    const RelAttrOrderNode &order_attr = select_sql.orders[i];

    // e.g. select * from t or select id from t
    if (common::is_blank(order_attr.attribute_name.c_str()) || (0 == strcmp(order_attr.attribute_name.c_str(), "*")) ||
        (0 == strcmp(order_attr.relation_name.c_str(), "*"))) {
      LOG_WARN("i do not know which field to order!");
      return RC::SCHEMA_FIELD_MISSING;
    } else if (!common::is_blank(order_attr.relation_name.c_str())) {
      const char *table_name = order_attr.relation_name.c_str();
      const char *field_name = order_attr.attribute_name.c_str();

      // order by t.field
      auto iter = table_map.find(table_name);
      if (iter == table_map.end()) {
        LOG_WARN("no such table in from list: %s", table_name);
        return RC::SCHEMA_FIELD_MISSING;
      }

      Table *table = iter->second;

      const FieldMeta *field_meta = table->table_meta().field(field_name);
      if (nullptr == field_meta) {
        LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name);
        return RC::SCHEMA_FIELD_MISSING;
      }

      order_fields.emplace_back(std::make_pair(Field(table, field_meta), order_attr.order_by_desc));
    } else {
      // order by id , name
      if (tables.size() != 1) {
        LOG_WARN("invalid. I do not know the attr's table. attr=%s", order_attr.attribute_name.c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }

      Table           *table      = tables[0];
      const FieldMeta *field_meta = table->table_meta().field(order_attr.attribute_name.c_str());
      if (nullptr == field_meta) {
        LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), order_attr.attribute_name.c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }

      order_fields.emplace_back(std::make_pair(Field(table, field_meta), order_attr.order_by_desc));
    }
  }

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;
  RC          rc          = FilterStmt::create(db,
      default_table,
      &table_map,
      select_sql.conditions.data(),
      static_cast<int>(select_sql.conditions.size()),
      filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  // everything alright
  SelectStmt *select_stmt = new SelectStmt();
  // TODO add expression copy
  select_stmt->tables_.swap(tables);
  select_stmt->query_fields_.swap(query_fields);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->order_fields_.swap(order_fields);
  // 此处直接加入聚合字段
  select_stmt->agg_fields_.swap(agg_fields);
  if(is_count_star){
    // 此处直接置位
    select_stmt->set_count_star();
  }
  stmt                      = select_stmt;
  return RC::SUCCESS;
}
