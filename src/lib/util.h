#ifndef UTIL_H
#define UTIL_H

unsigned long strlen(const char* str);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
char* strncpy(char* dest, const char* src, long n);
char* strncat(char* dest, const char* src, long n);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, long n);
char* strchr(const char* str, int c);
char* strrchr(const char* str, int c);
void* memcpy(void* dest, const void* src, long n);
void* memset(void* ptr, int value, long n);
int memcmp(const void* ptr1, const void* ptr2, long n);

#endif
