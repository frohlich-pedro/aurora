#pragma once

#include "../../kernel/mem.h"

#define E1000_CTRL 0x0000
#define E1000_STATUS 0x0008
#define E1000_EECD 0x0010
#define E1000_EERD 0x0014
#define E1000_CTRL_EXT 0x0018
#define E1000_ICR 0x00C0
#define E1000_IMS 0x00D0
#define E1000_RCTL 0x0100
#define E1000_TCTL 0x0400
#define E1000_RDBAL 0x2800
#define E1000_RDBAH 0x2804
#define E1000_RDLEN 0x2808
#define E1000_RDH 0x2810
#define E1000_RDT 0x2818
#define E1000_TDBAL 0x3800
#define E1000_TDBAH 0x3804
#define E1000_TDLEN 0x3808
#define E1000_TDH 0x3810
#define E1000_TDT 0x3818

#define NUM_RX_DESC    32
#define NUM_TX_DESC    8
#define PKT_BUF_SIZE   2048

typedef struct {
  unsigned int addr;
  unsigned short length;
  unsigned short checksum;
  unsigned char status;
  unsigned char errors;
  unsigned short special;
} __attribute__((packed)) rx_desc;

typedef struct {
  unsigned int addr;
  unsigned short length;
  unsigned char cso;
  unsigned char cmd;
  unsigned char status;
  unsigned char css;
  unsigned short special;
} __attribute__((packed)) tx_desc;

void e1000_init(void);
void e1000_send(void* data, unsigned short len);
int e1000_receive(void* buffer);

#define E1000_READ_REG(reg) (*(volatile unsigned int*)(e1000_regs + (reg)/4))
#define E1000_WRITE_REG(reg, val) (*(volatile unsigned int*)(e1000_regs + (reg)/4) = (val))
