#include "common/log/log.h"
#include "sql/operator/sort_physical_operator.h"
#include "storage/record/record.h"
#include "storage/table/table.h"

SortPhysicalOperator::SortPhysicalOperator(std::vector<std::pair<Field, bool> > order_fields,
    std::vector<std::pair<Field, std::string> > agg_fields, bool is_count_star)
    : order_fields_(order_fields), agg_fields_(agg_fields)
{
  is_count_star_ = is_count_star;
}

RC SortPhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  std::unique_ptr<PhysicalOperator> &child = children_[0];
  RC                                 rc    = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }
  LOG_DEBUG("trx opened!");

  return RC::SUCCESS;
}

RC SortPhysicalOperator::next()
{
  // 在这里获取总共的,此处要判断到底是需要聚合还是排序，首先可以先聚合，再派训
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }
  PhysicalOperator *child = children_[0].get();
  if (!finished_) {
    while (RC::SUCCESS == (rc = child->next())) {
      // 这个地方遇到了段错误？在这里导致无法查询，可能是将project注释的原因
      Tuple *tuple = child->current_tuple();
      LOG_DEBUG("get a tuple from child:%s",tuple->to_string().c_str());
      if (nullptr == tuple) {
        LOG_WARN("failed to get tuple from child: %s", strrc(rc));
        return rc;
      }
      // SortTuple sort_tuple;
      // sort_tuple.set_tuple(child->current_tuple());
      // 此处是截获所有的数据
      tuple_to_sort_.emplace_back(tuple);
    }
    LOG_DEBUG("beginning sort tuple!");
    // 此处已经获取了agg的情况，要作判断

    if (agg_fields_.size() > 0) {
      // 此处表示需要聚合，因此对tuple_to_sort来进行处理
      // 由于是要使用聚合函数，则先聚合，此处先不管排序即可
      // 聚合函数首先需要判断哪些字段需要作聚合，再来分别判断
      // 聚合函数的形式是一个字段对应一个聚合，因此要先取字段，再聚合
      if (agg_index_ == 0) {
        do_aggre(tuple_to_sort_);
        agg_index_++;
        return RC::SUCCESS;
      } else {
        return RC::RECORD_EOF;
      }
    }

    quicksort(tuple_to_sort_, 0, tuple_to_sort_.size() - 1);
    LOG_DEBUG("finish sort tuple!");
    finished_ = true;
    return RC::SUCCESS;
  }
  if (index_ < tuple_to_sort_.size()) {
    return RC::SUCCESS;
  } else {
    return RC::RECORD_EOF;
  }
}

RC SortPhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}

Tuple *SortPhysicalOperator::current_tuple()
{
  if (agg_fields_.size() > 0) {
    // 此处就只返回一个元组，按理说应该是可以的
    // 此处的tuple 不需要为project类型
    return &tuple_;
  } else {
    if (index_ < tuple_to_sort_.size()) {
      // return &(tuple_to_sort_[index_++]);
      return tuple_to_sort_[index_++];
    } else {
      return nullptr;
    }
  }
}

bool SortPhysicalOperator::comp(Tuple *&tuple1, Tuple *&tuple2)
{
  std::vector<std::pair<Field, bool> > sort_condition = order_fields_;
  const char                          *table_name     = nullptr;
  const char                          *field_name     = nullptr;
  Value                                value1;
  Value                                value2;
  RC                                   rc  = RC::SUCCESS;
  int                                  pos = 0;
  while (pos < sort_condition.size()) {
    table_name = sort_condition[pos].first.table_name();
    field_name = sort_condition[pos].first.field_name();
    TupleCellSpec spec(table_name, field_name);

    rc = tuple1->find_cell(spec, value1);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("failed to find value in tuple1!");
    }
    rc = tuple2->find_cell(spec, value2);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("failed to find value in tuple2!");
    }
    int result = value1.compare(value2);
    if (sort_condition[pos].second) {
      if (result > 0) {
        return true;
      } else if (result < 0) {
        return false;
      } else
        pos++;
    } else {
      if (result < 0) {
        return true;
      } else if (result > 0) {
        return false;
      } else
        pos++;
    }
  }
  return true;
}

int SortPhysicalOperator::partition(std::vector<Tuple *> &arr, int low, int high)
{
  Tuple *pivot = arr[high];
  int    i     = low - 1;

  for (int j = low; j < high; j++) {
    if (comp(arr[j], pivot)) {
      i++;
      std::swap(arr[i], arr[j]);
    }
  }

  std::swap(arr[i + 1], arr[high]);
  return i + 1;
}

// Quicksort function
void SortPhysicalOperator::quicksort(std::vector<Tuple *> &arr, int low, int high)
{
  if (low < high) {
    int pivotIndex = partition(arr, low, high);

    quicksort(arr, low, pivotIndex - 1);
    quicksort(arr, pivotIndex + 1, high);
  }
}

