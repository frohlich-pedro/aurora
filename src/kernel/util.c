int string_length(const char* s) {
  int i = 0;
  while (*(s + i) != '\0') ++i;
  return i;
}

void reverse(char* s) {
  int c, i, j;
  for (i = 0, j = string_length(s)-1; i < j; i++, j--) {
    c = *(s + i);
    *(s + i) = *(s + j);
    *(s + j) = c;
  }
}

void int_to_string(int n, char* str) {
  int i, sign;
  if ((sign = n) < 0) n = ~n + 1;

  i = 0;
  do {
    *(str + i++) = n % 10 + '0';
  } while ((n /= 10) > 0);

  if (sign < 0) *(str + i++) = '-';
  *(str + i) = '\0';

  reverse(str);
}

void append(char* s, char n) {
  int len = string_length(s);
  *(s + len) = n;
  *(s + len + 1) = '\0';
}

unsigned char backspace(char* s) {
  int len = string_length(s);
  if (len > 0) {
    *(s + len - 1) = '\0';
    return 1;
  }
  return 0;
}

int compare_string(const char* s1, const char* s2) {
  int i;
  for (i = 0; *(s1 + i) == *(s2 + i); i++) {
    if (*(s1 + i) == '\0') return 0;
  }
  return *(s1 + i) - *(s2 + i);
}
