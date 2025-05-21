#include "shell.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "bin/commands.h"

static const shell_command_t commands[] = {
  {"exit",    exit},
  {"clear",   clear},
  {"meminfo", meminfo},
  {"echo",    echo},
  {0, 0}
};

void print_shell_prompt(void) {
  print_string("<root@aurora:/># ");
}

void shell_init(void) {
  clear_screen();
  print_shell_prompt();
}

void execute_command(const char* input) {
  if (compare_string(input, "") == 0) {
    print_string("\n");
    print_shell_prompt();
    return;
  }

  const char* first_space = input;
  while (*first_space && *first_space != ' ') first_space++;
  int cmd_len = first_space - input;
  int command_found = 0;

  for (const shell_command_t* cmd = commands; cmd->name; cmd++) {
    if (compare_string_length(input, cmd->name, cmd_len) == 0) {
      const char* args = (*first_space) ? first_space + 1 : "";
      cmd->func(args);
      command_found = 1;
      break;
    }
  }

  if (!command_found) {
    print_string("Unknown command: ");
    print_string(input);
    print_string("\n");
  }

  print_shell_prompt();
}
