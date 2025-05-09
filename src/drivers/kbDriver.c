#include "kbDriver.h"
#include "vgaDriver.h"
#include "../include/io.h"

IDT_GATE idt[IDT_ENTRIES];
IDT_REGISTER idtReg;
ISR interruptHandler[IDT_ENTRIES];

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

const char* exceptionMessages[] = {
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

void setIdtGate(int n, unsigned int handler) {
  (idt + n)->lowOffset = LOW16(handler);
  (idt + n)->selector = 0x08;
  (idt + n)->always0 = 0;
  (idt + n)->flags = 0x8e;
  (idt + n)->highOffset = HIGH16(handler);
}

void loadIdt() {
  idtReg.base = (unsigned int)&idt;
  idtReg.limit = IDT_ENTRIES * sizeof(IDT_GATE) - 1;
  __asm__ volatile ("lidt (%0)" : : "r"(&idtReg));
}

void isrHandler(REGISTERS* r) {
  if (r->intN < 32) {
    kprintf(*(exceptionMessages + r->intN), VGA_WHITE);
    kprintf("\n", VGA_WHITE);
    while(1);
  }
}

void irqHandler(REGISTERS* r) {
  if (*(interruptHandler + r->intN) != 0) {
    (*(interruptHandler + r->intN))(r);
  }

  byteOutput(0x20, 0x20);
  if (r->intN >= 40) {
    byteOutput(0xa0, 0x20);
  }
}

static void keyboardHandler() {
  unsigned char scancode = byteInput(0x60);

  if (scancode == EXTENDED_SCANCODE) {
      byteOutput(0x20, 0x20);
      return;
  }

  unsigned char pressed = !(scancode & KEY_RELEASE_MASK);
  unsigned char keycode = scancode & ~KEY_RELEASE_MASK;
  
  if (pressed && keycode < sizeof(keymap) / sizeof(*(keymap + 0))) {
    if (*(*(keymap + keycode) + 0) != '\0') {
      kprintf(*(keymap + keycode), VGA_WHITE);
      kprintf(" ", VGA_WHITE);
    }
  }

  byteOutput(0x20, 0x20);
}

void isrInstall() {
  void (*isrHandlers[])() = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
    isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
  };
  
  for (int i = 0; i < 32; i++) {
    setIdtGate(i, (unsigned int)*(isrHandlers + i));
  }

  byteOutput(0x20, 0x11);
  byteOutput(0xa0, 0x11);
  byteOutput(0x21, 0x20);
  byteOutput(0xa1, 0x28);
  byteOutput(0x21, 0x04);
  byteOutput(0xa1, 0x02);
  byteOutput(0x21, 0x01);
  byteOutput(0xa1, 0x01);

  void (*irqHandlers[])() = {
    irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7,
    irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15
  };
    
  for (int i = 0; i < 16; i++) {
    setIdtGate(32 + i, (unsigned int)*(irqHandlers + i));
  }

  loadIdt();
}

void registerInterruptHandler(int irq, ISR handler) {
  *(interruptHandler + irq) = handler;
}

void initKeyboard() {
  while(byteInput(0x64) & 0x01) {
    byteInput(0x60);
  }

  registerInterruptHandler(IRQ1, keyboardHandler);
  byteOutput(0x21, byteInput(0x21) & 0xFD); 
}