#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "util.h"
#include "mem.h"
#include "../shell/shell.h"
#include "kernel_banner.h"

void start_kernel() {
  clear_screen();
  isr_install();

  asm volatile ("sti");

  init_keyboard();
  init_dynamic_mem();

  int* ptr = alloc(5);
  mem_free(ptr);

  print_banner();
  print_string("Enter \'help\' to see command list.\n\n");
  shell_init();
}
