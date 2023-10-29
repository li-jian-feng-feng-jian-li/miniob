/**
 * sort
 * @author ljf
 */
#pragma once

#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"

/**
 * @brief sort logical operator
 * @ingroup LogicalOperator
 */
class SortLogicalOperator : public LogicalOperator
{
public:
  SortLogicalOperator(std::vector<std::pair<Field, bool> > order_fields,std::vector<std::pair<Field, std::string> > agg_fields,bool is_count_star = false);
  virtual ~SortLogicalOperator() = default;

  LogicalOperatorType                  type() const override { return LogicalOperatorType::SORT; }
  std::vector<std::pair<Field, bool> > order_fields() { return order_fields_; }
  std::vector<std::pair<Field, std::string>> agg_fields() { return agg_fields_; }
  bool get_count_star(){
    return is_count_star_;
  }

private:
  std::vector<std::pair<Field, bool> > order_fields_;
  std::vector<std::pair<Field, std::string> > agg_fields_;
  bool is_count_star_ = false;
};