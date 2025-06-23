#include "basic.h"

static BasicLine* basic_lines = NULL_POINTER;
unsigned char in_basic_mode = 0;

void basic_init() {
  in_basic_mode = 1;
  print_string("AURORA-BASIC Interpreter v0.0\n", VGA_LIGHT_GREEN);
  print_string("Type 'list' to view lines, 'run' to execute, 'erase' to clear, or 'exit' to quit\n", VGA_LIGHT_CYAN);
}

void basic_cleanup() {
  basic_erase();
  in_basic_mode = 0;
}

void basic_add_line(int line_num, const char* content) {
  basic_remove_line(line_num);
  
  BasicLine* new_line = (BasicLine*)mem_alloc(sizeof(BasicLine));
  new_line->line_number = line_num;
  new_line->content = (char*)mem_alloc(string_length(content) + 1);
  strcpy(new_line->content, content);
  new_line->next = NULL_POINTER;
  
  if (basic_lines == NULL_POINTER || basic_lines->line_number > line_num) {
    new_line->next = basic_lines;
    basic_lines = new_line;
  } else {
    BasicLine* current = basic_lines;
    while (current->next != NULL_POINTER && current->next->line_number < line_num) {
      current = current->next;
    }
    new_line->next = current->next;
    current->next = new_line;
  }
}

void basic_remove_line(int line_num) {
  BasicLine* current = basic_lines;
  BasicLine* prev = NULL_POINTER;
  
  while (current != NULL_POINTER && current->line_number != line_num) {
    prev = current;
    current = current->next;
  }
  
  if (current != NULL_POINTER) {
    if (prev == NULL_POINTER) {
      basic_lines = current->next;
    } else {
      prev->next = current->next;
    }
    mem_free(current->content);
    mem_free(current);
  }
}

BasicLine* basic_find_line(int line_num) {
  BasicLine* current = basic_lines;
  while (current != NULL_POINTER && current->line_number != line_num) {
    current = current->next;
  }
  return current;
}

void basic_list() {
  BasicLine* current = basic_lines;
  while (current != NULL_POINTER) {
    print_int(current->line_number, VGA_LIGHT_GREEN);
    print_string(" ", VGA_WHITE);
    print_string(current->content, VGA_WHITE);
    print_nl();
    current = current->next;
  }
}

void basic_erase() {
  BasicLine* current = basic_lines;
  while (current != NULL_POINTER) {
    BasicLine* next = current->next;
    mem_free(current->content);
    mem_free(current);
    current = next;
  }
  basic_lines = NULL_POINTER;
  print_string("All lines erased\n", VGA_LIGHT_GREEN);
}

void basic_run() {
  BasicLine* current = basic_lines;
  int loop_counter = 0;

  while (current != NULL_POINTER) {
    char* cmd = current->content;
    char command[16];
    int i = 0;
    
    while (*cmd && *cmd != ' ' && i < sizeof(command)-1) {
      *(command + i++) = *cmd++;
    }
    *(command + i) = '\0';

    if (compare_string(command, "print") == 0 || 
      compare_string(command, "println") == 0) {
      while (*cmd && *cmd != '"') cmd++;
      
      if (*cmd == '"') {
        cmd++;
        char* text_start = cmd;
        while (*cmd && *cmd != '"') cmd++;
        
        if (*cmd == '"') {
          *cmd = '\0';
          print_string(text_start, VGA_WHITE);
          *cmd = '"';
          
          if (compare_string(command, "println") == 0) {
            print_nl();
          }
        }
      }
      current = current->next;
    }
    else if (compare_string(command, "end") == 0) {
      break;
    }
    else if (compare_string(command, "goto") == 0) {
      char* num_str = cmd;
      while (*num_str == ' ') num_str++;
      int line_num = atoi(num_str);
      BasicLine* target = basic_find_line(line_num);
      if (target) {
        current = target;
        continue;
      } else {
        print_string("Error: Line ", VGA_LIGHT_RED);
        print_int(line_num, VGA_LIGHT_RED);
        print_string(" not found\n", VGA_LIGHT_RED);
        break;
      }
    } else {
      print_string("Unknown command: ", VGA_LIGHT_RED);
      print_string(command, VGA_WHITE);
      print_nl();
      current = current->next;
    }
  }
}

void basic_execute_command(const char* input) {
  while (*input == ' ') input++;
  
  if (*input >= '0' && *input <= '9') {
    int line_num = atoi(input);
    while (*input >= '0' && *input <= '9') input++;
    while (*input == ' ') input++;
    
    if (*input) {
      basic_add_line(line_num, input);
    } else {
      basic_remove_line(line_num);
    }
    print_string("> ", VGA_LIGHT_GREEN);
    return;
  }
  
  if (compare_string_length(input, "list", 4) == 0) {
    basic_list();
  } 
  else if (compare_string_length(input, "run", 3) == 0) {
    basic_run();
  }
  else if (compare_string_length(input, "erase", 5) == 0) {
    basic_erase();
  }
  else if (compare_string_length(input, "exit", 4) == 0) {
    print_string("Exiting BASIC mode\n", VGA_LIGHT_GREEN);
    basic_cleanup();
    return;
  } else {
    print_string("Unknown BASIC command: ", VGA_LIGHT_RED);
    print_string(input, VGA_WHITE);
    print_nl();
  }
  
  if (in_basic_mode) {
    print_string("> ", VGA_LIGHT_GREEN);
  }
}
