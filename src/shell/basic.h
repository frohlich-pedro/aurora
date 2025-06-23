#pragma once

#include "../lib/modules/display.h"
#include "../lib/util.h"
#include "../mm/mem.h"

typedef struct BasicLine {
  int line_number;
  char* content;
  struct BasicLine* next;
} BasicLine;

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
