#include "pci.h"
#include "../ports.h"

pci_device_t pci_dev;

unsigned int pci_read(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset) {
  unsigned int address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
  port_dword_out(0xCF8, address);
  return port_dword_in(0xCFC);
}

void pci_write(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset, unsigned int value) {
  unsigned int address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
  port_dword_out(0xCF8, address);
  port_dword_out(0xCFC, value);
}

void detect_e1000() {
  for (int bus = 0; bus < 256; bus++) {
    for (int slot = 0; slot < 32; slot++) {
      unsigned int id = pci_read(bus, slot, 0, 0);
      if (id == 0x100E8086) {
        pci_dev.vendor_id = 0x8086;
        pci_dev.device_id = 0x100E;
        pci_dev.bar0 = pci_read(bus, slot, 0, 0x10) & ~0xF;
        
        unsigned int command = pci_read(bus, slot, 0, 0x4);
        command |= 0x6;
        pci_write(bus, slot, 0, 0x4, command);
        return;
      }
    }
  }
}
