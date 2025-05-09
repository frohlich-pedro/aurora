#include "kb_driver.h"
#include "vga_driver.h"
#include "../include/io.h"

idt_gate idt[IDT_ENTRIES];
idt_register idt_reg;
isr interrupt_handler[IDT_ENTRIES];

const char* keymap[128] = {
  "ERROR",
  "ESC",
  "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
  "BACKSPACE",
  "TAB",
  "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]",
  "ENTER",
  "L_CTRL",
  "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
  "L_SHIFT",
  "\\",
  "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/",
  "R_SHIFT",
  "KP_*",
  "L_ALT",
  "SPACE",
  "CAPSLOCK",
  "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
  "NUMLOCK",
  "SCROLLLOCK",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};

const char* exception_messages[] = {
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

void set_idt_gate(int n, unsigned int handler) {
  idt[n].low_offset = LOW_16(handler);
  idt[n].selector = 0x08;
  idt[n].always0 = 0;
  idt[n].flags = 0x8e;
  idt[n].high_offset = HIGH_16(handler);
}

void load_idt() {
  idt_reg.base = (unsigned int)&idt;
  idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate) - 1;
  __asm__ volatile ("lidt (%0)" : : "r"(&idt_reg));
}

void isr_handler(registers* r) {
  if (r->int_n < 32) {
    print_kernel(exception_messages[r->int_n], VGA_WHITE);
    print_kernel("\n", VGA_WHITE);
    while(1);
  }
}

void irq_handler(registers* r) {
  if (interrupt_handler[r->int_n] != 0) {
    interrupt_handler[r->int_n](r);
  }

  byte_output(0x20, 0x20);
  if (r->int_n >= 40) {
    byte_output(0xa0, 0x20);
  }
}

static void keyboard_handler() {
  unsigned char scancode = byte_input(0x60);
  unsigned char pressed, keycode;

  if (scancode == EXTENDED_SCANCODE) {
    byte_output(0x20, 0x20);
    return;
  }

  pressed = !(scancode & KEY_RELEASE_MASK);
  keycode = scancode & ~KEY_RELEASE_MASK;
  
  if (pressed && keycode < sizeof(keymap) / sizeof(*keymap)) {
    if (keymap[keycode][0] != '\0') {
      print_kernel(keymap[keycode], VGA_WHITE);
      print_kernel(" ", VGA_WHITE);
    }
  }

  byte_output(0x20, 0x20);
}

void isr_install() {
  void (*isr_handlers[])() = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
    isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
  };
  
  int i;
  for (i = 0; i < 32; i++) {
    set_idt_gate(i, (unsigned int)isr_handlers[i]);
  }

  byte_output(0x20, 0x11);
  byte_output(0xa0, 0x11);
  byte_output(0x21, 0x20);
  byte_output(0xa1, 0x28);
  byte_output(0x21, 0x04);
  byte_output(0xa1, 0x02);
  byte_output(0x21, 0x01);
  byte_output(0xa1, 0x01);

  void (*irq_handlers[])() = {
    irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7,
    irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15
  };
    
  for (i = 0; i < 16; i++) {
    set_idt_gate(32 + i, (unsigned int)irq_handlers[i]);
  }

  load_idt();
}

void register_interrupt_handler(int irq, isr handler) {
  interrupt_handler[irq] = handler;
}

void init_keyboard() {
  while(byte_input(0x64) & 0x01) {
    byte_input(0x60);
  }

  register_interrupt_handler(IRQ1, keyboard_handler);
  byte_output(0x21, byte_input(0x21) & 0xFD);
}
