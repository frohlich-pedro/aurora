#ifndef _STRING_H
#define _STRING_H

// Function pointer types for string operations
typedef void* (*string_mem_op_t)(void*, const void*, unsigned int);
typedef int (*string_cmp_op_t)(const void*, const void*, unsigned int);
typedef char* (*string_str_op_t)(char*, const char*, unsigned int);
typedef unsigned int (*string_len_op_t)(const char*);

// String operation IDs
enum string_ops {
    // Memory operations
    STRING_MEMCPY = 0,
    STRING_MEMMOVE,
    STRING_MEMSET,
    
    // String operations  
    STRING_STRCPY,
    STRING_STRNCPY,
    STRING_STRCAT,
    STRING_STRNCAT,
    STRING_STRLEN,
    STRING_STRCHR,
    STRING_STRRCHR,
    STRING_STRSTR,
    STRING_STRTOK,
    
    STRING_COUNT
};

// Special comparison operations (separate due to different return type)
enum string_cmp_ops {
    STRING_MEMCMP = 0,
    STRING_STRCMP,
    STRING_STRNCMP,
    STRING_STRSPN,
    STRING_STRCSPN,
    STRING_CMP_COUNT
};

// Operation arrays
extern string_mem_op_t string_mem_op_arr[STRING_COUNT];
extern string_cmp_op_t string_cmp_op_arr[STRING_CMP_COUNT];
extern string_str_op_t string_str_op_arr[STRING_COUNT];
extern string_len_op_t string_len_op_arr[1];

// Inline wrappers for memory operations
static inline void* memcpy(void* dest, const void* src, unsigned int n) {
    return string_mem_op_arr[STRING_MEMCPY](dest, src, n);
}

static inline void* memmove(void* dest, const void* src, unsigned int n) {
    return string_mem_op_arr[STRING_MEMMOVE](dest, src, n);
}

static inline void* memset(void* s, int c, unsigned int n) {
    return string_mem_op_arr[STRING_MEMSET](s, (const void*)(unsigned int)c, n);
}

static inline int memcmp(const void* s1, const void* s2, unsigned int n) {
    return string_cmp_op_arr[STRING_MEMCMP](s1, s2, n);
}

// Inline wrappers for string operations
static inline char* strcpy(char* dest, const char* src) {
    return string_str_op_arr[STRING_STRCPY](dest, src, 0);
}

static inline char* strncpy(char* dest, const char* src, unsigned int n) {
    return string_str_op_arr[STRING_STRNCPY](dest, src, n);
}

static inline char* strcat(char* dest, const char* src) {
    return string_str_op_arr[STRING_STRCAT](dest, src, 0);
}

static inline char* strncat(char* dest, const char* src, unsigned int n) {
    return string_str_op_arr[STRING_STRNCAT](dest, src, n);
}

static inline unsigned int strlen(const char* s) {
    return string_len_op_arr[0](s);
}

static inline int strcmp(const char* s1, const char* s2) {
    return string_cmp_op_arr[STRING_STRCMP](s1, s2, 0);
}

static inline int strncmp(const char* s1, const char* s2, unsigned int n) {
    return string_cmp_op_arr[STRING_STRNCMP](s1, s2, n);
}

static inline char* strchr(const char* s, int c) {
    return string_str_op_arr[STRING_STRCHR]((char*)s, (const char*)(unsigned int)c, 0);
}

static inline char* strrchr(const char* s, int c) {
    return string_str_op_arr[STRING_STRRCHR]((char*)s, (const char*)(unsigned int)c, 0);
}

static inline char* strstr(const char* haystack, const char* needle) {
    return string_str_op_arr[STRING_STRSTR]((char*)haystack, needle, 0);
}

static inline char* strtok(char* str, const char* delim) {
    return string_str_op_arr[STRING_STRTOK](str, delim, 0);
}

static inline unsigned int strspn(const char* s, const char* accept) {
    return string_cmp_op_arr[STRING_STRSPN](s, accept, 0);
}

static inline unsigned int strcspn(const char* s, const char* reject) {
    return string_cmp_op_arr[STRING_STRCSPN](s, reject, 0);
}

// Special case for strtoul (complex signature)
unsigned long strtoul(const char* str, char** endptr, int base);

#endif
