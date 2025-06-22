#include "syscall.h"
#include "idt.h"
#include "timer.h"
#include "rtc.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"

typedef struct {
  int pid;
  int status;
} process_t;

static process_t current_process = {1, 0};
static int next_pid = 2;

#define MAX_FDS 16
static int fd_table[MAX_FDS] = {0, 1, 2, -1};

static int (*syscall_table[])(registers_t*) = {
  [SYS_EXIT]    = (int(*)(registers_t*))syscall_exit,
  [SYS_WRITE]   = (int(*)(registers_t*))syscall_write,
  [SYS_READ]    = (int(*)(registers_t*))syscall_read,
  [SYS_OPEN]    = (int(*)(registers_t*))syscall_open,
  [SYS_CLOSE]   = (int(*)(registers_t*))syscall_close,
  [SYS_TIME]    = (int(*)(registers_t*))syscall_time,
  [SYS_SLEEP]   = (int(*)(registers_t*))syscall_sleep,
  [SYS_GETPID]  = (int(*)(registers_t*))syscall_getpid,
  [SYS_MALLOC]  = (int(*)(registers_t*))syscall_malloc,
  [SYS_FREE]    = (int(*)(registers_t*))syscall_free,
  [SYS_YIELD]   = (int(*)(registers_t*))syscall_yield
};

#define SYSCALL_COUNT (sizeof(syscall_table) / sizeof(syscall_table[0]))

void syscall_init() {
  set_idt_gate(SYSCALL_INT, (unsigned int)syscall_interrupt_handler);
}

extern void syscall_interrupt_handler();
asm(
    ".global syscall_interrupt_handler\n"
    "syscall_interrupt_handler:\n"
    "  pusha\n"
    "  mov %ds, %ax\n"
    "  push %eax\n"
    "  mov $0x10, %ax\n"
    "  mov %ax, %ds\n"
    "  mov %ax, %es\n"
    "  mov %ax, %fs\n"
    "  mov %ax, %gs\n"
    "  push %esp\n"
    "  call syscall_handler\n"
    "  add $4, %esp\n"
    "  pop %eax\n"
    "  mov %ax, %ds\n"
    "  mov %ax, %es\n"
    "  mov %ax, %fs\n"
    "  mov %ax, %gs\n"
    "  popa\n"
    "  iret\n"
);

void syscall_handler(registers_t* regs) {
  unsigned int syscall_num = regs->eax;
  
  if (syscall_num >= SYSCALL_COUNT || !syscall_table[syscall_num]) {
    regs->eax = SYSCALL_ERROR;
    return;
  }
  
  int result = syscall_table[syscall_num](regs);
  regs->eax = result;
}

int syscall_exit(int status) {
  current_process.status = status;
  print_string("Process exited with status: ", VGA_WHITE);
  char status_str[12];
  int_to_string(status, status_str);
  print_string(status_str, VGA_WHITE);
  print_nl();
    
  asm volatile("hlt");
  return SYSCALL_SUCCESS;
}

int syscall_write(int fd, const char* buffer, int count) {
  if (fd < 0 || fd >= MAX_FDS) {
    return SYSCALL_ERROR;
  }
  
  if (fd == 1 || fd == 2) {
    register int i asm("ecx") = 0;
    while (i < count && *(buffer + i)) {
      char str[2] = {*(buffer + i), '\0'};
      print_string(str, VGA_BLACK);
      i++;
    }
    return i;
  }
  
  return count;
}

int syscall_read(int fd, char* buffer, int count) {
  if (fd < 0 || fd >= MAX_FDS) {
    return SYSCALL_ERROR;
  }
  
  if (fd == 0) {
    while (!stdin_available()) {
      asm volatile("hlt");
    }
    
    int bytes_read = get_stdin_buffer(buffer, count);
    return bytes_read;
  }
  
  return 0;
}

int syscall_open(const char* filename, int flags) {
  register int i asm("ecx") = 3;
  while (i < MAX_FDS) {
    if (*(fd_table + i) == -1) {
      *(fd_table + i) = i;
      return i;
    }
    i++;
  }
  return SYSCALL_ERROR;
}

int syscall_close(int fd) {
  if (fd < 3 || fd >= MAX_FDS) {
    return SYSCALL_ERROR;
  }
  
  *(fd_table + fd) = -1;
  return SYSCALL_SUCCESS;
}

int syscall_time() {
  rtc_date_t date;
  rtc_read_date(&date);
  
  return date.hour * 3600 + date.minute * 60 + date.second;
}

int syscall_sleep(int milliseconds) {
  if (milliseconds <= 0) {
    return SYSCALL_ERROR;
  }
  
  sleep(milliseconds);
  return SYSCALL_SUCCESS;
}

int syscall_getpid() {
  return current_process.pid;
}

void* syscall_malloc(int size) {
  if (size <= 0) {
    return NULL_POINTER;
  }
  
  void* ptr = mem_alloc(size);
  return ptr;
}

int syscall_free(void* ptr) {
  if (ptr == NULL_POINTER) {
    return SYSCALL_ERROR;
  }
  
  mem_free(ptr);
  return SYSCALL_SUCCESS;
}

int syscall_yield() {
  return SYSCALL_SUCCESS;
}
