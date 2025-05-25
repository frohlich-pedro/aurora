#include "rtc.h"

unsigned char rtc_read_register(unsigned char reg) {
  asm volatile ("outb %0, %1" : : "a"(reg), "Nd"(RTC_INDEX_PORT));
  asm volatile ("inb %1, %0" : "=a"(reg) : "Nd"(RTC_DATA_PORT));
  return reg;
}

void rtc_write_register(unsigned char reg, unsigned char value) {
  asm volatile ("outb %0, %1" : : "a"(reg), "Nd"(RTC_INDEX_PORT));
  asm volatile ("outb %0, %1" : : "a"(value), "Nd"(RTC_DATA_PORT));
}

unsigned char rtc_is_updating() {
  return rtc_read_register(RTC_STATUS_A) & RTC_UPDATE_IN_PROGRESS;
}

void rtc_read_full(rtc_date_t* date) {
  while (rtc_is_updating());

  date->second = rtc_read_register(RTC_SECONDS);
  date->minute = rtc_read_register(RTC_MINUTES);
  date->hour = rtc_read_register(RTC_HOURS);
  date->day = rtc_read_register(RTC_DAY_OF_MONTH);
  date->month = rtc_read_register(RTC_MONTH);
  unsigned char raw_year = rtc_read_register(RTC_YEAR);
  unsigned char raw_century = rtc_read_register(0x32);

  unsigned char statusB = rtc_read_register(RTC_STATUS_B);
  date->is_24hour = statusB & RTC_24HOUR_MODE;
  date->is_pm = (date->hour & 0x80) && !date->is_24hour;

  if (!(statusB & RTC_BCD_MODE)) {
    date->second = bcd_to_bin(date->second);
    date->minute = bcd_to_bin(date->minute);
    date->hour = bcd_to_bin(date->hour);
    date->day = bcd_to_bin(date->day);
    date->month = bcd_to_bin(date->month);
    raw_year = bcd_to_bin(raw_year);
    raw_century = bcd_to_bin(raw_century);
  }

  date->year = (raw_century * 100) + raw_year;
}

unsigned char bcd_to_bin(unsigned char bcd) {
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

unsigned char bin_to_bcd(unsigned char bin) {
  return ((bin / 10) << 4) | (bin % 10);
}

unsigned char is_leap_year(unsigned char year) {
  unsigned short full_year = 2000 + year;
  return (full_year % 4 == 0 && full_year % 100 != 0) || (full_year % 400 == 0);
}

unsigned char days_in_month(unsigned char month, unsigned char year) {
  static const unsigned char days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month == 2 && is_leap_year(year)) return 29;
  return days[month - 1];
}

unsigned char day_of_week(unsigned char day, unsigned char month, unsigned short year) {
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

unsigned char rtc_calculate_last_sunday(unsigned char month, unsigned char year) {
  return days_in_month(month, year) - (5 + year + year / 4) % 7;
}

unsigned char rtc_calculate_nth_weekday(unsigned char month, unsigned char year, unsigned char n, unsigned char weekday) {
  unsigned char first_day = day_of_week(1, month, year);
  unsigned char offset = (weekday - first_day + 7) % 7;
  return 1 + offset + (n - 1) * 7;
}

void rtc_adjust_timezone(rtc_date_t* date, unsigned char offset) {
  date->hour += offset;
  if (date->hour >= 24) {
    date->hour -= 24;
    date->day++;
  } else if (date->hour < 0) {
    date->hour += 24;
    date->day--;
  }
}
