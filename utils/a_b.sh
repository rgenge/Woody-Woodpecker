#!/bin/sh
unset COLORMODE
echo "$1" > a
objdump -d "$1" >> a
echo "$2" > b
objdump -d "$2" >> b
