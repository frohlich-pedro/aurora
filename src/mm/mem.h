#pragma once

#define NULL_POINTER ((void*)0)
#define DYNAMIC_MEM_TOTAL_SIZE 4*1024
#define DYNAMIC_MEM_NODE_SIZE sizeof(dynamic_mem_node_t)

typedef struct dynamic_mem_node {
  unsigned int size;
  unsigned char used;
  struct dynamic_mem_node* next;
  struct dynamic_mem_node* prev;
} dynamic_mem_node_t;

static unsigned char dynamic_mem_area[DYNAMIC_MEM_TOTAL_SIZE];
static dynamic_mem_node_t* dynamic_mem_start;

void memory_copy(unsigned char* source, unsigned char* dest, unsigned int nbytes);
void init_dynamic_mem();
void print_dynamic_node_size();
void print_dynamic_mem();
void* mem_alloc(unsigned int size);
void mem_free(void *p);
void* alloc(int n);