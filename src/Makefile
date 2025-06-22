C_SOURCES = $(wildcard kernel/*.c lib/*.c lib/modules/*.c kernel/*.c shell/*.c shell/bin/*.c init/*.c mm/*.c)
HEADERS = $(wildcard kernel/*.h lib/*.h lib/modules/*.h kernel/*.h shell/*.h shell/bin/*.h)
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
	qemu-system-x86_64 -fda $< -device e1000,netdev=net0 -netdev user,id=net0 -audiodev oss,id=speaker -machine pcspk-audiodev=speaker

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
	$(RM) kernel/*.o boot/*.o boot/*.bin lib/*.o lib/modules/*.o kernel/*.o shell/*.o shell/bin/*.o init/*.o mm/*.o
