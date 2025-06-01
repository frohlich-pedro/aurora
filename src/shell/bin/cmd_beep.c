#include "commands.h"
#include "../../kernel/util.h"
#include "../../drivers/sound.h"

void cmd_beep(const char* args) {
  int freq = 440;
  int duration = 500;
  
  if (*args) {
    char args_copy[256];
    strcpy(args_copy, args);
    
    char* freq_str = args_copy;
    char* duration_str = 0;
    
    char* space = args_copy;
    while (*space && *space != ' ') space++;
    
    if (*space) {
      *space = '\0';
      duration_str = space + 1;
      
      while (*duration_str == ' ') duration_str++;
    }
    
    freq = atoi(freq_str);
    if (duration_str && *duration_str) {
      duration = atoi(duration_str);
    }
  }
  
  if (freq >= 20 && freq <= 20000 && duration >= 10 && duration <= 5000) {
    beep(freq, duration);
  }
}
