#include "shell.h"
#include "../lib/modules/display.h"
#include "../lib/modules/keyboard.h"
#include "../lib/util.h"
#include "../mm/memory.h"

static char shell_buffer[SHELL_BUFFER_SIZE];
static unsigned int buffer_pos = 0;
static unsigned char mounted_drive = 0xFF;
static char current_path[256] = "/";

static struct shell_command commands[] = {
	{"help", "show available commands", help},
	{"exit", "halts the CPU", exit},
	{"cls", "clears screen buffer", cls},
	{NULL, NULL, NULL}
};

void shell_init() {
	buffer_pos = 0;
	*(shell_buffer + 0) = '\0';
	put_str("Welcome to Aurora!\n");
	put_str("Type 'help' for available commands.\n");
	put_str("$ ");
	show();
}

void shell_run() {
	char c = keyboard_getchar();
	if (c == '\n') {
		put_char('\n');
		*(shell_buffer + buffer_pos) = '\0';
		if (buffer_pos > 0) {
			shell_parse_command(shell_buffer);
		}
		buffer_pos = 0;
		*(shell_buffer + 0) = '\0';
		put_str("$ ");
		show();
		return;
	}
	if (c == '\b') {
		if (buffer_pos > 0) {
			buffer_pos--;
			*(shell_buffer + buffer_pos) = '\0';
			backspace();
			show();
		}
		return;
	}
	if (buffer_pos < SHELL_BUFFER_SIZE - 1 && c >= ' ' && c <= '~') {
		*(shell_buffer + buffer_pos++) = c;
		*(shell_buffer + buffer_pos) = '\0';
		put_char(c);
		show();
	}
}

void shell_parse_command(char* input) {
	char* argv[MAX_ARGS];
	int argc = 0;
	char* ptr = input;
	
skip_spaces:
	if (*ptr == ' ') {
		ptr++;
		goto skip_spaces;
	}
	
	if (*ptr == '\0') return;
	
parse_args:
	if (*ptr && argc < MAX_ARGS - 1) {
		*(argv + argc++) = ptr;
		
find_arg_end:
		if (*ptr && *ptr != ' ') {
			ptr++;
			goto find_arg_end;
		}
		
		if (*ptr) {
			*ptr++ = '\0';
skip_more_spaces:
			if (*ptr == ' ') {
				ptr++;
				goto skip_more_spaces;
			}
		}
		goto parse_args;
	}
	
	*(argv + argc) = NULL;
	
	if (argc > 0) {
		shell_execute_command(*(argv + 0), argc, argv);
	}
}

void shell_execute_command(char* name, int argc, char** argv) {
	struct shell_command* cmd = commands;
	
find_command:
	if (cmd->name) {
		if (strcmp(cmd->name, name) == 0) {
			cmd->function(argc, argv);
			return;
		}
		cmd++;
		goto find_command;
	}
	
	put_str("Command not found: ");
	put_str(name);
	put_str("\n");
	show();
}

void cls(int argc, char** argv) {
	(void)argc;
	(void)argv;
	clear();
}

void help(int argc, char** argv) {
	(void)argc;
	(void)argv;
	put_str("Available commands:\n");
	struct shell_command* cmd = commands;
	
list_commands:
	if (cmd->name) {
		put_str("  ");
		put_str(cmd->name);
		put_str(" - ");
		put_str(cmd->description);
		put_str("\n");
		cmd++;
		goto list_commands;
	}
}

void exit(int argc, char** argv) {
	(void)argc;
	(void)argv;
	put_str("Halting system...\n");
	show();
	
halt:
	__asm__ volatile ("hlt");
	goto halt;
}
