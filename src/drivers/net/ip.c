#include "ip.h"
#include "arp.h"
#include "eth.h"
#include "icmp.h"
#include "../../kernel/util.h"

unsigned short ip_checksum(unsigned short* data, int len) {
  unsigned int sum = 0;
  while (len > 1) {
    sum += *data++;
    len -= 2;
  }
  if (len > 0) {
    sum += *(unsigned char*)data;
  }
  while (sum >> 16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }
  return ~sum;
}

void ip_send(unsigned char* dst_ip, unsigned char protocol, void* data, unsigned short len) {
  unsigned char packet[sizeof(eth_header_t) + sizeof(ip_header_t) + len];
  eth_header_t* eth = (eth_header_t*)packet;
  ip_header_t* ip = (ip_header_t*)(packet + sizeof(eth_header_t));
  unsigned char* payload = (unsigned char*)(ip + 1);
  
  unsigned char* dst_mac = arp_lookup(dst_ip);
  if (!dst_mac) {
    arp_request(dst_ip);
    return;
  }
  
  memcpy(eth->dst_mac, dst_mac, 6);
  memcpy(eth->src_mac, my_mac, 6);
  eth->ethertype = ETHERTYPE_IP;
  
  ip->version_ihl = 0x45;
  ip->dscp_ecn = 0;
  ip->total_len = sizeof(ip_header_t) + len;
  ip->total_len = ((ip->total_len & 0xFF00) >> 8) | ((ip->total_len & 0x00FF) << 8);
  ip->ident = 0;
  ip->flags_frag_off = 0;
  ip->ttl = 64;
  ip->proto = protocol;
  ip->checksum = 0;
  memcpy(ip->src_ip, my_ip, 4);
  memcpy(ip->dst_ip, dst_ip, 4);
  
  unsigned int sum = 0;
  unsigned char* ip_bytes = (unsigned char*)ip;
  for (int i = 0; i < sizeof(ip_header_t); i += 2) {
    sum += (ip_bytes[i] << 8) | ip_bytes[i+1];
  }
  while (sum >> 16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }
  ip->checksum = ~sum;
  
  memcpy(payload, data, len);
  eth_send(packet, sizeof(eth_header_t) + sizeof(ip_header_t) + len);
}

void ip_receive(ip_header_t* ip) {
  if (memcmp(ip->dst_ip, my_ip, 4) != 0) return;
  
  switch (ip->proto) {
    case IP_PROTO_ICMP:
      icmp_receive(ip, (icmp_header_t*)(ip + 1));
      break;
  }
}
