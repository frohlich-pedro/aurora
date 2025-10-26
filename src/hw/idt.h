#pragma once

#define IDT_ENTRIES 256
#define KERNEL_CS 0x08

typedef struct {
  unsigned short low_offset;
  unsigned short sel;
  unsigned char always0;
  unsigned char flags;
  unsigned short high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct {
  unsigned short limit;
  unsigned int base;
} __attribute__((packed)) idt_register_t;

typedef void (*idt_op_t)(void*, void*);

extern idt_gate_t idt[IDT_ENTRIES];
extern idt_register_t idt_reg;
extern idt_op_t idt_op_arr[];

enum idt_ops {
    IDT_OP_SET_GATE = 0,
    IDT_OP_LOAD,
    IDT_OP_COUNT
};

static inline void set_idt_gate(int n, unsigned int handler) {
    void* args[] = {(void*)(unsigned int)n, (void*)(unsigned int)handler};
    idt_op_arr[IDT_OP_SET_GATE](&idt, args);
}

static inline void load_idt() {
    idt_op_arr[IDT_OP_LOAD](&idt, (void*)0);
}
