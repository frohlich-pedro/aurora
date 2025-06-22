#include "keyboard.h"
#include "../../kernel/ports.h"
#include "../../kernel/isr.h"
#include "display.h"
#include "../util.h"
#include "../../shell/shell.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT_MAKE 0x2A
#define RSHIFT_MAKE 0x36
#define LSHIFT_BREAK (LSHIFT_MAKE | 0x80)
#define RSHIFT_BREAK (RSHIFT_MAKE | 0x80)

static char key_buffer[256];
static unsigned int key_buffer_pos = 0;
static unsigned char shift_pressed = 0;
static unsigned char caps_lock = 0;

static char stdin_buffer[256];
static unsigned int stdin_pos = 0;
static unsigned char stdin_ready = 0;

#define SC_MAX 57

const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                        '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
                        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
                        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
                        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

const char shifted_sc_ascii[] = {'?', '?', '!', '@', '#', '$', '%', '^',
                               '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                               'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G',
                               'H', 'J', 'K', 'L', ':', '"', '~', '?', '|', 'Z', 'X', 'C', 'V',
                               'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};

static void keyboard_callback(registers_t* regs) {
  unsigned char scancode = port_byte_in(0x60);
  unsigned char is_break = scancode & 0x80;
  unsigned char make_code = scancode & 0x7F;

  if (make_code == LSHIFT_MAKE || make_code == RSHIFT_MAKE) {
    shift_pressed = !is_break;
    return;
  }

  if (is_break) return;

  if (make_code == BACKSPACE) {
    if (key_buffer_pos > 0) {
      key_buffer_pos--;
      print_backspace();
    }
    if (stdin_pos > 0 && !stdin_ready) {
      stdin_pos--;
    }
  } else if (make_code == ENTER) {
    print_nl();
    key_buffer[key_buffer_pos] = '\0';
    
    register int i asm("ecx") = 0;
    while (i < key_buffer_pos && i < sizeof(stdin_buffer) - 1) {
      stdin_buffer[i] = key_buffer[i];
      i++;
    }
    stdin_buffer[i] = '\n';
    stdin_pos = i + 1;
    stdin_ready = 1;
    
    execute_command(key_buffer);
    key_buffer_pos = 0;
  } else if (make_code == 0x3A) {
    caps_lock = !caps_lock;
  } else if (make_code == 0x39) {
    if (key_buffer_pos < sizeof(key_buffer) - 1) {
      key_buffer[key_buffer_pos++] = ' ';
      print_string(" ", VGA_WHITE);
    }
    if (stdin_pos < sizeof(stdin_buffer) - 1 && !stdin_ready) {
      stdin_buffer[stdin_pos++] = ' ';
    }
  } else if (make_code <= SC_MAX) {
    if (key_buffer_pos < sizeof(key_buffer) - 1) {
      unsigned char uppercase = shift_pressed ^ caps_lock;
      char letter = uppercase ? shifted_sc_ascii[make_code] : sc_ascii[make_code];
        
      if (letter != '?') {
        key_buffer[key_buffer_pos++] = letter;
        char str[2] = {letter, '\0'};
        print_string(str, VGA_WHITE);
        
        if (stdin_pos < sizeof(stdin_buffer) - 1 && !stdin_ready) {
          stdin_buffer[stdin_pos++] = letter;
        }
      }
    }
  }
}

int get_stdin_buffer(char* buffer, int max_count) {
  if (!stdin_ready || stdin_pos == 0) {
    return 0;
  }
  
  int bytes_to_copy = (stdin_pos < max_count) ? stdin_pos : max_count;
  
  register int i asm("ecx") = 0;
  while (i < bytes_to_copy) {
    buffer[i] = stdin_buffer[i];
    i++;
  }
  
  stdin_ready = 0;
  stdin_pos = 0;
  
  return bytes_to_copy;
}

int stdin_available() {
  return stdin_ready ? stdin_pos : 0;
}

void clear_stdin_buffer() {
  stdin_ready = 0;
  stdin_pos = 0;
}

void init_keyboard() {
  register_interrupt_handler(IRQ1, keyboard_callback);
}
