#include "keyboard.h"
#include "display.h"
#include "../shell/shell.h"

keyboard_t keyboard;

static const char sc_ascii_lower[] = {
  '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 
  '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
  '?', '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
  '?', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '
};

static const char sc_ascii_upper[] = {
  '?', '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 
  '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 
  '?', '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 
  '?', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '
};

static void init_op(keyboard_t* kb, void* arg) {
  (void)arg;
  kb->buffer_pos = 0;
  kb->buffer[0] = '\0';
  kb->flags = 0;
  
  register_interrupt_handler(IRQ1, keyboard_handler);
}

static void backspace_op(keyboard_t* kb, void* arg) {
  (void)arg;
  if (kb->buffer_pos > 0) {
    kb->buffer_pos--;
    kb->buffer[kb->buffer_pos] = '\0';
  }
}

static void append_op(keyboard_t* kb, void* arg) {
  char letter = (char)(unsigned int)arg;
  
  if (kb->buffer_pos < KEYBOARD_BUFFER_SIZE - 1) {
    kb->buffer[kb->buffer_pos] = letter;
    kb->buffer_pos++;
    kb->buffer[kb->buffer_pos] = '\0';
  }
}

static void clear_buffer_op(keyboard_t* kb, void* arg) {
  (void)arg;
  kb->buffer_pos = 0;
  kb->buffer[0] = '\0';
}

static void set_flags_op(keyboard_t* kb, void* arg) {
  kb->flags = (unsigned char)(unsigned int)arg;
}

static void set_shift_op(keyboard_t* kb, void* arg) {
  int pressed = (int)(unsigned int)arg;
  kb->shift_pressed = pressed;
}

static void process_scancode_op(keyboard_t* kb, void* arg) {
  unsigned char scancode = (unsigned char)(unsigned int)arg;

  if (scancode > SC_MAX && scancode != LEFT_SHIFT_RELEASE && scancode != RIGHT_SHIFT_RELEASE) 
    return;

  switch(scancode) {
  case BACKSPACE:
    if (kb->buffer_pos > 0) {
      keyboard_backspace();
      shell_process_input('\b');
    }
    break;

  case ENTER:
    keyboard_append('\n');
    shell_process_input('\n');
    break;

  case LEFT_SHIFT:
  case RIGHT_SHIFT:
    keyboard_set_shift(1);
    break;

  case LEFT_SHIFT_RELEASE:
  case RIGHT_SHIFT_RELEASE:
    keyboard_set_shift(0);
    break;

  default:
    if (scancode <= SC_MAX) {
      const char* ascii_table = kb->shift_pressed ? sc_ascii_upper : sc_ascii_lower;
      char letter = ascii_table[scancode];

      if (letter != '?') {
        keyboard_append(letter);
        shell_process_input(letter);
        print_char(letter, WHITE);
      }
    }
    break;
  }
}

keyboard_op_t keyboard_op_arr[KB_OP_COUNT] = {
  init_op,
  backspace_op,
  append_op,
  clear_buffer_op,
  set_flags_op,
  set_shift_op,
  process_scancode_op
};

void keyboard_handler(registers_t* regs) {
  (void)regs;
  unsigned char scancode = byte_in(0x60);
  
  // Use the function pointer array for scancode processing
  keyboard_process_scancode(scancode);
  
  byte_out(0x20, 0x20);
}
