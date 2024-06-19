#!/bin/sh
nasm -f elf64 WOODYcode.s -o WOODYcode64.o
ld -s WOODYcode64.o -o WOODYcode_elf
objcopy -j.text -O binary WOODYcode64.o WOODY_blob_64.bin
xxd -p -c 8 WOODY_blob_64.bin
