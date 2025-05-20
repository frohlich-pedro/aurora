#pragma once

#include "../drivers/display.h"

typedef struct {
  const char* name;
  void (*func)(const char*);
} ShellCommand;

void execute_command(const char* input);
void print_shell_prompt(void);
void shell_init(void);
