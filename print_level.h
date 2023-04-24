#ifndef PRINT_LEVEL_H
#define PRINT_LEVEL_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>

#define LENGHT_TYPE_LINE 11
#define MAX_LENGHT 128

extern int errno;

struct print_functionalities {
    int inodes;
    int group_owner;
    int user_owner;
    int file_size;
    int permission; // --- --- --- to print
    int only_content; // we have to print example.txt (only_content = 0) or path/example.txt (= -1)
    int date;
    char type_of_line[LENGHT_TYPE_LINE]; // we have to print ├── or └── before the name of the path
    int long_vertical_line[100];
    int num_files;
    int num_dirs;
};

void set_default_print_functionalities(struct print_functionalities *print_func){
    print_func->file_size = -1; // 11 chars
    print_func->group_owner = -1; // min 8 chars
    print_func->inodes = -1; // 8 chars
    print_func->only_content = 0; 
    print_func->permission = -1; // 10 chars
    print_func->user_owner = -1; //min 8 chars 
    print_func->date = -1; // 12 [ 3 month, 2 day, 5 xy:zk]
    print_func->num_dirs = 0;
    print_func->num_files = 0;
    for (int i = 1; i < 100; i++){
        print_func->long_vertical_line[i] = -1;
    }
    strcpy(print_func->type_of_line, "\0");
}

void print_information_visit(struct print_functionalities *print_func){
    printf("\n %d directories, %d files", print_func->num_dirs, print_func->num_files);
}

void set_name(char* fullpath, char *dest, struct print_functionalities *print_func){
    if (print_func->only_content == -1){
        stpcpy(dest, fullpath);
        return;
    }
    int i, size;
    size = strlen(fullpath);
    int last = -1;

    for (i = 0; i < size; i++){
        if (*(fullpath + i) == '/'){
            last = i;
        }
    }
    // +1 and -1 are to not save the "/"
    for (i = 0; i < size - last -1; i++){
        *(dest+i) = *(fullpath + last +i +1);
    }
    return;
}

void add_permission_info(struct stat *fileStat){
    if (S_ISDIR(fileStat->st_mode)){
        printf("d");
    } else { printf("-");}

    if (fileStat->st_mode & S_IRUSR){
        printf("r");
    } else { printf("-");}

    if (fileStat->st_mode & S_IWUSR){
        printf("w");
    } else { printf("-");}

    if (fileStat->st_mode & S_IXUSR){
        printf("x");
    } else { printf("-");}

    if (fileStat->st_mode & S_IRGRP){
        printf("r");
    } else { printf("-");}

    if (fileStat->st_mode & S_IWGRP){
        printf("w");
    } else { printf("-");}

    if (fileStat->st_mode & S_IXGRP){
        printf("x");
    } else { printf("-");}

    if (fileStat->st_mode & S_IROTH){
        printf("r");
    } else { printf("-");}

    if (fileStat->st_mode & S_IWOTH){
        printf("w");
    } else { printf("-");}

    if (fileStat->st_mode & S_IXOTH){
        printf("x");
    } else { printf("-");}

}

void clear_time(char *dest, char *time){
    // ctime format: "DayName MonthName Hour:Minutes:Seconds ....."
    for (int i = 4; i < 16; i++){
        *(dest +i-4) = *(time +i);
    }
}


void print_requested_information(char *fullpath, struct print_functionalities *print_func){
    int i = 0;
    struct stat fileStat;
    
    int errnum;
    if (stat(fullpath, &fileStat) != 0){
        errnum = errno;
        printf("I try to acces the information abount this file: %s\nError value: %d", fullpath, errnum);
        exit(EXIT_FAILURE);
        return;
    }

    if (print_func->inodes != -1){
        if (i > 0){
            printf(" ");
        } else {printf("[");}

        printf("%llu", fileStat.st_ino);
        i++;
    }

    if (print_func->permission != -1){
        if (i > 0){
            printf(" ");
        } else {printf("[");}

        add_permission_info(&fileStat);
        i++;
    }

    if (print_func->user_owner != -1){
        if (i > 0){
            printf(" ");
        } else {printf("[");}

        struct passwd *pws;
        pws = getpwuid(fileStat.st_uid);
        printf("%s\t", pws->pw_name);
        i++;
    }

    if (print_func->group_owner != -1){
        if (i > 0){
            printf(" ");
        } else {printf("[");}

        struct group *g;
        g = getgrgid(fileStat.st_gid);
        printf("%s\t", g->gr_name);
        i++;
    }

    if (print_func->file_size != -1){
        if (i > 0){
            printf(" ");
        } else {printf("[");}

        printf("%lld", fileStat.st_size);
        i++;
    }

    if (print_func->date != -1){
        if (i > 0){
            printf(" ");
        } else {printf("[");}

        char s[13];
        char mtime[30];
        strcpy(mtime, (char*)ctime(&(fileStat.st_mtimespec)));
        clear_time(s, mtime);
        printf("%s", s);
        i++;
    }

    if (i > 0){
        printf("]  ");
    }
}


void print_content(char *fullpath, int *lvl, struct print_functionalities *print_func){
    char content[strlen(fullpath) +1];
    memset(content, '\0', sizeof(char) * (strlen(fullpath) +1));
    set_name(fullpath, content, print_func);

    int i;
    // at level 0 and 1 we will not print
    for (i = 0; i < 3 * (*lvl -1); i++){
        if (i % 3 == 0){
            if (print_func->long_vertical_line[i / 3] != -1){
                printf("│");
            }
            else {printf(" ");}
        }
        printf(" ");
    }

    printf("%s", print_func->type_of_line);

    print_requested_information(fullpath, print_func);

    printf("%s\n", content);

}

#endif