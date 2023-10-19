/**
 * update
 * @author ljf
 */
#pragma once

#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"

/**
 * @brief update logical operator
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, std::vector<Value> value, std::vector<const char *> field_name);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }

  Table                    *table() const { return table_; }
  std::vector<Value>        value() const { return value_; }
  std::vector<const char *> field_name() const { return field_name_; }

private:
  Table                    *table_ = nullptr;
  std::vector<Value>        value_;
  std::vector<const char *> field_name_;
};