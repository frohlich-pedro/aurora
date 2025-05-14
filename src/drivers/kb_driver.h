#ifndef KB_DRIVER_H
#define KB_DRIVER_H

#include "../include/io.h"

// IDT Configuration
#define IDT_ENTRIES     256
#define IDT_PRESENT     0x80
#define IDT_DPL0        0x00
#define IDT_GATE_INT    0x8E  // 64-bit interrupt gate

// IDT Structures
struct idt_gate {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_mid;
    unsigned int offset_high;
    unsigned int reserved;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned long base;
} __attribute__((packed));

extern struct idt_ptr idtp;  // IDT pointer declaration

// Keyboard Configuration
#define KBD_DATA_PORT   0x60
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC_EOI         0x20
#define KBD_IRQ         1
#define KBD_VECTOR      0x21
#define KBD_BUFFER_SIZE 256

// Keyboard Buffer
extern volatile char kbd_buffer[KBD_BUFFER_SIZE];
extern volatile unsigned int kbd_buffer_head;
extern volatile unsigned int kbd_buffer_tail;

// Function Declarations
void keyboard_interrupt_handler(void);
void keyboard_buffer_put(char c);
char keyboard_buffer_get(void);
void keyboard_install(void);
void pic_remap(void);
void idt_set_gate(unsigned char num, unsigned long base);
void idt_init(void);

// Assembly Linkage
extern void keyboard_isr(void);
void load_idt(struct idt_ptr *ptr);

#endif
