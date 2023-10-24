#include "common/log/log.h"
#include "sql/operator/sort_physical_operator.h"
#include "storage/record/record.h"
#include "storage/table/table.h"

SortPhysicalOperator::SortPhysicalOperator(std::vector<std::pair<Field, bool> > order_fields)
    : order_fields_(order_fields)
{}

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
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }
  PhysicalOperator *child = children_[0].get();
  if (!finished_) {
    while (RC::SUCCESS == (rc = child->next())) {
      Tuple *tuple = child->current_tuple();
      LOG_DEBUG("get a tuple from child:%s",tuple->to_string().c_str());
      if (nullptr == tuple) {
        LOG_WARN("failed to get tuple from child: %s", strrc(rc));
        return rc;
      }
      // SortTuple sort_tuple;
      // sort_tuple.set_tuple(child->current_tuple());
      tuple_to_sort_.emplace_back(tuple);
    }
    LOG_DEBUG("beginning sort tuple!");
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
  if (index_ < tuple_to_sort_.size()) {
    //return &(tuple_to_sort_[index_++]);
    return tuple_to_sort_[index_++];
  } else {
    return nullptr;
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
  int       i     = low - 1;

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
