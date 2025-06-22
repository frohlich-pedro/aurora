#pragma once

#define RTC_INDEX_PORT 0x70
#define RTC_DATA_PORT 0x71
#define RTC_SECONDS 0x00
#define RTC_MINUTES 0x02
#define RTC_HOURS 0x04
#define RTC_DAY_OF_MONTH 0x07
#define RTC_MONTH 0x08
#define RTC_YEAR 0x09
#define RTC_STATUS_A 0x0A
#define RTC_STATUS_B 0x0B
#define RTC_STATUS_C 0x0C
#define RTC_STATUS_D 0x0D
#define RTC_24HOUR_MODE 0x02
#define RTC_BCD_MODE 0x04
#define RTC_UPDATE_IN_PROGRESS 0x80

typedef struct {
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char day;
  unsigned char month;
  unsigned short year;
  unsigned char is_pm;
  unsigned char is_24hour;
} rtc_date_t;

unsigned char rtc_read_reg(unsigned char reg);
void rtc_write(unsigned char reg, unsigned char value);
unsigned char rtc_updating();
void rtc_read_date(rtc_date_t* date);
void rtc_read_time(rtc_date_t* date);
unsigned char bcd2bin(unsigned char bcd);
unsigned char bin2bcd(unsigned char bin);
unsigned char is_leap(unsigned char year);
unsigned char month_days(unsigned char month, unsigned char year);
unsigned char get_weekday(unsigned char day, unsigned char month, unsigned short year);
unsigned char last_sunday(unsigned char month, unsigned char year);
unsigned char nth_weekday(unsigned char month, unsigned char year, unsigned char n, unsigned char weekday);
void rtc_adjust_tz(rtc_date_t* date, unsigned char offset);
