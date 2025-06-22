#pragma once

#include "../lib/modules/display.h"
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
  {"beep", cmd_beep, "plays a beep - usage: beep [freq] [duration]"},
  {"echo", echo, "prints arguments to screen - usage: echo [text]"},
  {"fetch", fetch, "displays system information"},
  {0, 0, 0}
};

void execute_command(const char* input);
void print_shell_prompt();
void shell_init();
