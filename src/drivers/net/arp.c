#include "arp.h"
#include "eth.h"
#include "../../kernel/util.h"

#define ARP_CACHE_SIZE 10

typedef struct {
  unsigned char ip[4];
  unsigned char mac[6];
} arp_cache_entry_t;

static arp_cache_entry_t arp_cache[ARP_CACHE_SIZE];
unsigned char my_ip[4] = {10, 0, 2, 15};

void arp_init() {
  memset(arp_cache, 0, sizeof(arp_cache));
}

void arp_request(unsigned char* target_ip) {
  unsigned char packet[sizeof(eth_header_t) + sizeof(arp_packet_t)];
  eth_header_t* eth = (eth_header_t*)packet;
  arp_packet_t* arp = (arp_packet_t*)(packet + sizeof(eth_header_t));
  
  memcpy(eth->dst_mac, broadcast_mac, 6);
  memcpy(eth->src_mac, my_mac, 6);
  eth->ethertype = ETHERTYPE_ARP;
  
  arp->htype = ARP_HTYPE_ETHERNET;
  arp->ptype = ARP_PTYPE_IP;
  arp->hlen = 6;
  arp->plen = 4;
  arp->oper = ARP_OP_REQUEST;
  memcpy(arp->sha, my_mac, 6);
  memcpy(arp->spa, my_ip, 4);
  memset(arp->tha, 0, 6);
  memcpy(arp->tpa, target_ip, 4);
  
  eth_send(packet, sizeof(packet));
}

void arp_reply(unsigned char* target_mac, unsigned char* target_ip) {
  unsigned char packet[sizeof(eth_header_t) + sizeof(arp_packet_t)];
  eth_header_t* eth = (eth_header_t*)packet;
  arp_packet_t* arp = (arp_packet_t*)(packet + sizeof(eth_header_t));
  
  memcpy(eth->dst_mac, target_mac, 6);
  memcpy(eth->src_mac, my_mac, 6);
  eth->ethertype = ETHERTYPE_ARP;
  
  arp->htype = ARP_HTYPE_ETHERNET;
  arp->ptype = ARP_PTYPE_IP;
  arp->hlen = 6;
  arp->plen = 4;
  arp->oper = ARP_OP_REPLY;
  memcpy(arp->sha, my_mac, 6);
  memcpy(arp->spa, my_ip, 4);
  memcpy(arp->tha, target_mac, 6);
  memcpy(arp->tpa, target_ip, 4);
  
  eth_send(packet, sizeof(packet));
}

void arp_receive(arp_packet_t* arp) {
  if (arp->oper == ARP_OP_REQUEST) {
    if (memcmp(arp->tpa, my_ip, 4) == 0) {
      arp_reply(arp->sha, arp->spa);
    }
  }
  else if (arp->oper == ARP_OP_REPLY) {
    for (int i = 1; i < ARP_CACHE_SIZE; i++) {
      if (arp_cache[i].ip[0] == 0) {
        memcpy(arp_cache[i].ip, arp->spa, 4);
        memcpy(arp_cache[i].mac, arp->sha, 6);
        break;
      }
    }
  }
}

unsigned char* arp_lookup(unsigned char* ip) {
  for (int i = 0; i < ARP_CACHE_SIZE; i++) {
    if (memcmp(arp_cache[i].ip, ip, 4) == 0) {
      return arp_cache[i].mac;
    }
  }
  return 0;
}
