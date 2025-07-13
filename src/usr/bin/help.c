#include "../shell.h"
#include "../../lib/util.h"

#define MAX_NAME_LENGTH 20

void help(const char* args) {
  (void)args;

  const shell_command_t* cmd = commands;
  print_string("Available commands:\n", VGA_LIGHT_GREEN);
  
  do {
    print_string("  ", VGA_WHITE);
    print_string(cmd->name, VGA_LIGHT_GREEN);
    
    int spaces = MAX_NAME_LENGTH - string_length(cmd->name);
    
    while (spaces-- > 0) {
      print_string(" ", VGA_WHITE);
    }
    
    print_string(" - ", VGA_WHITE);
    print_string(cmd->definition, VGA_WHITE);
    print_string("\n", VGA_WHITE);
    
    cmd++;
  } while (cmd->name != 0);
}