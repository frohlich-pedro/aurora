#include "idt.h"
#include "../lib/util.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, unsigned int handler) {
  register idt_gate_t* gate asm("eax") = idt + n;
  gate->low_offset = low_16(handler);
  gate->sel = KERNEL_CS;
  gate->always0 = 0;
  gate->flags = 0x8E;
  gate->high_offset = high_16(handler);
}

void load_idt() {
  idt_reg.base = (unsigned int)idt;
  idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
  asm volatile ("lidt (%0)" : : "r" (&idt_reg));
}