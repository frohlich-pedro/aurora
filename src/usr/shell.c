#include "shell.h"
#include "../lib/util.h"
#include "../mm/mem.h"
#include "bin/commands.h"
#include "basic.h"

void shell_init() {
  print_string("> ", VGA_LIGHT_GREEN);
}

void execute_command(const char* input) {
  if (in_basic_mode) {
    basic_execute_command(input);
    return;
  }
  
  while (*input == ' ') input++;
  
  if (!*input) {
    print_string("\n", VGA_WHITE);
    shell_init();
    return;
  }

  char cmd_buffer[256];
  strcpy(cmd_buffer, input);
  
  char* cmd = cmd_buffer;
  char* args = cmd_buffer;
  
  while (*args && *args != ' ') args++;
  
  if (*args) {
    *args = '\0';
    args++;
      
    while (*args == ' ') args++;
  } else {
    args = "";
  }

  const shell_command_t* current_cmd = commands;
  while (current_cmd->name) {
    if (compare_string(cmd, current_cmd->name) == 0) {
      current_cmd->func(args);
      shell_init();
      return;
    }
    current_cmd++;
  }

  print_string("Unknown command: ", VGA_LIGHT_RED);
  print_string(input, VGA_WHITE);
  print_string("\n", VGA_WHITE);
  shell_init();
}