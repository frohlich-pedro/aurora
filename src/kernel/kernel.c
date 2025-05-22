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

  asm volatile ("sti");

  init_keyboard();
  init_dynamic_mem();

  int* ptr = alloc(5);
  mem_free(ptr);

  print_string("  --  Welcome to Aurora  --  \n\n");
  print_string("+-----------------------------------------------------------------------------+\n");
  print_string("|                                                                             |\n");
  print_string("|  ,adPPYYba,  88       88  8b,dPPYba,   ,adPPYba,   8b,dPPYba,  ,adPPYYba,   |\n");
  print_string("|  \"\"     `Y8  88       88  88P\'   \"Y8  a8\"     \"8a  88P\'   \"Y8  \"\"     `Y8   |\n");
  print_string("|  ,adPPPPP88  88       88  88          8b       d8  88          ,adPPPPP88   |\n");
  print_string("|  88,    ,88  \"8a,   ,a88  88          \"8a,   ,a8\"  88          88,    ,88   |\n");
  print_string("|  `\"8bbdP\"Y8   `\"YbbdP\'Y8  88           `\"YbbdP\"\'   88          `\"8bbdP\"Y8   |\n");
  print_string("|                                                                             |\n");
  print_string("+-----------------------------------------------------------------------------+\n");
  print_string("\n\nEnter \'help\' to see command list.\n\n");
  
  shell_init();
}


