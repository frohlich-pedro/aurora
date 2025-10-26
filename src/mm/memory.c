#include "memory.h"

unsigned char static_heap[HEAP_SIZE];
static memblock_t* heap_head = (void*)0;
memory_op_t memory_op_arr[MEMORY_OP_COUNT];

static void* memory_init_op(void* arg) {
  void** args = (void**)arg;
  void* start = args[0];
  unsigned long size = (unsigned long)args[1];

  heap_head = (memblock_t*)start;
  heap_head->size = size - sizeof(memblock_t);
  heap_head->is_free = 1;
  heap_head->next = (void*)0;
  return (void*)0;
}

static void lazy_init() {
  if (!heap_head) {
    void* init_args[] = {(void*)static_heap, (void*)HEAP_SIZE};
    memory_op_arr[MEMORY_OP_INIT](init_args);
  }
}

static memblock_t* locate_fit(unsigned long req_size) {
  memblock_t *scan = heap_head;

  while (scan) {
    if (scan->is_free && scan->size >= req_size) {
      if (scan->size > req_size + sizeof(memblock_t) + 8) {
        memblock_t *split = (memblock_t*)((char*)scan +
          sizeof(memblock_t) + req_size);
        split->size = scan->size - req_size - sizeof(memblock_t);
        split->is_free = 1;
        split->next = scan->next;

        scan->size = req_size;
        scan->next = split;
      }

      scan->is_free = 0;
      return scan;
    }
    scan = scan->next;
  }

  return (void*)0;
}

static void* alloc_op(void* arg) {
  unsigned long size = (unsigned long)arg;

  if (!size) return (void*)0;

  lazy_init();
  size = (size + 7) & ~7;

  memblock_t *block = locate_fit(size);
  if (block) {
    return (void*)((char*)block + sizeof(memblock_t));
  }
  return (void*)0;
}

static void* release_op(void* arg) {
  void* ptr = arg;
  if (!ptr) return (void*)0;

  memblock_t *block = (memblock_t*)((char*)ptr - sizeof(memblock_t));
  block->is_free = 1;

  if (block->next && block->next->is_free) {
    block->size += block->next->size + sizeof(memblock_t);
    block->next = block->next->next;
  }

  memblock_t *walker = heap_head;
  while (walker && walker->next != block) {
    walker = walker->next;
  }

  if (walker && walker->is_free) {
    walker->size += block->size + sizeof(memblock_t);
    walker->next = block->next;
  }

  return (void*)0;
}

static void* hexdump_op(void* arg) {
  void** args = (void**)arg;
  const void *addr = args[0];
  unsigned int length = (unsigned int)(unsigned long)args[1];
  unsigned char color = (unsigned char)(unsigned int)args[2];

  const unsigned char *data = (const unsigned char*)addr; // This is the key fix!
  unsigned int offset = 0;
  unsigned int lines_printed = 0;
  const int max_lines = 20;

  while (offset < length && lines_printed < max_lines) {
    // Address (32-bit) - show actual address being dumped
    print_hex((unsigned int)((unsigned long)data + offset), 8, color);
    print(": ", color);

    // Hex bytes (8 bytes per line)
    int i = 0;
    while (i < 8) {
      if (offset + i < length) {
        // Actually read from memory at the calculated address
        print_hex(data[offset + i], 2, color);
      } else {
        print("  ", color);
      }
      
      // Single space between bytes
      if (i < 7) {
        print(" ", color);
      }
      i++;
    }

    // Separator
    print("  ", color);

    // ASCII representation (8 characters)
    i = 0;
    while (i < 8) {
      if (offset + i < length) {
        unsigned char c = data[offset + i];
        if (c >= 32 && c <= 126) {
          print_char(c, color);
        } else {
          print_char('.', color);
        }
      } else {
        print_char(' ', color);
      }
      i++;
    }

    print_char('\n', color);
    offset += 8;
    lines_printed++;
  }

  // Show truncation message if needed
  if (offset < length) {
    print("[... ", color);
    print_hex(lines_printed * 8, 4, color);
    print(" bytes shown of ", color);
    print_hex(length, 4, color);
    print(" total ...]\n", color);
  }

  return (void*)0;
}

memory_op_t memory_op_arr[MEMORY_OP_COUNT] = {
  memory_init_op,
  alloc_op,
  release_op,
  hexdump_op
};

