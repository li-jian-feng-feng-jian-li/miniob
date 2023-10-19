#include "sql/operator/update_logical_operator.h"

UpdateLogicalOperator::UpdateLogicalOperator(Table *table, std::vector<Value> value, std::vector<const char *> field_name)
    : table_(table), value_(value), field_name_(field_name)
{}