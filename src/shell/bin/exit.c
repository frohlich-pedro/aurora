#include "../shell.h"
#include "../../kernel/util.h"

void exit() {
  print_string("Stopping the CPU. Bye!\n");
  asm volatile ("hlt");
}
