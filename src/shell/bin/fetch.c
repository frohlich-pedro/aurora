#include "../../lib/modules/display.h"
#include "../../lib/util.h"
#include "../../kernel/rtc.h"
#include "../../kernel/timer.h"

#define OS_NAME "AuroraOS"
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
  
  print_string("OS:      ", VGA_LIGHT_MAGENTA);
  print_string(OS_NAME, VGA_LIGHT_CYAN);
  print_nl();
  
  print_string("version: ", VGA_LIGHT_MAGENTA);
  print_string(OS_VERSION, VGA_LIGHT_CYAN);
  print_nl();
  
  print_string("uptime:  ", VGA_LIGHT_MAGENTA);
  char uptime_str[32];
  int_to_string(hours, uptime_str);
  print_string(uptime_str, VGA_LIGHT_CYAN);
  print_string("h ", VGA_LIGHT_CYAN);
  int_to_string(minutes, uptime_str);
  print_string(uptime_str, VGA_LIGHT_CYAN);
  print_string("m ", VGA_LIGHT_CYAN);
  int_to_string(seconds, uptime_str);
  print_string(uptime_str, VGA_LIGHT_CYAN);
  print_string("s", VGA_LIGHT_CYAN);
  print_nl();
  
  print_string("build:   ", VGA_LIGHT_MAGENTA);
  print_string(BUILD_DATE " " BUILD_TIME, VGA_LIGHT_CYAN);
  print_nl();
  print_nl();
  
  print_color(0x00);
  print_color(0x11);
  print_color(0x22);
  print_color(0x33);
  print_color(0x44);
  print_color(0x55);
  print_color(0x66);
  print_color(0x77);
  print_nl();
  
  print_color(0x88);
  print_color(0x99);
  print_color(0xAA);
  print_color(0xBB);
  print_color(0xCC);
  print_color(0xDD);
  print_color(0xEE);
  print_color(0xFF);
  print_nl();
  print_nl();
}
