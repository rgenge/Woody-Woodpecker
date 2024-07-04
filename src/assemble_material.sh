#!/bin/sh

# Use only .text section:
nasm -f elf64 material.s -o material.o || exit
ld material.o -o buzz || exit
objcopy -j.text -j.data -O binary material.o material.bin || exit
cp ./material.bin ..

# Use of another full ELF TBI.
