array _days_of_month[12]

utc        = time(0)
local_time = utc+(9*3600)

this_year  = int( tm_year(local_time) )
leap_year  = int( (this_year%4)==1) ? (((this_year % 2000) == 0) ? 0 : 1) : 0
this_month = int( tm_mon(local_time) ) + 1
wday       = int( tm_wday(local_time) )
mday       = int( tm_mday(local_time) )

_days_of_month[1]  = 31
_days_of_month[2]  = 28 + leap_year
_days_of_month[3]  = 31
_days_of_month[4]  = 30
_days_of_month[5]  = 31
_days_of_month[6]  = 30
_days_of_month[7]  = 31
_days_of_month[8]  = 31
_days_of_month[9]  = 30
_days_of_month[10] = 31
_days_of_month[11] = 30
_days_of_month[12] = 31

days_of_this_month = _days_of_month[ this_month ]

tm_str_day_befor(_d)   = strftime("%Y-%m-%d 00:00:00", local_time - _d * 24 * 3600)
tm_str_week_befor(_w)  = strftime("%Y-%m-%d 00:00:00", local_time - (wday * 24 * 3600) - (_w * 7 * 24 * 3600) )
tm_str_month_befor(_m) = strftime(sprintf("%%Y-%02d-01 00:00:00",(this_month>_m)?(this_month-_m):0), local_time )
