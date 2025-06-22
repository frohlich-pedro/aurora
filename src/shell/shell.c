#include "shell.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "bin/commands.h"

void shell_init() {
  print_string(" > ", VGA_BLACK);
}

void execute_command(const char* input) {
  while (*input == ' ') input++;
  
  if (!*input) {
    print_string("\n", VGA_BLACK);
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
  print_string(input, VGA_BLACK);
  print_string("\n", VGA_BLACK);
  shell_init();
}
