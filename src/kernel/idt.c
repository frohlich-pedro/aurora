#include "idt.h"

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idt_pointer;

void idt_set_gate(unsigned char num,
	unsigned int base,
	unsigned short selector,
	unsigned char flags) {
	(idt + num)->base_low = base & 0xFFFF;
	(idt + num)->base_high = (base >> 16) & 0xFFFF;
	(idt + num)->selector = selector;
	(idt + num)->reserved = 0;
	(idt + num)->flags = flags;
}

void idt_init() {
	idt_pointer.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
	idt_pointer.base = (unsigned int)&idt;
	struct idt_entry* entry = idt;
	struct idt_entry* end = idt + IDT_ENTRIES;
init_loop:
	if (entry < end) {
		entry->base_low = 0;
		entry->base_high = 0;
		entry->selector = 0;
		entry->reserved = 0;
		entry->flags = 0;
		entry++;
		goto init_loop;
	}
	idt_flush((unsigned int)&idt_pointer);
}
