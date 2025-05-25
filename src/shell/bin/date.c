#include "../../cpu/rtc.h"
#include "../../drivers/display.h"
#include "../../kernel/util.h"
#include "commands.h"

void date(const char* args) {
  (void)args;
  
  rtc_date_t date;
  rtc_read_full(&date);
  
  // Create separate buffers for each component
  char day_str[3] = {0};
  char month_str[3] = {0};
  char year_str[5] = {0};
  
  // Convert each component separately
  int_to_string(date.day, day_str);
  int_to_string(date.month, month_str);
  int_to_string(date.year, year_str);
  
  // Pad with leading zeros if needed
  if (date.day < 10) {
    day_str[1] = day_str[0];
    day_str[0] = '0';
  }
  if (date.month < 10) {
    month_str[1] = month_str[0];
    month_str[0] = '0';
  }
  
  // Build the final date string (DD/MM/YYYY format)
  char date_str[11];
  date_str[0] = day_str[0];
  date_str[1] = day_str[1];
  date_str[2] = '/';
  date_str[3] = month_str[0];
  date_str[4] = month_str[1];
  date_str[5] = '/';
  // Copy year (last 4 digits)
  date_str[6] = year_str[0];
  date_str[7] = year_str[1];
  date_str[8] = year_str[2];
  date_str[9] = year_str[3];
  date_str[10] = '\0';
  
  print_string(date_str);
  print_string("\n");
}
