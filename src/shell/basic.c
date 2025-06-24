#include "basic.h"

static BasicLine* basic_lines = NULL_POINTER;
static Variable variables[MAX_VARS];
static GosubFrame gosub_stack[MAX_GOSUB_STACK];
static int gosub_stack_top = -1;
static int random_seed = 1;
static BasicLine* current_line = NULL_POINTER;

unsigned char in_basic_mode = 0;

void basic_init() {
  in_basic_mode = 1;
  basic_clear_variables();
  gosub_stack_top = -1;
  random_seed = 1;
  current_line = NULL_POINTER;
  
  print_string("AURORA-BASIC Interpreter v1.0\n", VGA_LIGHT_GREEN);
}

void basic_cleanup() {
  basic_erase();
  in_basic_mode = 0;
  print_string("> ", VGA_LIGHT_GREEN);
}

void basic_clear_variables() {
  for (int i = 0; i < MAX_VARS; i++) {
    variables[i].value = 0;
    variables[i].is_set = 0;
  }
}

void basic_set_variable(char var, int value) {
  if (var >= 'a' && var <= 'z') {
    variables[var - 'a'].value = value;
    variables[var - 'a'].is_set = 1;
  }
}

int basic_get_variable(char var) {
  if (var >= 'a' && var <= 'z') {
    return variables[var - 'a'].value;
  }
  return 0;
}

void basic_randomize() {
  random_seed = 12345;
}

int basic_random(int max) {
  if (max <= 0) return 0;
  random_seed = (random_seed * 1103515245 + 12345) & 0x7fffffff;
  return (random_seed % max) + 1;
}

int basic_parse_number(const char** str) {
  const char* s = *str;
  int result = 0;
  int sign = 1;
  
  while (*s == ' ') s++;
  
  if (*s == '-') {
    sign = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }
  
  while (*s >= '0' && *s <= '9') {
    result = result * 10 + (*s - '0');
    s++;
  }
  
  *str = s;
  return result * sign;
}

char* basic_find_next_token(char* str) {
  while (*str && *str != ' ' && *str != '\t') str++;
  while (*str == ' ' || *str == '\t') str++;
  return str;
}

int basic_evaluate_expression(const char* expr) {
  const char* s = expr;
  int result = 0;
  char operator = '+';
  int first_term = 1;
  
  while (*s == ' ') s++;
  
  while (*s) {
    int value = 0;
    
    while (*s == ' ') s++;
    if (*s == '\0') break;
    
    if (*s >= 'a' && *s <= 'z') {
      value = basic_get_variable(*s);
      s++;
    }
    else if (string_length(s) >= 4 && s[0] == 'r' && s[1] == 'n' && s[2] == 'd' && s[3] == '(') {
      s += 4;
      int max_val = basic_evaluate_expression(s);
      while (*s && *s != ')') s++;
      if (*s == ')') s++;
      value = basic_random(max_val);
    }
    else if (string_length(s) >= 4 && s[0] == 'a' && s[1] == 'b' && s[2] == 's' && s[3] == '(') {
      s += 4;
      int abs_val = basic_evaluate_expression(s);
      while (*s && *s != ')') s++;
      if (*s == ')') s++;
      value = (abs_val < 0) ? -abs_val : abs_val;
    }
    else if (*s == '(') {
      s++;
      value = basic_evaluate_expression(s);
      while (*s && *s != ')') s++;
      if (*s == ')') s++;
    }
    else if ((*s >= '0' && *s <= '9') || (*s == '-' && *(s+1) >= '0' && *(s+1) <= '9')) {
      value = basic_parse_number(&s);
    }
    else {
      s++;
      continue;
    }
    
    if (first_term) {
      result = value;
      first_term = 0;
    } else {
      switch (operator) {
        case '+': result += value; break;
        case '-': result -= value; break;
        case '*': result *= value; break;
        case '/': 
          if (value != 0) {
            result /= value;
          } else {
            print_string("Division by zero error\n", VGA_LIGHT_RED);
            return 0;
          }
          break;
      }
    }
    
    while (*s == ' ') s++;
    if (*s == '+' || *s == '-' || *s == '*' || *s == '/') {
      operator = *s;
      s++;
    } else {
      break;
    }
  }
  
  return result;
}

