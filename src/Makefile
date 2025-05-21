# $@ = target file
# $< = first dependency
# $^ = all dependencies

C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c shell/*.c shell/bin/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h shell/*.h shell/bin/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC ?= x86_64-elf-gcc
LD ?= x86_64-elf-ld
CFLAGS = -g -m32 -ffreestanding -fno-pie -fno-stack-protector -I.

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
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

clean:
	$(RM) *.bin *.o *.dis *.elf
	$(RM) kernel/*.o boot/*.o boot/*.bin drivers/*.o cpu/*.o shell/*.o shell/bin/*.o
