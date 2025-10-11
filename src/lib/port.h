#pragma once

unsigned char byte_in(unsigned short port);
void byte_out(unsigned short port, unsigned char data);
unsigned short word_in(unsigned short port);
void word_out(unsigned short port, unsigned short data);
