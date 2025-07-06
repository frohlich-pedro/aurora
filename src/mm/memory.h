#ifndef MEMORY_H
#define MEMORY_H

#define NULL (void*)0

#define HEAP_START 0x200000
#define HEAP_SIZE  0x800000

struct block {
	unsigned int size;
	int free;
	struct block *next;
};

void memory_init(void *start, unsigned int size);
void* alloc(unsigned int size);
void free(void* ptr);

#endif
