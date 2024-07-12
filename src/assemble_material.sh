#!/bin/sh

nasm -f elf64 material.s -o material.o || exit
ld material.o -o buzz || exit # just for curiosity
objcopy -j.text -O binary material.o material.bin || exit
cp ./material.bin ..
