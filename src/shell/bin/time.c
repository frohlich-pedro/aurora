#include "../../cpu/rtc.h"
#include "../../drivers/display.h"
#include "../../kernel/util.h"
#include "commands.h"

void time(const char* args) {
  (void)args;
  
  rtc_date_t date;
  rtc_read_full(&date);
  
  char time_str[9];
  char *p = time_str;
  
  *p++ = '0' + (date.hour / 10);
  *p++ = '0' + (date.hour % 10);
  *p++ = ':';
  *p++ = '0' + (date.minute / 10);
  *p++ = '0' + (date.minute % 10);
  *p++ = ':';
  *p++ = '0' + (date.second / 10);
  *p++ = '0' + (date.second % 10);
  *p = '\0';
  
  print_string(time_str);
  print_string("\n");
}
