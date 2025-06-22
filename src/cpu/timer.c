#include "timer.h"
#include "isr.h"
#include "../drivers/ports.h"
#include "../drivers/display.h"
#include "../kernel/util.h"

unsigned int tick = 0;
static unsigned int panel_update_counter = 0;

static void timer_callback(registers_t* regs) {
  tick++;
  
  panel_update_counter++;
  if (panel_update_counter >= 1000) {
    draw_panel();
    panel_update_counter = 0;
  }
}

void init_timer(unsigned int freq) {
  register_interrupt_handler(IRQ0, timer_callback);
  unsigned int divisor = 1193180 / freq;
  port_byte_out(0x43, 0x36);
  port_byte_out(0x40, divisor & 0xFF);
  port_byte_out(0x40, divisor >> 8);
}

void sleep(unsigned int milliseconds) {
  unsigned int start_tick = tick;
  asm volatile("sti");
  while (tick - start_tick < milliseconds) {
    asm volatile("hlt");
  }
}
