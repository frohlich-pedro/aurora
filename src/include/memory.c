void memoryCopy(char* dest, char* src, unsigned int bytes) {
  unsigned int *d = (unsigned int*)dest;
  unsigned int *s = (unsigned int*)src;

  for (int i = 0; i < bytes / 4; i++) {
    *(d + i) = *(s + i);
  }

  for (int i = (bytes & ~3); i < bytes; i++) {
    *(dest + i) = *(src + i);
  }
}
