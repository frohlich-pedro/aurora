#include "vga_driver.h"
#include "../include/io.h"
#include "../include/memory.h"

int get_offset(int row, int col) {
  return row * MAX_COLS + col;
}

int get_row(int offset) {
  return offset / MAX_COLS;
}

int get_col(int offset) {
  return offset % MAX_COLS;
}

void set_cursor(int offset) {
  byte_output(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
  byte_output(VGA_DATA_REGISTER, (unsigned char)((offset >> 8) & 0xff));
  byte_output(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
  byte_output(VGA_DATA_REGISTER, (unsigned char)(offset & 0xff));
}

int get_cursor() {
  byte_output(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
  int offset = byte_input(VGA_DATA_REGISTER) << 8;
  byte_output(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
  offset += byte_input(VGA_DATA_REGISTER);
  return offset;
}

void set_char(char character, unsigned char color, int offset) {
  unsigned short* vga_buffer = (unsigned short*)VIDEO_ADDRESS;
  *(vga_buffer + offset) = (color << 8) | (character & 0xFF);
}

void kprintf(const char* string, unsigned char color) {
  int offset = get_cursor();
  int i = 0;
  while (string[i] != '\0') {
    if (string[i] == '\n') {
      offset = get_offset(get_row(offset) + 1, 0);
    } else {
      set_char(string[i], color, offset);
      offset++;
    }

    if (get_col(offset) >= MAX_COLS) {
      offset = get_offset(get_row(offset) + 1, 0);
    }

    if (get_row(offset) >= MAX_ROWS) {
      offset = scroll_screen(offset);
    }

    i++;
  }
  set_cursor(offset);
}

void kcls() {
  unsigned int fill = (VGA_WHITE << 8 | ' ') * 0x00010001;
  unsigned int* buffer = (unsigned int*)VIDEO_ADDRESS;
  for (int i = 0; i < (MAX_ROWS * MAX_COLS) / 2; i++) {
    buffer[i] = fill;
  }
  set_cursor(0);
}

int scroll_screen(int offset) {
  memory_copy((char*)VIDEO_ADDRESS, (char*)VIDEO_ADDRESS + MAX_COLS * 2, MAX_COLS * (MAX_ROWS - 1) * 2);
  for (int i = 0; i < MAX_COLS; i++) {
    set_char(' ', VGA_WHITE, get_offset(MAX_ROWS - 1, i));
  }
  return get_offset(MAX_ROWS - 1, get_col(offset) % MAX_COLS);
}
