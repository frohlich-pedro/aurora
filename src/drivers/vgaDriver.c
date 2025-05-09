#include "vgaDriver.h"
#include "../include/io.h"
#include "../include/memory.h"

static int scrollScreen(int offset);

void setCursor(int offset) {
  byteOutput(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
  byteOutput(VGA_DATA_REGISTER, (unsigned char)((offset >> 8) & 0xff));
  byteOutput(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
  byteOutput(VGA_DATA_REGISTER, (unsigned char)(offset & 0xff));
}

int getCursor() {
  byteOutput(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
  int offset = byteInput(VGA_DATA_REGISTER) << 8;
  byteOutput(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
  offset += byteInput(VGA_DATA_REGISTER);
  return offset;
}

void setChar(char character, unsigned char color, int offset) {
  unsigned short* vgaBuffer = (unsigned short*)VIDEO_ADDRESS;
  *(vgaBuffer + offset) = (color << 8) | character;
}

void kprintf(const char* string, unsigned char color) {
  int offset = getCursor();
  int i = 0;

  while (*(string + i) != '\0') {
    if (*(string + i) == '\n') {
      offset = getOffset(getRow(offset) + 1, 0);
    } else {
      setChar(*(string + i), color, offset);
      offset++;
    }

    if (getCol(offset) >= MAX_COLS) {
      offset = getOffset(getRow(offset) + 1, 0);
    }
      
    if (getRow(offset) >= MAX_ROWS) {
      offset = scrollScreen(offset);
    }
    i++;
  }
  setCursor(offset);
}

void kcls() {
  unsigned int fill = (VGA_WHITE << 8 | ' ') * 0x00010001;
  unsigned int* buffer = (unsigned int*)VIDEO_ADDRESS;
  for (int i = 0; i < (MAX_ROWS * MAX_COLS)/2; i++) {
    *(buffer + i) = fill;
  }
  setCursor(0);
}

static int scrollScreen(int offset) {
  memoryCopy(
    (char*)VIDEO_ADDRESS,
    (char*)VIDEO_ADDRESS + MAX_COLS * 2,
    MAX_COLS * (MAX_ROWS - 1) * 2
  );

  for (int col = 0; col < MAX_COLS; col++) {
    setChar(' ', VGA_WHITE, getOffset(MAX_ROWS - 1, col));
  }

  int newRow = MAX_ROWS - 1;
  int newCol = getCol(offset);
    
  if (newCol >= MAX_COLS) {
    newCol = 0;
  }
    
  return getOffset(newRow, newCol);
}
