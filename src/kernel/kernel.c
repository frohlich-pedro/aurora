#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "util.h"
#include "mem.h"
#include "../shell/shell.h"

void start_kernel() {
  clear_screen();
  isr_install();

  asm volatile ("sti");

  init_keyboard();
  init_dynamic_mem();

  int* ptr = alloc(5);
  mem_free(ptr);
  
  shell_init();
}
