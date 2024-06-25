#!/bin/sh

# Use only .text section:
nasm -f elf64 WOODYcode.s -o WOODYcode.o
objcopy -j.text -O binary WOODYcode.o WOODY_blob.bin
cp ./WOODY_blob.bin ..

# Or use full ELF:
#nasm -f elf64 WOODYcode.s -o WOODYcode.o
#ld $1 WOODYcode.o -s -o WOODY_blob.bin
#strip -s WOODY_blob.bin
#cp ./WOODY_blob.bin ..

# Use full ELF:
#gcc ./buzz.c -o ./buzz
#cp ./buzz ../WOODY_blob.bin
