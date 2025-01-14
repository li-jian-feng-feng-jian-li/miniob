/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2023/06/28.
//

#include <sstream>
#include "sql/parser/value.h"
#include "storage/field/field.h"
#include "common/log/log.h"
#include "common/lang/comparator.h"
#include "common/lang/string.h"
#include "util/date_util.h"

const char *ATTR_TYPE_NAME[] = {"undefined", "chars", "dates", "ints", "floats", "booleans"};

const char *attr_type_to_string(AttrType type)
{
  if (type >= UNDEFINED && type <= FLOATS) {
    return ATTR_TYPE_NAME[type];
  }
  return "unknown";
}
AttrType attr_type_from_string(const char *s)
{
  for (unsigned int i = 0; i < sizeof(ATTR_TYPE_NAME) / sizeof(ATTR_TYPE_NAME[0]); i++) {
    if (0 == strcmp(ATTR_TYPE_NAME[i], s)) {
      return (AttrType)i;
    }
  }
  return UNDEFINED;
}

Value::Value(int val) { set_int(val); }

Value::Value(float val) { set_float(val); }

Value::Value(bool val) { set_boolean(val); }

Value::Value(const char *s, bool is_date, int len /*= 0*/)
{
  if (is_date)
    set_date(s, len);
  else
    set_string(s, len);
}

void Value::set_null(bool is_null) { is_null_ = is_null; }

void Value::set_data(char *data, int length)
{
  // LOG_DEBUG("set_data() calls!");
  if (is_null_) {
    set_string(data, length);
    return;
  }
  switch (attr_type_) {
    case CHARS: {
      set_string(data, length);
    } break;
    case DATES: {
      num_value_.date_value_ = *(int *)data;
      length_                = length;
    } break;
    case INTS: {
      num_value_.int_value_ = *(int *)data;
      length_               = length;
    } break;
    case FLOATS: {
      num_value_.float_value_ = *(float *)data;
      length_                 = length;
    } break;
    case BOOLEANS: {
      num_value_.bool_value_ = *(int *)data != 0;
      length_                = length;
    } break;
    default: {
      LOG_WARN("unknown data type: %d", attr_type_);
    } break;
  }
}
void Value::set_int(int val)
{
  // LOG_DEBUG("set_int() calls!");
  attr_type_            = INTS;
  num_value_.int_value_ = val;
  length_               = sizeof(val);
}

void Value::set_float(float val)
{
  // LOG_DEBUG("set_float() calls!");
  attr_type_              = FLOATS;
  num_value_.float_value_ = val;
  length_                 = sizeof(val);
}
void Value::set_boolean(bool val)
{
  // LOG_DEBUG("set_boolean() calls!");
  attr_type_             = BOOLEANS;
  num_value_.bool_value_ = val;
  length_                = sizeof(val);
}
void Value::set_string(const char *s, int len /*= 0*/)
{
  // LOG_DEBUG("set_string() calls!len is %d",len);
  // TODO should i change the type of attr_type_ to its initial type?
  attr_type_ = CHARS;
  if (len > 0) {
    len = strnlen(s, len);
    str_value_.assign(s, len);
  } else {
    str_value_.assign(s);
  }
  length_ = str_value_.length();
  // LOG_DEBUG("now str is %s,len is %d",str_value_.c_str(),length_);
}

void Value::set_date(const char *s, int len)
{
  // LOG_DEBUG("set_date() calls!");
  attr_type_ = DATES;
  int y, m, d;
  sscanf(s, "%d-%d-%d", &y, &m, &d);
  bool is_valid = check_date_invalid(y, m, d);
  if (is_valid) {
    int date_to_int        = y * 10000 + m * 100 + d;
    num_value_.date_value_ = date_to_int;
  } else
    num_value_.date_value_ = 0;
  length_ = sizeof(num_value_.date_value_);
}

void Value::set_value(const Value &value)
{
  // LOG_DEBUG("set_value() calls!");
  if (value.is_null()) {
    set_string(value.get_string().c_str());
    return;
  }
  switch (value.attr_type_) {
    case INTS: {
      set_int(value.get_int());
    } break;
    case FLOATS: {
      set_float(value.get_float());
    } break;
    case CHARS: {
      set_string(value.get_string().c_str());
    } break;
    case DATES: {
      set_date(value.get_string().c_str());
    } break;
    case BOOLEANS: {
      set_boolean(value.get_boolean());
    } break;
    case UNDEFINED: {
      ASSERT(false, "got an invalid value type");
    } break;
  }
}

const char *Value::data() const
{
  switch (attr_type_) {
    case CHARS: {
      return str_value_.c_str();
    } break;
    default: {
      return (const char *)&num_value_;
    } break;
  }
}

