#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"

#include "util.h"
#include "mem.h"

void* alloc(int n) {
  int *ptr = (int *) mem_alloc(n * sizeof(int));
  if (ptr == NULL_POINTER) {
    print_string("Memory not allocated.\n");
  } else {
    for (int i = 0; i < n; ++i) {
    }

    for (int i = 0; i < n; ++i) {
    }
  }
  return ptr;
}

void start_kernel() {
  clear_screen();
  isr_install();

  asm volatile("sti");

  init_keyboard();
  init_dynamic_mem();

  int *ptr1 = alloc(5);
  int *ptr2 = alloc(10);

  mem_free(ptr1);

  int *ptr3 = alloc(2);

  mem_free(ptr2);

  mem_free(ptr3);

  print_string("> ");
}

void execute_command(char *input) {
  if (compare_string(input, "EXIT") == 0) {
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
