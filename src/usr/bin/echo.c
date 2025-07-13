#include "../../lib/modules/display.h"

void echo(const char* args) {
  if (*args) {
    print_string(args, VGA_WHITE);
  }
  print_nl();
}