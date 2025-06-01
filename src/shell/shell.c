#include "shell.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "bin/commands.h"

void print_shell_prompt() {
  print_string("> ");
}

void shell_init() {
  print_shell_prompt();
}

void execute_command(const char* input) {
  while (*input == ' ') input++;
  
  if (!*input) {
    print_string("\n");
    print_shell_prompt();
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
      print_shell_prompt();
      return;
    }
    current_cmd++;
  }

  print_string("Unknown command: ");
  print_string(input);
  print_string("\n");
  print_shell_prompt();
}
