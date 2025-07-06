void main() {
	unsigned char* vidmem_ptr = (unsigned char*) 0xb8000;
	unsigned char* vidmem_end = vidmem_ptr + (80 * 25 * 2);
cls:
	if (vidmem_ptr < vidmem_end) {
		*vidmem_ptr++ = ' ';
		*vidmem_ptr++ = 0xf;
		goto cls;
	}
	vidmem_ptr = (unsigned char*) 0xb8000;
	const char* string = "Hello, World!";
put:
	if (*string) {
		*vidmem_ptr++ = *string++;
		*vidmem_ptr++ = 0xf;
		goto put;
	}
hlt:
	goto hlt;
}
