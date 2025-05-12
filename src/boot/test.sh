#!/bin/bash
nasm -f bin boot.asm -o boot.bin || exit 1

gcc -m64 -ffreestanding -c test.c -o test.o || exit 1
nasm -f elf64 kernel_entry.asm -o kernel_entry.o || exit 1
ld -nostdlib -Ttext=0x1000 -e _start kernel_entry.o test.o -o test.elf || exit 1
objcopy -O binary test.elf test.bin || exit 1

KERNEL_SIZE=$(stat -c%s kernel.bin)
if (( KERNEL_SIZE > 8192 )); then
    echo "Error: Kernel too large (max 8KB)"
    exit 1
fi

dd if=/dev/zero of=disk.img bs=512 count=2880 status=none || exit 1
dd if=boot.bin of=disk.img conv=notrunc status=none || exit 1
dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc status=none || exit 1

qemu-system-x86_64 -drive format=raw,file=disk.img -no-reboot -d cpu_reset
