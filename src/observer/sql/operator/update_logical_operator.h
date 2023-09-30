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
  UpdateLogicalOperator(Table *table, const Value *value, const char *field_name);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }

  Table       *table() const { return table_; }
  const Value *value() const { return value_; }
  const char  *field_name() const { return field_name_; }

private:
  Table       *table_      = nullptr;
  const Value *value_      = nullptr;
  const char  *field_name_ = nullptr;
};