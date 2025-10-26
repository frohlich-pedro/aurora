#ifndef _MATH_H
#define _MATH_H

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Function pointer type for math operations
typedef int (*math_op_t)(void*);

// Math operation IDs
enum math_ops {
    MATH_ABS = 0,
    MATH_LABS,
    MATH_MIN,
    MATH_MAX,
    MATH_POW,
    MATH_SQRT,
    MATH_RAND,
    MATH_COUNT
};

extern math_op_t math_op_arr[MATH_COUNT];

// Inline wrappers
static inline int abs(int x) {
    return math_op_arr[MATH_ABS]((void*)(int)x);
}

static inline long labs(long x) {
    return (long)math_op_arr[MATH_LABS]((void*)x);
}

static inline int min(int a, int b) {
    void* args[] = {(void*)(int)a, (void*)(int)b};
    return math_op_arr[MATH_MIN](args);
}

static inline int max(int a, int b) {
    void* args[] = {(void*)(int)a, (void*)(int)b};
    return math_op_arr[MATH_MAX](args);
}

static inline int pow(int base, int exponent) {
    void* args[] = {(void*)(int)base, (void*)(int)exponent};
    return math_op_arr[MATH_POW](args);
}

static inline int sqrt(int x) {
    return math_op_arr[MATH_SQRT]((void*)(int)x);
}

// Special case for rand/srand (no function pointer needed for srand)
int rand(void);
void srand(unsigned int seed);

#endif
