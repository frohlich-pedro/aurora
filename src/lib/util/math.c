#include "math.h"

static unsigned int rand_seed = 1;

static int abs_op(void* arg) {
    int x = (int)(void*)arg;
    return x < 0 ? -x : x;
}

static int labs_op(void* arg) {
    long x = (long)(void*)arg;
    return (int)(x < 0 ? -x : x);
}

static int min_op(void* arg) {
    void** args = (void**)arg;
    int a = (int)(void*)args[0];
    int b = (int)(void*)args[1];
    return a < b ? a : b;
}

static int max_op(void* arg) {
    void** args = (void**)arg;
    int a = (int)(void*)args[0];
    int b = (int)(void*)args[1];
    return a > b ? a : b;
}

static int pow_op(void* arg) {
    void** args = (void**)arg;
    int base = (int)(void*)args[0];
    int exponent = (int)(void*)args[1];
    int result = 1;
    
    if (exponent < 0) {
        return 0;
    }
    
    while (exponent > 0) {
        result *= base;
        exponent--;
    }
    
    return result;
}

static int sqrt_op(void* arg) {
    int x = (int)(void*)arg;
    
    if (x < 0) {
        return 0;
    }
    if (x == 0 || x == 1) {
        return x;
    }
    
    int start = 1, end = x, result = 0;
    
    while (start <= end) {
        int mid = (start + end) / 2;
        
        if (mid * mid == x) {
            return mid;
        }
        
        if (mid * mid < x) {
            start = mid + 1;
            result = mid;
        } else {
            end = mid - 1;
        }
    }
    
    return result;
}

static int rand_op(void* arg) {
    (void)arg;
    rand_seed = rand_seed * 1103515245 + 12345;
    return (unsigned int)(rand_seed / 65536) % 32768;
}

int rand(void) {
    return rand_op((void*)0);
}

void srand(unsigned int seed) {
    rand_seed = seed;
}

math_op_t math_op_arr[MATH_COUNT] = {
    abs_op,
    labs_op,
    min_op,
    max_op,
    pow_op,
    sqrt_op,
    rand_op
};
