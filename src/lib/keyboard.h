#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "port.h"
#include "../hw/isr.h"

#define KEYBOARD_BUFFER_SIZE 256
#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define LEFT_SHIFT_RELEASE 0xAA
#define RIGHT_SHIFT_RELEASE 0xB6
#define SC_MAX 57

typedef struct {
  union {
    struct {
      char buffer[KEYBOARD_BUFFER_SIZE];
      unsigned int buffer_pos;
      union {
        struct {
          unsigned char shift_pressed : 1;
          unsigned char : 7;
        };
        unsigned char flags;
      };
    };
    unsigned char padding[256];
  };
} keyboard_t;

typedef void (*keyboard_op_t)(keyboard_t*, void*);

extern keyboard_t keyboard;
extern keyboard_op_t keyboard_op_arr[];

enum keyboard_ops {
  KB_OP_INIT = 0,
  KB_OP_BACKSPACE,
  KB_OP_APPEND,
  KB_OP_CLEAR_BUFFER,
  KB_OP_SET_FLAGS,
  KB_OP_COUNT
};

static inline void keyboard_init(void) {
  keyboard_op_arr[KB_OP_INIT](&keyboard, (void*)0);
}

static inline void keyboard_backspace(void) {
  keyboard_op_arr[KB_OP_BACKSPACE](&keyboard, (void*)0);
}

static inline void keyboard_append(char letter) {
  keyboard_op_arr[KB_OP_APPEND](&keyboard, (void*)(unsigned int)letter);
}

static inline void keyboard_clear_buffer(void) {
  keyboard_op_arr[KB_OP_CLEAR_BUFFER](&keyboard, (void*)0);
}

static inline void keyboard_set_flags(unsigned char flags) {
  keyboard_op_arr[KB_OP_SET_FLAGS](&keyboard, (void*)(unsigned int)flags);
}

void keyboard_handler(registers_t* regs);

#endif
