#include <stdint.h>

unsigned char port_byte_in(uint16_t port) {
  unsigned char result;
  asm("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

void port_byte_out(uint16_t port, uint8_t data) {
  asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(uint16_t port) {
  unsigned short result;
  asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

void port_word_out(uint16_t port, uint16_t data) {
  asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}
