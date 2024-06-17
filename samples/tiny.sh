#!/bin/sh

mkdir -p dumps

gcc -m64 tinyA.c -o A64
gcc -m64 tinyB.c -o B64
echo -n "Test A B 64: "
./A64 && ./B64
echo

gcc -m32 tinyA.c -o A32
gcc -m32 tinyB.c -o B32
echo -n "Test A B 32: "
./A32 && ./B32
echo

gcc -m64 return.c -o return64
echo -n "Test return 64 (42): "
./return64
echo $?;

gcc -m32 return.c -o return32
echo -n "Test return 32 (42): "
./return32
echo $?;
