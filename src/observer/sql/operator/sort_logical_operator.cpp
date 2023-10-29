#include "sql/operator/sort_logical_operator.h"

// 此处只定义实现
SortLogicalOperator::SortLogicalOperator(std::vector<std::pair<Field, bool> > order_fields,std::vector<std::pair<Field, std::string> > agg_fields,bool is_count_star)
    : order_fields_(order_fields), agg_fields_(agg_fields)
{
    is_count_star_ =  is_count_star;
}