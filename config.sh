#!/bin/bash

caminho=""

while [ ! -d "$caminho" ]; do
    echo "Digite um caminho válido:"
    read caminho

    if [ ! -d "$caminho" ]; then
        echo "O caminho inserido não é válido. Tente novamente."
    fi
done

echo $caminho > ~/.dw/dw.conf

(crontab -l 2>/dev/null; echo "0 0 * * * ~/.dw/run.sh") | crontab -
~/.dw/run.sh
