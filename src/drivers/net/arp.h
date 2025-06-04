#pragma once

#include "../../kernel/util.h"
#include "eth.h"

#define ARP_HTYPE_ETHERNET 1
#define ARP_PTYPE_IP 0x0800
#define ARP_OP_REQUEST 1
#define ARP_OP_REPLY 2

typedef struct {
  unsigned short htype;
  unsigned short ptype;
  unsigned char hlen;
  unsigned char plen;
  unsigned short oper;
  unsigned char sha[6];
  unsigned char spa[4];
  unsigned char tha[6];
  unsigned char tpa[4];
} __attribute__((packed)) arp_packet_t;

void arp_init();
void arp_request(unsigned char* target_ip);
void arp_reply(unsigned char* target_mac, unsigned char* target_ip);
void arp_receive(arp_packet_t* arp);
unsigned char* arp_lookup(unsigned char* ip);
