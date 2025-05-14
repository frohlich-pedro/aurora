#include "kb_driver.h"
#include "../include/memory.h"

idt_gate idt[IDT_ENTRIES];
idt_ptr idtp;

void idt_init(void) {
  idtp.limit = sizeof(struct idt_gate) * IDT_ENTRIES - 1;
  idtp.base = (unsigned long)&idt;
    
  for (int i = 0; i < IDT_ENTRIES; i++) {
    (idt + i)->offset_low = 0;
    (idt + i)->selector = 0;
    (idt + i)->zero = 0;
    (idt + i)->type_attr = 0;
    (idt + i)->offset_mid = 0;
    (idt + i)->offset_high = 0;
    (idt + i)->reserved = 0;
  }
}

void idt_set_gate(unsigned char n, unsigned long base) {
  (idt + n)->offset_low = base & 0xFFFF;
  (idt + n)->selector = 0x08;
  (idt + n)->zero = 0;
  (idt + n)->type_attr = IDT_PRESENT | IDT_DPL0 | IDT_GATE_INT;
  (idt + n)->offset_mid = (base >> 16) & 0xffff;
  (idt + n)->offset_high = (base >> 32) & 0xffffffff;
  (idt + n)->reserved = 0;
}