void basic_input_number(char var) {
  print_string("? ", VGA_LIGHT_CYAN);
  
  char input_buffer[16];
  int buffer_pos = 0;
  
  basic_set_variable(var, 42);
  print_string("42\n", VGA_WHITE);
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
  basic_clear_variables();
  gosub_stack_top = -1;
  print_string("All lines and variables cleared\n", VGA_LIGHT_GREEN);
}

int basic_execute_line(BasicLine* line) {
  char* cmd = line->content;
  char command[16];
  int i = 0;
  
  while (*cmd == ' ') cmd++;
  
  while (*cmd && *cmd != ' ' && i < sizeof(command)-1) {
    *(command + i++) = *cmd++;
  }
  *(command + i) = '\0';
  
  while (*cmd == ' ') cmd++;
  
  if (compare_string(command, "rem") == 0) {
    return 0;
  }
  
else if (compare_string(command, "print") == 0 || 
         compare_string(command, "println") == 0) {
  
  int suppress_newline = 0;
  
  while (*cmd) {
    while (*cmd == ' ') cmd++;
    if (!*cmd) break;
    
    if (*cmd == '"') {
      cmd++;
      char* text_start = cmd;
      while (*cmd && *cmd != '"') cmd++;
      if (*cmd == '"') {
        *cmd = '\0';
        print_string(text_start, VGA_WHITE);
        *cmd = '"';
        cmd++;
      }
    }
    else if (*cmd == ';') {
      suppress_newline = 1;
      cmd++;
      while (*cmd == ' ') cmd++;
      
      if (!*cmd) break;
      
      suppress_newline = 0;
    }
    else {
      char* expr_start = cmd;
      char* expr_end = cmd;
      int paren_count = 0;
      
      while (*expr_end && (*expr_end != ';' || paren_count > 0)) {
        if (*expr_end == '(') paren_count++;
        else if (*expr_end == ')') paren_count--;
        expr_end++;
      }
      
      char saved_char = *expr_end;
      *expr_end = '\0';
      
      int value = basic_evaluate_expression(expr_start);
      print_int(value, VGA_WHITE);
      
      *expr_end = saved_char;
      cmd = expr_end;
    }
  }
  
  if (!suppress_newline) {
    print_nl();
  }
  return 0;
}
  else if (compare_string(command, "let") == 0) {
    if (*cmd >= 'a' && *cmd <= 'z') {
      char var = *cmd;
      cmd++;
      while (*cmd == ' ') cmd++;
      if (*cmd == '=') {
        cmd++;
        while (*cmd == ' ') cmd++;
        int value = basic_evaluate_expression(cmd);
        basic_set_variable(var, value);
      }
    }
    return 0;
  }
  
  else if (command[0] >= 'a' && command[0] <= 'z' && command[1] == '\0') {
    char var = command[0];
    while (*cmd == ' ') cmd++;
    if (*cmd == '=') {
      cmd++;
      while (*cmd == ' ') cmd++;
      int value = basic_evaluate_expression(cmd);
      basic_set_variable(var, value);
    }
    return 0;
  }
  
  else if (compare_string(command, "if") == 0) {
    char* left_start = cmd;
    char* comparison_pos = cmd;
    
    while (*comparison_pos && *comparison_pos != '=' && *comparison_pos != '<' && *comparison_pos != '>') {
      comparison_pos++;
    }
    
    if (*comparison_pos == '\0') {
      print_string("if: No comparison operator found\n", VGA_LIGHT_RED);
      return -1;
    }
    
    char left_expr[64];
    int left_len = comparison_pos - left_start;
    strncpy(left_expr, left_start, left_len);
    left_expr[left_len] = '\0';
    
    char op = *comparison_pos;
    comparison_pos++;
    if (op == '<' && *comparison_pos == '>') {
      op = '!';
      comparison_pos++;
    }
    
    while (*comparison_pos == ' ') comparison_pos++;
    char* right_start = comparison_pos;
    
    char* then_pos = right_start;
    while (*then_pos && !(then_pos[0] == 't' && then_pos[1] == 'h' && 
                          then_pos[2] == 'e' && then_pos[3] == 'n')) {
      then_pos++;
    }
    
    char right_expr[64];
    int right_len = then_pos - right_start;
    strncpy(right_expr, right_start, right_len);
    right_expr[right_len] = '\0';
    
    int left = basic_evaluate_expression(left_expr);
    int right = basic_evaluate_expression(right_expr);
    
    cmd = then_pos;
    if (*cmd) cmd += 4;
    while (*cmd == ' ') cmd++;
    
    int condition = 0;
    switch (op) {
      case '=': condition = (left == right); break;
      case '<': condition = (left < right); break;
      case '>': condition = (left > right); break;
      case '!': condition = (left != right); break;
    }
    
    if (condition) {
      if (*cmd >= '0' && *cmd <= '9') {
        int line_num = atoi(cmd);
        BasicLine* target = basic_find_line(line_num);
        if (target) {
          current_line = target;
          return 1;
        }
      } else {
        BasicLine temp_line;
        temp_line.content = cmd;
        temp_line.line_number = line->line_number;
        basic_execute_line(&temp_line);
      }
    }
    return 0;
  }
  
  else if (compare_string(command, "gosub") == 0) {
    if (gosub_stack_top >= MAX_GOSUB_STACK - 1) {
      print_string("gosub stack overflow\n", VGA_LIGHT_RED);
      return -1;
    }
    
    int line_num = atoi(cmd);
    BasicLine* target = basic_find_line(line_num);
    if (target) {
      gosub_stack_top++;
      gosub_stack[gosub_stack_top].return_line = line->line_number;
      current_line = target;
      return 1;
    } else {
      print_string("gosub: Line not found\n", VGA_LIGHT_RED);
      return -1;
    }
  }
  
  else if (compare_string(command, "return") == 0) {
    if (gosub_stack_top < 0) {
      print_string("return without gosub\n", VGA_LIGHT_RED);
      return -1;
    }
    
    int return_line = gosub_stack[gosub_stack_top].return_line;
    gosub_stack_top--;
    
    BasicLine* target = basic_find_line(return_line);
    if (target && target->next) {
      current_line = target->next;
      return 1;
    }
    return 0;
  }
  
  else if (compare_string(command, "input") == 0) {
    if (*cmd >= 'a' && *cmd <= 'z') {
      basic_input_number(*cmd);
    }
    return 0;
  }
  
  else if (compare_string(command, "randomize") == 0) {
    basic_randomize();
    return 0;
  }
  
  else if (compare_string(command, "goto") == 0) {
    int line_num = atoi(cmd);
    BasicLine* target = basic_find_line(line_num);
    if (target) {
      current_line = target;
      return 1;
    } else {
      print_string("goto: Line not found\n", VGA_LIGHT_RED);
      return -1;
    }
  }
  
  else if (compare_string(command, "end") == 0) {
    return -1;
  }
  
  else {
    print_string("Unknown statement: ", VGA_LIGHT_RED);
    print_string(command, VGA_WHITE);
    print_nl();
  }
  
  return 0;
}

void basic_run() {
  basic_clear_variables();
  gosub_stack_top = -1;
  current_line = basic_lines;
  
  while (current_line != NULL_POINTER) {
    int result = basic_execute_line(current_line);
    
    if (result == -1) {
      break;
    } else if (result == 1) {
      continue;
    } else {
      current_line = current_line->next;
    }
  }
  
  print_string("Program ended\n", VGA_LIGHT_GREEN);
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
