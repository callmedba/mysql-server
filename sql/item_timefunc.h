#ifndef ITEM_TIMEFUNC_INCLUDED
#define ITEM_TIMEFUNC_INCLUDED

/* Copyright (c) 2000, 2011, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */


/* Function items used by mysql */

class MY_LOCALE;

bool get_interval_value(Item *args,interval_type int_type,
			       String *str_value, INTERVAL *interval);

class Item_func_period_add :public Item_int_func
{
public:
  Item_func_period_add(Item *a,Item *b) :Item_int_func(a,b) {}
  longlong val_int();
  const char *func_name() const { return "period_add"; }
  void fix_length_and_dec() 
  { 
    fix_char_length(6); /* YYYYMM */
  }
};


class Item_func_period_diff :public Item_int_func
{
public:
  Item_func_period_diff(Item *a,Item *b) :Item_int_func(a,b) {}
  longlong val_int();
  const char *func_name() const { return "period_diff"; }
  void fix_length_and_dec()
  { 
    fix_char_length(6); /* YYYYMM */
  }
};


class Item_func_to_days :public Item_int_func
{
public:
  Item_func_to_days(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "to_days"; }
  void fix_length_and_dec() 
  { 
    fix_char_length(6);
    maybe_null=1; 
  }
  enum_monotonicity_info get_monotonicity_info() const;
  longlong val_int_endpoint(bool left_endp, bool *incl_endp);
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};


class Item_func_to_seconds :public Item_int_func
{
public:
  Item_func_to_seconds(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "to_seconds"; }
  void fix_length_and_dec() 
  { 
    fix_char_length(6);
    maybe_null=1; 
  }
  enum_monotonicity_info get_monotonicity_info() const;
  longlong val_int_endpoint(bool left_endp, bool *incl_endp);
  bool check_partition_func_processor(uchar *bool_arg) { return FALSE;}

  bool intro_version(uchar *int_arg)
  {
    int *input_version= (int*)int_arg;
    /* This function was introduced in 5.5 */
    int output_version= max(*input_version, 50500);
    *input_version= output_version;
    return 0;
  }

  /* Only meaningful with date part and optional time part */
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};


