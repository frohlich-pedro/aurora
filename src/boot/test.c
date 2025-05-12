void main(void) {
    volatile unsigned char* vga_buffer = (volatile unsigned char*)0xb8000;
    unsigned int i = 0, j = 0;

    do {
        *(vga_buffer + i) = ' ';
        *(vga_buffer + (i + 1)) = 0x0f;
        i += 2;
    } while (i < 80 * 25 * 2);

    const char* string = "Hello, World!";

    do {
        *(vga_buffer + i) = *(string + j);
        *(vga_buffer + (i + 1)) = 0x0f;
        i += 2;
        j += 1;
    } while (*(string + i) != '\0');
    
    while(1);
}
