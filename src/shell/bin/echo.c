#include "../shell.h"
#include "../../kernel/util.h"

void echo(const char* args) {
  if (compare_string(args, "") == 0) {
    print_string("\n");
    return;
  }

  if (*args == '"' || *args == '\'') {
    char quote = *args;
    const char* end = args + 1;
        
    while (*end && *end != quote) end++;
        
    if (*end == quote) {
      char temp = *(end + 1);
      *((char*)end) = '\0';
      print_string(args + 1);
      *((char*)end) = temp;
      print_string("\n");
      return;
    }
  }
    
  print_string(args);
  print_string("\n");
}
