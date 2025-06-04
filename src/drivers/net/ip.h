#pragma once

#include "../../kernel/util.h"

#define IP_PROTO_ICMP 1
#define IP_PROTO_TCP  6
#define IP_PROTO_UDP  17

extern unsigned char my_ip[4];

typedef struct {
  unsigned char version_ihl;
  unsigned char dscp_ecn;
  unsigned short total_len;
  unsigned short ident;
  unsigned short flags_frag_off;
  unsigned char ttl;
  unsigned char proto;
  unsigned short checksum;
  unsigned char src_ip[4];
  unsigned char dst_ip[4];
} __attribute__((packed)) ip_header_t;

unsigned short ip_checksum(unsigned short* data, int len);
void ip_send(unsigned char* dst_ip, unsigned char protocol, void* data, unsigned short len);
void ip_receive(ip_header_t* ip);
