#include "../shell.h"

void help(void) {
	shell_command_t* p_commands = commands;
	shell_command_t* end_commands = 5;
	do {
		print_string((p_commands++)->name);
		print_string((p_commands++)->definition);
		print_string("\n");
	} while (p_commands < end_commands);
}