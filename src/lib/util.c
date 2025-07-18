#include "util.h"
#include "modules/display.h"

void* memset(void* s, int c, unsigned int n) {
  unsigned char* p = s;
  while (n--) {
    *p++ = (unsigned char)c;
  }
  return s;
}

void* memcpy(void* dest, const void* src, unsigned int n) {
  unsigned char* d = dest;
  const unsigned char* s = src;
  while (n--) {
    *d++ = *s++;
  }
  return dest;
}

int memcmp(const void* s1, const void* s2, unsigned int n) {
  const unsigned char* p1 = s1;
  const unsigned char* p2 = s2;
  while (n--) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }
  return 0;
}

int string_length(const char* s) {
  const char* p = s;
  while (*p) ++p;
  return p - s;
}

void reverse(char* s) {
  char* start = s;
  char* end = s + string_length(s) - 1;
  char c;
  
  do {
    c = *start;
    *start = *end;
    *end = c;
    start++;
    end--;
  } while (start < end);
}

void int_to_string(int n, char* str) {
  char* orig = str;
  int sign;
  
  if ((sign = n) < 0) n = ~n + 1;
  
  do {
    *str++ = n % 10 + '0';
  } while ((n /= 10) > 0);
  
  if (sign < 0) *str++ = '-';
  *str = '\0';
  
  reverse(orig);
}

void append(char* s, char n) {
  char* end = s + string_length(s);
  *end++ = n;
  *end = '\0';
}

unsigned char backspace(char* s) {
  char* end = s + string_length(s);
  if (end > s) {
    *--end = '\0';
    return 1;
  }
  return 0;
}

int compare_string(const char* s1, const char* s2) {
  while (*s1 && *s1 == *s2) {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

int compare_string_length(const char* s1, const char* s2, int len) {
  do {
    s1++;
    s2++;
  } while (len-- && *s1 && *s1 == *s2);
  return (len == -1) ? 0 : *s1 - *s2;
}

void strcpy(char* dest, const char* src) {
  while (*src) {
    *dest++ = *src++;
  }
  *dest = '\0';
}
void strcat(char* dest, const char* src) {
  while (*dest) dest++;
  strcpy(dest, src);
}

int atoi(const char* str) {
  int result = 0;
  int sign = 1;
  
  if (*str == '-') {
    sign = -1;
    str++;
  }
  
  while (*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }
  
  return sign * result;
}

void print_int(int n, unsigned char color) {
  char str[16];
  int_to_string(n, str);
  print_string(str, color);
}

void strncpy(char* dest, const char* src, int n) {
  while (n > 0 && (*dest++ = *src++)) {
    n--;
  }

  while (n-- > 0) {
    *dest++ = '\0';
  }
}