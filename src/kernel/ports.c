#include "ports.h"

unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile ("inb %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

void outb(unsigned short port, unsigned char data) {
    __asm__ volatile ("outb %0, %1" : : "a" (data), "dN" (port));
}

unsigned short inw(unsigned short port) {
    unsigned short result;
    __asm__ volatile ("inw %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

void outw(unsigned short port, unsigned short data) {
    __asm__ volatile ("outw %0, %1" : : "a" (data), "dN" (port));
}

unsigned int inl(unsigned int port) {
    unsigned int result;
    __asm__ volatile ("inl %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

void outl(unsigned int port, unsigned int data) {
    __asm__ volatile ("outl %0, %1" : : "a" (data), "dN" (port));
}


void insb(unsigned short port, void* addr, unsigned int count) {
    __asm__ volatile ("rep insb" : "=D" (addr), "=c" (count) : "d" (port), "0" (addr), "1" (count) : "memory");
}

void outsb(unsigned short port, const void* addr, unsigned int count) {
    __asm__ volatile ("rep outsb" : "=S" (addr), "=c" (count) : "d" (port), "0" (addr), "1" (count) : "memory");
}

void insw(unsigned short port, void* addr, unsigned int count) {
    __asm__ volatile ("rep insw" : "=D" (addr), "=c" (count) : "d" (port), "0" (addr), "1" (count) : "memory");
}

void outsw(unsigned short port, const void* addr, unsigned int count) {
    __asm__ volatile ("rep outsw" : "=S" (addr), "=c" (count) : "d" (port), "0" (addr), "1" (count) : "memory");
}

void insl(unsigned int port, void* addr, unsigned int count) {
    __asm__ volatile ("rep insl" : "=D" (addr), "=c" (count) : "d" (port), "0" (addr), "1" (count) : "memory");
}

void outsl(unsigned int port, const void* addr, unsigned int count) {
    __asm__ volatile ("rep outsl" : "=S" (addr), "=c" (count) : "d" (port), "0" (addr), "1" (count) : "memory");
}

void io_wait() {
    __asm__ volatile ("outb %%al, $0x80" : : "a" (0));
}

unsigned char inb_p(unsigned short port) {
    unsigned char result = inb(port);
    io_wait();
    return result;
}

void outb_p(unsigned short port, unsigned char data) {
    outb(port, data);
    io_wait();
}

unsigned short inw_p(unsigned short port) {
    unsigned short result = inw(port);
    io_wait();
    return result;
}

void outw_p(unsigned short port, unsigned short data) {
    outw(port, data);
    io_wait();
}

unsigned int inl_p(unsigned int port) {
    unsigned int result = inl(port);
    io_wait();
    return result;
}

void outl_p(unsigned int port, unsigned int data) {
    outl(port, data);
    io_wait();
}

unsigned char inb_sync(unsigned short port) {
    unsigned char result;
    __asm__ volatile ("inb %1, %0" : "=a" (result) : "dN" (port) : "memory");
    return result;
}

void outb_sync(unsigned short port, unsigned char data) {
    __asm__ volatile ("outb %0, %1" : : "a" (data), "dN" (port) : "memory");
}

int safe_inb(unsigned short port, unsigned char* result) {
    if (result == (void*)0) {
        return -1;
    }
    
    *result = inb(port);
    return 0;
}

int safe_outb(unsigned short port, unsigned char data) {
    outb(port, data);
    return 0;
}
