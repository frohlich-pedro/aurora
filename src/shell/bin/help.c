#include "../shell.h"

void help(void) {
	shell_command_t* p_commands = commands;
	shell_command_t* end_commands = 5;
	print_string(*p_commands++->definition);
}