#pragma once

#define low_16(address) (unsigned short)((address) & 0xFFFF)
#define high_16(address) (unsigned short)(((address) >> 16) & 0xFFFF)

int string_length(const char* s);
int compare_string(const char* s1, const char* s2);
void reverse(char* s);
void int_to_string(int n, char* str);
unsigned char backspace(char* s);
void append(char* s, char n);
