#!/bin/bash

path=""

while [ ! -d "$path" ]; do
    echo "Enter a valid path:"
    read path

    if [ ! -d "$path" ]; then
        echo "The entered path is not valid. Please try again."
    fi
done

echo $path > ~/.dw/dw_wpps_path

while true; do
    echo "Choose an option:"
    echo "1. Run the script every t minutes"
    echo "2. Execute the script every day"

    read option

    case $option in
        1)
            echo "Enter the value of t in minutes:"
            read t
            # Set up the cron job to run the script every t minutes
            (crontab -l ; echo "*/$t * * * * ~/.dw/run.sh") | crontab -
            echo "Configured to change wallpaper every $t minutes."
            ~/.dw/run.sh
            break
            ;;
        2)
            # Set up the cron job to run the script daily at midnight
            cp essential/anacrontab ~/.anacron/etc/
            cp essential/daily-wallpaper.desktop ~/.config/autostart/

            echo -e "\nExec=/usr/sbin/anacron -s -t $HOME/.anacron/etc/anacrontab -S $HOME/.anacron/spool" >> ~/.config/autostart/daily-wallpaper.desktop
            (crontab -l ; echo "0 0 * * * /usr/sbin/anacron -s -t $HOME/.anacron/etc/anacrontab -S $HOME/.anacron/spool") | crontab -
            echo "Configured to change wallpaper every day."
            /usr/sbin/anacron -s -t $HOME/.anacron/etc/anacrontab -S $HOME/.anacron/spool
            break
            ;;
        *)
            echo "Invalid option. Please try again."
            ;;
    esac
done