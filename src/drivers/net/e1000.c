#include "e1000.h"
#include "pci.h"
#include "../ports.h"
#include "../../kernel/util.h"

volatile unsigned int* e1000_regs;

__attribute__((aligned(16))) static rx_desc rx_ring[NUM_RX_DESC];
__attribute__((aligned(16))) static tx_desc tx_ring[NUM_TX_DESC];
static unsigned char rx_buffers[NUM_RX_DESC][PKT_BUF_SIZE];
static unsigned char tx_buffers[NUM_TX_DESC][PKT_BUF_SIZE];

void e1000_init(void) {
  detect_e1000();
  e1000_regs = (volatile unsigned int*)pci_dev.bar0;
  
  *(e1000_regs + (E1000_CTRL / 4)) |= 0x04000000;
  while (*(e1000_regs + (E1000_CTRL / 4)) & 0x04000000);
  
  unsigned char mac[6] = {0x52, 0x54, 0x00, 0x12, 0x34, 0x56};
  *(e1000_regs + (0x5400 / 4)) = *(unsigned int*)mac;
  *(e1000_regs + (0x5404 / 4)) = *(unsigned short*)&(*(mac + 4));
  
  for (int i = 0; i < NUM_RX_DESC; i++) {
    (*(rx_ring + i)).addr = (unsigned int)&(*(*(rx_buffers + i) + 0));
    (*(rx_ring + i)).status = 0;
  }

  *(e1000_regs + (E1000_RDBAL / 4)) = (unsigned int)rx_ring;
  *(e1000_regs + (E1000_RDBAH / 4)) = 0;
  *(e1000_regs + (E1000_RDLEN / 4)) = NUM_RX_DESC * sizeof(rx_desc);
  *(e1000_regs + (E1000_RDH / 4)) = 0;
  *(e1000_regs + (E1000_RDT / 4)) = NUM_RX_DESC - 1;
  
  for (int i = 0; i < NUM_TX_DESC; i++) {
    (*(tx_ring + i)).addr = (unsigned int)&tx_buffers[i][0];
    (*(tx_ring + i)).cmd = 0;
    (*(tx_ring + i)).status = 0;
  }

  *(e1000_regs + E1000_TDBAL / 4) = (unsigned int)tx_ring;
  *(e1000_regs + E1000_TDBAH / 4) = 0;
  *(e1000_regs + E1000_TDLEN / 4) = NUM_TX_DESC * sizeof(tx_desc);
  *(e1000_regs + E1000_TDH / 4) = 0;
  *(e1000_regs + E1000_TDT / 4) = 0;
  
  *(e1000_regs + E1000_CTRL / 4) |= 0x2;
  *(e1000_regs + E1000_RCTL / 4) = 0x1;
  *(e1000_regs + E1000_TCTL / 4) = 0x10;
}

void e1000_send(void* data, unsigned short len) {
  if (len > PKT_BUF_SIZE) return;
  
  unsigned int tail = e1000_regs[E1000_TDT / 4];
  tx_desc* desc = &(*(tx_ring + tail));
  
  memory_copy(data, (void*)desc->addr, len);
  
  desc->length = len;
  desc->cmd = 0x9;
  desc->status = 0;
  
  *(e1000_regs + E1000_TDT / 4) = (tail + 1) % NUM_TX_DESC;
  
  while (!(desc->status & 0xFF));
}

int e1000_receive(void* buffer) {
  unsigned int tail = e1000_regs[E1000_RDT / 4];
  unsigned int next = (tail + 1) % NUM_RX_DESC;
  
  if (rx_ring[next].status & 0x1) {
    unsigned short len = rx_ring[next].length;
    if (len > PKT_BUF_SIZE) len = PKT_BUF_SIZE;
    memory_copy((void*)rx_ring[next].addr, buffer, len);
    rx_ring[next].status = 0;
    e1000_regs[E1000_RDT / 4] = next;
    return len;
  }
  return 0;
}
