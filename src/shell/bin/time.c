#include "../../cpu/rtc.h"
#include "../../drivers/display.h"
#include "../../kernel/util.h"
#include "commands.h"

void time(const char* args) {
  (void)args;
  
  rtc_date_t date;
  rtc_read_full(&date);
  
  // Create separate buffers for each component
  char hour_str[3] = {0};
  char minute_str[3] = {0};
  char second_str[3] = {0};
  
  // Convert each component separately
  int_to_string(date.hour, hour_str);
  int_to_string(date.minute, minute_str);
  int_to_string(date.second, second_str);
  
  // Pad with leading zeros if needed
  if (date.hour < 10) {
    hour_str[1] = hour_str[0];
    hour_str[0] = '0';
  }
  if (date.minute < 10) {
    minute_str[1] = minute_str[0];
    minute_str[0] = '0';
  }
  if (date.second < 10) {
    second_str[1] = second_str[0];
    second_str[0] = '0';
  }
  
  // Build the final time string
  char time_str[9];
  time_str[0] = hour_str[0];
  time_str[1] = hour_str[1];
  time_str[2] = ':';
  time_str[3] = minute_str[0];
  time_str[4] = minute_str[1];
  time_str[5] = ':';
  time_str[6] = second_str[0];
  time_str[7] = second_str[1];
  time_str[8] = '\0';
  
  print_string(time_str);
  print_string("\n");
}
