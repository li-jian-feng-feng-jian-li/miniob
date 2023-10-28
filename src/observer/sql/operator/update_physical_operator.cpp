#include "common/log/log.h"
#include "sql/operator/update_physical_operator.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include "sql/stmt/update_stmt.h"

UpdatePhysicalOperator::UpdatePhysicalOperator(
    Table *table, std::vector<UpdateValueSqlNode> value, std::vector<const char *> field_name)
    : table_(table), value_(value), field_name_(field_name)
{}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  for (auto &p : children_) {
    std::unique_ptr<PhysicalOperator> &child = p;
    RC                                 rc    = child->open(trx);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to open child operator: %s", strrc(rc));
      return rc;
    }
  }

  trx_ = trx;
  LOG_DEBUG("trx opened!");

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next()
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }
  std::vector<Record> rec_to_upd;
  PhysicalOperator   *child = children_.back().get();
  while (RC::SUCCESS == (rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple  = static_cast<RowTuple *>(tuple);
    Record   &old_record = row_tuple->record();
    rec_to_upd.emplace_back(old_record);
  }

  std::vector<Value> update_value;
  int                child_oper_index = 0;
  for (auto &p : value_) {
    if (p.is_value) {
      update_value.emplace_back(std::get<1>(p.update_value));
    } else {
      PhysicalOperator  *child = children_[child_oper_index++].get();
      std::vector<Value> select_value;
      while (RC::SUCCESS == (rc = child->next())) {
        Tuple *tuple = child->current_tuple();
        if (nullptr == tuple) {
          LOG_WARN("failed to get current record: %s", strrc(rc));
          return rc;
        }

        ProjectTuple *project_tuple = static_cast<ProjectTuple *>(tuple);
        if (project_tuple->cell_num() != 1) {
          LOG_WARN("select operation returns with more than one field!");
          rc = RC::INVALID_ARGUMENT;
          return rc;
        }

        Value value;
        rc = project_tuple->cell_at(0, value);
        if (rc != RC::SUCCESS) {
          LOG_WARN("failed to find cell at 0: %s",strrc(rc));
          return rc;
        }
        select_value.emplace_back(value);
      }
      if (select_value.size() > 1) {
        LOG_WARN("select operation returns more than one row!");
        rc = RC::INVALID_ARGUMENT;
        return rc;
      } else if (select_value.size() == 0) {
        Value *value = new Value("null", false);
        value->set_null();
        update_value.emplace_back(*value);
        delete value;
      } else {
        update_value.emplace_back(select_value[0]);
      }
    }
  }

  for (int i = 0; i < rec_to_upd.size(); i++) {
    rc = trx_->update_record(table_, rec_to_upd[i], update_value, field_name_);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to update record: %s", strrc(rc));
      return rc;
    }
  }
  LOG_DEBUG("update finished!");

  return RC::RECORD_EOF;
}

RC UpdatePhysicalOperator::close()
{
  if (!children_.empty()) {
    for (int i = 0; i < children_.size(); i++) {
      children_[i]->close();
    }
  }
  return RC::SUCCESS;
}
