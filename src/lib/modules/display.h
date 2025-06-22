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

void print_nl(void);
void clear_screen(void);
void print_backspace(void);
int get_cursor(void);
void init_curses_display(void);
void update_panel(void);
void refresh_display(void);
void clear_content_area(void);
void print_string(const char* string, unsigned char text_color);
void get_content_cursor_position(int* row, int* col);
void set_content_cursor_position(int row, int col);
void print_at_position(int row, int col, const char* text, unsigned char color);
void print_centered(int row, int start_col, int width, const char* text, unsigned char color);
void fill_area(int start_row, int end_row, int start_col, int end_col, char character, unsigned char color);
void draw_horizontal_line(int row, int start_col, int end_col, char character, unsigned char color);
void draw_panel();
void print_color(unsigned char color);
