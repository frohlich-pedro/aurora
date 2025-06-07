#include "isr.h"
#include "idt.h"
#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"

isr_t interrupt_handlers[256];

char* exception_messages[] = {
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault",
  "Unknown Interrupt",
  "Coprocessor Fault",
  "Alignment Check",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};

void isr_install() {
  unsigned int* handlers = (unsigned int[]){
    (unsigned int)isr0,
    (unsigned int)isr1,
    (unsigned int)isr2,
    (unsigned int)isr3,
    (unsigned int)isr4,
    (unsigned int)isr5,
    (unsigned int)isr6,
    (unsigned int)isr7,
    (unsigned int)isr8,
    (unsigned int)isr9,
    (unsigned int)isr10,
    (unsigned int)isr11,
    (unsigned int)isr12,
    (unsigned int)isr13,
    (unsigned int)isr14,
    (unsigned int)isr15,
    (unsigned int)isr16,
    (unsigned int)isr17,
    (unsigned int)isr18,
    (unsigned int)isr19,
    (unsigned int)isr20,
    (unsigned int)isr21,
    (unsigned int)isr22,
    (unsigned int)isr23,
    (unsigned int)isr24,
    (unsigned int)isr25,
    (unsigned int)isr26,
    (unsigned int)isr27,
    (unsigned int)isr28,
    (unsigned int)isr29,
    (unsigned int)isr30,
    (unsigned int)isr31
  };

  register int i asm("ecx") = 0;
  do {
    set_idt_gate(i, *(handlers + i));
    i++;
  } while (i < 32);

  unsigned char* ports = (unsigned char[]) {0x20,0xA0,0x21,0xA1,0x21,0xA1,0x21,0xA1,0x21,0xA1};
  unsigned char* values = (unsigned char[]) {0x11,0x11,0x20,0x28,0x04,0x02,0x01,0x01,0x00,0x00};
  
  register int j asm("ebx") = 0;
  while (j < 10) {
    port_byte_out(*(ports + j), *(values + j));
    j++;
  }

  unsigned int* irq_handlers = (unsigned int[]) {
    (unsigned int)irq0,
    (unsigned int)irq1,
    (unsigned int)irq2,
    (unsigned int)irq3,
    (unsigned int)irq4,
    (unsigned int)irq5,
    (unsigned int)irq6,
    (unsigned int)irq7,
    (unsigned int)irq8,
    (unsigned int)irq9,
    (unsigned int)irq10,
    (unsigned int)irq11,
    (unsigned int)irq12,
    (unsigned int)irq13,
    (unsigned int)irq14,
    (unsigned int)irq15
  };

  register int k asm("edx") = 0;
  do {
    set_idt_gate(32 + k, *(irq_handlers + k));
    k++;
  } while (k < 16);

  load_idt();
}

void isr_handler(registers_t* r) {
  char s[3];
  int_to_string(r->int_no, s);
  print_string("INT: ", VGA_WHITE);
  print_string(s, VGA_WHITE);
  print_string(" - ", VGA_WHITE);
  print_string(*(exception_messages + r->int_no), VGA_WHITE);
  print_nl();
}

void register_interrupt_handler(unsigned char n, isr_t handler) {
  *(interrupt_handlers + n) = handler;
}

void irq_handler(registers_t* r) {
  isr_t handler = *(interrupt_handlers + r->int_no);
  if (handler) handler(r);

  port_byte_out(0x20, 0x20);
  if (r->int_no >= 40) port_byte_out(0xA0, 0x20);
}
