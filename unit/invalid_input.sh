#!/bin/bash

error_exit() {
	echo "$1" && exit 1 
}

head -c 5042 /dev/urandom > noise;
./woody_woodpacker noise && error_exit "[ KO ] Invalid file was not invalidated." || echo "[ OK ] Invalid file was correctly rejected."
rm noise

