#include "display.h"
#include "ports.h"
#include "../kernel/mem.h"
#include "../kernel/util.h"

void set_cursor(int offset) {
  offset >>= 1;
  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int get_cursor() {
  port_byte_out(REG_SCREEN_CTRL, 14);
  int offset = port_byte_in(REG_SCREEN_DATA) << 8;
  port_byte_out(REG_SCREEN_CTRL, 15);
  offset += port_byte_in(REG_SCREEN_DATA);
  return offset << 1;
}

int get_offset(int col, int row) {
  return (row * MAX_COLS + col) << 1;
}

int get_row_from_offset(int offset) {
  return offset / (MAX_COLS << 1);
}

int move_offset_to_new_line(int offset) {
  return get_offset(0, get_row_from_offset(offset) + 1);
}

void set_char_at_video_memory(char character, int offset) {
  unsigned char* vidmem = (unsigned char*)VIDEO_ADDRESS;
  *(vidmem + offset) = character;
  *(vidmem + offset + 1) = WHITE_ON_BLACK;
}

int scroll_ln(int offset) {
  memory_copy(
    (unsigned char*)(get_offset(0, 1) + VIDEO_ADDRESS),
    (unsigned char*)(get_offset(0, 0) + VIDEO_ADDRESS),
    MAX_COLS * (MAX_ROWS - 1) * 2
  );
  
  for (int col = 0; col < MAX_COLS; col++) {
    set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
  }
  
  return offset - (MAX_COLS << 1);
}

void print_string(const char* string) {
  int offset = get_cursor();
  
  int i = 0;
  while (*(string + i) != 0) {
    if (offset >= MAX_ROWS * MAX_COLS * 2) {
      offset = scroll_ln(offset);
    }
    
    if (*(string + i) == '\n') {
      offset = move_offset_to_new_line(offset);
    } else {
      set_char_at_video_memory(*(string + i), offset);
      offset += 2;
    }
    
    i++;
  }
  
  set_cursor(offset);
}

void print_nl() {
  int new_offset = move_offset_to_new_line(get_cursor());
  if (new_offset >= MAX_ROWS * MAX_COLS * 2) {
    new_offset = scroll_ln(new_offset);
  }
  set_cursor(new_offset);
}

void clear_screen() {
  int screen_size = MAX_COLS * MAX_ROWS;
  for (int i = 0; i < screen_size; i++) {
    set_char_at_video_memory(' ', i << 1);
  }
  
  set_cursor(get_offset(0, 0));
}

void print_backspace() {
  int new_cursor = get_cursor() - 2;
  set_char_at_video_memory(' ', new_cursor);
  set_cursor(new_cursor);
}
