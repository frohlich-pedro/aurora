#include "../kernel/isr.h"
#include "../kernel/irq.h"
#include "../kernel/idt.h"
#include "../lib/modules/display.h"
#include "../mm/memory.h"
#include "../lib/modules/keyboard.h"
#include "../bin/shell.h"

void main() {
	idt_init();
	isr_init();
	irq_init();
	__asm__ volatile ("sti");
	display_init();
	keyboard_init();
	show();
	shell_init();
loop:
	shell_run();
	goto loop;
}
