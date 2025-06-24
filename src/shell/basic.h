#pragma once

#include "../lib/modules/display.h"
#include "../lib/util.h"
#include "../mm/mem.h"

#define MAX_VARS 26
#define MAX_FOR_LOOPS 10
#define MAX_GOSUB_STACK 10
#define MAX_INPUT_LENGTH 256

typedef struct BasicLine {
  int line_number;
  char* content;
  struct BasicLine* next;
} BasicLine;

typedef struct {
  int value;
  char is_set;
} Variable;

typedef struct {
  int line_number;
  int counter;
  int limit;
  int step;
  char var;
} ForLoop;

typedef struct {
  int return_line;
} GosubFrame;

extern unsigned char in_basic_mode;

void basic_init();
void basic_cleanup();
void basic_execute_command(const char* input);
void basic_run();
void basic_list();
void basic_erase();
void basic_add_line(int line_num, const char* content);
void basic_remove_line(int line_num);
BasicLine* basic_find_line(int line_num);

int basic_evaluate_expression(const char* expr);
void basic_set_variable(char var, int value);
int basic_get_variable(char var);
void basic_clear_variables();
int basic_parse_number(const char** str);
char* basic_find_next_token(char* str);
int basic_execute_line(BasicLine* line);
void basic_input_number(char var);
void basic_randomize();
int basic_random(int max);
