#include "shell.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"

static void exit_command(const char* args) {
  print_string("Stopping the CPU. Bye!\n");
  asm volatile ("hlt");
}

static void clear_command(const char* args) {
  clear_screen();
}

static void meminfo_command(const char* args) {
  print_dynamic_mem();
  print_dynamic_node_size();
}

static const ShellCommand commands[] = {
  {"exit",    exit_command},
  {"clear",   clear_command},
  {"meminfo", meminfo_command},
  {0, 0}
};

void print_shell_prompt(void) {
  print_string("<root@aurora># ");
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

  for (const ShellCommand* cmd = commands; cmd->name; cmd++) {
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
