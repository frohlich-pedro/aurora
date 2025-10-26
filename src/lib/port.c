#include "port.h"

static unsigned char byte_in_op(unsigned short port) {
    unsigned char result;
    asm volatile ("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

static unsigned short word_in_op(unsigned short port) {
    unsigned short result;
    asm volatile ("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

static void byte_out_op(unsigned short port, unsigned char data) {
    asm volatile ("out %%al, %%dx" : : "a" (data), "d" (port));
}

static void word_out_op(unsigned short port, unsigned short data) {
    asm volatile ("out %%ax, %%dx" : : "a" (data), "d" (port));
}

port_in_op_t port_in_arr[PORT_IN_OP_COUNT] = {
    { .byte = byte_in_op },      // PORT_IN_BYTE_OP
    { .word = word_in_op }       // PORT_IN_WORD_OP
};

port_out_op_t port_out_arr[PORT_OUT_OP_COUNT] = {
    { .byte = byte_out_op },     // PORT_OUT_BYTE_OP
    { .word = word_out_op }      // PORT_OUT_WORD_OP
};
