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

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set EFER.LME
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging
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
    jmp kernel_offset      ; Jump to kernel at 0x1000

[bits 32]
setup_paging:
    ; PML4 at 0xA000 (avoid conflict with kernel at 0x1000)
    mov edi, 0xA000
    mov cr3, edi
    xor eax, eax
    mov ecx, 0x3000 / 4
    rep stosd

    ; Set up PML4 → PDP → PD
    mov dword [0xA000], 0xB003  ; PML4[0] → PDP at 0xB000
    mov dword [0xB000], 0xC003  ; PDP[0] → PD at 0xC000
    mov dword [0xC000], 0x83    ; PD[0] → 2MB page (identity-mapped)
    ret
