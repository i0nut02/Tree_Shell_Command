#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "create_visit_array.h"
#include "sort_strings.h"
#include "print_level.h"

#define MAX_PATH 260

void print_help_message(){ //TODO in print func...
   printf("\
         -a: Print files and directories wich start with dot\n \
        -d: Print and visit only directories\n \
        -f: Prints the full path prefix for each file\n \
        -p: Print the file type and permissions for each file\n \
        -s: Print the size of each file in bytes along with the name\n \
        -u: Print the username\n \
        -g: Print the group name\n \
        -D: Print the date of the last modification time for the file visited\n \
        -r: Sort the output in reverse alphabetic order\n \
        -t: Sort the output by last modification time instead of alphabetically\n \
        -L: Max display depth of the directory tree\n \
        --inodes: Prints the inode number of the file or directory\n \
        --dirsfirst: List directories before files\n");
}

void recursive_visit(char *fullpath, int *lvl, struct visit_conditions *visit_cond, 
                     struct sort_conditions *sort_cond, struct print_functionalities *print_func){

    print_content(fullpath, lvl, print_func);

    if (isFile(fullpath) != 0 || *lvl == visit_cond->max_level){ // if isFIle return 0 then fullpath is a directory
        print_func->num_files += 1;
        return;
    }

    print_func->num_dirs += 1;
    int size;
    int string_lenght;
    char **array;

    get_dimension(fullpath, &size, &string_lenght, visit_cond);

    add_elements(fullpath, &array, &size, &string_lenght, visit_cond);

    sort_array(&array, &size, sort_cond);

    *lvl += 1;

    if (size > 1){
        print_func->long_vertical_line[*lvl -1] = 0;
    }

    for(int i=0; i < size; i++){
        if ( ( (i == 0 && *lvl != 1 && size == 1 ) || (i == size-1))){
            strcpy(print_func->type_of_line, "└── ");
            print_func->long_vertical_line[*lvl -1] = -1;
        }
        else {
            strcpy(print_func->type_of_line, "├── ");
        }
        char new_path[strlen(fullpath) + 1 + strlen(array[i])];
        get_new_path(new_path, fullpath, array[i]);

        free(array[i]);
        recursive_visit(new_path, lvl, visit_cond, sort_cond, print_func);
    }

    free(array);
    *lvl -= 1;
}

int main(int argc, char *argv[]){
    char fullpath[MAX_PATH];
    int level = 0;

    struct visit_conditions visit_cond;
    struct sort_conditions sort_cond;
    struct print_functionalities print_func;

    set_default_sort_conditions(&sort_cond);
    set_default_visit_conditions(&visit_cond);
    set_default_print_functionalities(&print_func);

    strcpy(fullpath, ".");
    
    
    if (argc == 1){
        printf("Invalid input please insert tree\n");
        return 0;
    }
    
    if (strcmp(argv[1], "tree") != 0){
        printf("tree isn't the second word in the command line\n");
        return 0;
    }

    int indx = 2;
    
    while(indx < argc){
        if (strcmp(argv[indx], "--help") == 0){
            print_help_message();
            return 0;
        }
        else if (strcmp(argv[indx], "-a") == 0){
            visit_cond.dot_files = 0;
        }
        else if (strcmp(argv[indx], "-d") == 0){
            visit_cond.only_dirs = 0;
        }
        else if (strcmp(argv[indx], "-f") == 0){
            print_func.only_content = -1;
        }
        else if (strcmp(argv[indx], "-p") == 0){
            print_func.permission = 0;
        }
        else if (strcmp(argv[indx], "-s") == 0){
            print_func.file_size = 0;
        }
        else if (strcmp(argv[indx], "-u") == 0){
            print_func.user_owner = 0;
        }
        else if (strcmp(argv[indx], "-g") == 0){
            print_func.group_owner = 0;
        }
        else if (strcmp(argv[indx], "-D") == 0){
            print_func.date = 0;
        }
        else if (strcmp(argv[indx], "--inodes") == 0){
            print_func.inodes = 0;
        }
        else if (strcmp(argv[indx], "-r") == 0){
            sort_cond.order = -1;
        }
        else if (strcmp(argv[indx], "-t") == 0){
            sort_cond.alphabetic_order = -1;
        }
        else if (strcmp(argv[indx], "--dirsfirst") == 0){
            sort_cond.dirs_first = 0;
        }
        else if (strcmp(argv[indx], "-L") == 0){
            if (++indx >= argc){
                printf("expexted a int after -L command\n");
                return 0;
            }
            int d = 0;
            char digit[strlen(argv[indx])];
            strcpy(digit, argv[indx]);
            for (int i = 0; i < strlen(argv[indx]); i++){
                d *= 10;
                if (digit[i] >= '0' & digit[i] <= '9'){
                    d += digit[i] - '0';
                }
                else{
                    printf("%s is not an int, expexted a int after -L command\n", digit);
                    return 0;
                }
            }
            visit_cond.max_level = d;
        }
        else{
            if (isFile(argv[indx]) == 0 && strcmp(fullpath, ".") == 0){
                strcpy(fullpath, argv[indx]);
                //printf("%s is a directory", argv[indx]);
            }
            else{
                printf("Sorry but i don't recognize %s, try again and check tree --help for more information", argv[indx]);
                return 0;
            }
        }

        indx++;
    }

    recursive_visit(fullpath, &level, &visit_cond, &sort_cond, &print_func);
    print_information_visit(&print_func);
    return 0;
}
