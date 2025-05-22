#include "../shell.h"
#include "../../kernel/mem.h"

void meminfo() {
  print_dynamic_mem();
  print_dynamic_node_size();
}
