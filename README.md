# Aurora Kernel

A ground-up operating system kernel written from scratch. Aurora is an independent project—not based on Linux, BSD, or any existing codebase. The name honors my cat Aurora, who was my constant companion during late coding sessions.

## Features

- **Custom architecture**: Built entirely from scratch with original design decisions
- **Minimalist approach**: Focus on clean, understandable code over feature bloat  
- **Educational**: Designed to explore fundamental OS concepts without legacy constraints
- **Active development**: Continuously evolving as a learning platform

## Getting Started

### Prerequisites
```
qemu-system-i386    # Virtualization
nasm                # Assembler
gcc                 # C compiler  
make (gmake in BSD) # Build system
```

### Building & Running
```bash
# Build and launch Aurora
make all

# Clean build artifacts
make clean
```

## Project Goals

Aurora aims to be a testbed for experimenting with kernel design patterns and system programming concepts. This is a solo project by a 17-year-old developer, so don't expect production-ready stability—but do expect genuine passion for systems programming and continuous improvement.

## Development Philosophy

- **Learning over perfection**: Every bug is a lesson, every feature a chance to grow
- **Performance first**: Performance over readability, i'm tired of the other way, x86 all the way. What you gonna do about it???
- **Experimental**: Not afraid to try unconventional approaches
- **Community-driven feedback**: Constructive criticism helps Aurora evolve

## Contributing

Feedback, suggestions, and constructive criticism are welcome. This project thrives on learning from the community.

---

*In Memory of Aurora*
