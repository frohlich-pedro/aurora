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
#include "../drivers/net/net.h"

void start_kernel() {
  clear_screen();
  isr_install();

  init_timer(1000);

  asm volatile ("sti");

  init_keyboard();
  print_string("KEYBOARD LOADED");
  sleep(250);

  init_dynamic_mem();
  print_string("DYNAMIC MEMORY LOADED");
  sleep(250);

  net_init();
  print_string("NETWORK LOADED");
  sleep(250);

  print_banner();
  print_string("Enter \'help\' to see command list.\n\n");
  shell_init();
}
