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

hexdump A32 > dumps/A32_hexdump
objdump -a A32 > dumps/A32_objdump
readelf -a A32 > dumps/A32_readelf

hexdump A64 > dumps/A64_hexdump
objdump -a A64 > dumps/A64_objdump
readelf -a A64 > dumps/A64_readelf

hexdump B32 > dumps/B32_hexdump
objdump -a B32 > dumps/B32_objdump
readelf -a B32 > dumps/B32_readelf

hexdump B64 > dumps/B64_hexdump
objdump -a B64 > dumps/B64_objdump
readelf -a B64 > dumps/B64_readelf

hexdump return32 > dumps/return32_hexdump
objdump -a return32 > dumps/return32_objdump
readelf -a return32 > dumps/return32_readelf

hexdump return64 > dumps/return64_hexdump
objdump -a return64 > dumps/return64_objdump
readelf -a return64 > dumps/return64_readelf

cd ..

hexdump sample > playground/dumps/sample_hexdump
objdump -a sample > playground/dumps/sample_objdump
readelf -a sample > playground/dumps/sample_readelf

exit;
make
./woody_woodpacker playground/return64
echo
