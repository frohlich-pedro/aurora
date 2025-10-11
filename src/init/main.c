#include "../lib/display.h"
#include "../hw/idt.h"
#include "../hw/isr.h"
#include "../lib/keyboard.h"

void main(void) {
  display_init();

  isr_install();
  asm volatile ("sti");

  keyboard_init();

  print("-- Welcome to AuroraOS --\n\n", YELLOW);
  print("Copyright:", LIGHT_CYAN);
  print("\tPedro Henrique Nielson Frohlich, 2025\n", WHITE);
  print("Version:", LIGHT_CYAN);
  print("\t0.0.0\n\n", WHITE);
  
  print("  ", 0x00);
  print("  ", 0x11);
  print("  ", 0x22);
  print("  ", 0x33);
  print("  ", 0x44);
  print("  ", 0x55);
  print("  ", 0x66);
  print("  ", 0x77);
  print("\n", WHITE);
  print("  ", 0x88);
  print("  ", 0x99);
  print("  ", 0xaa);
  print("  ", 0xbb);
  print("  ", 0xcc);
  print("  ", 0xdd);
  print("  ", 0xee);
  print("  ", 0xff);
  print("\n\n", WHITE);

  print("> ", LIGHT_GREEN);

  while (1) {
    asm volatile ("hlt");
  }
}
