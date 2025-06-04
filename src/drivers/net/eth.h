#pragma once

#include "../../kernel/util.h"

#define ETHERTYPE_IP  0x0800
#define ETHERTYPE_ARP 0x0806

extern unsigned char my_mac[6];
extern unsigned char broadcast_mac[6];

typedef struct {
  unsigned char dst_mac[6];
  unsigned char src_mac[6];
  unsigned short ethertype;
} __attribute__((packed)) eth_header_t;

void eth_init();
void eth_send(void* data, unsigned short len);
