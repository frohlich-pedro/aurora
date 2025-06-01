#include "sound.h"
#include "../drivers/ports.h"
#include "../cpu/timer.h"

static void play_sound(unsigned int freq) {
  unsigned int divisor = 1193180 / freq;
  unsigned char tmp;
  
  port_byte_out(0x43, 0xB6);
  port_byte_out(0x42, (unsigned char)(divisor));
  port_byte_out(0x42, (unsigned char)(divisor >> 8));
  
  tmp = port_byte_in(0x61);
  if (tmp != (tmp | 3)) {
    port_byte_out(0x61, tmp | 3);
  }
}

static void stop_sound() {
  unsigned char tmp = port_byte_in(0x61) & 0xFC;
  port_byte_out(0x61, tmp);
}

void beep(unsigned int freq, unsigned int duration) {
  play_sound(freq);
  sleep(duration);
  stop_sound();
}
