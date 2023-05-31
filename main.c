#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

/*
 Parametros:

 - argv[1]: d o r
 - argv[2]: imagenSecreta
 - argv[3]: k
 - argv[4]: directorio

*/

const char *get_filename_ext(const char *filename);

// 0 = d, 1 = r
char mode = 0;
const char * filename;
DIR* dir; 
int k;

int main(int argc, char const *argv[]) {
    /* code */
    printf("Args: %d\n", argc);

    if(argc != 5){
        printf("Wrong number of arguments\n");
    }

    if(strcmp("bmp", get_filename_ext(argv[2])) != 0){
        printf("file must have .bmp extension\n");
        return 1;
    }
    filename = argv[2];

    if(strcmp(argv[1], "d") == 0){
        // TODO: argv[2] debe existir
        // Encode
    } else if (strcmp(argv[1], "r") == 0){
        mode = 1;
        // Recover
    } else {
        printf("Error: First argument must be either 'd' or 'r'");
        return 1;
    }

    k = atoi(argv[3]);
    if(k < 3 || k > 8){
        printf("k must be between 3 and 8\n");
        return 1;
    }

    dir = opendir(argv[4]);
    if(dir == NULL){
        printf("Directory does not exist\n");
        return 1;
    }
    return 0;
}


const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}