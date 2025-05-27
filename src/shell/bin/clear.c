#include "../shell.h"
#include "../../drivers/display.h"

void clear(const char* args) {
  asm volatile ("cli");
  clear_screen();
  asm volatile ("sti");
}