class Item_func_dayofmonth :public Item_int_func
{
public:
  Item_func_dayofmonth(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "dayofmonth"; }
  void fix_length_and_dec() 
  { 
    fix_char_length(2); /* 1..31 */
    maybe_null=1; 
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};


class Item_func_month :public Item_func
{
public:
  Item_func_month(Item *a) :Item_func(a) { collation.set_numeric(); }
  longlong val_int();
  double val_real()
  { DBUG_ASSERT(fixed == 1); return (double) Item_func_month::val_int(); }
  String *val_str(String *str) 
  {
    longlong nr= val_int();
    if (null_value)
      return 0;
    str->set(nr, collation.collation);
    return str;
  }
  const char *func_name() const { return "month"; }
  enum Item_result result_type () const { return INT_RESULT; }
  void fix_length_and_dec() 
  { 
    fix_char_length(2);
    maybe_null= 1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};


class Item_func_monthname :public Item_str_func
{
  MY_LOCALE *locale;
public:
  Item_func_monthname(Item *a) :Item_str_func(a) {}
  const char *func_name() const { return "monthname"; }
  String *val_str(String *str);
  void fix_length_and_dec();
  bool check_partition_func_processor(uchar *int_arg) {return TRUE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};


class Item_func_dayofyear :public Item_int_func
{
public:
  Item_func_dayofyear(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "dayofyear"; }
  void fix_length_and_dec() 
  { 
    fix_char_length(3);
    maybe_null= 1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};


class Item_func_hour :public Item_int_func
{
public:
  Item_func_hour(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "hour"; }
  void fix_length_and_dec()
  {
    fix_char_length(2); /* 0..23 */
    maybe_null=1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_time_args();
  }
};


class Item_func_minute :public Item_int_func
{
public:
  Item_func_minute(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "minute"; }
  void fix_length_and_dec()
  {
    fix_char_length(2); /* 0..59 */
    maybe_null=1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_time_args();
  }
};


class Item_func_quarter :public Item_int_func
{
public:
  Item_func_quarter(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "quarter"; }
  void fix_length_and_dec()
  { 
     fix_char_length(1); /* 1..4 */
     maybe_null=1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};


class Item_func_second :public Item_int_func
{
public:
  Item_func_second(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "second"; }
  void fix_length_and_dec() 
  { 
    fix_char_length(2); /* 0..59 */
    maybe_null=1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_time_args();
  }
};


class Item_func_week :public Item_int_func
{
public:
  Item_func_week(Item *a,Item *b) :Item_int_func(a,b) {}
  longlong val_int();
  const char *func_name() const { return "week"; }
  void fix_length_and_dec()
  { 
    fix_char_length(2); /* 0..54 */
    maybe_null=1;
  }
};

class Item_func_yearweek :public Item_int_func
{
public:
  Item_func_yearweek(Item *a,Item *b) :Item_int_func(a,b) {}
  longlong val_int();
  const char *func_name() const { return "yearweek"; }
  void fix_length_and_dec()
  { 
    fix_char_length(6); /* YYYYWW */
    maybe_null=1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};


class Item_func_year :public Item_int_func
{
public:
  Item_func_year(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "year"; }
  enum_monotonicity_info get_monotonicity_info() const;
  longlong val_int_endpoint(bool left_endp, bool *incl_endp);
  void fix_length_and_dec()
  { 
    fix_char_length(4); /* 9999 */
    maybe_null=1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};


class Item_func_weekday :public Item_func
{
  bool odbc_type;
public:
  Item_func_weekday(Item *a,bool type_arg)
    :Item_func(a), odbc_type(type_arg) { collation.set_numeric(); }
  longlong val_int();
  double val_real() { DBUG_ASSERT(fixed == 1); return (double) val_int(); }
  String *val_str(String *str)
  {
    DBUG_ASSERT(fixed == 1);
    str->set(val_int(), &my_charset_bin);
    return null_value ? 0 : str;
  }
  const char *func_name() const
  {
     return (odbc_type ? "dayofweek" : "weekday");
  }
  enum Item_result result_type () const { return INT_RESULT; }
  void fix_length_and_dec()
  {
    fix_char_length(1);
    maybe_null= 1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_date_args();
  }
};

class Item_func_dayname :public Item_func_weekday
{
  MY_LOCALE *locale;
 public:
  Item_func_dayname(Item *a) :Item_func_weekday(a,0) {}
  const char *func_name() const { return "dayname"; }
  String *val_str(String *str);
  enum Item_result result_type () const { return STRING_RESULT; }
  void fix_length_and_dec();
  bool check_partition_func_processor(uchar *int_arg) {return TRUE;}
};


/*
  Abstract class for functions returning "struct timeval".
*/
class Item_timeval_func :public Item_func
{
public:
  Item_timeval_func() :Item_func() { }
  Item_timeval_func(Item *a) :Item_func(a) { }
  /**
    Return timestamp in "struct timeval" format.
    @param OUT tm The value is store here.
    @retval false On success
    @retval true  On error
  */
  virtual bool val_timeval(struct timeval *tm)= 0;
  longlong val_int();
  double val_real();
  String *val_str(String *str);
  my_decimal *val_decimal(my_decimal *decimal_value);
  enum Item_result result_type() const
  {
    return decimals ? DECIMAL_RESULT : INT_RESULT;
  }
};


class Item_func_unix_timestamp :public Item_timeval_func
{
public:
  Item_func_unix_timestamp() :Item_timeval_func() {}
  Item_func_unix_timestamp(Item *a) :Item_timeval_func(a) {}
  const char *func_name() const { return "unix_timestamp"; }
  enum_monotonicity_info get_monotonicity_info() const;
  longlong val_int_endpoint(bool left_endp, bool *incl_endp);
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  /*
    UNIX_TIMESTAMP() depends on the current timezone
    (and thus may not be used as a partitioning function)
    when its argument is NOT of the TIMESTAMP type.
  */
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_timestamp_args();
  }
  void fix_length_and_dec()
  {
    fix_length_and_dec_and_charset_datetime(11, arg_count ==  0 ?  0 :
                                                args[0]->datetime_precision());
  }
  bool val_timeval(struct timeval *tm);
};


class Item_func_time_to_sec :public Item_int_func
{
public:
  Item_func_time_to_sec(Item *item) :Item_int_func(item) {}
  longlong val_int();
  const char *func_name() const { return "time_to_sec"; }
  void fix_length_and_dec()
  {
    maybe_null= TRUE;
    fix_char_length(10);
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_time_args();
  }
};


/**
  Abstract class for functions returning TIME, DATE, DATETIME types
  whose data type is known at constructor time.
*/
class Item_temporal_func :public Item_func
{
protected:
  bool check_precision();
public:
  Item_temporal_func() :Item_func() {}
  Item_temporal_func(Item *a) :Item_func(a) {}
  Item_temporal_func(Item *a, Item *b) :Item_func(a, b) {}
  Item_temporal_func(Item *a, Item *b, Item *c) :Item_func(a, b, c) {}
  enum Item_result result_type () const
  {
    return STRING_RESULT;
  }
  CHARSET_INFO *charset_for_protocol() const
  {
    return &my_charset_bin;
  }
  Field *tmp_table_field(TABLE *table)
  {
    return tmp_table_field_from_field_type(table, 0);
  }
  uint time_precision()
  {
    DBUG_ASSERT(fixed);
    return decimals;
  }
  uint datetime_precision()
  {
    DBUG_ASSERT(fixed);
    return decimals;
  }
  enum Item_result numeric_context_result_type() const
  {
    DBUG_ASSERT(fixed);
    return decimals ? DECIMAL_RESULT : INT_RESULT;
  }
};


/**
  Abstract class for functions returning TIME, DATE, DATETIME or string values,
  whose data type depends on parameters and is set at fix_field time.
*/
class Item_temporal_hybrid_func :public Item_str_func
{
protected:
  sql_mode_t sql_mode; // sql_mode value is cached here in fix_length_and_dec()
  enum_field_types cached_field_type; // TIME, DATE, DATETIME or STRING
  String ascii_buf; // Conversion buffer
  /**
    Get "native" temporal value as MYSQL_TIME
    @param OUT ltime       The value is stored here.
    @param IN  fuzzy_date  Date flags.
    @retval    false       On success.
    @retval    true        On error.
  */
  virtual bool val_datetime(MYSQL_TIME *ltime, uint fuzzy_date)= 0; 

public:
  Item_temporal_hybrid_func(Item *a, Item *b) :Item_str_func(a, b),
    sql_mode(0)
  { }
  enum Item_result result_type () const { return STRING_RESULT; }
  enum Item_result numeric_context_result_type() const
  {
    DBUG_ASSERT(fixed);
    return cached_field_type == MYSQL_TYPE_STRING ? STRING_RESULT :
           decimals ? DECIMAL_RESULT : INT_RESULT;
  }
  enum_field_types field_type() const { return cached_field_type; }
  const CHARSET_INFO *charset_for_protocol() const
  {
    /*
      Can return TIME, DATE, DATETIME or VARCHAR depending on arguments.
      Send using "binary" when TIME, DATE or DATETIME,
      or using collation.collation when VARCHAR
      (which is fixed from @collation_connection in fix_length_and_dec).
    */
    DBUG_ASSERT(fixed == 1);
    return cached_field_type == MYSQL_TYPE_STRING ?
                                collation.collation : &my_charset_bin;
  }
  Field *tmp_table_field(TABLE *table)
  {
    return tmp_table_field_from_field_type(table, 0);
  }
  longlong val_int() { return val_int_from_decimal(); }
  double val_real() { return val_real_from_decimal(); }
  my_decimal *val_decimal(my_decimal *decimal_value);
  int save_in_field(Field *field, bool no_conversions);
  longlong val_temporal();
  /**
    Return string value in ASCII character set.
  */
  String *val_str_ascii(String *str);
  /**
    Return string value in @@character_set_connection.
  */
  String *val_str(String *str)
  {
    return val_str_from_val_str_ascii(str, &ascii_buf);
  }
  bool get_date(MYSQL_TIME *ltime, uint fuzzydate);
  bool get_time(MYSQL_TIME *ltime);
};


/*
  This can't be a Item_str_func, because the val_real() functions are special
*/

/**
  Abstract class for functions returning DATE values.
*/
class Item_date_func :public Item_temporal_func
{
public:
  Item_date_func() :Item_temporal_func()
  { unsigned_flag= 1; }
  Item_date_func(Item *a) :Item_temporal_func(a)
  { unsigned_flag= 1; }
  Item_date_func(Item *a, Item *b) :Item_temporal_func(a, b)
  { unsigned_flag= 1; }
  enum_field_types field_type() const { return MYSQL_TYPE_DATE; }
  bool get_time(MYSQL_TIME *ltime);
  String *val_str(String *str);
  longlong val_int();
  longlong val_date_temporal();
  double val_real() { return (double) val_int(); }
  const char *func_name() const { return "date"; }
  void fix_length_and_dec()
  { 
    fix_length_and_dec_and_charset_datetime(MAX_DATE_WIDTH, 0);
  }
  my_decimal *val_decimal(my_decimal *decimal_value)
  {
    DBUG_ASSERT(fixed == 1);
    return  val_decimal_from_date(decimal_value);
  }
  int save_in_field(Field *field, bool no_conversions)
  {
    return save_date_in_field(field);
  }
  // All date functions must implement get_date()
  // to avoid use of generic Item::get_date()
  // which converts to string and then parses the string as DATE.
  virtual bool get_date(MYSQL_TIME *res, uint fuzzy_date)= 0;
};


/**
  Abstract class for functions returning DATETIME values.
*/
class Item_datetime_func :public Item_temporal_func
{
public:
  Item_datetime_func() :Item_temporal_func()
  { unsigned_flag= 1; }
  Item_datetime_func(Item *a) :Item_temporal_func(a)
  { unsigned_flag= 1; }
  Item_datetime_func(Item *a,Item *b) :Item_temporal_func(a,b)
  { unsigned_flag= 1; }
  Item_datetime_func(Item *a,Item *b, Item *c) :Item_temporal_func(a,b,c)
  { unsigned_flag= 1; }
  enum_field_types field_type() const { return MYSQL_TYPE_DATETIME; }
  double val_real() { return val_real_from_decimal(); }
  String *val_str(String *str);
  longlong val_int();
  longlong val_date_temporal();
  my_decimal *val_decimal(my_decimal *decimal_value)
  {
    DBUG_ASSERT(fixed == 1);
    return  val_decimal_from_date(decimal_value);
  }
  int save_in_field(Field *field, bool no_conversions)
  {
    return save_date_in_field(field);
  }
  bool get_time(MYSQL_TIME *ltime);
  // All datetime functions must implement get_date()
  // to avoid use of generic Item::get_date()
  // which converts to string and then parses the string as DATETIME.
  virtual bool get_date(MYSQL_TIME *res, uint fuzzy_date)= 0;
};


/**
  Abstract class for functions returning TIME values.
*/
class Item_time_func :public Item_temporal_func
{
public:
  Item_time_func() :Item_temporal_func() {}
  Item_time_func(Item *a) :Item_temporal_func(a) {}
  Item_time_func(Item *a,Item *b) :Item_temporal_func(a,b) {}
  Item_time_func(Item *a, Item *b, Item *c) :Item_temporal_func(a, b ,c) {}
  enum_field_types field_type() const { return MYSQL_TYPE_TIME; }
  double val_real() { return val_real_from_decimal(); }
  my_decimal *val_decimal(my_decimal *decimal_value)
  {
    DBUG_ASSERT(fixed == 1);
    return  val_decimal_from_time(decimal_value);
  }
  int save_in_field(Field *field, bool no_conversions)
  {
    return save_time_in_field(field);
  }
  longlong val_int();
  longlong val_time_temporal();
  bool get_date(MYSQL_TIME *res, uint fuzzy_date)
  {
    return get_date_from_time(res);
  }
  String *val_str(String *str);
  // All time functions must implement get_time()
  // to avoid use of generic Item::get_time()
  // which converts to string and then parses the string as TIME.
  virtual bool get_time(MYSQL_TIME *res)= 0;
};


/**
  DATE'2010-01-01'
*/
class Item_date_literal :public Item_date_func
{
  MYSQL_TIME ctime;
  longlong ctime_as_temporal;
public:
  Item_date_literal(MYSQL_TIME *ltime) :Item_date_func()
  {
    ctime= *ltime;
    ctime_as_temporal= TIME_to_longlong_date_packed(&ctime);
    fix_length_and_dec();
    fixed= 1;
  }
  const char *func_name() const { return "date_literal"; }
  void print(String *str, enum_query_type query_type);
  longlong val_date_temporal()
  {
    DBUG_ASSERT(fixed);
    return ctime_as_temporal;
  }
  bool get_date(MYSQL_TIME *ltime, uint fuzzy_date)
  {
    DBUG_ASSERT(fixed);
    *ltime= ctime;
    return false;
  }
  void fix_length_and_dec()
  {
    fix_length_and_dec_and_charset_datetime(MAX_DATE_WIDTH, 0);
  }
  bool check_partition_func_processor(uchar *int_arg)
  {
    return FALSE;
  }
  bool basic_const_item() const { return true; }
  bool const_item() const { return true; }
  table_map used_tables() const { return (table_map) 0L; }
  void cleanup()
  {
    // See Item_basic_const::cleanup()
    if (orig_name)
      name= orig_name;
  }
  bool eq(const Item *item, bool binary_cmp) const;
};


/**
  TIME'10:10:10'
*/
class Item_time_literal :public Item_time_func
{
  MYSQL_TIME ctime;
  longlong ctime_as_temporal;
public:
  Item_time_literal(MYSQL_TIME *ltime, uint dec_arg) :Item_time_func()
  {
    ctime= *ltime;
    ctime_as_temporal= TIME_to_longlong_time_packed(&ctime);
    decimals= min(dec_arg, DATETIME_MAX_DECIMALS);
    fix_length_and_dec();
    fixed= 1;
  }
  const char *func_name() const { return "time_literal"; }
  void print(String *str, enum_query_type query_type);
  longlong val_time_temporal()
  {
    DBUG_ASSERT(fixed);
    return ctime_as_temporal;
  }
  bool get_time(MYSQL_TIME *ltime)
  {
    DBUG_ASSERT(fixed);
    *ltime= ctime;
    return false;
  }
  void fix_length_and_dec()
  {
    fix_length_and_dec_and_charset_datetime(MAX_TIME_WIDTH, decimals);
  }
  bool check_partition_func_processor(uchar *int_arg)
  {
    return FALSE;
  }
  bool basic_const_item() const { return true; }
  bool const_item() const { return true; }
  table_map used_tables() const { return (table_map) 0L; }
  void cleanup()
  {
    // See Item_basic_const::cleanup()
    if (orig_name)
      name= orig_name;
  }
  bool eq(const Item *item, bool binary_cmp) const;
};


/**
  TIMESTAMP'2001-01-01 10:20:30'
*/
class Item_datetime_literal :public Item_datetime_func
{
  MYSQL_TIME ctime;
  longlong ctime_as_temporal;
public:
  Item_datetime_literal(MYSQL_TIME *ltime, uint dec_arg) :Item_datetime_func()
  {
    ctime= *ltime;
    ctime_as_temporal= TIME_to_longlong_datetime_packed(&ctime);
    decimals= min(dec_arg, DATETIME_MAX_DECIMALS);
    fix_length_and_dec();
    fixed= 1;
  }
  const char *func_name() const { return "datetime_literal"; }
  void print(String *str, enum_query_type query_type);
  longlong val_date_temporal()
  {
    DBUG_ASSERT(fixed);
    return ctime_as_temporal;
  }
  bool get_date(MYSQL_TIME *ltime, uint fuzzy_date)
  {
    DBUG_ASSERT(fixed);
    *ltime= ctime;
    return false;
  }
  void fix_length_and_dec()
  {
    fix_length_and_dec_and_charset_datetime(MAX_DATETIME_WIDTH, decimals);
  }
  bool check_partition_func_processor(uchar *int_arg)
  {
    return FALSE;
  }
  bool basic_const_item() const { return true; }
  bool const_item() const { return true; }
  table_map used_tables() const { return (table_map) 0L; }
  void cleanup()
  {
    // See Item_basic_const::cleanup()
    if (orig_name)
      name= orig_name;
  }
  bool eq(const Item *item, bool binary_cmp) const;
};


/* Abstract CURTIME function. Children should define what time zone is used */

class Item_func_curtime :public Item_time_func
{
  char buff[9*2+32];
  uint buff_length;
  MYSQL_TIME ctime;
  longlong ctime_as_temporal;
protected:
  void store_now_in_TIME_tz(THD *thd, Time_zone *tz, MYSQL_TIME *now_time);
  /* 
    Abstract method that defines which time zone is used for conversion.
    Converts time current time in my_time_t representation to broken-down
    MYSQL_TIME representation using UTC-SYSTEM or per-thread time zone.
  */
  virtual void store_now_in_TIME(MYSQL_TIME *now_time)=0;
public:
  Item_func_curtime(uint8 dec_arg) :Item_time_func() { decimals= dec_arg; }
  longlong val_time_temporal()
  {
    DBUG_ASSERT(fixed == 1);
    return ctime_as_temporal;
  }
  String *val_str(String *str);
  void fix_length_and_dec();
  bool get_time(MYSQL_TIME *ltime);
};


class Item_func_curtime_local :public Item_func_curtime
{
protected:
  virtual void store_now_in_TIME(MYSQL_TIME *now_time);
public:
  Item_func_curtime_local(ulong dec_arg) :Item_func_curtime(dec_arg) {}
  const char *func_name() const { return "curtime"; }
};


class Item_func_curtime_utc :public Item_func_curtime
{
protected:
  virtual void store_now_in_TIME(MYSQL_TIME *now_time);
public:
  Item_func_curtime_utc(ulong dec_arg) :Item_func_curtime(dec_arg) {}
  const char *func_name() const { return "utc_time"; }
};


/* Abstract CURDATE function. See also Item_func_curtime. */

class Item_func_curdate :public Item_date_func
{
  longlong ctime_as_longlong; // Initialized in fix_length_and_dec
  MYSQL_TIME ctime;           // Initialized in fix_length_and_dec
protected:
  virtual void store_now_in_TIME(MYSQL_TIME *now_time)=0;
public:
  Item_func_curdate() :Item_date_func() {}
  longlong val_date_temporal()
  {
    DBUG_ASSERT(fixed == 1);
    return (ctime_as_longlong) ;
  }
  String *val_str(String *str);
  void fix_length_and_dec();
  bool get_date(MYSQL_TIME *res, uint fuzzy_date);
};


class Item_func_curdate_local :public Item_func_curdate
{
protected:
  void store_now_in_TIME(MYSQL_TIME *now_time);
public:
  Item_func_curdate_local() :Item_func_curdate() {}
  const char *func_name() const { return "curdate"; }
};


class Item_func_curdate_utc :public Item_func_curdate
{
protected:
  void store_now_in_TIME(MYSQL_TIME *now_time);
public:
  Item_func_curdate_utc() :Item_func_curdate() {}
  const char *func_name() const { return "utc_date"; }
};


/* Abstract CURRENT_TIMESTAMP function. See also Item_func_curtime */

class Item_func_now :public Item_datetime_func
{
protected:
  longlong ctime_as_longlong;
  char buff[20*2+32];	// +32 to make my_snprintf_{8bit|ucs2} happy
  uint buff_length;
  MYSQL_TIME ctime;
  virtual void store_now_in_TIME(MYSQL_TIME *now_time)=0;
public:
  Item_func_now(uint8 dec_arg) :Item_datetime_func() { decimals= dec_arg; }
  longlong val_date_temporal()
  {
    DBUG_ASSERT(fixed == 1);
    return ctime_as_longlong;
  }
  int save_in_field(Field *to, bool no_conversions);
  String *val_str(String *str);
  void fix_length_and_dec();
  bool get_date(MYSQL_TIME *res, uint fuzzy_date);
};


class Item_func_now_local :public Item_func_now
{
protected:
  virtual void store_now_in_TIME(MYSQL_TIME *now_time);
public:
  Item_func_now_local(ulong dec_arg) :Item_func_now(dec_arg) {}
  const char *func_name() const { return "now"; }
  virtual enum Functype functype() const { return NOW_FUNC; }
};


class Item_func_now_utc :public Item_func_now
{
protected:
  virtual void store_now_in_TIME(MYSQL_TIME *now_time);
public:
  Item_func_now_utc(ulong dec_arg) :Item_func_now(dec_arg) {}
  const char *func_name() const { return "utc_timestamp"; }
};


/*
  This is like NOW(), but always uses the real current time, not the
  query_start(). This matches the Oracle behavior.
*/
class Item_func_sysdate_local :public Item_datetime_func
{
public:
  Item_func_sysdate_local(uint8 dec_arg) :
    Item_datetime_func() { decimals= dec_arg; }
  bool const_item() const { return 0; }
  const char *func_name() const { return "sysdate"; }
  void fix_length_and_dec();
  bool get_date(MYSQL_TIME *res, uint fuzzy_date);
  void update_used_tables()
  {
    Item_datetime_func::update_used_tables();
    used_tables_cache|= RAND_TABLE_BIT;
  }
};


class Item_func_from_days :public Item_date_func
{
public:
  Item_func_from_days(Item *a) :Item_date_func(a) {}
  const char *func_name() const { return "from_days"; }
  bool get_date(MYSQL_TIME *res, uint fuzzy_date);
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return has_date_args() || has_time_args();
  }
};


class Item_func_date_format :public Item_str_func
{
  int fixed_length;
  const bool is_time_format;
  String value;
public:
  Item_func_date_format(Item *a,Item *b,bool is_time_format_arg)
    :Item_str_func(a,b),is_time_format(is_time_format_arg) {}
  String *val_str(String *str);
  const char *func_name() const
    { return is_time_format ? "time_format" : "date_format"; }
  void fix_length_and_dec();
  uint format_length(const String *format);
  bool eq(const Item *item, bool binary_cmp) const;
};


class Item_func_from_unixtime :public Item_datetime_func
{
  THD *thd;
 public:
  Item_func_from_unixtime(Item *a) :Item_datetime_func(a) {}
  const char *func_name() const { return "from_unixtime"; }
  void fix_length_and_dec();
  bool get_date(MYSQL_TIME *res, uint fuzzy_date);
};


/* 
  We need Time_zone class declaration for storing pointers in
  Item_func_convert_tz.
*/
class Time_zone;

/*
  This class represents CONVERT_TZ() function.
  The important fact about this function that it is handled in special way.
  When such function is met in expression time_zone system tables are added
  to global list of tables to open, so later those already opened and locked
  tables can be used during this function calculation for loading time zone
  descriptions.
*/
class Item_func_convert_tz :public Item_datetime_func
{
  /*
    If time zone parameters are constants we are caching objects that
    represent them (we use separate from_tz_cached/to_tz_cached members
    to indicate this fact, since NULL is legal value for from_tz/to_tz
    members.
  */
  bool from_tz_cached, to_tz_cached;
  Time_zone *from_tz, *to_tz;
 public:
  Item_func_convert_tz(Item *a, Item *b, Item *c):
    Item_datetime_func(a, b, c), from_tz_cached(0), to_tz_cached(0) {}
  const char *func_name() const { return "convert_tz"; }
  void fix_length_and_dec();
  bool get_date(MYSQL_TIME *res, uint fuzzy_date);
  void cleanup();
};


class Item_func_sec_to_time :public Item_time_func
{
public:
  Item_func_sec_to_time(Item *item) :Item_time_func(item) {}
  double val_real()
  {
    DBUG_ASSERT(fixed == 1);
    return (double) Item_func_sec_to_time::val_int();
  }
  void fix_length_and_dec()
  { 
    maybe_null=1;
    fix_length_and_dec_and_charset_datetime(MAX_TIME_WIDTH,
                                            min(args[0]->decimals,
                                                DATETIME_MAX_DECIMALS));
  }
  const char *func_name() const { return "sec_to_time"; }
  bool get_time(MYSQL_TIME *ltime);
};


class Item_date_add_interval :public Item_temporal_hybrid_func
{
  String value;
  bool get_date_internal(MYSQL_TIME *res, uint fuzzy_date);
  bool get_time_internal(MYSQL_TIME *res);
protected:
  bool val_datetime(MYSQL_TIME *ltime, uint fuzzy_date);

public:
  const interval_type int_type; // keep it public
  const bool date_sub_interval; // keep it public
  Item_date_add_interval(Item *a,Item *b,interval_type type_arg,bool neg_arg)
    :Item_temporal_hybrid_func(a, b),
     int_type(type_arg), date_sub_interval(neg_arg) {}
  const char *func_name() const { return "date_add_interval"; }
  void fix_length_and_dec();
  bool eq(const Item *item, bool binary_cmp) const;
  void print(String *str, enum_query_type query_type);
};


class Item_extract :public Item_int_func
{
  bool date_value;
 public:
  const interval_type int_type; // keep it public
  Item_extract(interval_type type_arg, Item *a)
    :Item_int_func(a), int_type(type_arg) {}
  longlong val_int();
  enum Functype functype() const { return EXTRACT_FUNC; }
  const char *func_name() const { return "extract"; }
  void fix_length_and_dec();
  bool eq(const Item *item, bool binary_cmp) const;
  virtual void print(String *str, enum_query_type query_type);
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    switch (int_type) {
    case INTERVAL_YEAR:
    case INTERVAL_YEAR_MONTH:
    case INTERVAL_QUARTER:
    case INTERVAL_MONTH:
    /* case INTERVAL_WEEK: Not allowed as partitioning function, bug#57071 */
    case INTERVAL_DAY:
      return !has_date_args();
    case INTERVAL_DAY_HOUR:
    case INTERVAL_DAY_MINUTE:
    case INTERVAL_DAY_SECOND:
    case INTERVAL_DAY_MICROSECOND:
      return !has_datetime_args();
    case INTERVAL_HOUR:
    case INTERVAL_HOUR_MINUTE:
    case INTERVAL_HOUR_SECOND:
    case INTERVAL_MINUTE:
    case INTERVAL_MINUTE_SECOND:
    case INTERVAL_SECOND:
    case INTERVAL_MICROSECOND:
    case INTERVAL_HOUR_MICROSECOND:
    case INTERVAL_MINUTE_MICROSECOND:
    case INTERVAL_SECOND_MICROSECOND:
      return !has_time_args();
    default:
      /*
        INTERVAL_LAST is only an end marker,
        INTERVAL_WEEK depends on default_week_format which is a session
        variable and cannot be used for partitioning. See bug#57071.
      */
      break;
    }
    return true;
  }
};


class Item_typecast :public Item_str_func
{
public:
  Item_typecast(Item *a) :Item_str_func(a) {}
  String *val_str(String *a)
  {
    DBUG_ASSERT(fixed == 1);
    String *tmp=args[0]->val_str(a);
    null_value=args[0]->null_value;
    if (tmp)
      tmp->set_charset(collation.collation);
    return tmp;
  }
  void fix_length_and_dec()
  {
    collation.set(&my_charset_bin);
    max_length=args[0]->max_length;
  }
  virtual const char* cast_type() const= 0;
  virtual void print(String *str, enum_query_type query_type);
};


class Item_char_typecast :public Item_typecast
{
  int cast_length;
  const CHARSET_INFO *cast_cs, *from_cs;
  bool charset_conversion;
  String tmp_value;
public:
  Item_char_typecast(Item *a, int length_arg, const CHARSET_INFO *cs_arg)
    :Item_typecast(a), cast_length(length_arg), cast_cs(cs_arg) {}
  enum Functype functype() const { return CHAR_TYPECAST_FUNC; }
  bool eq(const Item *item, bool binary_cmp) const;
  const char *func_name() const { return "cast_as_char"; }
  const char* cast_type() const { return "char"; };
  String *val_str(String *a);
  void fix_length_and_dec();
  virtual void print(String *str, enum_query_type query_type);
};


class Item_date_typecast :public Item_date_func
{
public:
  Item_date_typecast(Item *a) :Item_date_func(a) { maybe_null= 1; }
  void print(String *str, enum_query_type query_type);
  const char *func_name() const { return "cast_as_date"; }
  bool get_date(MYSQL_TIME *ltime, uint fuzzy_date);
  const char *cast_type() const { return "date"; }
};


class Item_time_typecast :public Item_time_func
{
  bool detect_precision_from_arg;
public:
  Item_time_typecast(Item *a): Item_time_func(a)
  {
    detect_precision_from_arg= true;
  }
  Item_time_typecast(Item *a, uint8 dec_arg): Item_time_func(a)
  {
    detect_precision_from_arg= false;
    decimals= dec_arg;
  }
  void print(String *str, enum_query_type query_type);
  const char *func_name() const { return "cast_as_time"; }
  bool get_time(MYSQL_TIME *ltime);
  const char *cast_type() const { return "time"; }
  void fix_length_and_dec()
  {
    maybe_null= 1;
    fix_length_and_dec_and_charset_datetime(MAX_TIME_WIDTH,
                                            detect_precision_from_arg ?
                                            args[0]->time_precision() :
                                            decimals);
  }
};


class Item_datetime_typecast :public Item_datetime_func
{
  bool detect_precision_from_arg;
public:
  Item_datetime_typecast(Item *a) :Item_datetime_func(a)
  {
    detect_precision_from_arg= true;
  }
  Item_datetime_typecast(Item *a, uint8 dec_arg) :Item_datetime_func(a)
  {
    detect_precision_from_arg= false;
    decimals= dec_arg;
  }
  void print(String *str, enum_query_type query_type);
  const char *func_name() const { return "cast_as_datetime"; }
  const char *cast_type() const { return "datetime"; }
  void fix_length_and_dec()
  {
    maybe_null= 1;
    fix_length_and_dec_and_charset_datetime(MAX_DATETIME_WIDTH,
                                            detect_precision_from_arg ?
                                            args[0]->datetime_precision():
                                            decimals);
  }
  bool get_date(MYSQL_TIME *res, uint fuzzy_date);
};


class Item_func_makedate :public Item_date_func
{
public:
  Item_func_makedate(Item *a, Item *b) :Item_date_func(a, b) { maybe_null= 1; }
  const char *func_name() const { return "makedate"; }
  bool get_date(MYSQL_TIME *ltime, uint fuzzy_date);
};


class Item_func_add_time :public Item_temporal_hybrid_func
{
  const bool is_date;
  int sign;
  bool val_datetime(MYSQL_TIME *time, uint fuzzy_date);
public:
  Item_func_add_time(Item *a, Item *b, bool type_arg, bool neg_arg)
    :Item_temporal_hybrid_func(a, b), is_date(type_arg)
  {
    sign= neg_arg ? -1 : 1;
  }
  void fix_length_and_dec();
  void print(String *str, enum_query_type query_type);
  const char *func_name() const { return "add_time"; }
};


class Item_func_timediff :public Item_time_func
{
public:
  Item_func_timediff(Item *a, Item *b) :Item_time_func(a, b) {}
  const char *func_name() const { return "timediff"; }
  void fix_length_and_dec()
  {
    uint dec= max(args[0]->time_precision(), args[1]->time_precision());
    fix_length_and_dec_and_charset_datetime(MAX_TIME_WIDTH, dec);
    maybe_null= 1;
  }
  bool get_time(MYSQL_TIME *ltime);
};

class Item_func_maketime :public Item_time_func
{
public:
  Item_func_maketime(Item *a, Item *b, Item *c) :Item_time_func(a, b, c) 
  {
    maybe_null= TRUE;
  }
  void fix_length_and_dec()
  {
    fix_length_and_dec_and_charset_datetime(MAX_TIME_WIDTH,
                                            min(args[2]->decimals,
                                                DATETIME_MAX_DECIMALS));
  }
  const char *func_name() const { return "maketime"; }
  bool get_time(MYSQL_TIME *ltime);
};

class Item_func_microsecond :public Item_int_func
{
public:
  Item_func_microsecond(Item *a) :Item_int_func(a) {}
  longlong val_int();
  const char *func_name() const { return "microsecond"; }
  void fix_length_and_dec() 
  { 
    maybe_null=1;
  }
  bool check_partition_func_processor(uchar *int_arg) {return FALSE;}
  bool check_valid_arguments_processor(uchar *int_arg)
  {
    return !has_time_args();
  }
};


class Item_func_timestamp_diff :public Item_int_func
{
  const interval_type int_type;
public:
  Item_func_timestamp_diff(Item *a,Item *b,interval_type type_arg)
    :Item_int_func(a,b), int_type(type_arg) {}
  const char *func_name() const { return "timestampdiff"; }
  longlong val_int();
  void fix_length_and_dec()
  {
    maybe_null=1;
  }
  virtual void print(String *str, enum_query_type query_type);
};


enum date_time_format
{
  USA_FORMAT, JIS_FORMAT, ISO_FORMAT, EUR_FORMAT, INTERNAL_FORMAT
};

class Item_func_get_format :public Item_str_ascii_func
{
public:
  const timestamp_type type; // keep it public
  Item_func_get_format(timestamp_type type_arg, Item *a)
    :Item_str_ascii_func(a), type(type_arg)
  {}
  String *val_str_ascii(String *str);
  const char *func_name() const { return "get_format"; }
  void fix_length_and_dec()
  {
    maybe_null= 1;
    decimals=0;
    fix_length_and_charset(17, default_charset());
  }
  virtual void print(String *str, enum_query_type query_type);
};


class Item_func_str_to_date :public Item_temporal_hybrid_func
{
  timestamp_type cached_timestamp_type;
  bool const_item;
  void fix_from_format(const char *format, uint length);
protected:
  bool val_datetime(MYSQL_TIME *ltime, uint fuzzy_date);
public:
  Item_func_str_to_date(Item *a, Item *b)
    :Item_temporal_hybrid_func(a, b), const_item(false)
  {}
  const char *func_name() const { return "str_to_date"; }
  void fix_length_and_dec();
};


class Item_func_last_day :public Item_date_func
{
public:
  Item_func_last_day(Item *a) :Item_date_func(a) { maybe_null= 1; }
  const char *func_name() const { return "last_day"; }
  bool get_date(MYSQL_TIME *res, uint fuzzy_date);
};


/* Function prototypes */

bool make_date_time(DATE_TIME_FORMAT *format, MYSQL_TIME *l_time,
                    timestamp_type type, String *str);

#endif /* ITEM_TIMEFUNC_INCLUDED */
