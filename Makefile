ASM = nasm
CC = gcc
LD = ld
DD = dd
QEMU = qemu-system-x86_64

ASMFLAGS = -f elf64
CFLAGS = -ffreestanding -nostdlib -m64 -I./src -Wall -Wextra
LDFLAGS = -m elf_x86_64 -Ttext 0x1000 -Tdata 0x2000 --oformat binary

OUT = out
BOOT_SRC = src/boot/boot.asm
ASM_SRC = src/boot/kernel_entry.asm src/boot/interrupts.asm
C_SRC = src/kernel.c \
         src/drivers/vga_driver.c \
         src/drivers/kb_driver.c \
         src/drivers/idt.c \
         src/include/io.c \
         src/include/memory.c

all: os.img

os.img: $(OUT)/boot.bin $(OUT)/kernel.bin
	$(DD) if=/dev/zero of=$(OUT)/os.img bs=512 count=2880
	$(DD) if=$(OUT)/boot.bin of=$(OUT)/os.img conv=notrunc
	$(DD) if=$(OUT)/kernel.bin of=$(OUT)/os.img seek=1 conv=notrunc

$(OUT)/boot.bin: $(BOOT_SRC)
	@mkdir -p $(OUT)
	cd src/boot && $(ASM) -f bin boot.asm -o ../../$(OUT)/boot.bin

$(OUT)/kernel.bin: $(OUT)/kernel_entry.o $(OUT)/interrupts.o \
                   $(OUT)/kernel.o $(OUT)/vga_driver.o \
                   $(OUT)/kb_driver.o $(OUT)/idt.o \
                   $(OUT)/io.o $(OUT)/memory.o
	$(LD) $(LDFLAGS) -o $(OUT)/kernel.bin $^

$(OUT)/kernel_entry.o: src/boot/kernel_entry.asm
	@mkdir -p $(OUT)
	cd src/boot && $(ASM) $(ASMFLAGS) kernel_entry.asm -o ../../$(OUT)/kernel_entry.o

$(OUT)/interrupts.o: src/boot/interrupts.asm
	@mkdir -p $(OUT)
	cd src/boot && $(ASM) $(ASMFLAGS) interrupts.asm -o ../../$(OUT)/interrupts.o

$(OUT)/kernel.o: src/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/vga_driver.o: src/drivers/vga_driver.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/kb_driver.o: src/drivers/kb_driver.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/idt.o: src/drivers/idt.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/io.o: src/include/io.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/memory.o: src/include/memory.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUT)

run: os.img
	$(QEMU) -fda $(OUT)/os.img -no-reboot -d int

.PHONY: all clean run
