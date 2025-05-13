#include "io.h"

unsigned char byte_input(unsigned short port) {
    unsigned char result;
    asm volatile("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void byte_output(unsigned short port, unsigned char data) {
    asm volatile("out %%al, %%dx" : : "a"(data), "d"(port));
}
