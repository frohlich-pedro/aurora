[org 0x7c00]
[bits 16]

kernel_offset equ 0x1000
mov [boot_drive], dl

mov bp, 0x9000
mov sp, bp

call load_kernel
call switch_to_32bit

jmp $

%include "disk.asm"
%include "gdt.asm"
%include "switch_to_32bit.asm"

[bits 16]
load_kernel:
  mov bx, kernel_offset
  mov dh, 16
  mov dl, [boot_drive]
  call disk_load
  ret

[bits 32]
begin_32bit:
  call kernel_offset
  jmp $

boot_drive db 0
times 510 - ($ - $$) db 0
dw 0xaa55