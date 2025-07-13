C_SOURCES = $(wildcard kernel/*.c lib/*.c lib/modules/*.c kernel/*.c usr/*.c usr/bin/*.c init/*.c mm/*.c)
HEADERS = $(wildcard kernel/*.h lib/*.h lib/modules/*.h kernel/*.h usr/*.h usr/bin/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o kernel/interrupt.o}

CC = gcc
LD = ld
CFLAGS = -Os -g -m32 -ffreestanding -fno-pie -fno-stack-protector -I.

all: run

kernel.bin: boot/kernel_entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: boot/mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $< -audiodev pa,id=speaker -machine pcspk-audiodev=speaker

kernel.elf: boot/kernel_entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^

%.o: %.c ${HEADERS}
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

clean:
	$(RM) *.bin *.o *.dis *.elf
	$(RM) kernel/*.o boot/*.o boot/*.bin lib/*.o lib/modules/*.o kernel/*.o usr/*.o usr/bin/*.o init/*.o mm/*.o