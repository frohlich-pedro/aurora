#include "../shell.h"
#include "../../kernel/util.h"

void exit(const char* args) {
  print_string("Stopping the CPU. Bye!\n", VGA_WHITE);
  asm volatile ("hlt");
}
