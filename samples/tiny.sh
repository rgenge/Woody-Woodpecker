#!/bin/sh

mkdir -p dumps

gcc -m64 return.c -o return64
echo -n "Test return 64 (42): "
./return64
echo $?;

gcc -m64 sample.c -o sample64
echo -n "Test sample64: "
./sample64
