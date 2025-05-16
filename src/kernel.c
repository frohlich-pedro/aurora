#include "drivers/vga_driver.h"
#include "drivers/kb_driver.h"

void main(void) {
  kcls();
  kprintf("Hello, World!\n", VGA_WHITE);
  keyboard_install();

  while (1) {
    char c = keyboard_buffer_get();
    if (c) {
      char str[2] = {c, '\0'};
      kprintf(str, VGA_WHITE);
    }
  }
}
