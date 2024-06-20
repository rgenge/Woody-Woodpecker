#!/bin/sh
unset COLORMODE
echo "sample64" > a
elfsdump/elfsdump samples/return64 >> a
echo "woody" > b
elfsdump/elfsdump ./woody >> b
