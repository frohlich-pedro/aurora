#include "mem.h"
#include "../drivers/display.h"
#include "util.h"

void memory_copy(unsigned char* source, unsigned char* dest, unsigned int nbytes) {
  while (nbytes--) {
    *dest++ = *source++;
  }
}

void init_dynamic_mem() {
  dynamic_mem_start = (dynamic_mem_node_t*) dynamic_mem_area;
  dynamic_mem_start->size = DYNAMIC_MEM_TOTAL_SIZE - DYNAMIC_MEM_NODE_SIZE;
  dynamic_mem_start->next = NULL_POINTER;
  dynamic_mem_start->prev = NULL_POINTER;
}

void print_dynamic_node_size() {
  char node_size_string[256];
  int_to_string(DYNAMIC_MEM_NODE_SIZE, node_size_string);
  print_string("DYNAMIC_MEM_NODE_SIZE = ", VGA_BLACK);
  print_string(node_size_string, VGA_BLACK);
  print_nl();
}

void print_dynamic_mem_node(dynamic_mem_node_t *node) {
  char size_string[256];
  int_to_string(node->size, size_string);
  print_string("{size = ", VGA_BLACK);
  print_string(size_string, VGA_BLACK);
  char used_string[256];
  int_to_string(node->used, used_string);
  print_string("; used = ", VGA_BLACK);
  print_string(used_string, VGA_BLACK);
  print_string("}; ", VGA_BLACK);
}

void print_dynamic_mem() {
  dynamic_mem_node_t *current = dynamic_mem_start;
  print_string("[", VGA_BLACK);
  while (current != NULL_POINTER) {
    print_dynamic_mem_node(current);
    current = current->next;
  }
  print_string("]\n", VGA_BLACK);
}

void* find_best_mem_block(dynamic_mem_node_t* dynamic_mem, unsigned int size) {
  dynamic_mem_node_t* best_mem_block = NULL_POINTER;
  unsigned int best_mem_block_size = DYNAMIC_MEM_TOTAL_SIZE + 1;
  
  for (dynamic_mem_node_t* current = dynamic_mem; current; current = current->next) {
    if ((!current->used) &&
        (current->size >= (size + DYNAMIC_MEM_NODE_SIZE)) &&
        (current->size <= best_mem_block_size)) {
      best_mem_block = current;
      best_mem_block_size = current->size;
    }
  }
  
  return best_mem_block;
}

void* mem_alloc(unsigned int size) {
  dynamic_mem_node_t* best_mem_block = find_best_mem_block(dynamic_mem_start, size);
  if (best_mem_block != NULL_POINTER) {
    best_mem_block->size = best_mem_block->size - size - DYNAMIC_MEM_NODE_SIZE;
    dynamic_mem_node_t* mem_node_allocate = (dynamic_mem_node_t*) ((unsigned char*) best_mem_block + DYNAMIC_MEM_NODE_SIZE + best_mem_block->size);
    mem_node_allocate->size = size;
    mem_node_allocate->used = 1;
    mem_node_allocate->next = best_mem_block->next;
    mem_node_allocate->prev = best_mem_block;
    
    if (best_mem_block->next != NULL_POINTER) {
      best_mem_block->next->prev = mem_node_allocate;
    }
    best_mem_block->next = mem_node_allocate;
    
    return (void*) ((unsigned char*) mem_node_allocate + DYNAMIC_MEM_NODE_SIZE);
  }
  
  return NULL_POINTER;
}

void* merge_next_node_into_current(dynamic_mem_node_t* current_mem_node) {
  dynamic_mem_node_t* next_mem_node = current_mem_node->next;
  if (next_mem_node != NULL_POINTER && !next_mem_node->used) {
    current_mem_node->size += next_mem_node->size + DYNAMIC_MEM_NODE_SIZE;
    current_mem_node->next = next_mem_node->next;
    
    if (current_mem_node->next != NULL_POINTER) {
      current_mem_node->next->prev = current_mem_node;
    }
  }
  
  return current_mem_node;
}

void* merge_current_node_into_previous(dynamic_mem_node_t* current_mem_node) {
  dynamic_mem_node_t* prev_mem_node = current_mem_node->prev;
  if (prev_mem_node != NULL_POINTER && !prev_mem_node->used) {
    prev_mem_node->size += current_mem_node->size + DYNAMIC_MEM_NODE_SIZE;
    prev_mem_node->next = current_mem_node->next;
    
    if (current_mem_node->next != NULL_POINTER) {
      current_mem_node->next->prev = prev_mem_node;
    }
  }
}

void mem_free(void* p) {
  if (p == NULL_POINTER) {
    return;
  }
  
  dynamic_mem_node_t* current_mem_node = (dynamic_mem_node_t*) ((unsigned char*) p - DYNAMIC_MEM_NODE_SIZE);
  if (current_mem_node == NULL_POINTER) {
    return;
  }
  
  current_mem_node->used = 0;
  current_mem_node = merge_next_node_into_current(current_mem_node);
  merge_current_node_into_previous(current_mem_node);
}

void* alloc(int n) {
  int* ptr = (int*) mem_alloc(n * sizeof(int));
  if (ptr == NULL_POINTER) {
    print_string("Memory not allocated.\n", VGA_RED);
  }
  
  return ptr;
}
