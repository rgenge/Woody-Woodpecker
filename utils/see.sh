#!/bin/sh

objdump -D $1
readelf -a $1
../elfsdump/elfsdump $1
