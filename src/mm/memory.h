#ifndef MEMORY_H
#define MEMORY_H

#include "../lib/display.h"

#define HEAP_SIZE 0x500000

typedef struct memblock_s {
  unsigned long size;
  unsigned char is_free;
  struct memblock_s *next;
} memblock_t;

typedef void* (*memory_op_t)(void* arg);

enum memory_ops {
  MEMORY_OP_INIT = 0,
  MEMORY_OP_ALLOC,
  MEMORY_OP_RELEASE,
  MEMORY_OP_HEXDUMP,
  MEMORY_OP_COUNT
};

extern memory_op_t memory_op_arr[MEMORY_OP_COUNT];
extern unsigned char static_heap[HEAP_SIZE];

static inline void memory_init(void *start, unsigned long size) {
  void* args[] = {start, (void*)size};
  memory_op_arr[MEMORY_OP_INIT](args);
}

static inline void* alloc(unsigned long size) {
  return memory_op_arr[MEMORY_OP_ALLOC]((void*)size);
}

static inline void release(void *ptr) {
  memory_op_arr[MEMORY_OP_RELEASE](ptr);
}

static inline void hexdump(const void *addr, unsigned int length, unsigned char color) {
  void* args[] = {(void*)addr, (void*)(unsigned long)length, (void*)(unsigned int)color};
  memory_op_arr[MEMORY_OP_HEXDUMP](args);
}

#endif
