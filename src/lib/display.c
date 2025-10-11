#include "display.h"

display_t display;

static void update_cursor(void);

static void clear_op(display_t* disp, void* arg) {
  display_char_t* p = &disp->buffer[0][0];
  display_char_t* e = p + SCREEN_WIDTH * SCREEN_HEIGHT;

  while (p < e) {
    p->character = ' ';
    p->attribute = disp->current_color;
    p++;
  }

  disp->cursor_x = 0;
  disp->cursor_y = 0;
  update_cursor();
}

static void putchar_op(display_t* disp, void* arg) {
  void** args = (void**)arg;
  char c = (char)(unsigned int)args[0];
  unsigned char color = (unsigned char)(unsigned int)args[1];

  if (c == '\n') {
    disp->cursor_x = 0;
    disp->cursor_y++;
  } else if (c == '\r') {
    disp->cursor_x = 0;
  } else if (c == '\b') {
    if (disp->cursor_x > 0) {
      disp->cursor_x--;
      disp->buffer[disp->cursor_y][disp->cursor_x].character = ' ';
      disp->buffer[disp->cursor_y][disp->cursor_x].attribute = color;
      }
  } else if (c == '\t') {
    disp->cursor_x = (disp->cursor_x + 8) & ~7;
  } else {
    disp->buffer[disp->cursor_y][disp->cursor_x].character = c;
    disp->buffer[disp->cursor_y][disp->cursor_x].attribute = color;
    disp->cursor_x++;
  }

  if (disp->cursor_x >= SCREEN_WIDTH) {
    disp->cursor_x = 0;
    disp->cursor_y++;
  }

  if (disp->cursor_y >= SCREEN_HEIGHT) {
    display_op_arr[OP_SCROLL](disp, (void*)(unsigned int)color);
    disp->cursor_y = SCREEN_HEIGHT - 1;
  }

  update_cursor();
}

static void puts_op(display_t* disp, void* arg) {
  void** args = (void**)arg;
  const char* str = (const char*)args[0];
  unsigned char color = (unsigned char)(unsigned int)args[1];

  while (*str) {
    void* char_args[] = {(void*)(unsigned int)*str, (void*)(unsigned int)color};
    display_op_arr[OP_PUTCHAR](disp, char_args);
    str++;
  }
}

static void set_cursor_op(display_t* disp, void* arg) {
  int* pos = (int*)arg;
  disp->cursor_x = *pos++;
  disp->cursor_y = *pos;
  update_cursor();
}

static void scroll_op(display_t* disp, void* arg) {
  unsigned char color = (unsigned char)(unsigned int)arg;

  int y = 0;
  while (y < SCREEN_HEIGHT - 1) {
    int x = 0;
    while (x < SCREEN_WIDTH) {
      disp->buffer[y][x] = disp->buffer[y + 1][x];
      x++;
    }
    y++;
  }

  display_char_t* p = &disp->buffer[SCREEN_HEIGHT - 1][0];
  display_char_t* e = p + SCREEN_WIDTH;

  while (p < e) {
    p->character = ' ';
    p->attribute = color;
    p++;
  }
}

static void set_color_op(display_t* disp, void* arg) {
  disp->current_color = (unsigned char)(unsigned int)arg;
}

static void update_cursor(void) {
  unsigned short position = display.cursor_y * SCREEN_WIDTH + display.cursor_x;
  byte_out(0x3D4, 0x0F);
  byte_out(0x3D5, (unsigned char)(position & 0xFF));
  byte_out(0x3D4, 0x0E);
  byte_out(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

display_op_t display_op_arr[OP_COUNT] = {
  clear_op,
  putchar_op,
  puts_op,
  set_cursor_op,
  scroll_op,
  set_color_op
};

