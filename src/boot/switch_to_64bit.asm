[bits 16]
switch_to_64bit:
    cli
    lgdt [gdt32_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE32_SEG:protected_mode

[bits 32]
protected_mode:
    mov ax, DATA32_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call setup_paging

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xc0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [gdt64_descriptor]
    jmp CODE64_SEG:long_mode

[bits 64]
long_mode:
    mov ax, DATA64_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rbp, 0x90000
    mov rsp, rbp
    jmp kernel_offset

[bits 32]
setup_paging:
    mov edi, 0xa000
    mov cr3, edi
    xor eax, eax
    mov ecx, 0x3000 / 4
    rep stosd

    mov dword [0xa000], 0xb003
    mov dword [0xb000], 0xc003
    mov dword [0xc000], 0x83
    ret
