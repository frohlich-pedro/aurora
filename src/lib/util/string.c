#include "string.h"

void* memcpy(void* dest, const void* src, unsigned int n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    unsigned char* d_end = d + n;

    while (d < d_end) {
        *d++ = *s++;
    }
    return dest;
}

void* memmove(void* dest, const void* src, unsigned int n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    if (d < s) {
        unsigned char* d_end = d + n;
        while (d < d_end) {
            *d++ = *s++;
        }
    } else {
        unsigned char* d_end = d + n;
        const unsigned char* s_end = s + n;
        while (d_end > d) {
            *(--d_end) = *(--s_end);
        }
    }
    return dest;
}

void* memset(void* s, int c, unsigned int n) {
    unsigned char* p = (unsigned char*)s;
    unsigned char* p_end = p + n;

    while (p < p_end) {
        *p++ = (unsigned char)c;
    }
    return s;
}

int memcmp(const void* s1, const void* s2, unsigned int n) {
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;
    const unsigned char* p1_end = p1 + n;

    while (p1 < p1_end) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    return 0;
}

char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while ((*d++ = *src++));
    return dest;
}

char* strncpy(char* dest, const char* src, unsigned int n) {
    char* d = dest;
    const char* s = src;
    char* d_end = d + n;

    while (d < d_end && *s) {
        *d++ = *s++;
    }
    while (d < d_end) {
        *d++ = '\0';
    }
    return dest;
}

char* strcat(char* dest, const char* src) {
    char* d = dest;
    while (*d) d++;
    while ((*d++ = *src++));
    return dest;
}

char* strncat(char* dest, const char* src, unsigned int n) {
    char* d = dest;
    while (*d) d++;

    const char* s = src;
    char* d_start = d;
    unsigned int copied = 0;

    while (*s && copied < n) {
        *d++ = *s++;
        copied++;
    }
    *d = '\0';
    return dest;
}

unsigned int strlen(const char* s) {
    const char* p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, unsigned int n) {
    if (n == 0) return 0;

    while (--n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char* strchr(const char* s, int c) {
    while (*s) {
        if (*s == (char)c) {
            return (char*)s;
        }
        s++;
    }
    return (void*)0;
}

char* strrchr(const char* s, int c) {
    const char* last = (void*)0;
    while (*s) {
        if (*s == (char)c) {
            last = s;
        }
        s++;
    }
    return (char*)last;
}

char* strstr(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack;

    const char* h = haystack;
    while (*h) {
        const char* h2 = h;
        const char* n = needle;

        while (*h2 && *n && *h2 == *n) {
            h2++;
            n++;
        }

        if (!*n) return (char*)h;
        h++;
    }
    return (void*)0;
}

static char* strtok_save = (void*)0;

char* strtok(char* str, const char* delim) {
    if (!str && !strtok_save) return (void*)0;

    if (str) {
        strtok_save = str;
    }

    char* start = strtok_save;

    while (*start && strchr(delim, *start)) {
        start++;
    }

    if (!*start) {
        strtok_save = (void*)0;
        return (void*)0;
    }

    char* end = start;
    while (*end && !strchr(delim, *end)) {
        end++;
    }

    if (*end) {
        *end = '\0';
        strtok_save = end + 1;
    } else {
        strtok_save = (void*)0;
    }

    return start;
}

unsigned int strspn(const char* s, const char* accept) {
    const char* p = s;
    while (*p && strchr(accept, *p)) {
        p++;
    }
    return (unsigned int)(p - s);
}

unsigned int strcspn(const char* s, const char* reject) {
    const char* p = s;
    while (*p && !strchr(reject, *p)) {
        p++;
    }
    return (unsigned int)(p - s);
}
