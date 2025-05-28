#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "display.h"
#include "../kernel/util.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT_MAKE 0x2A
#define RSHIFT_MAKE 0x36
#define LSHIFT_BREAK (LSHIFT_MAKE | 0x80)
#define RSHIFT_BREAK (RSHIFT_MAKE | 0x80)

static char key_buffer[256];
static unsigned char shift_pressed = 0;
static unsigned char caps_lock = 0;

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
    if (backspace(key_buffer)) {
      print_backspace();
    }
  } else if (make_code == ENTER) {
    print_nl();
    execute_command(key_buffer);
    *(key_buffer + 0) = '\0';
  } else if (make_code == 0x3A) {
    caps_lock = !caps_lock;
  } else if (make_code <= SC_MAX) {
    unsigned char uppercase = shift_pressed ^ caps_lock;
    char letter = uppercase ? *(shifted_sc_ascii + make_code) : *(sc_ascii + make_code);
        
    if (letter != '?') {
      append(key_buffer, letter);
      char str[2] = {letter, '\0'};
      print_string(str);
    }
  }
}

void init_keyboard() {
  register_interrupt_handler(IRQ1, keyboard_callback);
}
