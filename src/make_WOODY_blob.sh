#!/bin/sh
nasm -f elf64 WOODYcode.s -o WOODYcode.o
objcopy -j.text -O binary WOODYcode.o WOODY_blob.bin
