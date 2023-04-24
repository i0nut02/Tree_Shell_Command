#ifndef SORT_STRINGS_H
#define SORT_STRINGS_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "create_visit_array.h"


struct sort_conditions{
    int order; //ascending = +1, descending = -1
    int alphabetic_order; // if = -1 then is sort by last modification time
    int dirs_first;
};

void set_default_sort_conditions(struct sort_conditions *sort_cond){
    sort_cond->alphabetic_order = 0;
    sort_cond->dirs_first = -1;
    sort_cond->order = +1;
}


void swap(char ***array, int i, int j){
    char temp[strlen((*array)[i])];
    strcpy(temp, (*array)[i]);
    strcpy((*array)[i], (*array)[j]);
    strcpy((*array)[j], temp);
}


int compare(char *s1, char *s2, struct sort_conditions *sort_cond){
    if (sort_cond->dirs_first == -1 || isFile(s1) == isFile(s2)){
        if (strcmp(s1, s2) == 0) {
            return 0; // same mtime and alphabetic order;
        }

        if (sort_cond->alphabetic_order != -1){
            //printf("s1: %s, s2: %s\n", s1, s2);
            return strcmp(s1, s2) * sort_cond->order;
        }

        else {
            struct stat fileStat1;
            struct stat fileStat2;
            if (stat(s1, &fileStat1) < 0 || stat(s2, &fileStat2)){
                printf("I try to acces the information abount this file: %s or %s", s1, s2);
                exit(0);
                return 0;
            }

            time_t date1, date2;
            date1 = fileStat1.st_mtimespec.tv_sec;
            date2 = fileStat2.st_mtimespec.tv_sec;

            if (difftime(date1, date2) == 0){
                return 0;
            }
            else if (difftime(date1, date2) > 0){
                return sort_cond->order;
            }
            else{ return -1 * sort_cond->order; }
        }

    } else{
        if (isFile(s1) == 0){
            return -1; // dirs first
        }
        return 1;
    }
}


void quick_sort(char ***array, int start, int size, 
                int random_num, struct sort_conditions *sort_cond){

    if (size-start <= 1){
        return;
    }

    int index_pivot = random_num % (size-start) + start;
    char pivot[strlen((*array)[index_pivot])];
    strcpy(pivot, (*array)[index_pivot]);
    int j = start;

    for (int i = start; i < size; i++){
        if (compare((*array)[i], pivot, sort_cond) < 0 && i != j && i !=index_pivot){
            swap(array, i, j);
            j++;
        }
        if (compare((*array)[j], pivot, sort_cond) < 0){
            j++;
        }
    }
    
    if (j >= size | j == start){
        quick_sort(array, start, size, random_num +1, sort_cond);
    }
    else{
        quick_sort(array, start, j, random_num, sort_cond);
        quick_sort(array, j, size, random_num, sort_cond);
    }
}

void sort_array(char ***array, int *size, struct sort_conditions *sort_cond){
    quick_sort(array, 0, *size, 0, sort_cond);
}

#endif