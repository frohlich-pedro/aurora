#pragma once

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define VGA_BLACK 0x00
#define VGA_BLUE 0x01
#define VGA_GREEN 0x02
#define VGA_CYAN 0x03
#define VGA_RED 0x04
#define VGA_MAGENTA 0x05
#define VGA_BROWN 0x06
#define VGA_GRAY 0x07
#define VGA_DARK_GRAY 0x08
#define VGA_LIGHT_BLUE 0x09
#define VGA_LIGHT_GREEN 0x0a
#define VGA_LIGHT_CYAN 0x0b
#define VGA_LIGHT_RED 0x0c
#define VGA_LIGHT_MAGENTA 0x0d
#define VGA_YELLOW 0x0e
#define VGA_WHITE 0x0f


#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void print_string(const char* string, unsigned char color);
void print_nl(void);
void clear_screen(void);
void print_backspace(void);
int get_cursor(void);
