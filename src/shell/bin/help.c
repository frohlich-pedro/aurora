#include "../shell.h"

void help(const char* args) {
  (void)args;

  const shell_command_t* cmd = commands;
  asm volatile ("cli");
  print_string("Available commands:\n", VGA_WHITE);
  do {
    print_string("  ", VGA_WHITE);
    print_string(cmd->name, VGA_WHITE);
    print_string(" - ", VGA_WHITE);
    print_string(cmd->definition, VGA_WHITE);
    print_string("\n", VGA_WHITE);
    cmd++;
  } while (cmd->name != 0);
  asm volatile ("sti");
}
