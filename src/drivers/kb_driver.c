#include "kb_driver.h"
#include "../include/memory.h"

volatile char kbd_buffer[KBD_BUFFER_SIZE];
volatile unsigned int kbd_buffer_head = 0;
volatile unsigned int kbd_buffer_tail = 0;

void pic_remap(void) {
    // ICW1: Initialize PIC
    byte_output(PIC1_COMMAND, 0x11);
    byte_output(0xA0, 0x11);

    // ICW2: Remap IRQs
    byte_output(PIC1_DATA, 0x20);
    byte_output(0xA1, 0x28);

    // ICW3: Configure master/slave
    byte_output(PIC1_DATA, 0x04);
    byte_output(0xA1, 0x02);

    // ICW4: Enable 8086 mode
    byte_output(PIC1_DATA, 0x01);
    byte_output(0xA1, 0x01);

    // Mask all interrupts except IRQ1 (keyboard)
    byte_output(PIC1_DATA, 0xFD);
    byte_output(0xA1, 0xFF);
}

void keyboard_buffer_put(char c) {
    unsigned int next = (kbd_buffer_head + 1) % KBD_BUFFER_SIZE;
    if (next != kbd_buffer_tail) {
        kbd_buffer[kbd_buffer_head] = c;
        kbd_buffer_head = next;
    }
}

char keyboard_buffer_get(void) {
    if (kbd_buffer_head == kbd_buffer_tail) return 0;
    char c = kbd_buffer[kbd_buffer_tail];
    kbd_buffer_tail = (kbd_buffer_tail + 1) % KBD_BUFFER_SIZE;
    return c;
}

static const char scan_code_to_ascii[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

void keyboard_interrupt_handler(void) {
    unsigned char scan_code = byte_input(KBD_DATA_PORT);
    if (scan_code < sizeof(scan_code_to_ascii)) {
        char ch = scan_code_to_ascii[scan_code];
        if (ch) keyboard_buffer_put(ch);
    }
    byte_output(PIC1_COMMAND, PIC_EOI);  // EOI sent here
}

void keyboard_install(void) {
    idt_init();
    pic_remap();
    idt_set_gate(KBD_VECTOR, (unsigned long)keyboard_isr);  // Points to the ASM ISR
    load_idt(&idtp);
    asm volatile ("sti");
}
