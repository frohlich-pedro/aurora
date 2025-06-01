#include "../../drivers/display.h"

void echo(const char* args) {
  if (*args) {
    print_string(args);
  }
  print_nl();
}
