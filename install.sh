#!/bin/bash

mkdir -p build
gcc -O3 -std=c11 -pedantic -s src/daily-wallpapers.c -lmagic -o build/daily-wallpapers.bin

mkdir ~/.dw
mkdir -p ~/.anacron/{etc,spool}
cp essential/run.sh ~/.dw
cp build/daily-wallpapers.bin ~/.dw

scripts/config.sh