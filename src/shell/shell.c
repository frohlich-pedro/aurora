#include "shell.h"

shell_t shell;
shell_command_t shell_commands[] = {
    {"help", shell_cmd_help, "Shows available commands"},
    {"clear", shell_cmd_clear, "Clears the screen"},
    {"echo", shell_cmd_echo, "Echoes arguments back to the screen"}
};

const int shell_command_count = sizeof(shell_commands) / sizeof(shell_command_t);

static void init_op(shell_t* sh, void* arg) {
    (void)arg;

    sh->buffer_pos = 0;
    sh->buffer[0] = '\0';
    sh->argc = 0;
    sh->flags = 0;

    void** argv_p = (void**)sh->argv;
    void** argv_e = argv_p + SHELL_MAX_ARGS;

    while (argv_p < argv_e) {
        *argv_p++ = (void*)0;
    }
}

static void process_input_op(shell_t* sh, void* arg) {
    char input = (char)(unsigned int)arg;
    
    switch (input) {
        case '\n':
            print_char('\n', WHITE);
            shell_execute_command();
            break;
            
        case '\b':
            if (sh->buffer_pos > 0) {
                sh->buffer_pos--;
                sh->buffer[sh->buffer_pos] = '\0';
                print_char('\b', WHITE);
                print_char(' ', WHITE);
                print_char('\b', WHITE);
            }
            break;
            
        default:
            if (sh->buffer_pos < SHELL_BUFFER_SIZE - 1) {
                sh->buffer[sh->buffer_pos] = input;
                sh->buffer_pos++;
                sh->buffer[sh->buffer_pos] = '\0';
            }
            break;
    }
}

static void execute_command_op(shell_t* sh, void* arg) {
    (void)arg;

    if (sh->buffer_pos == 0) {
        shell_print_prompt();
        return;
    }

    shell_op_arr[SHELL_OP_PARSE_ARGS](sh, (void*)0);

    int command_found = 0;
    shell_command_t* cmd_p = shell_commands;
    shell_command_t* cmd_e = cmd_p + shell_command_count;

    while (cmd_p < cmd_e) {
        if (strcmp(sh->argv[0], cmd_p->name) == 0) {
            cmd_p->func(sh->argc, sh->argv);
            command_found = 1;
            break;
        }
        cmd_p++;
    }

    if (!command_found && sh->argc > 0) {
        print("Command not found: ", LIGHT_RED);
        print(sh->argv[0], WHITE);
        print("\n", WHITE);
    }

    shell_clear_buffer();
    shell_print_prompt();
}

static void parse_args_op(shell_t* sh, void* arg) {
  (void)arg;
  
  sh->argc = 0;
  char* token = sh->buffer;
  char* end = sh->buffer + sh->buffer_pos;
  
  while (token < end && *token == ' ') {
    token++;
  }
  
  while (token < end && sh->argc < SHELL_MAX_ARGS - 1) {
    sh->argv[sh->argc++] = token;
    
    while (token < end && *token != ' ') {
      token++;
    }
    
    if (token < end) {
      *token++ = '\0';
      
      while (token < end && *token == ' ') {
        token++;
      }
    }
  }
  
  sh->argv[sh->argc] = (void*)0;
}

static void print_prompt_op(shell_t* sh, void* arg) {
  (void)arg;
  (void)sh;
  
  print(SHELL_PROMPT, LIGHT_GREEN);
}

static void clear_buffer_op(shell_t* sh, void* arg) {
    (void)arg;

    sh->buffer_pos = 0;
    sh->buffer[0] = '\0';
    sh->argc = 0;

    void** argv_p = (void**)sh->argv;
    void** argv_e = argv_p + SHELL_MAX_ARGS;

    while (argv_p < argv_e) {
        *argv_p++ = (void*)0;
    }
}

shell_op_t shell_op_arr[SHELL_OP_COUNT] = {
    init_op,
    process_input_op,
    execute_command_op,
    parse_args_op,
    print_prompt_op,
    clear_buffer_op
};

int shell_cmd_help(int argc, char** argv) {
    (void)argc;
    (void)argv;

    print("Available commands:\n", LIGHT_CYAN);

    shell_command_t* cmd_p = shell_commands;
    shell_command_t* cmd_e = cmd_p + shell_command_count;

    while (cmd_p < cmd_e) {
        print(" - ", WHITE);
        print(cmd_p->name, LIGHT_GREEN);
        print(":\t", WHITE);
        print(cmd_p->description, WHITE);
        print("\n", WHITE);
        cmd_p++;
    }

    return 0;
}

int shell_cmd_clear(int argc, char** argv) {
  (void)argc;
  (void)argv;
  
  clear_screen();
  return 0;
}

int shell_cmd_echo(int argc, char** argv) {
    char** arg_p = argv + 1;
    char** arg_e = argv + argc;

    while (arg_p < arg_e) {
        print(*arg_p, WHITE);
        if (arg_p + 1 < arg_e) {
            print(" ", WHITE);
        }
        arg_p++;
    }

    print("\n", WHITE);
    return 0;
}
