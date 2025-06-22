#include "../shell.h"
#include "../../mm/mem.h"

void meminfo(const char* args) {
  print_dynamic_mem();
  print_dynamic_node_size();
}
