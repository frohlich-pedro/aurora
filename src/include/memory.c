#include "memory.h"

void memory_copy(char* dest, char* src, unsigned int bytes) {
  unsigned int* d = (unsigned int*)dest;
  unsigned int* s = (unsigned int*)src;

  int i;
  for (i = 0; i < bytes / 4; i++) {
    *(d + i) = *(s + i);
  }

  for (i = (bytes & ~3); i < bytes; i++) {
    *(dest + i) = *(src + i);
  }
}
