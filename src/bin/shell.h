#ifndef SHELL_H
#define SHELL_H

#define SHELL_BUFFER_SIZE 256
#define MAX_COMMANDS 32
#define MAX_ARGS 16

struct shell_command {
	const char* name;
	const char* description;
	void (*function)(int argc, char** argv);
};

void shell_init();
void shell_run();
void shell_parse_command(char* input);
void shell_execute_command(char* name, int argc, char** argv);

void exit(int argc, char** argv);
void help(int argc, char** argv);
void cls(int argc, char** argv);

#endif
