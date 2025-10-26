#ifndef SHELL_H
#define SHELL_H

#include "../lib/display.h"
#include "../lib/keyboard.h"
#include "../lib/util/string.h"

#define SHELL_BUFFER_SIZE 256
#define SHELL_MAX_ARGS 16
#define SHELL_PROMPT "> "

typedef int (*shell_command_func_t)(int argc, char** argv);

typedef struct {
  const char* name;
  shell_command_func_t func;
  const char* description;
} shell_command_t;

typedef struct {
  char buffer[SHELL_BUFFER_SIZE];
  unsigned int buffer_pos;
  char* argv[SHELL_MAX_ARGS];
  int argc;
  unsigned char flags;
} shell_t;

typedef void (*shell_op_t)(shell_t*, void*);

extern shell_t shell;
extern shell_op_t shell_op_arr[];

enum shell_ops {
  SHELL_OP_INIT = 0,
  SHELL_OP_PROCESS_INPUT,
  SHELL_OP_EXECUTE_COMMAND,
  SHELL_OP_PARSE_ARGS,
  SHELL_OP_PRINT_PROMPT,
  SHELL_OP_CLEAR_BUFFER,
  SHELL_OP_COUNT
};

extern shell_command_t shell_commands[];
extern const int shell_command_count;

static inline void shell_init(void) {
  shell_op_arr[SHELL_OP_INIT](&shell, (void*)0);
}

static inline void shell_process_input(char input) {
  shell_op_arr[SHELL_OP_PROCESS_INPUT](&shell, (void*)(unsigned int)input);
}

static inline void shell_execute_command(void) {
  shell_op_arr[SHELL_OP_EXECUTE_COMMAND](&shell, (void*)0);
}

static inline void shell_print_prompt(void) {
  shell_op_arr[SHELL_OP_PRINT_PROMPT](&shell, (void*)0);
}

static inline void shell_clear_buffer(void) {
  shell_op_arr[SHELL_OP_CLEAR_BUFFER](&shell, (void*)0);
}

int shell_cmd_help(int argc, char** argv);
int shell_cmd_clear(int argc, char** argv);
int shell_cmd_echo(int argc, char** argv);
int shell_cmd_hexdump(int argc, char** argv);
int shell_cmd_basic(int argc, char** argv);

#endif
