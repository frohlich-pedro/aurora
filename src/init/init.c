#include "../kernel/idt.h"
#include "../kernel/isr.h"
#include "../kernel/timer.h"
#include "../lib/modules/display.h"
#include "../lib/modules/keyboard.h"
#include "../lib/util.h"
#include "../mm/mem.h"
#include "../shell/shell.h"
#include "../lib/modules/sound.h"
#include "../kernel/syscall.h"

#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

void main() {
  
  isr_install();
  asm volatile ("sti");

  init_timer(1000);
  
  beep(525, 250);
  print_string("SPEAKER LOADED\n", VGA_WHITE);

  init_keyboard();
  print_string("KEYBOARD LOADED\n", VGA_WHITE);

  init_dynamic_mem();
  print_string("DYNAMIC MEMORY LOADED\n", VGA_WHITE);
  
  syscall_init();
  syscall_write(1, "SYSCALLS LOADED\n", 16);

  sleep(500);
  
  init_curses_display();
  clear_content_area();
  
  print_string("  --  Welcome to Aurora  --  \n\n", VGA_LIGHT_MAGENTA);
  
  print_string("Version:   ", VGA_LIGHT_GREEN);
  print_string("0.0.0\n", VGA_YELLOW);
  
  print_string("Build:     ", VGA_LIGHT_GREEN);
  print_string(BUILD_DATE " " BUILD_TIME "\n", VGA_YELLOW);
  
  print_string("Copyright: ", VGA_LIGHT_GREEN);
  print_string("2025, AuroraOS\n\n", VGA_YELLOW);
  
  print_string("Enter 'help' to see command list.\n\n", VGA_LIGHT_CYAN);
    
  draw_panel();
  
  shell_init();
}
