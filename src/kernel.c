#include "drivers/kbDriver.h"
#include "drivers/vgaDriver.h"

void main(void) {
  __asm__ volatile ("cli");

  kcls();
  kprintf("Hello, World!\n", VGA_WHITE);

  isrInstall();
  initKeyboard();

  __asm__ volatile ("sti");
  while(1) __asm__ volatile ("hlt");
}