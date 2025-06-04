#pragma once

typedef struct {
  unsigned short vendor_id;
  unsigned short device_id;
  unsigned int bar0;
} pci_device_t;

extern pci_device_t pci_dev;

unsigned int pci_read(unsigned char bus, unsigned char slot, 
                     unsigned char func, unsigned char offset);
void pci_write(unsigned char bus, unsigned char slot, 
              unsigned char func, unsigned char offset, 
              unsigned int value);
void detect_e1000(void);
