

bool check_date_invalid(int y, int m, int d)
{
    static int months[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool is_leap = (y%400==0 || (y%100 && y%4==0));
    return y > 0
        && (m > 0)&&(m <= 12)
        && (d > 0)&&(d <= ((m==2 && is_leap)?1:0) + months[m]);
}

std::string date_str(int date_int){
    std::stringstream ss;
      ss << date_int;
      std::string date_string = ss.str();
      date_string.insert(4, "-");
      date_string.insert(7, "-");
      return date_string;
}