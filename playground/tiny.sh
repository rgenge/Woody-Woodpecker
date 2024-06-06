#!/bin/sh

gcc tinyA.c -o A
gcc tinyB.c -o B
./A && ./B
echo
cd ..
make
./woody_woodpacker playground/A
