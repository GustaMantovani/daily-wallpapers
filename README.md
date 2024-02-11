# Daily Wallpapers
This repository contains a simple script to dynamically and automatically change GNOME wallpapers by cycling through images in a folder.

## Proposal
Unlike Windows, GNOME does not support selecting multiple images to serve as desktop backgrounds. Therefore, I decided to create a script that, to some extent, would make this possible.

## Approach
The project aims not to be inconvenient or inconsistent, allowing users to still create, move, or delete files in the directory containing the images that will serve as wallpaper.

The overall functioning of the code involves managing the current wallpaper state, selecting the next wallpaper from a directory specified by the user, and interacting with GNOME environment settings.

Upon execution, the script checks if there is a record of the current wallpaper path in a file called `curr_wpp`. If there is no registered path, the script selects the first valid image file in the directory provided by the user. This path is then set as the wallpaper and registered in the `curr_wpp` file.

If there is already a registered path, the script searches for the next valid image file in the directory, maintaining a cyclic sequence. The idea is to provide continuous and automatic rotation of wallpapers, ensuring visual variety on the desktop.

To interact with GNOME environment settings, the script utilizes the `gsettings` tool to change both the wallpaper to light mode and dark mode.

The use of the `libmagic` library is incorporated into the code to identify image files in the directory. This is essential to prevent unrelated files from being considered as wallpapers. At this point, the `magic` library is used to read the header of files in the directory indicated by the user, checking the MIME-type, which indicates the type of data sets the file represents. This approach goes beyond merely checking the file extension, a technique involving string manipulation, which may not always reflect the true type of the file.

The script also includes measures for user feedback. In case of problems reading the current path or if it is not possible to access the wallpaper path, notifications are displayed using the `notify-send` command.

The program is executed through the `run.sh` script, which exports the necessary graphical environment variables for run-level 3 and serves the directory path chosen by the user, information that is saved in the `dw_wpps_path` file, in the standard input.

## Automation and Scheduling
The project implements two scheduling mechanisms for automatically executing the daily wallpaper change: Anacron and GNOME Autostart.

### Anacron:

Anacron is a task scheduler that differs from traditional Cron in handling machines that may not always be on. It is particularly useful for ensuring that periodic tasks are executed even if the system is turned off for part of the time. In the project, Anacron is used to schedule the daily execution of the `run.sh` script, which, in turn, invokes the main C program to perform the wallpaper change.

The `anacrontab` file contains the specific configuration for this task. The line `1 0 wallpaper.daily ~/.dw/run.sh` in the `anacrontab` defines that the `run.sh` script will be executed daily at midnight (1 hour and 0 minutes), associated with the task named "wallpaper.daily". Thus, even if the system is off at midnight, Anacron will ensure that the automatic wallpaper change is performed as soon as the system is started again.

### GNOME Autostart:

GNOME Autostart is a mechanism that allows applications and scripts to be automatically executed during the GNOME graphical environment startup process. The `daily-wallpaper.desktop` file is placed in the `~/.config/autostart/` folder, instructing GNOME to start the `run.sh` script during startup.

This startup file, using the `[Desktop Entry]` entry, specifies the execution of Anacron when initializing the GNOME session. Anacron, in turn, through the command `usr/sbin/anacron -s -t $HOME/.anacron/etc/anacrontab -S $HOME/.anacron/spool`, checks if the job that changes the wallpaper has already been executed on the last day. Thus, the automatic wallpaper change also occurs immediately after logging into the graphical environment, providing a seamless and automated experience.

## Usability
As a user, you only need to run the `install.sh` script, after which you will be prompted to provide the full path to the directory with the images you want to alternate as your background. After that, the script will be up and running.

If you want to change the current wallpaper manually, you can run the command `~/.dw/run.sh`.

## Notes
- It is important to provide the full path correctly (including '/' at the end; e.g., /home/user/Pictures/). This is a limitation that would be too much work to resolve, and my goal was to develop a simple project.
- Due to its simplicity, the program has some limitations. Any changes to the current image in the directory specified for the program may cause some inconsistency in image switching; however, it will not interrupt or disrupt the wallpaper cycle. It may just restart the cycle from the first candidate in the pointed directory. However, the cycle will reestablish and function normally.
- There is, however, a single technical limitation that may impact script execution, related to graphical environment variables in Linux run-level 5, as mentioned in the `run.sh` script of the project's source code.
- To solve this problem, these graphical environment variables are imported into the `run.sh` script. Despite not being loaded at the level in question, they are still accessible. However, note that the path of these variables may vary between different systems. If you encounter issues, you can check the value of these variables on your system through a terminal. Replace the values in the `~/.dw/run.sh` file as needed. To learn more about run-levels, I recommend watching Fábio Akita's video: [Como Funciona o Boot de um Linux](https://youtu.be/5F6BbhgvFOE?si=uMYG04_Ye5bLE9Gf&t=2011), where he addresses the subject in question.
- The project provides only two scheduling options for script execution times. This is because manipulating task scheduling configuration files in Linux can be complicated due to their sensitive configuration nature on many systems. Still, you can customize these schemes using the same tools the project uses. For example, if you want to change wallpaper switching based on a time 't' in hours, days, or weeks, you can edit the `/etc/crontab` file as you prefer. To learn more about cron, read the tool's manual on your Linux system (`cron(8)`).
- If you want to change the settings so that the script is executed considering that the computer will not be on all the time, you can edit the `~/.anacron/etc/anacrontab` file or change the script that edits it (`config.sh`). To learn more about Anacron, read the tool's manual on your Linux system (`anacron(8)`).
- The scheduling method using cron was not mentioned because it is very simple and easy to understand. You simply add an entry to execute the `run.sh` script in some crontab.