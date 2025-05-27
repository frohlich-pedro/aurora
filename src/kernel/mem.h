#pragma once

#define NULL_POINTER ((void*)0)

void memory_copy(unsigned char* source, unsigned char* dest, unsigned int nbytes);
void init_dynamic_mem();
void print_dynamic_node_size();
void print_dynamic_mem();
void* mem_alloc(unsigned int size);
void mem_free(void *p);
void* alloc(int n);
