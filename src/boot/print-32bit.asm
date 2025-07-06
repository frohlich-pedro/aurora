bits 32
VIDEO_MEMORY	equ	0xb8000
WHITE_ON_BLACK	equ	0xf

print32:
	pusha
	mov	edx,	VIDEO_MEMORY

print32_loop:
	mov	al,	[ebx]
	mov	ah,	WHITE_ON_BLACK

	cmp	al,	0x0
	je	print32_done

	mov	[edx],	ax
	add	ebx,	0x1
	add	edx,	0x2

	jmp	print32_loop

print32_done:
	popa
	ret
