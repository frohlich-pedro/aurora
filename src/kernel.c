#include "drivers/vga_driver.h"

void main(void) {
    asm volatile("cli");

    clear_screen();
    print_kernel("Hello, World!\n", VGA_WHITE);

    isr_install();
    init_keyboard();

    asm volatile ("sti");
    while (1) asm volatile ("hlt");
}
