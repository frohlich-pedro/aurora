[extern isr_handler]
[extern irq_handler]

isr_common_stub:
  pusha
  mov ax, ds
  push eax
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  push esp
  call isr_handler
  pop eax
  pop eax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  popa
  add esp, 8
  iret

irq_common_stub:
  pusha
  mov ax, ds
  push eax
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  push esp
  call irq_handler
  pop ebx
  pop ebx
  mov ds, bx
  mov es, bx
  mov fs, bx
  mov gs, bx
  popa
  add esp, 8
  iret

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

isr0:
  push 0
  push 0
  jmp isr_common_stub

isr1:
  push 0
  push 1
  jmp isr_common_stub

isr2:
  push 0
  push 2
  jmp isr_common_stub

isr3:
  push 0
  push 3
  jmp isr_common_stub

isr4:
  push 0
  push 4
  jmp isr_common_stub

isr5:
  push 0
  push 5
  jmp isr_common_stub

isr6:
  push 0
  push 6
  jmp isr_common_stub

isr7:
  push 0
  push 7
  jmp isr_common_stub

isr8:
  push 8
  jmp isr_common_stub

isr9:
  push 0
  push 9
  jmp isr_common_stub

isr10:
  push 10
  jmp isr_common_stub

isr11:
  push 11
  jmp isr_common_stub

isr12:
  push 12
  jmp isr_common_stub

isr13:
  push 13
  jmp isr_common_stub

isr14:
  push 14
  jmp isr_common_stub

isr15:
  push 0
  push 15
  jmp isr_common_stub

isr16:
  push 0
  push 16
  jmp isr_common_stub

isr17:
  push 0
  push 17
  jmp isr_common_stub

isr18:
  push 0
  push 18
  jmp isr_common_stub

isr19:
  push 0
  push 19
  jmp isr_common_stub

isr20:
  push 0
  push 20
  jmp isr_common_stub

isr21:
  push 0
  push 21
  jmp isr_common_stub

isr22:
  push 0
  push 22
  jmp isr_common_stub

isr23:
  push 0
  push 23
  jmp isr_common_stub

isr24:
  push 0
  push 24
  jmp isr_common_stub

isr25:
  push 0
  push 25
  jmp isr_common_stub

isr26:
  push 0
  push 26
  jmp isr_common_stub

isr27:
  push 0
  push 27
  jmp isr_common_stub

isr28:
  push 0
  push 28
  jmp isr_common_stub

isr29:
  push 0
  push 29
  jmp isr_common_stub

isr30:
  push 0
  push 30
  jmp isr_common_stub

isr31:
  push 0
  push 31
  jmp isr_common_stub

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

irq0:
  push 0
  push 32
  jmp irq_common_stub

irq1:
  push 1
  push 33
  jmp irq_common_stub

irq2:
  push 2
  push 34
  jmp irq_common_stub

irq3:
  push 3
  push 35
  jmp irq_common_stub

irq4:
  push 4
  push 36
  jmp irq_common_stub

irq5:
  push 5
  push 37
  jmp irq_common_stub

irq6:
  push 6
  push 38
  jmp irq_common_stub

irq7:
  push 7
  push 39
  jmp irq_common_stub

irq8:
  push 8
  push 40
  jmp irq_common_stub

irq9:
  push 9
  push 41
  jmp irq_common_stub

irq10:
  push 10
  push 42
  jmp irq_common_stub

irq11:
  push 11
  push 43
  jmp irq_common_stub

irq12:
  push 12
  push 44
  jmp irq_common_stub

irq13:
  push 13
  push 45
  jmp irq_common_stub

irq14:
  push 14
  push 46
  jmp irq_common_stub

irq15:
  push 15
  push 47
  jmp irq_common_stub
