#include "display.h"
#include "../../kernel/ports.h"
#include "../../mm/mem.h"
#include "../util.h"
#include "../../kernel/timer.h"
#include "../../kernel/rtc.h"

#define PANEL_HEIGHT 1
#define PANEL_COLOR (VGA_GRAY | (VGA_GRAY << 4))
#define PANEL_TEXT_COLOR (VGA_BLACK | (VGA_GRAY << 4))
#define CONTENT_AREA_COLOR (VGA_WHITE | (VGA_BLUE << 4))
#define CONTENT_TEXT_COLOR VGA_WHITE
#define BORDER_COLOR (VGA_BLACK | (VGA_WHITE << 4))

#define CONTENT_START_ROW (PANEL_HEIGHT)
#define CONTENT_ROWS (MAX_ROWS - CONTENT_START_ROW)
#define CONTENT_COLS MAX_COLS

static int content_cursor_row = 0;
static int content_cursor_col = 0;

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

void set_char_at_video_memory(char character, int offset, unsigned char color) {
  unsigned char* vidmem = (unsigned char*)VIDEO_ADDRESS;
  *(vidmem + offset) = character;
  *(vidmem + offset + 1) = color;
}

void fill_area(int start_row, int end_row, int start_col, int end_col, char character, unsigned char color) {
  int row = start_row;
  while (row < end_row) {
    int col = start_col;
    while (col < end_col) {
      int offset = get_offset(col, row);
      set_char_at_video_memory(character, offset, color);
      col++;
    }
    row++;
  }
}

void print_at_position(int row, int col, const char* text, unsigned char color) {
  int offset = get_offset(col, row);
  while (*text && col < MAX_COLS) {
    set_char_at_video_memory(*text, offset, color);
    text++;
    offset += 2;
    col++;
  }
}

void print_centered(int row, int start_col, int width, const char* text, unsigned char color) {
  int text_len = string_length(text);
  int padding = (width - text_len) / 2;
  int pos = start_col + padding;
  if (pos < start_col) pos = start_col;
  print_at_position(row, pos, text, color);
}

void format_two_digit(char* buffer, int value) {
  buffer[0] = (value / 10) + '0';
  buffer[1] = (value % 10) + '0';
}

void get_time_string(char* buffer) {
  rtc_date_t rtc_time;
  rtc_read_date(&rtc_time);
  
  unsigned char hour = rtc_time.hour;
  if (!rtc_time.is_24hour && rtc_time.is_pm && hour != 12) {
    hour += 12;
  } else if (!rtc_time.is_24hour && !rtc_time.is_pm && hour == 12) {
    hour = 0;
  }
  
  format_two_digit(&buffer[0], hour);
  buffer[2] = ':';
  format_two_digit(&buffer[3], rtc_time.minute);
  buffer[5] = ':';
  format_two_digit(&buffer[6], rtc_time.second);
  buffer[8] = '\0';
}

void get_date_string(char* buffer) {
  rtc_date_t rtc_date;
  rtc_read_date(&rtc_date);
  
  int_to_string(rtc_date.year, buffer);
  int year_len = string_length(buffer);
  
  buffer[year_len] = '-';
  format_two_digit(&buffer[year_len + 1], rtc_date.month);
  buffer[year_len + 3] = '-';
  format_two_digit(&buffer[year_len + 4], rtc_date.day);
  buffer[year_len + 6] = '\0';
}

void get_date_string_short(char* buffer) {
  rtc_date_t rtc_date;
  rtc_read_date(&rtc_date);
  
  format_two_digit(&buffer[3], rtc_date.day);
  buffer[2] = '/';
  format_two_digit(&buffer[0], rtc_date.month);
  buffer[5] = '/';
  format_two_digit(&buffer[6], rtc_date.year % 100);
  buffer[8] = '\0';
}

void draw_panel() {
  fill_area(0, PANEL_HEIGHT, 0, MAX_COLS, ' ', PANEL_COLOR);
  
  char time_str[16];
  char date_str[16];
  get_time_string(time_str);
  get_date_string_short(date_str);
  
  print_at_position(0, 2, date_str, PANEL_TEXT_COLOR);
  print_centered(0, 0, MAX_COLS, "AuroraOS", PANEL_TEXT_COLOR);
  print_at_position(0, MAX_COLS - 10, time_str, PANEL_TEXT_COLOR);
}

