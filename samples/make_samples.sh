#!/bin/sh
gcc -m64 return.c -o return42
gcc -m64 sample.c -o sample
gcc -m64 sample_complex.c -o sample_complex
