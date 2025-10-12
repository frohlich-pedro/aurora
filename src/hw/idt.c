#include "idt.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

static void set_idt_gate_op(void* idt_ptr, void* arg) {
    idt_gate_t* idt = (idt_gate_t*)idt_ptr;
    void** args = (void**)arg;
    int n = (int)(unsigned int)args[0];
    unsigned int handler = (unsigned int)args[1];
    
    (idt + n)->low_offset = (unsigned short)((handler) & 0xFFFF);
    (idt + n)->sel = KERNEL_CS;
    (idt + n)->always0 = 0;
    (idt + n)->flags = 0x8E;
    (idt + n)->high_offset = (unsigned short)(((handler) >> 16) & 0xFFFF);
}

static void load_idt_op(void* idt_ptr, void* arg) {
    (void)arg;
    idt_gate_t* idt = (idt_gate_t*)idt_ptr;
    
    idt_reg.base = (unsigned int) idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile ("lidt (%0)" : : "r" (&idt_reg));
}

idt_op_t idt_op_arr[IDT_OP_COUNT] = {
    set_idt_gate_op,
    load_idt_op
};
