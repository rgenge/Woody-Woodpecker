#!/bin/sh
unset COLORMODE
echo "$1" > a
objdump -d "$1" >> a
objdump -D "$1" >> a
readelf -a "$1" >> a
xxd "$1" >> a
echo "$2" > b
objdump -d "$2" >> b
objdump -D "$2" >> b
readelf -a "$2" >> b
xxd "$2" >> b
vimdiff a b
