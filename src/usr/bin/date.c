#include "../../kernel/rtc.h"
#include "../../lib/modules/display.h"
#include "../../lib/util.h"

void date(const char* args) {
  (void)args;
  
  rtc_date_t date;
  rtc_read_date(&date);
  
  char date_str[11];
  char *p = date_str;
  
  *p++ = '0' + (date.day / 10);
  *p++ = '0' + (date.day % 10);
  *p++ = '/';
  *p++ = '0' + (date.month / 10);
  *p++ = '0' + (date.month % 10);
  *p++ = '/';
  int_to_string(date.year, p);
  p += 4;
  *p = '\0';
  
  print_string(date_str, VGA_WHITE);
  print_string("\n", VGA_WHITE);
}