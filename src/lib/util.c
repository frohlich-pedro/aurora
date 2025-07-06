#include "util.h"

unsigned long strlen(const char* str) {
	const char* p = str;
loop:
	if (*p++) goto loop;
	return (unsigned long)(p - str - 1);
}

char* strcpy(char* dest, const char* src) {
	char* d = dest;
loop:
	if ((*d++ = *src++)) goto loop;
	return dest;
}

char* strcat(char* dest, const char* src) {
	char* d = dest;
skip:
	if (*d) {
		d++;
		goto skip;
	}
loop:
	if ((*d++ = *src++)) goto loop;
	return dest;
}

char* strncpy(char* dest, const char* src, long n) {
	char* d = dest;
loop:
	if (n > 0) {
		*d++ = *src;
		if (*src) src++;
		n--;
		goto loop;
	}
	return dest;
}

char* strncat(char* dest, const char* src, long n) {
	char* d = dest;
skip:
	if (*d) {
		d++;
		goto skip;
	}
loop:
	if (n > 0 && *src) {
		*d++ = *src++;
		n--;
		goto loop;
	}
	*d = '\0';
	return dest;
}

int strcmp(const char* str1, const char* str2) {
loop:
	if (*str1 && *str2 && *str1 == *str2) {
		str1++;
		str2++;
		goto loop;
	}
	return *str1 - *str2;
}

int strncmp(const char* str1, const char* str2, long n) {
loop:
	if (n > 0 && *str1 && *str2 && *str1 == *str2) {
		str1++;
		str2++;
		n--;
		goto loop;
	}
	return (n == 0) ? 0 : (*str1 - *str2);
}

char* strchr(const char* str, int c) {
	char ch = (char)c;
loop:
	if (*str == ch) return (char*)str;
	if (*str) {
		str++;
		goto loop;
	}
	return 0;
}

char* strrchr(const char* str, int c) {
	char ch = (char)c;
	char* last = 0;
loop:
	if (*str == ch) last = (char*)str;
	if (*str) {
		str++;
		goto loop;
	}
	return last;
}

void* memcpy(void* dest, const void* src, long n) {
	char* d = (char*)dest;
	const char* s = (const char*)src;
loop:
	if (n > 0) {
		*d++ = *s++;
		n--;
		goto loop;
	}
	return dest;
}

void* memset(void* ptr, int value, long n) {
	char* p = (char*)ptr;
	char val = (char)value;
loop:
	if (n > 0) {
		*p++ = val;
		n--;
		goto loop;
	}
	return ptr;
}

int memcmp(const void* ptr1, const void* ptr2, long n) {
	const char* p1 = (const char*)ptr1;
	const char* p2 = (const char*)ptr2;
loop:
	if (n > 0) {
		if (*p1 != *p2) return *p1 - *p2;
		p1++;
		p2++;
		n--;
		goto loop;
	}
	return 0;
}
