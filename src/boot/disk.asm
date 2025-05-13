disk_load:
    pusha
    push dx

    mov ah, 0x02
    mov al, dh
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00
    int 0x13
    jc disk_error

    pop dx
    cmp al, dh
    jne sectors_error
    popa
    ret

disk_error:
    mov si, disk_error_msg
    call print_msg
    jmp disk_loop

sectors_error:
    mov si, sectors_error_msg
    call print_msg
    jmp disk_loop

print_msg:
    pusha
    mov ah, 0x0e
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    popa
    ret

disk_error_msg db "Disk error!", 0
sectors_error_msg db "Sector error!", 0

disk_loop:
    jmp $
