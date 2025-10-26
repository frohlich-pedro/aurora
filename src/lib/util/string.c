#include "string.h"

// Memory operations
static void* memcpy_op(void* dest, const void* src, unsigned int n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    unsigned char* d_end = d + n;

    while (d < d_end) {
        *d++ = *s++;
    }

    return dest;
}

static void* memmove_op(void* dest, const void* src, unsigned int n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    if (d < s) {
        unsigned char* d_end = d + n;
        while (d < d_end) {
            *d++ = *s++;
        }
    } else {
        unsigned char* d_end = d;
        d += n;
        s += n;
        while (d_end < d) {
            *(--d) = *(--s);
        }
    }

    return dest;
}

static void* memset_op(void* s, const void* c_ptr, unsigned int n) {
    unsigned char* p = (unsigned char*)s;
    unsigned char c = (unsigned char)(unsigned int)c_ptr;
    unsigned char* p_end = p + n;

    while (p < p_end) {
        *p++ = c;
    }

    return s;
}

// Comparison operations
static int memcmp_op(const void* s1, const void* s2, unsigned int n) {
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;
    
    while (n-- > 0) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    return 0;
}

static int strcmp_op(const void* s1_ptr, const void* s2_ptr, unsigned int unused) {
    (void)unused;
    const char* s1 = (const char*)s1_ptr;
    const char* s2 = (const char*)s2_ptr;
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

static int strncmp_op(const void* s1_ptr, const void* s2_ptr, unsigned int n) {
    const char* s1 = (const char*)s1_ptr;
    const char* s2 = (const char*)s2_ptr;
    
    if (n == 0) return 0;

    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }

    if (n == 0) return 0;
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

static int strspn_op(const void* s_ptr, const void* accept_ptr, unsigned int unused) {
    (void)unused;
    const char* s = (const char*)s_ptr;
    const char* accept = (const char*)accept_ptr;
    const char* p = s;
    while (*p && strchr(accept, *p)) {
        p++;
    }
    return (int)(p - s);
}

static int strcspn_op(const void* s_ptr, const void* reject_ptr, unsigned int unused) {
    (void)unused;
    const char* s = (const char*)s_ptr;
    const char* reject = (const char*)reject_ptr;
    const char* p = s;
    while (*p && !strchr(reject, *p)) {
        p++;
    }
    return (int)(p - s);
}

// String operations
static char* strcpy_op(char* dest, const char* src, unsigned int unused) {
    (void)unused;
    char* d = dest;
    while ((*d++ = *src++));
    return dest;
}

static char* strncpy_op(char* dest, const char* src, unsigned int n) {
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

static char* strcat_op(char* dest, const char* src, unsigned int unused) {
    (void)unused;
    char* d = dest;
    while (*d) d++;
    while ((*d++ = *src++));
    return dest;
}

static char* strncat_op(char* dest, const char* src, unsigned int n) {
    char* d = dest;
    while (*d) d++;

    const char* s = src;
    unsigned int copied = 0;

    while (*s && copied < n) {
        *d++ = *s++;
        copied++;
    }

    *d = '\0';

    return dest;
}

static unsigned int strlen_op(const char* s) {
    const char* p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static char* strchr_op(char* s_ptr, const char* c_ptr, unsigned int unused) {
    (void)unused;
    char* s = s_ptr;
    char c = (char)(unsigned int)c_ptr;
    
    while (*s) {
        if (*s == c) {
            return s;
        }
        s++;
    }
    return (void*)0;
}

static char* strrchr_op(char* s_ptr, const char* c_ptr, unsigned int unused) {
    (void)unused;
    char* s = s_ptr;
    char c = (char)(unsigned int)c_ptr;
    char* last = (void*)0;
    
    while (*s) {
        if (*s == c) {
            last = s;
        }
        s++;
    }
    return last;
}

static char* strstr_op(char* haystack_ptr, const char* needle_ptr, unsigned int unused) {
    (void)unused;
    char* haystack = haystack_ptr;
    const char* needle = needle_ptr;
    
    if (!*needle) return haystack;

    char* h = haystack;
    while (*h) {
        char* h2 = h;
        const char* n = needle;

        while (*h2 && *n && *h2 == *n) {
            h2++;
            n++;
        }

        if (!*n) return h;
        h++;
    }
    return (void*)0;
}

static char* strtok_save = (void*)0;

static char* strtok_op(char* str, const char* delim, unsigned int unused) {
    (void)unused;
    if (!str && !strtok_save) return (void*)0;

    if (str) {
        strtok_save = str;
    }

    // Skip leading delimiters
    char* start = strtok_save;
    while (*start && strchr(delim, *start)) {
        start++;
    }

    if (!*start) {
        strtok_save = (void*)0;
        return (void*)0;
    }

    // Find end of token
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

// Operation arrays
string_mem_op_t string_mem_op_arr[STRING_COUNT] = {
    memcpy_op,
    memmove_op,
    memset_op,
    // These slots are unused but kept for array size consistency
    (string_mem_op_t)0,
    (string_mem_op_t)0,
    (string_mem_op_t)0,
    (string_mem_op_t)0,
    (string_mem_op_t)0,
    (string_mem_op_t)0
};

string_cmp_op_t string_cmp_op_arr[STRING_CMP_COUNT] = {
    memcmp_op,
    strcmp_op,
    strncmp_op,
    strspn_op,
    strcspn_op
};

string_str_op_t string_str_op_arr[STRING_COUNT] = {
    strcpy_op,
    strncpy_op,
    strcat_op,
    strncat_op,
    (string_str_op_t)0,  // strlen has different signature
    strchr_op,
    strrchr_op,
    strstr_op,
    strtok_op
};

string_len_op_t string_len_op_arr[1] = {
    strlen_op
};

// strtoul remains unchanged due to complex signature
unsigned long strtoul(const char* str, char** endptr, int base) {
    const char* s = str;
    unsigned long result = 0;

    // Skip whitespace
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') {
        s++;
    }

    // Handle optional sign
    if (*s == '+') {
        s++;
    }

    if (base == 0) {
        if (*s == '0') {
            if (s[1] == 'x' || s[1] == 'X') {
                base = 16;
                s += 2;
            } else {
                base = 8;
                s++;
            }
        } else {
            base = 10;
        }
    } else if (base == 16) {
        if (*s == '0' && (s[1] == 'x' || s[1] == 'X')) {
            s += 2;
        }
    }

    while (*s) {
        int digit;

        if (*s >= '0' && *s <= '9') {
            digit = *s - '0';
        } else if (*s >= 'a' && *s <= 'z') {
            digit = *s - 'a' + 10;
        } else if (*s >= 'A' && *s <= 'Z') {
            digit = *s - 'A' + 10;
        } else {
            break;
        }

        if (digit >= base) {
            break;
        }

        // Check for overflow
        if (result > (~0UL - digit) / base) {
            // Overflow occurred
            result = ~0UL;
            break;
        }

        result = result * base + digit;
        s++;
    }

    if (endptr) {
        *endptr = (char*)s;
    }

    return result;
}
