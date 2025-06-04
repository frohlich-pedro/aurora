#include "eth.h"
#include "e1000.h"

unsigned char my_mac[6] = {0x52, 0x54, 0x00, 0x12, 0x34, 0x56};
unsigned char broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void eth_init() {}

void eth_send(void* data, unsigned short len) {
  e1000_send(data, len);
}
