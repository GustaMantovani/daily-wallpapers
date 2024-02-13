#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <magic.h>
#include <string.h>
#include <dirent.h>

FILE * openDwRecord(const char *mode){
  FILE *file;
  char *home = getenv("HOME");
  char *record_path;

  record_path = (char *)malloc(strlen(home)+strlen("/.dw/curr_wpp")+1);

  strcpy(record_path,home);
  strcat(record_path,"/.dw/curr_wpp");
  
  file = fopen(record_path, mode);

  free(record_path);
  return file;
}

void readCurrentWallpaper(char *currentWallpaper){
  FILE *file;
  file = openDwRecord("r");
  if(file == NULL){
    strcpy(currentWallpaper, "\0");
    return;
  } else {
    if(fgets(currentWallpaper, PATH_MAX, file) == NULL) 
      strcpy(currentWallpaper, "\0"); 
    fclose(file);
  }  
}

void changeWallpaper(char *wallpaperPath){
  char cmd[ARG_MAX];

  // dark mode
  strcpy(cmd, "gsettings set org.gnome.desktop.background picture-uri-dark ");
  strcat(cmd, wallpaperPath);
  system(cmd);

  // light mode
  strcpy(cmd, "gsettings set org.gnome.desktop.background picture-uri ");
  strcat(cmd, wallpaperPath);
  system(cmd);
}

void setFirstCandidateWallpaper(char *path){
  DIR *dir = opendir(path);
  char pathMoreDir[PATH_MAX], *mimeTypeOnly;
  magic_t magic_cookie;
  FILE *file;

  struct dirent *entry;
  magic_cookie = magic_open(MAGIC_MIME_TYPE);
  magic_load(magic_cookie, NULL);

  while ((entry = readdir(dir)) != NULL) {
    if(entry->d_type == 8){

      strcpy(pathMoreDir, path);
      strcat(pathMoreDir, entry->d_name);
      mimeTypeOnly = strtok(magic_file(magic_cookie, pathMoreDir),"/");

      if(!strcmp(mimeTypeOnly, "image")){
        changeWallpaper(pathMoreDir);

        file = openDwRecord("w");
        if(file == NULL){
          system("notify-send -t 3000 'Daily Wallpaper' 'Could not access the current wallpaper path'");
          exit(-1);
        }

        fprintf(file, "%s", pathMoreDir);

        fclose(file);
        magic_close(magic_cookie);
        closedir(dir);
        return;
      }
    }   
  }
  magic_close(magic_cookie);
  closedir(dir);
}

void setNextCandidateWallpaper(char *path, char *currentWallpaperPath){
  DIR *dir = opendir(path);
  char pathMoreDir[PATH_MAX], *mimeTypeOnly;
  magic_t magic_cookie;
  FILE *file;

  struct dirent *entry;
  magic_cookie = magic_open(MAGIC_MIME_TYPE);
  magic_load(magic_cookie, NULL);

  while ((entry = readdir(dir)) != NULL) {
    if(entry->d_type == 8){

      strcpy(pathMoreDir, path);
      strcat(pathMoreDir, entry->d_name);
      mimeTypeOnly = strtok(magic_file(magic_cookie, pathMoreDir),"/");

      if(!strcmp(pathMoreDir, currentWallpaperPath) && !strcmp(mimeTypeOnly, "image")){
        
        // read entries until the next image mime type, then set that entry
        while((entry = readdir(dir)) != NULL){
          if(entry->d_type == 8){
            strcpy(pathMoreDir, path);
            strcat(pathMoreDir, entry->d_name);
            mimeTypeOnly = strtok(magic_file(magic_cookie, pathMoreDir),"/");

            if(!strcmp(mimeTypeOnly, "image")){
              changeWallpaper(pathMoreDir);
             
              file = openDwRecord("w");
              if(file == NULL){
                system("notify-send -t 3000 'Daily Wallpaper' 'Could not access the current wallpaper path'");
                exit(-1);
              }

              fprintf(file, "%s", pathMoreDir);

              magic_close(magic_cookie);
              closedir(dir);
              fclose(file);
              return;
            } 
          }
        }
        setFirstCandidateWallpaper(path);
        magic_close(magic_cookie);
        closedir(dir);
        return;
      }
    }   
  }
  setFirstCandidateWallpaper(path);
  magic_close(magic_cookie);
  closedir(dir);
}

int main(){
  char currentWallpaper[PATH_MAX];
  char path[PATH_MAX];

  // stdin
  fgets(path, PATH_MAX, stdin);
  path[strlen(path)-1] = '\0';

  readCurrentWallpaper(currentWallpaper);

  if(strlen(currentWallpaper) == 0){
    // set the first candidate as wallpaper
    setFirstCandidateWallpaper(path);
  } else {
    // search the list and set the next (if it's the first or no longer in the list, set the first)
    setNextCandidateWallpaper(path, currentWallpaper);
  }

  return 0;
}