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
// Created by wangyunlai.wyl on 2021/5/19.
//

#include "storage/index/bplus_tree_index.h"
#include "common/log/log.h"

BplusTreeIndex::~BplusTreeIndex() noexcept { close(); }

RC BplusTreeIndex::create(
    const char *file_name, const IndexMeta &index_meta, const std::vector<const FieldMeta *> &field_meta)
{
  if (inited_) {
    LOG_WARN("Failed to create index due to the index has been created before. file_name:%s, index:%s",
        file_name,
        index_meta.name()
        );
    return RC::RECORD_OPENNED;
  }

  Index::init(index_meta, field_meta);
  std::vector<AttrType> types;
  std::vector<int>      lengths;
  std::vector<int>      offsets;
  for (int i = 0; i < field_meta.size(); i++) {
    types.emplace_back(field_meta[i]->type());
    lengths.emplace_back(field_meta[i]->len());
    offsets.emplace_back(field_meta[i]->offset());
  }

  RC rc = index_handler_.create(file_name, types, lengths, offsets);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to create index_handler, file_name:%s, index:%s, rc:%s",
        file_name,
        index_meta.name(),
        strrc(rc));
    return rc;
  }

  inited_ = true;
  LOG_INFO(
      "Successfully create index, file_name:%s, index:%s", file_name, index_meta.name());
  return RC::SUCCESS;
}

RC BplusTreeIndex::open(
    const char *file_name, const IndexMeta &index_meta, const std::vector<const FieldMeta *> &field_meta)
{
  LOG_DEBUG("file_name:%s",file_name);
  if (inited_) {
    LOG_WARN("Failed to open index due to the index has been initedd before. file_name:%s, index:%s",
        file_name,
        index_meta.name()
        );
    return RC::RECORD_OPENNED;
  }

  LOG_DEBUG("start init index!");
  Index::init(index_meta, field_meta);
  LOG_DEBUG("finish init index!");

  LOG_DEBUG("start open index_handler!");
  // TODO solve bug
  RC rc = index_handler_.open(file_name);
  LOG_DEBUG("finish open index_handler!");
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to open index_handler, file_name:%s, index:%s, rc:%s",
        file_name,
        index_meta.name(),
        strrc(rc));
    return rc;
  }

  inited_ = true;
  LOG_INFO(
      "Successfully open index, file_name:%s, index:%s", file_name, index_meta.name());
  return RC::SUCCESS;
}

RC BplusTreeIndex::close()
{
  if (inited_) {
    LOG_INFO("Begin to close index, index:%s", index_meta_.name());
    index_handler_.close();
    inited_ = false;
  }
  LOG_INFO("Successfully close index.");
  return RC::SUCCESS;
}

RC BplusTreeIndex::insert_entry(const char *record, const RID *rid)
{
  RC rc = RC::SUCCESS;
  // int key_offset   = 0;
  // int total_length = 0;
  // for (int i = 0; i < field_metas_.size(); i++) {
  //   total_length += field_metas_[i]->len();
  // }
  // char *key = (char *)malloc(total_length);
  // for (int i = 0; i < field_metas_.size(); i++) {
  //   memcpy(key + key_offset, record + field_metas_[i]->offset(), field_metas_[i]->len());
  //   key_offset += field_metas_[i]->len();
  // }
  // memcpy(key + key_offset, rid, sizeof(rid));
  bool is_unique = index_meta_.is_unique();
  rc = index_handler_.insert_entry(record, rid, is_unique);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  return rc;
}

RC BplusTreeIndex::delete_entry(const char *record, const RID *rid)
{
  // RC  rc           = RC::SUCCESS;
  // int key_offset   = 0;
  // int total_length = 0;
  // for (int i = 0; i < field_metas_.size(); i++) {
  //   total_length += field_metas_[i]->len();
  // }
  // char *key = (char *)malloc(total_length);
  // for (int i = 0; i < field_metas_.size(); i++) {
  //   memcpy(key + key_offset, record + field_metas_[i]->offset(), field_metas_[i]->len());
  //   key_offset += field_metas_[i]->len();
  // }
  // memcpy(key + key_offset, rid, sizeof(rid));
  // rc = index_handler_.delete_entry(record, rid);
  // if (rc != RC::SUCCESS) {
  //   return rc;
  // }
  // return rc;
  return index_handler_.delete_entry(record, rid);
}

IndexScanner *BplusTreeIndex::create_scanner(std::vector<Value> *left_key, std::vector<bool> *left_inclusive,
    std::vector<Value> *right_key, std::vector<bool> *right_inclusive, int comp_num)
{
  BplusTreeIndexScanner *index_scanner = new BplusTreeIndexScanner(index_handler_);
  LOG_DEBUG("open index scanner!");
  RC                     rc = index_scanner->open(left_key, left_inclusive, right_key, right_inclusive, comp_num);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open index scanner. rc=%d:%s", rc, strrc(rc));
    delete index_scanner;
    return nullptr;
  }
  return index_scanner;
}

RC BplusTreeIndex::sync() { return index_handler_.sync(); }

////////////////////////////////////////////////////////////////////////////////
BplusTreeIndexScanner::BplusTreeIndexScanner(BplusTreeHandler &tree_handler) : tree_scanner_(tree_handler) {}

BplusTreeIndexScanner::~BplusTreeIndexScanner() noexcept { tree_scanner_.close(); }

RC BplusTreeIndexScanner::open(std::vector<Value> *left_key, std::vector<bool> *left_inclusive,
    std::vector<Value> *right_key, std::vector<bool> *right_inclusive, int comp_num)
{
  return tree_scanner_.open(left_key, left_inclusive, right_key, right_inclusive, comp_num);
}

RC BplusTreeIndexScanner::next_entry(RID *rid) { return tree_scanner_.next_entry(*rid); }

RC BplusTreeIndexScanner::destroy()
{
  delete this;
  return RC::SUCCESS;
}
