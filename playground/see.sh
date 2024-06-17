#!/bin/sh

objdump -d $1
readelf -a $1
../elfsdump/elfsdump $1
