global _start
[bits 32]

section .multiboot
align 4
  dd 0x1BADB002
  dd 0x00
  dd -(0x1BADB002 + 0x00)

section .text
_start:
  [extern start_kernel]
  call start_kernel
  jmp $
