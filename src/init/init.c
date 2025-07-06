#include "../kernel/isr.h"
#include "../kernel/irq.h"
#include "../kernel/idt.h"
#include "../mm/memory.h"

void main() {
	idt_init();
	isr_init();
	irq_init();
	__asm__ volatile ("sti");
loop:
	goto loop;
}
