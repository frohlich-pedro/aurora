ASM = nasm
CC = gcc
LD = ld
DD = dd
QEMU = qemu-system-i386

ASMFLAGS = -f elf32
CFLAGS = -ffreestanding -nostdlib -m32 -I./src -Wall -Wextra
LDFLAGS = -m elf_i386 -Ttext 0x1000 -Tdata 0x2000 --oformat binary

OUT = out
BOOT_SRC = src/boot/boot.asm
ASM_SRC = src/boot/kernel_entry.asm src/boot/interrupts.asm
C_SRC = src/kernel.c src/drivers/vgaDriver.c src/drivers/kbDriver.c src/include/io.c src/include/memory.c

all: os.img

os.img: $(OUT)/boot.bin $(OUT)/kernel.bin
	$(DD) if=/dev/zero of=$(OUT)/os.img bs=512 count=2880
	$(DD) if=$(OUT)/boot.bin of=$(OUT)/os.img conv=notrunc
	$(DD) if=$(OUT)/kernel.bin of=$(OUT)/os.img seek=1 conv=notrunc

$(OUT)/boot.bin: $(BOOT_SRC)
	@mkdir -p $(OUT)
	cd src/boot && $(ASM) -f bin boot.asm -o ../../$(OUT)/boot.bin

$(OUT)/kernel.bin: $(OUT)/kernel_entry.o $(OUT)/kernel.o $(OUT)/interrupts.o \
                   $(OUT)/vgaDriver.o $(OUT)/kbDriver.o $(OUT)/io.o $(OUT)/memory.o
	$(LD) $(LDFLAGS) -o $(OUT)/kernel.bin $^

$(OUT)/kernel_entry.o: src/boot/kernel_entry.asm
	@mkdir -p $(OUT)
	$(ASM) $(ASMFLAGS) $< -o $@

$(OUT)/kernel.o: src/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/interrupts.o: src/boot/interrupts.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(OUT)/vgaDriver.o: src/drivers/vgaDriver.c src/drivers/vgaDriver.h src/include/io.h src/include/memory.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/kbDriver.o: src/drivers/kbDriver.c src/drivers/kbDriver.h src/drivers/vgaDriver.h src/include/io.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/io.o: src/include/io.c src/include/io.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/memory.o: src/include/memory.c src/include/memory.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUT)

run: os.img
	$(QEMU) -fda $(OUT)/os.img

.PHONY: all clean run
