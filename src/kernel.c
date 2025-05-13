#include "drivers/vga_driver.h"

void main(void) {
    kcls();
    kprintf("Hello, World!\n", VGA_WHITE);
    
    while (1);
}
