#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "util.h"
#include "mem.h"
#include "../shell/shell.h"
#include "kernel_banner.h"
#include "../drivers/sound.h"

#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

void start_kernel() {
  clear_screen();
  isr_install();
  asm volatile ("sti");

  init_timer(1000);

  beep(525, 250);
  print_string("SPEAKER LOADED\n", VGA_GREEN);
  sleep(25);

  init_keyboard();
  print_string("KEYBOARD LOADED\n", VGA_GREEN);
  sleep(25);

  init_dynamic_mem();
  print_string("DYNAMIC MEMORY LOADED\n", VGA_GREEN);
  sleep(25);

  sleep(250);
  clear_screen();
  print_string("  --  Welcome to Aurora  --  \n\n", VGA_LIGHT_MAGENTA);
  print_banner();
  
  print_string("Version:     ", VGA_YELLOW);
  print_string("0.0.0\n", VGA_WHITE);
  
  print_string("Build:       ", VGA_YELLOW);
  print_string(BUILD_DATE " " BUILD_TIME "\n", VGA_WHITE);
  
  print_string("Copyright:   ", VGA_YELLOW);
  print_string("2025, AuroraOS\n\n", VGA_WHITE);
  
  print_string("Enter 'help' to see command list.\n\n", VGA_WHITE);
  shell_init();
}
