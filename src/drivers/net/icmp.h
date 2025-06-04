#pragma once

#include "../../kernel/util.h"
#include "ip.h"

#define ICMP_ECHO_REPLY   0
#define ICMP_ECHO_REQUEST 8

typedef struct {
  unsigned char type;
  unsigned char code;
  unsigned short checksum;
  unsigned short id;
  unsigned short seq;
} __attribute__((packed)) icmp_header_t;

void icmp_receive(ip_header_t* ip, icmp_header_t* icmp);
void icmp_reply(ip_header_t* ip, icmp_header_t* icmp);
