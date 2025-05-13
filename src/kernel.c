#include "drivers/vga_driver.h"

void main(void) {
    clear_screen();
    print_kernel("Hello, World!\n", VGA_WHITE);
    
    while (1);
}
