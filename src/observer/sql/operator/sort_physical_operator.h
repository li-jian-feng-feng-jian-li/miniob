/**
 * @author ljf
 */

#pragma once

#include "sql/operator/physical_operator.h"

class Trx;
class SelectStmt;

/**
 * @brief 物理算子，update
 * @ingroup PhysicalOperator
 */
class SortPhysicalOperator : public PhysicalOperator
{
public:
  SortPhysicalOperator(std::vector<std::pair<Field, bool> > order_fields);

  virtual ~SortPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::SORT; }

  RC     open(Trx *trx) override;
  RC     next() override;
  RC     close() override;
  Tuple *current_tuple() override;
  bool comp(SortTuple &tuple1, SortTuple &tuple2);
  int partition(std::vector<SortTuple> &arr, int low, int high);
  void quicksort(std::vector<SortTuple> &arr, int low, int high);
private:
  std::vector<std::pair<Field, bool> > order_fields_;
  std::vector<SortTuple>               tuple_to_sort_;
  int                                  index_ = 0;
  bool                                 finished_ = false;
};