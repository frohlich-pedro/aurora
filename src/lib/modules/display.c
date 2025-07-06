#include "display.h"
#include "../../kernel/ports.h"
#include "../util.h"

static char buffer[COLS * ROWS * 2];
static unsigned int cursor_pos = 0;

void display_init() {
	clear();
	show();
}

void show() {
	memcpy((void*)VGA_ADDR, buffer, COLS * ROWS * 2);
	unsigned int pos = cursor_pos / 2;
	outb(0x3D4, 14);
	outb(0x3D5, (unsigned char)(pos >> 8));
	outb(0x3D4, 15);
	outb(0x3D5, (unsigned char)(pos & 0xFF));
}

void clear() {
	char* ptr = buffer;
	char* end = buffer + (COLS * ROWS * 2);
loop:
	if (ptr < end) {
		*ptr++ = ' ';
		*ptr++ = 0x0f;
		goto loop;
	}
	cursor_pos = 0;
}

static void scroll() {
	memcpy(buffer, buffer + (COLS * 2), COLS * (ROWS - 1) * 2);
	char* ptr = buffer + (COLS * (ROWS - 1) * 2);
	char* end = ptr + (COLS * 2);
loop:
	if (ptr < end) {
		*ptr++ = ' ';
		*ptr++ = 0x0f;
		goto loop;
	}
	cursor_pos -= COLS * 2;
}

void put_char(const char c) {
	if (c == '\n') {
		newline();
		return;
	}
	if (c == '\t') {
		tab();
		return;
	}
	if (c == '\b') {
		backspace();
		return;
	}
	if (cursor_pos >= COLS * ROWS * 2) {
		scroll();
	}
	char* ptr = buffer + cursor_pos;
	*ptr++ = c;
	*ptr++ = 0x0f;
	cursor_pos += 2;
}

void put_str(const char* str) {
	const char* ptr = str;
loop:
	if (*ptr) {
		put_char(*ptr++);
		goto loop;
	}
}

void newline() {
	unsigned int row = cursor_pos / (COLS * 2);
	cursor_pos = (row + 1) * COLS * 2;
	if (cursor_pos >= COLS * ROWS * 2) {
		scroll();
	}
}

void tab() {
	unsigned int current_col = (cursor_pos / 2) % COLS;
	unsigned int next_tab_stop = ((current_col / 8) + 1) * 8;
	
	if (next_tab_stop >= COLS) {
		newline();
		return;
	}
loop:
	if ((cursor_pos / 2) % COLS < next_tab_stop) {
		if (cursor_pos >= COLS * ROWS * 2) {
			scroll();
		}
		char* ptr = buffer + cursor_pos;
		*ptr++ = ' ';
		*ptr++ = 0x0f;
		cursor_pos += 2;
		goto loop;
	}
}

void backspace() {
	if (cursor_pos >= 2) {
		cursor_pos -= 2;
		char* ptr = buffer + cursor_pos;
		*ptr++ = ' ';
		*ptr++ = 0x0f;
	}
}
