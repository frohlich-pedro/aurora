#pragma once

#include "eth.h"
#include "arp.h"
#include "ip.h"
#include "icmp.h"

extern unsigned char my_ip[4];
extern unsigned char my_mac[6];
extern unsigned char broadcast_mac[6];

void net_init();
void net_receive();
