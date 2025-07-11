#include "isr.h"
#include "idt.h"

static isr_t interrupt_handlers[256];

const char* exception_messages[32] = {
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
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
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

void isr_init() {
	idt_set_gate(0, (unsigned int)isr0, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(1, (unsigned int)isr1, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(2, (unsigned int)isr2, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(3, (unsigned int)isr3, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(4, (unsigned int)isr4, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(5, (unsigned int)isr5, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(6, (unsigned int)isr6, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(7, (unsigned int)isr7, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(8, (unsigned int)isr8, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(9, (unsigned int)isr9, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(10, (unsigned int)isr10, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(11, (unsigned int)isr11, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(12, (unsigned int)isr12, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(13, (unsigned int)isr13, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(14, (unsigned int)isr14, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(15, (unsigned int)isr15, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(16, (unsigned int)isr16, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(17, (unsigned int)isr17, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(18, (unsigned int)isr18, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(19, (unsigned int)isr19, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(20, (unsigned int)isr20, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(21, (unsigned int)isr21, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(22, (unsigned int)isr22, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(23, (unsigned int)isr23, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(24, (unsigned int)isr24, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(25, (unsigned int)isr25, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(26, (unsigned int)isr26, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(27, (unsigned int)isr27, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(28, (unsigned int)isr28, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(29, (unsigned int)isr29, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(30, (unsigned int)isr30, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
	idt_set_gate(31, (unsigned int)isr31, 0x08,
		IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
}

void isr_install_handler(int isr, isr_t handler) {
	*(interrupt_handlers + isr) = handler;
}

void isr_uninstall_handler(int isr) {
	*(interrupt_handlers + isr) = 0;
}

void isr_handler(struct registers* regs) {
	if (interrupt_handlers[regs->int_no] != 0) {
		isr_t handler = *(interrupt_handlers + regs->int_no);
		handler(regs);
	} else {
		__asm__ volatile ("cli; hlt");
	}
}
