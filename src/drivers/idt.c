#include "kb_driver.h"
#include "../include/memory.h"

struct idt_gate idt[IDT_ENTRIES];
struct idt_ptr idtp;

void idt_init(void) {
  idtp.limit = sizeof(struct idt_gate) * IDT_ENTRIES - 1;
  idtp.base = (unsigned long)&idt;
    
  for (int i = 0; i < IDT_ENTRIES; i++) {
    idt[i].offset_low = 0;
    idt[i].selector = 0;
    idt[i].zero = 0;
    idt[i].type_attr = 0;
    idt[i].offset_mid = 0;
    idt[i].offset_high = 0;
    idt[i].reserved = 0;
  }
}

void idt_set_gate(unsigned char num, unsigned long base) {
  idt[num].offset_low = base & 0xFFFF;
  idt[num].selector = 0x08;
  idt[num].zero = 0;
  idt[num].type_attr = IDT_PRESENT | IDT_DPL0 | IDT_GATE_INT;
  idt[num].offset_mid = (base >> 16) & 0xFFFF;
  idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
  idt[num].reserved = 0;
}
