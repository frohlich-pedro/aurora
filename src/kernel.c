#include "drivers/kb_driver.h"
#include "drivers/vga_driver.h"

void main(void) {
  __asm__ volatile ("cli");

  clear_screen();
  print_kernel("Hello, World!\n", VGA_WHITE);

  isr_install();
  init_keyboard();

  __asm__ volatile ("sti");
  while(1) __asm__ volatile ("hlt");
}
