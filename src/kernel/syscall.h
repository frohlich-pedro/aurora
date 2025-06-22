#pragma once

#include "isr.h"
#include "../mm/mem.h"

#define SYS_EXIT    0
#define SYS_WRITE   1
#define SYS_READ    2
#define SYS_OPEN    3
#define SYS_CLOSE   4
#define SYS_TIME    5
#define SYS_SLEEP   6
#define SYS_GETPID  7
#define SYS_MALLOC  8
#define SYS_FREE    9
#define SYS_YIELD   10

#define SYSCALL_INT 0x80

#define SYSCALL_SUCCESS 0
#define SYSCALL_ERROR  -1

void syscall_init();
void syscall_handler(registers_t* regs);

int syscall_exit(int status);
int syscall_write(int fd, const char* buffer, int count);
int syscall_read(int fd, char* buffer, int count);
int syscall_open(const char* filename, int flags);
int syscall_close(int fd);
int syscall_time();
int syscall_sleep(int milliseconds);
int syscall_getpid();
void* syscall_malloc(int size);
int syscall_free(void* ptr);
int syscall_yield();

static inline int sys_exit(int status) {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_EXIT), "b"(status));
  return ret;
}

static inline int sys_write(int fd, const char* buffer, int count) {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_WRITE), "b"(fd), "c"(buffer), "d"(count));
  return ret;
}

static inline int sys_read(int fd, char* buffer, int count) {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_READ), "b"(fd), "c"(buffer), "d"(count));
  return ret;
}

static inline int sys_open(const char* filename, int flags) {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_OPEN), "b"(filename), "c"(flags));
  return ret;
}

static inline int sys_close(int fd) {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_CLOSE), "b"(fd));
  return ret;
}

static inline int sys_time() {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_TIME));
  return ret;
}

static inline int sys_sleep(int milliseconds) {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_SLEEP), "b"(milliseconds));
  return ret;
}

static inline int sys_getpid() {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_GETPID));
  return ret;
}

static inline void* sys_malloc(int size) {
  void* ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_MALLOC), "b"(size));
  return ret;
}

static inline int sys_free(void* ptr) {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_FREE), "b"(ptr));
  return ret;
}

static inline int sys_yield() {
  int ret;
  asm volatile ("int $0x80" : "=a"(ret) : "a"(SYS_YIELD));
  return ret;
}
