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


while true; do
    echo "Escolha uma opção:"
    echo "1. Rodar o script a cada t minutos"
    echo "2. Executar o script todo dia"

    read opcao

    case $opcao in
        1)
            echo "Digite o valor de t em minutos:"
            read t
            # Configurar o cron job para executar o script a cada t minutos
            (crontab -l ; echo "*/$t * * * * ~/.dw/run.sh") | crontab -
            echo "Configurado para mudar de wallpaper a cada $t minutos."
            break
            ;;
        2)
            # Configurar o cron job para executar o script sempre à meia-noite
            cp anacrontab ~/.anacron/etc/
            cp daily-wallpaper.desktop ~/.config/autostart/

            echo -e "\nExec=/usr/sbin/anacron -s -t $HOME/.anacron/etc/anacrontab -S $HOME/.anacron/spool" >> ~/.config/autostart/daily-wallpaper.desktop
            (crontab -l ; echo "0 0 * * * /usr/sbin/anacron -s -t $HOME/.anacron/etc/anacrontab -S $HOME/.anacron/spool") | crontab -
            echo "Configurado para trocar o wallpaper todo dia."
            break
            ;;
        *)
            echo "Opção inválida. Tente novamente."
            ;;
    esac
done

/usr/sbin/anacron -s -t $HOME/.anacron/etc/anacrontab -S $HOME/.anacron/spool
