#include "irq.h"
#include "idt.h"
#include "ports.h"

static isr_t irq_routines[16];

static void irq_remap() {
	unsigned char a1 = inb(PORT_PIC1_DATA);
	unsigned char a2 = inb(PORT_PIC2_DATA);
	outb(PORT_PIC1_CMD, 0x11);
	io_wait();
	outb(PORT_PIC2_CMD, 0x11);
	io_wait();
	outb(PORT_PIC1_DATA, 0x20);
	io_wait();
	outb(PORT_PIC2_DATA, 0x28);
	io_wait();
	outb(PORT_PIC1_DATA, 0x04);
	io_wait();
	outb(PORT_PIC2_DATA, 0x02);
	io_wait();
	outb(PORT_PIC1_DATA, 0x01);
	io_wait();
	outb(PORT_PIC2_DATA, 0x01);
	io_wait();
	outb(PORT_PIC1_DATA, a1);
	outb(PORT_PIC2_DATA, a2);
}

void irq_init() {
	irq_remap();
	idt_set_gate(32, (unsigned int)irq0, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(33, (unsigned int)irq1, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(34, (unsigned int)irq2, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(35, (unsigned int)irq3, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(36, (unsigned int)irq4, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(37, (unsigned int)irq5, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(38, (unsigned int)irq6, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(39, (unsigned int)irq7, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(40, (unsigned int)irq8, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(41, (unsigned int)irq9, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(42, (unsigned int)irq10, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(43, (unsigned int)irq11, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(44, (unsigned int)irq12, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(45, (unsigned int)irq13, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(46, (unsigned int)irq14, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(47, (unsigned int)irq15, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
}

void irq_install_handler(int irq, isr_t handler) {
	irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
	irq_routines[irq] = 0;
}

void irq_handler(struct registers* regs) {
	if (regs->int_no >= 40) {
		outb(PORT_PIC2_CMD, 0x20);
	}
	outb(PORT_PIC1_CMD, 0x20);
	if (*(irq_routines + regs->int_no - 32) != 0) {
		isr_t handler = irq_routines[regs->int_no - 32];
		handler(regs);
	}
}
