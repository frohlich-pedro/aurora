int string_length(const char* s) {
  const char* p = s;
  while (*p) ++p;
  return p - s;
}

void reverse(char* s) {
  char* start = s;
  char* end = s + string_length(s) - 1;
  char c;
  
  while (start < end) {
    c = *start;
    *start = *end;
    *end = c;
    start++;
    end--;
  }
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
  while (*s1 == *s2) {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

int compare_string_length(const char* s1, const char* s2, int len) {
  while (len-- && *s1 == *s2) {
    s1++;
    s2++;
  }
  return (len == -1) ? 0 : *s1 - *s2;
}
