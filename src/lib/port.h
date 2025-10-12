#pragma once

typedef unsigned char (*port_in_byte_op_t)(unsigned short port);
typedef unsigned short (*port_in_word_op_t)(unsigned short port);
typedef void (*port_out_byte_op_t)(unsigned short port, unsigned char data);
typedef void (*port_out_word_op_t)(unsigned short port, unsigned short data);

typedef union {
    port_in_byte_op_t byte;
    port_in_word_op_t word;
} port_in_op_t;

extern port_in_op_t port_in_arr[];

typedef union {
    port_out_byte_op_t byte;
    port_out_word_op_t word;
} port_out_op_t;

extern port_out_op_t port_out_arr[];

enum port_in_ops {
    PORT_IN_BYTE_OP = 0,
    PORT_IN_WORD_OP,
    PORT_IN_OP_COUNT
};

enum port_out_ops {
    PORT_OUT_BYTE_OP = 0,
    PORT_OUT_WORD_OP,
    PORT_OUT_OP_COUNT
};

static inline unsigned char byte_in(unsigned short port) {
    return port_in_arr[PORT_IN_BYTE_OP].byte(port);
}

static inline unsigned short word_in(unsigned short port) {
    return port_in_arr[PORT_IN_WORD_OP].word(port);
}

static inline void byte_out(unsigned short port, unsigned char data) {
    port_out_arr[PORT_OUT_BYTE_OP].byte(port, data);
}

static inline void word_out(unsigned short port, unsigned short data) {
    port_out_arr[PORT_OUT_WORD_OP].word(port, data);
}
