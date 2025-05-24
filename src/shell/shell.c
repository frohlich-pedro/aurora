#include "shell.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "bin/commands.h"

void print_shell_prompt(void) {
  print_string("> ");
}

void shell_init(void) {
  print_shell_prompt();
}

void execute_command(const char* input) {
  if (!*input) {
    print_string("\n");
    print_shell_prompt();
    return;
  }

  const char* first_space = input;
  while (*first_space && *first_space != ' ') first_space++;
  
  int cmd_len = first_space - input;
  const shell_command_t* cmd = commands;

  while (cmd->name) {
    if (compare_string_length(input, cmd->name, cmd_len) == 0) {
      const char* args = (*first_space) ? first_space + 1 : "";
      cmd->func(args);
      print_shell_prompt();
      return;
    }
    cmd++;
  }

  print_string("Unknown command: ");
  print_string(input);
  print_string("\n");
  print_shell_prompt();
}
