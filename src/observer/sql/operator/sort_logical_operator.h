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
  SortLogicalOperator(std::vector<std::pair<Field, bool> > order_fields);
  virtual ~SortLogicalOperator() = default;

  LogicalOperatorType                  type() const override { return LogicalOperatorType::SORT; }
  std::vector<std::pair<Field, bool> > order_fields() { return order_fields_; }

private:
  std::vector<std::pair<Field, bool> > order_fields_;
};