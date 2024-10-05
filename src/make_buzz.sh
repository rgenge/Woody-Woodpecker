#!/bin/sh

nasm -f elf64 buzz_buzzard.s -o buzz_buzzard.o || exit
ld buzz_buzzard.o -o buzz || exit # just for curiosity
objcopy -j.text -O binary buzz_buzzard.o ../buzz_buzzard.bin || exit
