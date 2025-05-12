[org 0x7c00]
[bits 16]

kernel_offset equ 0x1000
mov [boot_drive], dl

mov bp, 0x9000
mov sp, bp

call enable_a20
call load_kernel
call switch_to_64bit

jmp $

%include "disk.asm"
%include "gdt.asm"
%include "switch_to_64bit.asm"

[bits 16]
enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

load_kernel:
    mov bx, kernel_offset
    mov dh, 16             ; Load 16 sectors (8KB)
    mov dl, [boot_drive]
    call disk_load
    ret

boot_drive db 0
times 510 - ($ - $$) db 0
dw 0xaa55
