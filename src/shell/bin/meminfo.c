#include "../shell.h"
#include "../../kernel/mem.h"

void meminfo(const char* args) {
  asm volatile ("cli");
  print_dynamic_mem();
  print_dynamic_node_size();
  asm volatile ("sti");
}
