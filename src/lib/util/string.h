#ifndef _STRING_H
#define _STRING_H

void* memcpy(void* dest, const void* src, unsigned int n);
void* memmove(void* dest, const void* src, unsigned int n);
void* memset(void* s, int c, unsigned int n);
int memcmp(const void* s1, const void* s2, unsigned int n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, unsigned int n);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, unsigned int n);
unsigned int strlen(const char* s);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, unsigned int n);
char* strchr(const char* s, int c);
char* strrchr(const char* s, int c);
char* strstr(const char* haystack, const char* needle);
char* strtok(char* str, const char* delim);
unsigned int strspn(const char* s, const char* accept);
unsigned int strcspn(const char* s, const char* reject);

#endif