void init_curses_display() {
  fill_area(0, MAX_ROWS, 0, MAX_COLS, ' ', CONTENT_AREA_COLOR);
  
  draw_panel();
  
  content_cursor_row = 0;
  content_cursor_col = 0;
  set_cursor(get_offset(0, CONTENT_START_ROW));
}

int scroll_content_area() {
  unsigned char* video_mem = (unsigned char*)VIDEO_ADDRESS;
  unsigned char* first_content_line = video_mem + get_offset(0, CONTENT_START_ROW);
  unsigned char* second_content_line = video_mem + get_offset(0, CONTENT_START_ROW + 1);
  
  memory_copy(second_content_line, first_content_line, MAX_COLS * (CONTENT_ROWS - 1) * 2);
  
  unsigned char* last_content_line = video_mem + get_offset(0, MAX_ROWS - 1);
  unsigned char* end_line = last_content_line + (MAX_COLS * 2);
  while (last_content_line < end_line) {
    *last_content_line++ = ' ';
    *last_content_line++ = CONTENT_AREA_COLOR;
  }
  
  return get_offset(0, MAX_ROWS - 1);
}

void print_string(const char* string, unsigned char text_color) {
  int actual_row = CONTENT_START_ROW + content_cursor_row;
  int offset = get_offset(content_cursor_col, actual_row);
  
  while (*string) {
    if (content_cursor_row >= CONTENT_ROWS) {
      scroll_content_area();
      content_cursor_row = CONTENT_ROWS - 1;
      actual_row = CONTENT_START_ROW + content_cursor_row;
      offset = get_offset(content_cursor_col, actual_row);
    }
    
    if (*string == '\n') {
      content_cursor_row++;
      content_cursor_col = 0;
      actual_row = CONTENT_START_ROW + content_cursor_row;
      offset = get_offset(0, actual_row);
    } else {
      if (content_cursor_col >= MAX_COLS) {
        content_cursor_row++;
        content_cursor_col = 0;
        actual_row = CONTENT_START_ROW + content_cursor_row;
        offset = get_offset(0, actual_row);
      }
      
      unsigned char color = (text_color & 0x0F) | (CONTENT_AREA_COLOR & 0xF0);
      set_char_at_video_memory(*string, offset, color);
      content_cursor_col++;
      offset += 2;
    }
    
    string++;
  }
  
  set_cursor(offset);
}

void print_nl() {
  print_string("\n", CONTENT_TEXT_COLOR);
}

void clear_screen() {
  init_curses_display();
  content_cursor_row = 0;
  content_cursor_col = 0;
}

void clear_content_area() {
  fill_area(CONTENT_START_ROW, MAX_ROWS, 0, MAX_COLS, ' ', CONTENT_AREA_COLOR);
  content_cursor_row = 0;
  content_cursor_col = 0;
  set_cursor(get_offset(0, CONTENT_START_ROW));
}

void print_backspace() {
  if (content_cursor_col > 0) {
    content_cursor_col--;
  } else if (content_cursor_row > 0) {
    content_cursor_row--;
    content_cursor_col = MAX_COLS - 1;
  }
  
  int actual_row = CONTENT_START_ROW + content_cursor_row;
  int offset = get_offset(content_cursor_col, actual_row);
  set_char_at_video_memory(' ', offset, CONTENT_AREA_COLOR);
  set_cursor(offset);
}

void get_content_cursor_position(int* row, int* col) {
  *row = content_cursor_row;
  *col = content_cursor_col;
}

void set_content_cursor_position(int row, int col) {
  if (row >= 0 && row < CONTENT_ROWS && col >= 0 && col < MAX_COLS) {
    content_cursor_row = row;
    content_cursor_col = col;
    int actual_row = CONTENT_START_ROW + row;
    set_cursor(get_offset(col, actual_row));
  }
}

void print_color(unsigned char color) {
  int offset = get_cursor();
  const char* string = "  ";
  
  do {
    if (offset >= MAX_ROWS * MAX_COLS * 2) {
      offset = scroll_content_area();
    }
    
    if (*string == '\n') {
      offset = move_offset_to_new_line(offset);
    } else {
      set_char_at_video_memory(*string, offset, color);
      offset += 2;
    }
    
    string++;
  } while (*string);
  
  set_cursor(offset);
}