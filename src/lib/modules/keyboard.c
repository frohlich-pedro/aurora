#include "keyboard.h"
#include "../../kernel/ports.h"
#include "../../kernel/irq.h"
#include "../util.h"

static const char scancode_to_ascii[128] = {
	0, 27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', '\b', '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '[', ']', '\n', 0, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0, '*',
	0, ' ', 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '7',
	'8', '9', '-', '4', '5', '6', '+', '1',
	'2',  '3',  '0',  '.',  0,    0, 0,    0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

static const char scancode_to_ascii_shift[128] = {
	0, 27, '!', '@', '#', '$', '%', '^',
	'&', '*', '(', ')', '_', '+', '\b', '\t',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
	'O', 'P', '{', '}', '\n', 0, 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
	'"', '~', 0, '|', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', 0, '*',
	0, ' ', 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '7',
	'8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

static unsigned char keyboard_flags = 0;
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static unsigned int buffer_head = 0;
static unsigned int buffer_tail = 0;
static unsigned int buffer_count = 0;
static void keyboard_add_to_buffer(char c);
static char keyboard_get_from_buffer();
static unsigned char keyboard_read_data();
static void keyboard_wait_for_output();

void keyboard_init() {
	keyboard_flush_buffer();
	irq_install_handler(1, keyboard_handler);
loop:
	if (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OUTPUT_FULL) {
		inb(KEYBOARD_DATA_PORT);
		goto loop;
	}
}

void keyboard_handler() {
	unsigned char scancode = keyboard_read_data();
	if (scancode & 0x80) {
		scancode &= 0x7F;
		switch (scancode) {
		case KEY_LSHIFT:
		case KEY_RSHIFT:
			keyboard_flags &= ~KEYBOARD_FLAG_SHIFT;
			break;
		case KEY_CTRL:
			keyboard_flags &= ~KEYBOARD_FLAG_CTRL;
			break;
		case KEY_ALT:
			keyboard_flags &= ~KEYBOARD_FLAG_ALT;
			break;
		}
		return;
	}
	
	switch (scancode) {
	case KEY_LSHIFT:
	case KEY_RSHIFT:
		keyboard_flags |= KEYBOARD_FLAG_SHIFT;
		break;
	case KEY_CTRL:
		keyboard_flags |= KEYBOARD_FLAG_CTRL;
		break;
	case KEY_ALT:
		keyboard_flags |= KEYBOARD_FLAG_ALT;
		break;
	case KEY_CAPSLOCK:
		keyboard_flags ^= KEYBOARD_FLAG_CAPSLOCK;
		break;
	default: {
		char ascii;
		ascii = (keyboard_flags & KEYBOARD_FLAG_SHIFT)
		? *(scancode_to_ascii_shift + scancode)
		: *(scancode_to_ascii + scancode);
		
		if (ascii >= 'a' && ascii <= 'z' && 
			(keyboard_flags & KEYBOARD_FLAG_CAPSLOCK)) {
			ascii = ascii - 'a' + 'A';
		} else if (ascii >= 'A' && ascii <= 'Z' && 
				(keyboard_flags & KEYBOARD_FLAG_CAPSLOCK) && 
				!(keyboard_flags & KEYBOARD_FLAG_SHIFT)) {
			ascii = ascii - 'A' + 'a';
		}
		if (ascii != 0) {
			keyboard_add_to_buffer(ascii);
		}
		break;
	}
	}
}

char keyboard_getchar() {
loop:
	if (buffer_count == 0) {
		__asm__ volatile ("hlt");
		goto loop;
	}
	return keyboard_get_from_buffer();
}

int keyboard_getchar_nonblocking() {
	if (buffer_count == 0) {
		return 1;
	}
	return (int)keyboard_get_from_buffer();
}

void keyboard_flush_buffer() {
	buffer_head = 0;
	buffer_tail = 0;
	buffer_count = 0;
	char* ptr = keyboard_buffer;
	char* end = keyboard_buffer + KEYBOARD_BUFFER_SIZE;
loop:
	if (ptr != end) {
		*ptr++ = 0;
		goto loop;
	}
}


unsigned char keyboard_get_flags() {
	return keyboard_flags;
}

static void keyboard_add_to_buffer(char c) {
	if (buffer_count < KEYBOARD_BUFFER_SIZE) {
		*(keyboard_buffer + buffer_head) = c;
		buffer_head = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
		buffer_count++;
	}
}

static char keyboard_get_from_buffer() {
	if (buffer_count == 0) {
		return 0;
	}
	char c = *(keyboard_buffer + buffer_tail);
	buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
	buffer_count--;
	return c;
}

static unsigned char keyboard_read_data() {
    keyboard_wait_for_output();
    return inb(KEYBOARD_DATA_PORT);
}

static void keyboard_wait_for_output() {
loop:
	if (!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OUTPUT_FULL)) {
		goto loop;
	}
}
