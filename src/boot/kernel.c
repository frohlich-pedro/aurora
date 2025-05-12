void main(void) {
    volatile char *video = (volatile char*)0xB8000;
    const char *msg = "Hello, World!";
    
    for (int i = 0; msg[i]; i++) {
        video[i*2] = msg[i];
        video[i*2+1] = 0x0A;  // Green text
    }
    
    while(1);
}
