#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"


// TODO: function definitions here for ls2
void ls2(char* path, int indent){
    DIR* currDir = opendir(path);
    if(currDir == NULL){//load dir and check it
        printf("This directory doesnt exist\n");
        return;
    }
    struct dirent *currPath;
    while((currPath = readdir(currDir)) != NULL){//
        if(strcmp(".", currPath->d_name) != 0 && strcmp("..", currPath->d_name) != 0){
            struct stat statBuf;
            

            char *pathName = (char*)malloc((strlen(path) + strlen(currPath->d_name) + 2) * sizeof(char));//+2 for the null and /
            strcpy(pathName, path);
            strcat(pathName, "/");
            strcat(pathName, currPath->d_name);

            if(lstat(currPath->d_name, &statBuf) == -1){
                perror("lstat");
                return;
            }
            if(S_ISREG(statBuf.st_mode)){//its a file
                //printf("its a file");
                for(int i = 0; i < indent; i++){
                    printf("%s",INDENT);
                }
                printf("%s (%ld bytes)\n",currPath->d_name, statBuf.st_size);
            }
            else if(S_ISDIR(statBuf.st_mode)){
                //printf("its a directory");
                for(int i = 0; i < indent; i++){
                    printf("%s",INDENT);
                }
                printf("%s (directory)\n", currPath->d_name);
                ls2(pathName, indent + 1);
            }
        }
    }

}


