#ifndef VGA_DRIVER_H
#define VGA_DRIVER_H

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e
#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define VGA_WHITE 0x0f
#define VGA_BLACK 0x00
#define VGA_BLUE 0x01
#define VGA_GREEN 0x02
#define VGA_CYAN 0x03
#define VGA_RED 0x04
#define VGA_MAGENTA 0x05
#define VGA_BROWN 0x06
#define VGA_LIGHT_GRAY 0x07
#define VGA_DARK_GRAY 0x08
#define VGA_LIGHT_BLUE 0x09
#define VGA_LIGHT_GREEN 0x0a
#define VGA_LIGHT_CYAN 0x0b
#define VGA_LIGHT_RED 0x0c
#define VGA_LIGHT_MAGENTA 0x0d
#define VGA_YELLOW 0x0e

void set_cursor(int offset);
int get_cursor();
void set_char(char character, unsigned char color, int offset);
int scroll_screen(int offset);
void kprintf(const char* string, unsigned char color);
void kcls();

int get_offset(int row, int col);
int get_row(int offset);
int get_col(int offset);

#endif
