#ifndef DISPLAY_H
#define DISPLAY_H

#include "port.h"

#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

typedef enum {
  BLACK,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  BROWN,
  GRAY,
  DARK_GRAY,
  LIGHT_BLUE,
  LIGHT_GREEN,
  LIGHT_CYAN,
  LIGHT_RED,
  LIGHT_MAGENTA,
  YELLOW,
  WHITE
} color_t;

typedef enum {
  BG_BLACK         = BLACK << 4,
  BG_BLUE          = BLUE << 4,
  BG_GREEN         = GREEN << 4,
  BG_CYAN          = CYAN << 4,
  BG_RED           = RED << 4,
  BG_MAGENTA       = MAGENTA << 4,
  BG_BROWN         = BROWN << 4,
  BG_GRAY          = GRAY << 4,
  BG_DARK_GRAY     = DARK_GRAY << 4,
  BG_LIGHT_BLUE    = LIGHT_BLUE << 4,
  BG_LIGHT_GREEN   = LIGHT_GREEN << 4,
  BG_LIGHT_CYAN    = LIGHT_CYAN << 4,
  BG_LIGHT_RED     = LIGHT_RED << 4,
  BG_LIGHT_MAGENTA = LIGHT_MAGENTA << 4,
  BG_YELLOW        = YELLOW << 4,
  BG_WHITE         = WHITE << 4
} bg_color_t;

typedef struct {
  unsigned char character;
  unsigned char attribute;
} display_char_t;

typedef struct {
  display_char_t (*buffer)[SCREEN_WIDTH];
  int cursor_x;
  int cursor_y;
  unsigned char current_color;
} display_t;

typedef void (*display_op_t)(display_t*, void*);

extern display_t display;

enum display_ops {
  OP_CLEAR = 0,
  OP_PUTCHAR,
  OP_PUTS,
  OP_SET_CURSOR,
  OP_SCROLL,
  OP_SET_COLOR,
  OP_COUNT
};

extern display_op_t display_op_arr[OP_COUNT];

static inline void clear_screen() {
  display_op_arr[OP_CLEAR](&display, (void*)0);
}

static inline void print_char(char c, unsigned char color) {
  void* args[] = {(void*)(unsigned int)c, (void*)(unsigned int)color};
  display_op_arr[OP_PUTCHAR](&display, args);
}

static inline void print(const char* str, unsigned char color) {
  void* args[] = {(void*)str, (void*)(unsigned int)color};
  display_op_arr[OP_PUTS](&display, args);
}

static inline void set_cursor_pos(int x, int y) {
  int pos[] = {x, y};
  display_op_arr[OP_SET_CURSOR](&display, pos);
}

static inline void set_default_color(unsigned char color) {
  display_op_arr[OP_SET_COLOR](&display, (void*)(unsigned int)color);
}

static inline void put_char_at(int x, int y, char c, unsigned char color) {
  display.buffer[y][x].character = c;
  display.buffer[y][x].attribute = color;
}

static inline char get_char_at(int x, int y) {
  return display.buffer[y][x].character;
}

static inline void set_color_at(int x, int y, unsigned char color) {
  display.buffer[y][x].attribute = color;
}

static inline void display_init() {
  display.buffer = (display_char_t(*)[SCREEN_WIDTH])VIDEO_MEMORY;
  display.cursor_x = 0;
  display.cursor_y = 0;
  display.current_color = WHITE;

  display_op_arr[OP_CLEAR](&display, (void*)0);
}

#endif
