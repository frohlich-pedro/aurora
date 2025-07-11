C_SOURCES = $(wildcard init/*.c kernel/*.c lib/*.c mm/*.c lib/modules/*.c bin/*.c)
HEADERS = $(kernel/*.h mm/*.h lib/*.h lib/modules/*.h bin/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o kernel/interrupts.o}

CC = gcc
LD = ld

all: run

kernel.bin: boot/kernel_entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: boot/mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

echo: os-image.bin
	xxd $<

kernel.elf: boot/kernel_entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin -d guest_errors,int &
	i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	$(CC) -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

clean:
	$(RM) *.bin *.o *.dis *.elf
	$(RM) init/*.o
	$(RM) boot/*.o boot/*.bin
	$(RM) kernel/*.o
	$(RM) lib/*.o
	$(RM) mm/*.o
	$(RM) lib/modules/*.o
	$(RM) bin/*.o
