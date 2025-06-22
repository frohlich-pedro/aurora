#include "../shell.h"
#include "../../lib/util.h"

void exit(const char* args) {
  print_string("Stopping the CPU. Bye!\n", VGA_LIGHT_CYAN);
  asm volatile ("hlt");
}
