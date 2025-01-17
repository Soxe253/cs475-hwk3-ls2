#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
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
        stack_t *list = initstack();
        mode2(list, path, emp, indent);//recursive fucntion for mode 2
        printstack(list);
        freestack(list);//give stack memory back
    }

}
//recursively goes through each directory in search of the given file of emp
bool mode2(stack_t *list, char* path, char* emp, int indent){
    DIR* currDir = opendir(path);
    if(currDir == NULL){//load dir and check it
        printf("This directory doesnt exist\n");
        return FALSE;
    }
    struct dirent *currPath;
    bool foundFile = FALSE;
    while((currPath = readdir(currDir)) != NULL){
        if(strcmp(".", currPath->d_name) != 0 && strcmp("..", currPath->d_name) != 0){
            struct stat statBuf;//statbuf for using lstat
            
            char *pathName = (char*)malloc((strlen(path) + strlen(currPath->d_name) + 2) * sizeof(char));//+2 for the null and /
            strcpy(pathName, path);
            strcat(pathName, "/");
            strcat(pathName, currPath->d_name);//putting pathname together

            if(lstat(pathName, &statBuf) == -1){
                perror("lstat");
                return FALSE;
            }
            
            if(S_ISREG(statBuf.st_mode)){//its a file
                if(strcmp(currPath->d_name,emp) == 0){
                    char intStr[30];
                    snprintf(intStr, sizeof(intStr), " (%ld bytes)", statBuf.st_size);//allocate space
                    char *currNameF = (char*)malloc((strlen(currPath->d_name) + (indent * strlen(INDENT)) + strlen(intStr) + 1) * sizeof(char));// 1 for null
                    currNameF[0] = '\0';//null terminator
                    for(int i = 0; i < indent; i++){
                    strcat(currNameF, INDENT);
                    }
                    strcat(currNameF, currPath->d_name);//put file name together with indents and bytes
                   
                    strcat(currNameF, intStr);
                    push(list, currNameF);
                    
                    foundFile = TRUE;//set the found file to true
                }
            }
            
            else if(S_ISDIR(statBuf.st_mode)){//its a directory
            char *directory = " (directory)";
            char *currNameD = (char*)malloc((strlen(currPath->d_name) + (indent * strlen(INDENT)) + strlen(directory) + 1) * sizeof(char));// 1 for null
                currNameD[0] = '\0';//null terminator
                foundFile = mode2(list, pathName, emp, indent+1);//was the file found down this path
                bool childFound;//boolean used to ensure another not found directory will change the foundfile variable
                if(foundFile){//file found?
                    for(int i = 0; i < indent; i++){
                    strcat(currNameD, INDENT);
                    }
                    strcat(currNameD, currPath->d_name);
                    strcat(currNameD, directory);//building directory name + directory
                    push(list, currNameD);
                    childFound = TRUE;//if the file is ever found down this path dont change it
                }
                else{
                    free(currNameD);//give mem back
                }
                if(childFound){
                    foundFile = TRUE;//set it back to be correct
                }
            }
            free(pathName);//give mem back
        }
    }
    closedir(currDir);//close dir
    return foundFile;//return if the file was found
}


