#include "rtc.h"

unsigned char rtc_read_reg(unsigned char reg) {
  asm volatile ("outb %0, %1" : : "a"(reg), "Nd"(RTC_INDEX_PORT));
  asm volatile ("inb %1, %0" : "=a"(reg) : "Nd"(RTC_DATA_PORT));
  return reg;
}

void rtc_write(unsigned char reg, unsigned char value) {
  asm volatile ("outb %0, %1" : : "a"(reg), "Nd"(RTC_INDEX_PORT));
  asm volatile ("outb %0, %1" : : "a"(value), "Nd"(RTC_DATA_PORT));
}

unsigned char rtc_updating() {
  return rtc_read_reg(RTC_STATUS_A) & RTC_UPDATE_IN_PROGRESS;
}

void rtc_read_date(rtc_date_t* date) {
  while (rtc_updating());

  date->second = rtc_read_reg(RTC_SECONDS);
  date->minute = rtc_read_reg(RTC_MINUTES);
  date->hour = rtc_read_reg(RTC_HOURS);
  date->day = rtc_read_reg(RTC_DAY_OF_MONTH);
  date->month = rtc_read_reg(RTC_MONTH);
  unsigned char raw_year = rtc_read_reg(RTC_YEAR);
  unsigned char raw_century = rtc_read_reg(0x32);

  unsigned char statusB = rtc_read_reg(RTC_STATUS_B);
  date->is_24hour = statusB & RTC_24HOUR_MODE;
  date->is_pm = (date->hour & 0x80) && !date->is_24hour;

  if (!(statusB & RTC_BCD_MODE)) {
    date->second = bcd2bin(date->second);
    date->minute = bcd2bin(date->minute);
    date->hour = bcd2bin(date->hour);
    date->day = bcd2bin(date->day);
    date->month = bcd2bin(date->month);
    raw_year = bcd2bin(raw_year);
    raw_century = bcd2bin(raw_century);
  }

  date->year = (raw_century * 100) + raw_year;
}

unsigned char bcd2bin(unsigned char bcd) {
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

unsigned char bin2bcd(unsigned char bin) {
  return ((bin / 10) << 4) | (bin % 10);
}

unsigned char is_leap(unsigned char year) {
  unsigned short full_year = 2000 + year;
  return (full_year % 4 == 0 && full_year % 100 != 0) || (full_year % 400 == 0);
}

unsigned char month_days(unsigned char month, unsigned char year) {
  static const unsigned char days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month == 2 && is_leap(year)) return 29;
  return *(days + month - 1);
}

unsigned char get_weekday(unsigned char day, unsigned char month, unsigned short year) {
  if (month < 3) {
    month += 12;
    year--;
  }
  
  unsigned char q = day;
  unsigned char m = month;
  unsigned char k = year % 100;
  unsigned char j = year / 100;
  unsigned char h = (q + (13 * (m + 1) / 5) + k + (k / 4) + (j / 4) + 5 * j) % 7;
  
  return (h + 6) % 7;
}

unsigned char last_sunday(unsigned char month, unsigned char year) {
  return month_days(month, year) - (5 + year + year / 4) % 7;
}

unsigned char nth_weekday(unsigned char month, unsigned char year, unsigned char n, unsigned char weekday) {
  unsigned char first_day = get_weekday(1, month, year);
  unsigned char offset = (weekday - first_day + 7) % 7;
  return 1 + offset + (n - 1) * 7;
}

void rtc_adjust_tz(rtc_date_t* date, unsigned char offset) {
  date->hour += offset;
  if (date->hour >= 24) {
    date->hour -= 24;
    date->day++;
  } else if (date->hour < 0) {
    date->hour += 24;
    date->day--;
  }
}