// 此处需要实现聚合
// 程序在此处卡死了
void SortPhysicalOperator::do_aggre(std::vector<Tuple *> &arr)
{
  // 首先最开始需要实现,tuple 来写
  // 由于实现聚合此处只需要判断一个即可
  std::vector<std::pair<Field, std::string> > need_agg = agg_fields_;
  std::vector<Value>                          vals;
  std::vector<std::string>                    fields;
  const char                                 *table_name = nullptr;
  const char                                 *field_name = nullptr;
  int                                         pos        = 0;
  while (pos < need_agg.size()) {
    // 此处列出表名与字段名
    table_name = need_agg[pos].first.table_name();
    field_name = need_agg[pos].first.field_name();
    // 接下来将表名与字段名来找元组
    std::vector<Value> val_arr;
    // 此处来找元组
    for (int i = 0; i < arr.size(); i++) {
      // 通过元组来找值
      Value         val;
      TupleCellSpec spec(table_name, field_name);
      arr[i]->find_cell(spec, val);
      // 此处加入值
      // std::cout << val.to_string()<<std::endl;
      // 此处可以获取到
      val_arr.emplace_back(val);
    }
    Value val;

    std::string agg_name = need_agg[pos].second;
    do_aggre_one(val_arr, agg_name, val);
    // std::cout<<val.to_string();
    // 这里也可以获取到，主要是要以元组的形式返回
    // std::cout << val.to_string()<<std::endl;
    // 然后就是赋值
    // 重点此处的赋值需要改变成加值
    vals.emplace_back(val);
    fields.emplace_back(field_name);
    pos++;
  }
  // 此处再设置为vals
  // 这里的值都是对的，只不过是在读取时候i出现错误
  tuple_.set_cell(vals, fields);
}

// 此处可以定义多个元组方法来具体进行
// 此处定义单个聚合的方法
void SortPhysicalOperator::do_aggre_one(std::vector<Value> val_arr, std::string agg_func, Value &res)
{
  // 之间开始走
  // 此处假设value均已成功找出
  // Value res;
  // 首先需要判断type
  AttrType type = val_arr[0].attr_type();
  if (agg_func == "count" | agg_func == "COUNT") {
    // 此处确定返回类型肯定是整数
    attr_type = INTS;
    // 再遍历得到值即可
    // 此处可以直接返回数量
    // 此处需要构造一个只有一个数字的tuple
    // 此处之间返回总数目
    // int num = tuple_to_sort_.size();
    int num = 0;
    for (int i = 0; i < val_arr.size(); i++) {
      if (val_arr[i].is_null()) {
        if (is_count_star_) {
          num++;
        }
        continue;
      } else {
        num++;
      }
    }
    res = Value(num);

  } else if (agg_func == "max" | agg_func == "MAX") {
    // 此处返回最大值
    // 首先
    if (type == AttrType::BOOLEANS) {
      // 此处表示错误
      return;
    }
    auto val = val_arr[0];
    // 此处之间比较即可
    for (int i = 0; i < val_arr.size(); i++) {
      // 此处直接忽略null
      if (val_arr[i].is_null()) {
        // 如果是空值就不做比较
        continue;
      }
      // 使用compare函数来比较
      if (val_arr[i].compare(val) > 0) {
        val = val_arr[i];
      }
    }
    res = Value(val);
    return;
    // else if(type == AttrType::INTS){
    //   //等于ints，则可以之间比较大小
    //     int max = val_arr[0].get_int(); // 假设第一个元素是最大值
    //     for (int i = 1; i < val_arr.size(); i++) {
    //     if (val_arr[i].get_int() > max) {
    //         max = val_arr[i].get_int();
    //     }
    // }
    // // 循环完后则可以返回出值

    // }
    // else if(type == AttrType::FLOATS){
    //     float max = val_arr[0].get_float(); // 假设第一个元素是最大值
    //     for (int i = 1; i < val_arr.size(); i++) {
    //     if (val_arr[i].get_float() > max) {
    //         max = val_arr[i].get_float();
    //     }
    // }
    // else if(type == AttrType::DATES){

    // }
  } else if (agg_func == "min" | agg_func == "MIN") {
    if (type == AttrType::BOOLEANS) {
      // 此处表示错误
      return;
    }
    auto val = val_arr[0];
    // 此处之间比较即可
    for (int i = 0; i < val_arr.size(); i++) {
      // 使用compare函数来比较
      if (val_arr[i].is_null()) {
        // 如果是空值就不做比较
        continue;
      }
      if (val_arr[i].compare(val) < 0) {
        val = val_arr[i];
      }
    }
    res = Value(val);
    return;
  } else if (agg_func == "avg" | agg_func == "AVG") {
    // 日期不能求平均
    if (type == AttrType::CHARS | type == AttrType::BOOLEANS | type == AttrType::DATES) {
      // 此处表示错误
      return;
    }
    if (type == AttrType::INTS) {
      float sum = 0;
      for (int i = 0; i < val_arr.size(); i++) {
        // 此处再来进行加操作
        if (val_arr[i].is_null()) {
          // 如果是空值就不做比较
          continue;  // 是空值则不加
        }
        sum += val_arr[i].get_int();
      }
      float avg = sum / val_arr.size();
      res       = Value(avg);
      return;
    }
    if (type == AttrType::FLOATS) {
      float sum = 0;
      for (int i = 0; i < val_arr.size(); i++) {
        // 此处再来进行加操作
        if (val_arr[i].is_null()) {
          // 如果是空值就不做比较
          continue;
        }
        sum += val_arr[i].get_float();
      }
      float avg = sum / val_arr.size();
      res       = Value(avg);
      return;
    }
  } else if (agg_func == "sum" | agg_func == "SUM") {
    if (type == AttrType::CHARS | type == AttrType::BOOLEANS | type == AttrType::DATES) {
      // 此处表示错误
      return;
    }
    if (type == AttrType::INTS) {
      int sum = 0;
      for (int i = 0; i < val_arr.size(); i++) {
        // 此处再来进行加操作
        if (val_arr[i].is_null()) {
          // 如果是空值就不做比较
          continue;
        }
        sum += val_arr[i].get_int();
      }

      res = Value(sum);
      return;
    }
    if (type == AttrType::FLOATS) {
      float sum = 0;
      for (int i = 0; i < val_arr.size(); i++) {
        // 此处再来进行加操作
        if (val_arr[i].is_null()) {
          // 如果是空值就不做比较
          continue;
        }
        sum += val_arr[i].get_float();
      }

      res = Value(sum);
      return;
    }
  } else {
    return;
  }
}