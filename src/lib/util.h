#pragma once

#define low_16(address) (unsigned short)((address) & 0xFFFF)
#define high_16(address) (unsigned short)(((address) >> 16) & 0xFFFF)

int string_length(const char* s);
int compare_string(const char* s1, const char* s2);
int compare_string_length(const char* s1, const char* s2, int len);
void reverse(char* s);
void int_to_string(int n, char* str);
unsigned char backspace(char* s);
void append(char* s, char n);
void strcat(char* dest, const char* src);
void strcpy(char* dest, const char* src);
void strncpy(char* dest, const char* src, int n);
int atoi(const char* str);
void print_int(int n, unsigned char color);
void* memset(void* s, int c, unsigned int n);
void* memcpy(void* dest, const void* src, unsigned int n);
int memcmp(const void* s1, const void* s2, unsigned int n);
