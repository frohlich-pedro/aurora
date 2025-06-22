#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "util.h"
#include "mem.h"
#include "../shell/shell.h"
#include "../drivers/sound.h"
#include "../cpu/syscall.h"

#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

void start_kernel() {
  init_curses_display();
  
  isr_install();
  asm volatile ("sti");

  init_timer(1000);
  
  beep(525, 250);
  print_string("SPEAKER LOADED\n", VGA_GREEN);

  init_keyboard();
  print_string("KEYBOARD LOADED\n", VGA_GREEN);

  init_dynamic_mem();
  print_string("DYNAMIC MEMORY LOADED\n", VGA_GREEN);
  
  syscall_init();
  syscall_write(1, "SYSCALLS LOADED\n", 16);

  sleep(500);
  
  clear_content_area();
  
  print_string("  --  Welcome to Aurora  --  \n\n", VGA_RED);
  
  print_string("Version:   ", VGA_BLUE);
  print_string("0.0.0\n", VGA_BLACK);
  
  print_string("Build:     ", VGA_BLUE);
  print_string(BUILD_DATE " " BUILD_TIME "\n", VGA_BLACK);
  
  print_string("Copyright: ", VGA_BLUE);
  print_string("2025, AuroraOS\n\n", VGA_BLACK);
  
  print_string("Enter 'help' to see command list.\n\n", VGA_BLUE);
    
  draw_panel();
  
  shell_init();
}
