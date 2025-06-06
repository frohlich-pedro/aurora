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

void start_kernel() {
  clear_screen();
  isr_install();
  asm volatile ("sti");

  init_timer(1000);

  beep(525, 250);
  print_string("SPEAKER LOADED\n");
  sleep(250);

  init_keyboard();
  print_string("KEYBOARD LOADED\n");
  sleep(250);

  init_dynamic_mem();
  print_string("DYNAMIC MEMORY LOADED\n");
  sleep(250);

  print_banner();
  print_string("Enter \'help\' to see command list.\n\n");
  shell_init();
}
