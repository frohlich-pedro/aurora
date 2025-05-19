#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "util.h"
#include "mem.h"

void start_kernel() {
  clear_screen();
  isr_install();

  asm volatile("sti");

  init_keyboard();
  init_dynamic_mem();

  int* ptr = alloc(5);
  mem_free(ptr);

  print_string("> ");
}

void execute_command(const char* input) {
  if (compare_string(input, "exit") == 0) {
    print_string("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
  } else if (compare_string(input, "") == 0) {
    print_string("\n> ");
  } else {
    print_string("Unknown command: ");
    print_string(input);
    print_string("\n> ");
  }
}
