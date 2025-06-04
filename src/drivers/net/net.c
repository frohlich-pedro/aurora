#include "net.h"
#include "../../kernel/util.h"
#include "e1000.h"

void net_init() {
  eth_init();
  arp_init();
}

void net_receive() {
  unsigned char packet[1518];
  int len = e1000_receive(packet);
  if (len < sizeof(eth_header_t)) return;
  
  eth_header_t* eth = (eth_header_t*)packet;
  
  if (memcmp(eth->dst_mac, my_mac, 6) != 0 && 
      memcmp(eth->dst_mac, broadcast_mac, 6) != 0) {
    return;
  }
  
  switch (eth->ethertype) {
    case ETHERTYPE_ARP:
      arp_receive((arp_packet_t*)(packet + sizeof(eth_header_t)));
      break;
    case ETHERTYPE_IP:
      ip_receive((ip_header_t*)(packet + sizeof(eth_header_t)));
      break;
  }
}