std::string Value::to_string() const
{
  std::stringstream os;
  if(is_null_){
    os << "null";
    return os.str();
  }
  switch (attr_type_) {
    case INTS: {
      os << num_value_.int_value_;
    } break;
    case FLOATS: {
      os << common::double_to_str(num_value_.float_value_);
    } break;
    case BOOLEANS: {
      os << num_value_.bool_value_;
    } break;
    case CHARS: {
      os << str_value_;
    } break;
    case DATES: {
      os << date_str(num_value_.date_value_);
    } break;
    default: {
      LOG_WARN("unsupported attr type: %d", attr_type_);
    } break;
  }
  return os.str();
}

int Value::compare(const Value &other) const
{
  if (this->attr_type_ == other.attr_type_) {
    switch (this->attr_type_) {
      case INTS: {
        return common::compare_int((void *)&this->num_value_.int_value_, (void *)&other.num_value_.int_value_);
      } break;
      case FLOATS: {
        return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other.num_value_.float_value_);
      } break;
      case CHARS: {
        if (is_null_ && !other.is_null()) {
          return -1;
        } else if (is_null_ && other.is_null()) {
          return 0;
        } else if (!is_null_ && other.is_null()) {
          return 1;
        } else {
          return common::compare_string((void *)this->str_value_.c_str(),
              this->str_value_.length(),
              (void *)other.str_value_.c_str(),
              other.str_value_.length());
        }
      } break;
      case DATES: {
        return common::compare_int((void *)&this->num_value_.date_value_, (void *)&other.num_value_.date_value_);
      } break;
      case BOOLEANS: {
        return common::compare_int((void *)&this->num_value_.bool_value_, (void *)&other.num_value_.bool_value_);
      }
      default: {
        LOG_WARN("unsupported type: %d", this->attr_type_);
      }
    }
  } else if (this->attr_type_ == INTS) {
    float this_data = this->num_value_.int_value_;
    if (other.attr_type_ == FLOATS) {
      return common::compare_float((void *)&this_data, (void *)&other.num_value_.float_value_);
    }
    if (other.attr_type_ == CHARS) {
      if (other.is_null()) {
        return 1;
      } else {
        float other_data;
        try {
          other_data = std::stof(other.str_value_);
        } catch (std::invalid_argument &) {
          other_data = 0.0;
        }
        return common::compare_float((void *)&this_data, (void *)&other_data);
      }
    }

  } else if (this->attr_type_ == FLOATS) {
    float other_data;
    if (other.attr_type_ == INTS) {
      other_data = other.num_value_.int_value_;
      return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other_data);
    }
    if (other.attr_type_ == CHARS) {
      if (other.is_null()) {
        return 1;
      } else {
        try {
          other_data = std::stof(other.str_value_);
        } catch (std::invalid_argument &) {
          other_data = 0.0;
        }
        return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other_data);
      }
    }
  } else if (this->attr_type_ == CHARS) {
    if (is_null_) {
      return -1;
    } else {
      float other_data;
      if (other.attr_type_ == INTS)
        other_data = other.num_value_.int_value_;
      if (other.attr_type_ == FLOATS)
        other_data = other.num_value_.float_value_;
      float this_data;
      try {
        this_data = std::stof(this->str_value_);
      } catch (std::invalid_argument &) {
        this_data = 0.0;
      }
      return common::compare_float((void *)&this_data, (void *)&other_data);
    }
  }
  LOG_WARN("not supported");
  return -1;  // TODO return rc?
}

bool Value::is_null() const { return is_null_; }

int Value::get_int() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        return (int)(std::stol(str_value_));
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to number. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return 0;
      }
    }
    case INTS: {
      return num_value_.int_value_;
    }
    case FLOATS: {
      return (int)(num_value_.float_value_);
    }
    case BOOLEANS: {
      return (int)(num_value_.bool_value_);
    }
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return 0;
    }
  }
  return 0;
}

float Value::get_float() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        return std::stof(str_value_);
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to float. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return 0.0;
      }
    } break;
    case INTS: {
      return float(num_value_.int_value_);
    } break;
    case FLOATS: {
      return num_value_.float_value_;
    } break;
    case BOOLEANS: {
      return float(num_value_.bool_value_);
    } break;
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return 0;
    }
  }
  return 0;
}

std::string Value::get_string() const { return this->to_string(); }

int Value::get_date() const
{
  if (attr_type_ == DATES)
    return num_value_.date_value_;
  else {
    LOG_WARN("unknown data type. type=%d", attr_type_);
    return 0;
  }
}

bool Value::get_boolean() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        float val = std::stof(str_value_);
        if (val >= EPSILON || val <= -EPSILON) {
          return true;
        }

        int int_val = std::stol(str_value_);
        if (int_val != 0) {
          return true;
        }

        return !str_value_.empty();
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to float or integer. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return !str_value_.empty();
      }
    } break;
    case INTS: {
      return num_value_.int_value_ != 0;
    } break;
    case FLOATS: {
      float val = num_value_.float_value_;
      return val >= EPSILON || val <= -EPSILON;
    } break;
    case BOOLEANS: {
      return num_value_.bool_value_;
    } break;
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return false;
    }
  }
  return false;
}
