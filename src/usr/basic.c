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
  Variable* var = variables;
  Variable* end = variables + MAX_VARS;
clear_vars_loop:
  if (var >= end) goto clear_vars_done;
  var->value = 0;
  var->is_set = 0;
  var++;
  goto clear_vars_loop;
clear_vars_done:
  return;
}

void basic_set_variable(char var, int value) {
  if (var >= 'a' && var <= 'z') {
    Variable* v = variables + (var - 'a');
    v->value = value;
    v->is_set = 1;
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
  
parse_digits:
  if (*s < '0' || *s > '9') goto parse_done;
  result = result * 10 + (*s - '0');
  s++;
  goto parse_digits;
  
parse_done:
  *str = s;
  return result * sign;
}

char* basic_find_next_token(char* str) {
find_non_space:
  if (*str == '\0' || (*str != ' ' && *str != '\t')) goto find_space;
  str++;
  goto find_non_space;
  
find_space:
  if (*str == ' ' || *str == '\t') {
    str++;
    goto find_non_space;
  }
  return str;
}

int basic_evaluate_expression(const char* expr) {
  const char* s = expr;
  int result = 0;
  char operator = '+';
  int first_term = 1;
  
  while (*s == ' ') s++;
  
eval_loop:
  if (*s == '\0') goto eval_done;
  
  int value = 0;
  
  while (*s == ' ') s++;
  if (*s == '\0') goto eval_done;
  
  if (*s >= 'a' && *s <= 'z') {
    value = basic_get_variable(*s);
    s++;
    goto eval_apply_op;
  }
  else if (string_length(s) >= 4 && s[0] == 'r' && s[1] == 'n' && s[2] == 'd' && s[3] == '(') {
    s += 4;
    int max_val = basic_evaluate_expression(s);
    while (*s && *s != ')') s++;
    if (*s == ')') s++;
    value = basic_random(max_val);
    goto eval_apply_op;
  }
  else if (string_length(s) >= 4 && s[0] == 'a' && s[1] == 'b' && s[2] == 's' && s[3] == '(') {
    s += 4;
    int abs_val = basic_evaluate_expression(s);
    while (*s && *s != ')') s++;
    if (*s == ')') s++;
    value = (abs_val < 0) ? -abs_val : abs_val;
    goto eval_apply_op;
  }
  else if (*s == '(') {
    s++;
    value = basic_evaluate_expression(s);
    while (*s && *s != ')') s++;
    if (*s == ')') s++;
    goto eval_apply_op;
  }
  else if ((*s >= '0' && *s <= '9') || (*s == '-' && *(s+1) >= '0' && *(s+1) <= '9')) {
    value = basic_parse_number(&s);
    goto eval_apply_op;
  }
  else {
    s++;
    goto eval_loop;
  }
  
eval_apply_op:
  if (first_term) {
    result = value;
    first_term = 0;
    goto eval_next_op;
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
  
eval_next_op:
  while (*s == ' ') s++;
  if (*s == '+' || *s == '-' || *s == '*' || *s == '/') {
    operator = *s;
    s++;
    goto eval_loop;
  }
  
eval_done:
  return result;
}

void basic_input_number(char var) {
  print_string("? ", VGA_LIGHT_CYAN);
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
    return;
  }
  
  BasicLine* current = basic_lines;
insert_loop:
  if (current->next == NULL_POINTER || current->next->line_number > line_num) {
    new_line->next = current->next;
    current->next = new_line;
    return;
  }
  current = current->next;
  goto insert_loop;
}

void basic_remove_line(int line_num) {
  BasicLine* current = basic_lines;
  BasicLine* prev = NULL_POINTER;
  
remove_loop:
  if (current == NULL_POINTER) goto remove_done;
  if (current->line_number == line_num) goto remove_found;
  prev = current;
  current = current->next;
  goto remove_loop;
  
remove_found:
  if (prev == NULL_POINTER) {
    basic_lines = current->next;
  } else {
    prev->next = current->next;
  }
  mem_free(current->content);
  mem_free(current);
  
remove_done:
  return;
}

BasicLine* basic_find_line(int line_num) {
  BasicLine* current = basic_lines;
find_loop:
  if (current == NULL_POINTER) return NULL_POINTER;
  if (current->line_number == line_num) return current;
  current = current->next;
  goto find_loop;
}

void basic_list() {
  BasicLine* current = basic_lines;
list_loop:
  if (current == NULL_POINTER) return;
  print_int(current->line_number, VGA_LIGHT_GREEN);
  print_string(" ", VGA_WHITE);
  print_string(current->content, VGA_WHITE);
  print_nl();
  current = current->next;
  goto list_loop;
}

void basic_erase() {
  BasicLine* current = basic_lines;
erase_loop:
  if (current == NULL_POINTER) goto erase_done;
  BasicLine* next = current->next;
  mem_free(current->content);
  mem_free(current);
  current = next;
  goto erase_loop;
  
erase_done:
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
  
command_loop:
  if (*cmd == '\0' || *cmd == ' ' || i >= sizeof(command)-1) goto command_done;
  command[i++] = *cmd++;
  goto command_loop;
  
command_done:
  command[i] = '\0';
  
  while (*cmd == ' ') cmd++;
  
  if (compare_string(command, "rem") == 0) {
    return 0;
  }
  
  else if (compare_string(command, "print") == 0 || 
           compare_string(command, "println") == 0) {
    int suppress_newline = 0;
    
print_loop:
    if (*cmd == '\0') goto print_done;
    while (*cmd == ' ') cmd++;
    if (*cmd == '\0') goto print_done;
    
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
      if (*cmd == '\0') goto print_done;
      suppress_newline = 0;
    }
    else {
      char* expr_start = cmd;
      char* expr_end = cmd;
      int paren_count = 0;
      
expr_loop:
      if (*expr_end == '\0' || (*expr_end == ';' && paren_count == 0)) goto expr_done;
      if (*expr_end == '(') paren_count++;
      else if (*expr_end == ')') paren_count--;
      expr_end++;
      goto expr_loop;
      
expr_done:
      char saved_char = *expr_end;
      *expr_end = '\0';
      
      int value = basic_evaluate_expression(expr_start);
      print_int(value, VGA_WHITE);
      
      *expr_end = saved_char;
      cmd = expr_end;
    }
    goto print_loop;
    
print_done:
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
    
find_op_loop:
    if (*comparison_pos == '\0' || *comparison_pos == '=' || 
        *comparison_pos == '<' || *comparison_pos == '>') goto op_found;
    comparison_pos++;
    goto find_op_loop;
    
op_found:
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
find_then_loop:
    if (*then_pos == '\0' || 
        (then_pos[0] == 't' && then_pos[1] == 'h' && 
         then_pos[2] == 'e' && then_pos[3] == 'n')) goto then_found;
    then_pos++;
    goto find_then_loop;
    
then_found:
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
  
run_loop:
  if (current_line == NULL_POINTER) goto run_done;
  int result = basic_execute_line(current_line);
  
  if (result == -1) {
    goto run_done;
  } else if (result == 1) {
    goto run_loop;
  } else {
    current_line = current_line->next;
    goto run_loop;
  }
  
run_done:
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