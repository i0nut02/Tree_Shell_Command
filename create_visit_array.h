#ifndef CREATE_VISIT_ARRAY_H
#define CREATE_VISIT_ARRAY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h> 
#include <errno.h>


#define MAX_INT 100

extern int errno;


struct visit_conditions{
    int max_level;
    int only_dirs;
    int dot_files;
};

void set_default_visit_conditions(struct visit_conditions *visit_cond){
    visit_cond->dot_files = -1;
    visit_cond->max_level = MAX_INT;
    visit_cond->only_dirs = -1;
}

void get_new_path(char *dest, char *src_path, char *content){
    strcpy(dest, src_path);
    strcat(dest, "/");
    strcat(dest, content);
}

int isFile(const char* name){
    int errnum;
    DIR* directory = opendir(name);
    errnum = errno;

    if(directory != NULL){
        closedir(directory);
        return 0;
    }
    
    if(errnum == ENOTDIR){
        return 1;
    }
    printf("I can't open this file: %s\nError value: %d", name, errnum);
    exit(EXIT_FAILURE);
}


void get_dimension(char *fullpath, int *size, int *string_lenght, struct visit_conditions *visit_cond){
    *size = 0;
    *string_lenght = 0;
    DIR *d;
    struct dirent *dir;
    int errnum;

    d = opendir(fullpath);
    
    if (d == NULL){
        errnum = errno;
        printf("Value of error: %d, happening opening %s as dir", errnum, fullpath);
    }

    while (( dir = readdir(d) ) != NULL) {
        char new_full_path[strlen(fullpath) + 1 + strlen(dir->d_name)];
        get_new_path(new_full_path, fullpath, dir->d_name);

        if ((visit_cond->dot_files != -1 | strncmp(dir->d_name, ".", 1) != 0) & 
            (visit_cond->only_dirs == -1 | isFile(new_full_path) == 0)){
            if (dir->d_namlen > *string_lenght){
                *string_lenght = dir->d_namlen;
            }
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
                *size += 1;
            }
        }
    }
    *string_lenght += 1; // we have to add also \0
}

void add_elements(char *fullpath, char ***array, int *size, int *string_lenght, struct visit_conditions *visit_cond){
    DIR *d;
    struct dirent *dir;
    int errnum;

    *array = (char**)calloc(*size, sizeof(char*));
    d = opendir(fullpath);
    
    if (d == NULL){
        errnum = errno;
        printf("Value of error: %d, happening opening %s as dir", errnum, fullpath);
    }

    int i = 0;
    while (( dir = readdir(d) ) != NULL){
        char new_full_path[strlen(fullpath) + 1 + strlen(dir->d_name)];
        get_new_path(new_full_path, fullpath, dir->d_name);

        if ((visit_cond->dot_files != -1 | strncmp(dir->d_name, ".", 1) != 0) &
            (visit_cond->only_dirs == -1 | isFile(new_full_path) == 0)){

            if (strcmp(dir->d_name, ".") != 0 & strcmp(dir->d_name, "..") != 0){
                (*array)[i] = (char*)calloc(*string_lenght, sizeof(char));
                memset( (*array)[i], '\0', sizeof(char) * *string_lenght);
                strcpy((*array)[i], dir->d_name);
                i++;
            }
        }
    }
}

#endif