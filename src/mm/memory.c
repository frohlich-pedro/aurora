#include "memory.h"

static struct block* heap_start = NULL;

void memory_init(void *start, unsigned int size) {
	heap_start = (struct block*)start;
	heap_start->size = size - sizeof(struct block);
	heap_start->free = 1;
	heap_start->next = NULL;
}

static struct block* find_block(unsigned int size) {
	struct block *current = heap_start;

loop:
	if (!current) {
		return NULL;
	}

	if (!(current->free && current->size >= size)) {
		current = current->next;
		goto loop;
	}

	if (current->size > size + sizeof(struct block) + 8) {
		struct block *new_block = (struct block*)((char*)current + 
			sizeof(struct block) + size);
		new_block->size = current->size - size - sizeof(struct block);
		new_block->free = 1;
		new_block->next = current->next;
		current->size = size;
		current->next = new_block;
	}

	current->free = 0;
	return current;
}

void* alloc(unsigned int size) {
	if (!size) {
		return NULL;
	}
	size = (size + 7) & ~7;
	struct block *_block = find_block(size);
	if (_block) {
		return (void*)((char*)_block + sizeof(struct block));
	}
	return NULL;
}

void free(void* ptr) {
	if (!ptr) {
		return;
	}
	struct block *_block = (struct block*)((char*)ptr - 
		sizeof(struct block));
	_block->free = 1;
	if (_block->next && _block->next->free) {
		_block->size += _block->next->size + sizeof(struct block);
		_block->next = _block->next->next;
	}
	struct block *current = heap_start;
loop:
	if (current) {
		if (current->next != _block) {
			current = current->next;
			goto loop;
		}
		if (current->free) {
			current->size += _block->size + sizeof(struct block);
			current->next = _block->next;
		}
	}
}
