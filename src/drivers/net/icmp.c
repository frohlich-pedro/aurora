#include "icmp.h"
#include "ip.h"
#include "eth.h"
#include "arp.h"
#include "../../kernel/util.h"

void icmp_receive(ip_header_t* ip, icmp_header_t* icmp) {
  if (icmp->type == ICMP_ECHO_REQUEST) {
    icmp_reply(ip, icmp);
  }
}

void icmp_reply(ip_header_t* original_ip, icmp_header_t* original_icmp) {
  unsigned char packet[sizeof(eth_header_t) + sizeof(ip_header_t) + sizeof(icmp_header_t) + 64];
  eth_header_t* eth = (eth_header_t*)packet;
  ip_header_t* ip = (ip_header_t*)(packet + sizeof(eth_header_t));
  icmp_header_t* icmp = (icmp_header_t*)(ip + 1);
  unsigned char* payload = (unsigned char*)(icmp + 1);
  
  unsigned char* dst_mac = arp_lookup(original_ip->src_ip);
  if (!dst_mac) return;
  
  memcpy(eth->dst_mac, dst_mac, 6);
  memcpy(eth->src_mac, my_mac, 6);
  eth->ethertype = ETHERTYPE_IP;
  
  ip->version_ihl = 0x45;
  ip->dscp_ecn = 0;
  ip->total_len = sizeof(ip_header_t) + sizeof(icmp_header_t) + 64;
  ip->total_len = ((ip->total_len & 0xFF00) >> 8) | ((ip->total_len & 0x00FF) << 8);
  ip->ident = 0;
  ip->flags_frag_off = 0;
  ip->ttl = 64;
  ip->proto = IP_PROTO_ICMP;
  ip->checksum = 0;
  memcpy(ip->src_ip, my_ip, 4);
  memcpy(ip->dst_ip, original_ip->src_ip, 4);
  
  unsigned int ip_sum = 0;
  unsigned char* ip_bytes = (unsigned char*)ip;
  for (int i = 0; i < sizeof(ip_header_t); i += 2) {
    ip_sum += (ip_bytes[i] << 8) | ip_bytes[i+1];
  }
  while (ip_sum >> 16) {
    ip_sum = (ip_sum & 0xFFFF) + (ip_sum >> 16);
  }
  ip->checksum = ~ip_sum;
  
  icmp->type = ICMP_ECHO_REPLY;
  icmp->code = 0;
  icmp->checksum = 0;
  icmp->id = original_icmp->id;
  icmp->seq = original_icmp->seq;
  
  unsigned char* original_payload = (unsigned char*)(original_icmp + 1);
  memcpy(payload, original_payload, 64);
  
  unsigned int icmp_sum = 0;
  unsigned char* icmp_bytes = (unsigned char*)icmp;
  for (int i = 0; i < sizeof(icmp_header_t) + 64; i += 2) {
    if (i+1 < sizeof(icmp_header_t) + 64) {
      icmp_sum += (icmp_bytes[i] << 8) | icmp_bytes[i+1];
    } else {
      icmp_sum += icmp_bytes[i] << 8;
    }
  }
  while (icmp_sum >> 16) {
    icmp_sum = (icmp_sum & 0xFFFF) + (icmp_sum >> 16);
  }
  icmp->checksum = ~icmp_sum;
  
  eth_send(packet, sizeof(packet));
}
