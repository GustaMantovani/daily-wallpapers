#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <magic.h>
#include <string.h>
#include <dirent.h>

FILE * abriRegistroDw(const char *modo){
  FILE *arq;
  char *home = getenv("HOME");
  char *record_path;

  record_path = (char *)malloc(strlen(home)+strlen("/.dw/curr_wpp")+1);

  strcpy(record_path,home);
  strcat(record_path,"/.dw/curr_wpp");
  
  arq = fopen(record_path, modo);

  free(record_path);
  return arq;
}

void lerWallpaperAtual(char *wppAtual){
  FILE *arq;
  arq=abriRegistroDw("r");
  if(arq==NULL){
    strcpy(wppAtual, "\0");
    return;
  }else{
    if(fgets(wppAtual,PATH_MAX,arq)==NULL) strcpy(wppAtual, "\0"); 
    fclose(arq);
  }  
}

void trocarWallpaper(char *wppPath){
  char cmd[ARG_MAX];

  //modo dark
  strcpy(cmd, "gsettings set org.gnome.desktop.background picture-uri-dark ");
  strcat(cmd, wppPath);
  system(cmd);

  //modo claro
  strcpy(cmd, "gsettings set org.gnome.desktop.background picture-uri ");
  strcat(cmd, wppPath);
  system(cmd);
}

void setarPrimeiroCandidatoWpp(char *path){
  DIR *dir = opendir(path);
  char pathMoreDir[PATH_MAX],*mimeTypeOnly;
  magic_t magic_cookie;
  FILE *arq;

  struct dirent *entrada;
  magic_cookie = magic_open(MAGIC_MIME_TYPE);
  magic_load(magic_cookie, NULL);

  while ((entrada = readdir(dir)) != NULL) {
    if(entrada->d_type==8){

      strcpy(pathMoreDir, path);
      strcat(pathMoreDir, entrada->d_name);
      mimeTypeOnly = strtok(magic_file(magic_cookie, pathMoreDir),"/");

      if(!strcmp(mimeTypeOnly, "image")){
        trocarWallpaper(pathMoreDir);

        arq = abriRegistroDw("w");
        if(arq==NULL){
          system("notify-send -t 3000 'Daily Wallpaper' 'Não foi possível acessar o path do wallpaper atual'");
          exit(-1);
        }

        fprintf(arq, "%s", pathMoreDir);

        fclose(arq);
        magic_close(magic_cookie);
        closedir(dir);
        return;
      }
    }   
  }
  magic_close(magic_cookie);
  closedir(dir);
}

void setarProximoCandidatoWpp(char *path, char *pathWppAtual){
  DIR *dir = opendir(path);
  char pathMoreDir[PATH_MAX],*mimeTypeOnly;
  magic_t magic_cookie;
  FILE *arq;

  struct dirent *entrada;
  magic_cookie = magic_open(MAGIC_MIME_TYPE);
  magic_load(magic_cookie, NULL);

  while ((entrada = readdir(dir)) != NULL) {
    if(entrada->d_type==8){

      strcpy(pathMoreDir, path);
      strcat(pathMoreDir, entrada->d_name);
      mimeTypeOnly = strtok(magic_file(magic_cookie, pathMoreDir),"/");

      if(!strcmp(pathMoreDir, pathWppAtual) && !strcmp(mimeTypeOnly, "image")){
        
        //ler entradas até o próximo mime type image, então setar essa entrada
        while((entrada = readdir(dir))!=NULL){
          if(entrada->d_type==8){
            strcpy(pathMoreDir, path);
            strcat(pathMoreDir, entrada->d_name);
            mimeTypeOnly = strtok(magic_file(magic_cookie, pathMoreDir),"/");

            if(!strcmp(mimeTypeOnly, "image")){
              trocarWallpaper(pathMoreDir);
             
              arq = abriRegistroDw("w");
              if(arq==NULL){
                system("notify-send -t 3000 'Daily Wallpaper' 'Não foi possível acessar o path do wallpaper atual'");
                exit(-1);
              }

              fprintf(arq, "%s", pathMoreDir);

              magic_close(magic_cookie);
              closedir(dir);
              fclose(arq);
              return;
            } 
          }
        }
        setarPrimeiroCandidatoWpp(path);
        magic_close(magic_cookie);
        closedir(dir);
        return;
      }
    }   
  }
  setarPrimeiroCandidatoWpp(path);
  magic_close(magic_cookie);
  closedir(dir);
}

int main(){
  char wallpaperAtual[PATH_MAX];
  char path[PATH_MAX];

  //stdin
  fgets(path,PATH_MAX,stdin);
  path[strlen(path)-1]='\0';

  lerWallpaperAtual(wallpaperAtual);

  if(strlen(wallpaperAtual)==0){
    //setar o primeiro candidato como wallpaper
    setarPrimeiroCandidatoWpp(path);
  }else{
    //buscar na lista e setar o próximo (caso seja o primeiro ou não esteja mais na lista, setar o primeiro)
    setarProximoCandidatoWpp(path, wallpaperAtual);
  }

  return 0;
}