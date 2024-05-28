#!/bin/sh

error_exit() {
	echo "$1" && exit 1 
}

gcc -m32 -o sample32 res/sample.c || \
	error_exit "To compile 32bit, Linux depends on \`libc6-dev-i386\`."

./woody_woodpacker sample32 && \
	echo "[ KO ] It should have rejected the 32bit file." || \
	echo "[ OK ] The program rejected a 32bit binary."
