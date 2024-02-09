#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"


// TODO: function definitions here for ls2
//emp = exact pattern match
void ls2(char* path, char* emp, int indent){
    if(emp == NULL){//MODE 1
    DIR* currDir = opendir(path);
    if(currDir == NULL){//load dir and check it
        printf("This directory doesnt exist\n");
        return;
    }
    struct dirent *currPath;
    while((currPath = readdir(currDir)) != NULL){//
        if(strcmp(".", currPath->d_name) != 0 && strcmp("..", currPath->d_name) != 0){
            struct stat statBuf;//statbuf for using lstat
            
            char *pathName = (char*)malloc((strlen(path) + strlen(currPath->d_name) + 2) * sizeof(char));//+2 for the null and /
            strcpy(pathName, path);
            strcat(pathName, "/");
            strcat(pathName, currPath->d_name);//putting pathname together

            if(lstat(pathName, &statBuf) == -1){
                perror("lstat");
                return;
            }
    
            if(S_ISREG(statBuf.st_mode)){//its a file
                for(int i = 0; i < indent; i++){
                    printf("%s",INDENT);
                }
                printf("%s (%ld bytes)\n",currPath->d_name, statBuf.st_size);
                free(pathName);
            }
            else if(S_ISDIR(statBuf.st_mode)){//its a directory
                for(int i = 0; i < indent; i++){
                    printf("%s",INDENT);
                }
                printf("%s (directory)\n", currPath->d_name);
                ls2(pathName, NULL, indent + 1);//go into directory
                free(pathName);
            }
        }
    }
    
    closedir(currDir);//close directory
    }
    else if(emp != NULL){// MODE 2

    }

}


