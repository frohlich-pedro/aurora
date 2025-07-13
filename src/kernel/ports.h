#pragma once

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
unsigned short port_word_in(unsigned short port);
void port_word_out(unsigned short port, unsigned short data);
unsigned int port_dword_in(unsigned int port);
void port_dword_out(unsigned int port, unsigned int data);