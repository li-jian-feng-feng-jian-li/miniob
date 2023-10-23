#include "sql/operator/sort_logical_operator.h"

SortLogicalOperator::SortLogicalOperator(std::vector<std::pair<Field, bool> > order_fields)
    : order_fields_(order_fields)
{}