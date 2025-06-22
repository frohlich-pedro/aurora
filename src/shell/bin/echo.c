#include "../../drivers/display.h"

void echo(const char* args) {
  if (*args) {
    print_string(args, VGA_BLACK);
  }
  print_nl();
}
