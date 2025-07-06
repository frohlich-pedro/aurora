#ifndef PORTS_H
#define PORTS_H

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
unsigned short inw(unsigned short port);
void outw(unsigned short port, unsigned short data);
unsigned int inl(unsigned int port);
void outl(unsigned int port, unsigned int data);

void insb(unsigned short port, void* addr, unsigned int count);
void outsb(unsigned short port, const void* addr, unsigned int count);
void insw(unsigned short port, void* addr, unsigned int count);
void outsw(unsigned short port, const void* addr, unsigned int count);
void insl(unsigned int port, void* addr, unsigned int count);
void outsl(unsigned int port, const void* addr, unsigned int count);

void io_wait();
unsigned char inb_p(unsigned short port);
void outb_p(unsigned short port, unsigned char data);
unsigned short inw_p(unsigned short port);
void outw_p(unsigned short port, unsigned short data);
unsigned int inl_p(unsigned int port);
void outl_p(unsigned int port, unsigned int data);

unsigned char inb_sync(unsigned short port);
void outb_sync(unsigned short port, unsigned char data);

int safe_inb(unsigned short port, unsigned char* result);
int safe_outb(unsigned short port, unsigned char data);

#define PORT_READ_8(port) inb(port)
#define PORT_WRITE_8(port, data) outb(port, data)
#define PORT_READ_16(port) inw(port)
#define PORT_WRITE_16(port, data) outw(port, data)
#define PORT_READ_32(port) inl(port)
#define PORT_WRITE_32(port, data) outl(port, data)

#define PORT_COM1 0x3F8
#define PORT_COM2 0x2F8
#define PORT_LPT1 0x378
#define PORT_KEYBOARD 0x60
#define PORT_PIC1_CMD 0x20
#define PORT_PIC1_DATA 0x21
#define PORT_PIC2_CMD 0xA0
#define PORT_PIC2_DATA 0xA1
#define PORT_POST 0x80

#endif
