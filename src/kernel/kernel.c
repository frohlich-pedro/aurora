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

  beep(440, 1000);
  print_string("SPEAKER LOADED\n");
  sleep(500);

  init_keyboard();
  print_string("KEYBOARD LOADED\n");
  sleep(500);

  init_dynamic_mem();
  print_string("DYNAMIC MEMORY LOADED\n");
  sleep(500);

  net_init();
  print_string("NETWORK LOADED\n");
  sleep(500);

  print_banner();
  print_string("Enter \'help\' to see command list.\n\n");
  shell_init();
}
