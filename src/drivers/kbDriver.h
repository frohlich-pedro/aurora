#ifndef KBDRIVER_H
#define KBDRIVER_H

typedef struct {
  unsigned short lowOffset;
  unsigned short selector;
  unsigned char always0;
  unsigned char flags;
  unsigned short highOffset;
} __attribute__((packed)) IDT_GATE;

typedef struct {
  unsigned short limit;
  unsigned int base;
} __attribute__((packed)) IDT_REGISTER;

#define LOW16(address) ((unsigned short)((address) & 0xffff))
#define HIGH16(address) ((unsigned short)(((address) >> 16) & 0xffff))
#define IDT_ENTRIES 256
#define IRQ1 33
#define KEY_RELEASE_MASK 0x80
#define EXTENDED_SCANCODE 0xe0

typedef struct {
  unsigned int ds;
  unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
  unsigned int intN, errorCode;
  unsigned int eip, cs, eflags, useresp, ss;
} REGISTERS;

typedef void (*ISR)(REGISTERS*);

extern IDT_GATE idt[IDT_ENTRIES];
extern IDT_REGISTER idtReg;
extern ISR interruptHandler[IDT_ENTRIES];
extern const char* keymap[];

void isr0(); 
void isr1(); 
void isr2(); 
void isr3();
void isr4(); 
void isr5(); 
void isr6(); 
void isr7();
void isr8(); 
void isr9(); 
void isr10(); 
void isr11();
void isr12(); 
void isr13(); 
void isr14(); 
void isr15();
void isr16(); 
void isr17(); 
void isr18(); 
void isr19();
void isr20(); 
void isr21(); 
void isr22(); 
void isr23();
void isr24(); 
void isr25(); 
void isr26(); 
void isr27();
void isr28(); 
void isr29(); 
void isr30(); 
void isr31();
    
void irq0(); 
void irq1(); 
void irq2(); 
void irq3();
void irq4(); 
void irq5(); 
void irq6(); 
void irq7();
void irq8(); 
void irq9(); 
void irq10(); 
void irq11();
void irq12(); 
void irq13(); 
void irq14(); 
void irq15();
  
void isrHandler(REGISTERS* r);
void irqHandler(REGISTERS* r);

void setIdtGate(int n, unsigned int handler);
void loadIdt();
void isrInstall();
void registerInterruptHandler(int irq, ISR handler);
void initKeyboard();

#endif