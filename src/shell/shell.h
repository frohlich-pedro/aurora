#pragma once

#include "../drivers/display.h"
#include "bin/commands.h"

typedef struct {
  const char* name;
  void (*func)(const char*);
  const char* definition;
} shell_command_t;

static const shell_command_t commands[] = {
  {"help", help, "shows available commands"},
  {"exit", exit, "quit the kernel"},
  {"clear", clear, "clears screen"},
  {"meminfo", meminfo, "shows memory info"},
  {"time", time, "shows current time (HH:MM:SS)"},
  {"date", date, "shows current date (DD/MM/YYYY)"},
  {0, 0, 0}
};

void execute_command(const char* input);
void print_shell_prompt(void);
void shell_init(void);
