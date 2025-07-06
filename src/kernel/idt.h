#ifndef IDT_H
#define IDT_H

struct idt_entry {
	unsigned short base_low;
	unsigned short selector;
	unsigned char  reserved;
	unsigned char  flags;
	unsigned short base_high;
} __attribute__((packed));

struct idt_ptr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

#define IDT_ENTRIES 256
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0 0x00
#define IDT_FLAG_RING1 0x20
#define IDT_FLAG_RING2 0x40
#define IDT_FLAG_RING3 0x60
#define IDT_FLAG_INTERRUPT_GATE 0x0E
#define IDT_FLAG_TRAP_GATE 0x0F

void idt_init();
void idt_set_gate(unsigned char num,
	unsigned int base,
	unsigned short selector,
	unsigned char flags);
extern void idt_flush(unsigned int);

#endif
