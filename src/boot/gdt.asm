align 4
gdt32_start:
    dq 0x0
gdt32_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt32_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt32_end:

gdt32_descriptor:
    dw gdt32_end - gdt32_start - 1
    dd gdt32_start

CODE32_SEG equ gdt32_code - gdt32_start
DATA32_SEG equ gdt32_data - gdt32_start

align 4
gdt64_start:
    dq 0x0
gdt64_code:
    dq 0x00209a0000000000
gdt64_data:
    dq 0x0000920000000000
gdt64_end:

gdt64_descriptor:
    dw gdt64_end - gdt64_start - 1
    dq gdt64_start

CODE64_SEG equ gdt64_code - gdt64_start
DATA64_SEG equ gdt64_data - gdt64_start
