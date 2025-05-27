#include "../shell.h"

void help(const char* args) {
  (void)args;

  const shell_command_t* cmd = commands;
  asm volatile ("cli");
  print_string("Available commands:\n");
  do {
    print_string("  ");
    print_string(cmd->name);
    print_string(" - ");
    print_string(cmd->definition);
    print_string("\n");
    cmd++;
  } while (cmd->name != 0);
  asm volatile ("sti");
}
