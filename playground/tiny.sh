#!/bin/sh

gcc tinyA.c -o A
gcc tinyB.c -o B
./A && ./B
echo
gcc return.c -o return

cd ..
make
./woody_woodpacker playground/return
echo
