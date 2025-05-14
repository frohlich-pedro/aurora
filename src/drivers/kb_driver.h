#ifndef KB_DRIVER_H
#define KB_DRIVER_H

#include "../include/io.h"

#define IDT_ENTRIES 256
#define IDT_PRESENT 0x80
#define IDT_DPL0 0x00
#define IDT_GATE_INT 0x8E

typedef struct {
  unsigned short offset_low;
  unsigned short selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short offset_mid;
  unsigned int offset_high;
  unsigned int reserved;
} __attribute__((packed)) idt_gate;

typedef struct {
  unsigned short limit;
  unsigned long base;
} __attribute__((packed)) idt_ptr;

extern idt_ptr idtp;

#define KBD_DATA_PORT 0x60
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC_EOI 0x20
#define KBD_IRQ 1
#define KBD_VECTOR 0x21
#define KBD_BUFFER_SIZE 256

extern volatile char kbd_buffer[KBD_BUFFER_SIZE];
extern volatile unsigned int kbd_buffer_head;
extern volatile unsigned int kbd_buffer_tail;

void keyboard_interrupt_handler();
void keyboard_buffer_put(char c);
char keyboard_buffer_get();
void keyboard_install();
void pic_remap();
void idt_set_gate(unsigned char num, unsigned long base);
void idt_init();

extern void keyboard_isr();
void load_idt(idt_ptr *ptr);

#endif
