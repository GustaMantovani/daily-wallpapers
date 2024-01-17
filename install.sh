#!/bin/bash
gcc daily-wallpapers.c -lmagic

mkdir ~/.dw
mkdir -p ~/.anacron/{etc,spool}
cp run.sh ~/.dw
cp a.out ~/.dw

./config.sh


