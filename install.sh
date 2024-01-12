#!/bin/bash
gcc daily-wallpapers.c -lmagic

mkdir ~/.dw
cp run.sh ~/.dw
cp a.out ~/.dw

./config.sh


