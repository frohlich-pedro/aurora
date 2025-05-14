#include "drivers/vga_driver.h"
#include "drivers/kb_driver.h"

void main(void) {
  kcls();
  kprintf("Keyboard test - press keys:\n", VGA_WHITE);
  keyboard_install();
    
  while (1) {
    char c = keyboard_buffer_get();
    if (c) {
      kprintf("%c", c);
    }
  }
}
