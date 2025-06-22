global _start
[bits 32]

section .text
_start:
  [extern main]
  call main
  jmp $
