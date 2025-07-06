#ifndef DISPLAY_H
#define DISPLAY_H

#define VGA_ADDR 0xb8000
#define COLS 80
#define ROWS 25

void display_init();
void show();
void clear();
void put_char(char c);
void put_str(const char* str);
void newline();
void tab();
void backspace();

#endif
