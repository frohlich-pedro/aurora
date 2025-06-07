#include "../../drivers/display.h"
#include "../../kernel/util.h"
#include "../../cpu/rtc.h"
#include "../../cpu/timer.h"

#define OS_NAME "aurora"
#define OS_VERSION "0.0.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

void fetch(const char* args) {
  (void)args;
  
  unsigned int seconds = tick / 1000;
  unsigned int minutes = seconds / 60;
  unsigned int hours = minutes / 60;
  seconds %= 60;
  minutes %= 60;
  
  rtc_date_t date;
  rtc_read_date(&date);
  
  print_string("kernel:  ", VGA_LIGHT_GREEN);
  print_string(OS_NAME, VGA_WHITE);
  print_nl();
  
  print_string("version: ", VGA_LIGHT_GREEN);
  print_string(OS_VERSION, VGA_WHITE);
  print_nl();
  
  print_string("uptime:  ", VGA_LIGHT_GREEN);
  char uptime_str[32];
  int_to_string(hours, uptime_str);
  print_string(uptime_str, VGA_WHITE);
  print_string("h ", VGA_WHITE);
  int_to_string(minutes, uptime_str);
  print_string(uptime_str, VGA_WHITE);
  print_string("m ", VGA_WHITE);
  int_to_string(seconds, uptime_str);
  print_string(uptime_str, VGA_WHITE);
  print_string("s", VGA_WHITE);
  print_nl();
  
  print_string("build:   ", VGA_LIGHT_GREEN);
  print_string(BUILD_DATE " " BUILD_TIME, VGA_WHITE);
  print_nl();
  print_nl();
  
  print_string("  ", 0x00);
  print_string("  ", 0x11);
  print_string("  ", 0x22);
  print_string("  ", 0x33);
  print_string("  ", 0x44);
  print_string("  ", 0x55);
  print_string("  ", 0x66);
  print_string("  ", 0x77);
  print_nl();
  
  print_string("  ", 0x88);
  print_string("  ", 0x99);
  print_string("  ", 0xaa);
  print_string("  ", 0xbb);
  print_string("  ", 0xcc);
  print_string("  ", 0xdd);
  print_string("  ", 0xee);
  print_string("  ", 0xff);
  print_nl();
}
