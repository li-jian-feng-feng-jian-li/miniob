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
  SortPhysicalOperator(std::vector<std::pair<Field, bool> > order_fields,std::vector<std::pair<Field, std::string> > agg_fields,bool is_agg_star = false);

  virtual ~SortPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::SORT; }

  RC     open(Trx *trx) override;
  RC     next() override;
  RC     close() override;
  Tuple *current_tuple() override;
  bool   comp(Tuple *&tuple1, Tuple *&tuple2);
  int    partition(std::vector<Tuple *> &arr, int low, int high);
  void   quicksort(std::vector<Tuple *> &arr, int low, int high);
  void   do_aggre(std::vector<Tuple *> &arr);
  void do_aggre_one(std::vector<Value> value_arr,std::string agg_func,Value &res);

private:
  std::vector<std::pair<Field, bool> > order_fields_;
  std::vector<std::pair<Field, std::string> > agg_fields_;
  std::vector<Tuple *>                 tuple_to_sort_;
  int                                  index_    = 0;
  bool                                 finished_ = false;
  AttrType                             attr_type;
  SortTuple                            tuple_;
  int                                  agg_index_ = 0;
  bool                                 is_count_star_ = false;
};