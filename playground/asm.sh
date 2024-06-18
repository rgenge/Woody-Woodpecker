#!/bin/sh

nasm -f elf64 -O3 $1.s -o $1.o
ld $1.o -s -o $1
strip -s $1
