#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include "shadowgen/shadowgen.h"
#include "bmpReader/bmp.h"

/*
 Parametros:

 - argv[1]: d o r
 - argv[2]: imagenSecreta
 - argv[3]: k
 - argv[4]: directorio

*/

const char *get_filename_ext(const char *filename);
const int checkFileCount(DIR * dir);

int main(int argc, char const *argv[]) {
    srand(time(0));

    const char * filename;
    DIR* dir;
    FILE* file; 
    int k, n;

    if(argc != 5){
        printf("Wrong number of arguments\n");
    }

    k = atoi(argv[3]);
    if(k < 3 || k > 8){
        printf("k must be between 3 and 8\n");
        return EXIT_FAILURE;
    }


    if(strcmp("bmp", get_filename_ext(argv[2])) != 0){
        printf("file must have .bmp extension\n");
        return EXIT_FAILURE;
    }
    filename = argv[2];

    dir = opendir(argv[4]);
    if(dir == NULL){
        printf("Directory does not exist\n");
        return EXIT_FAILURE;
    }

    n = checkFileCount(dir);
    if(n < k){
        printf("Directory must contain at least k = %d .bmp images\n", k);
        closedir(dir);
    }

    if(strcmp(argv[1], "d") == 0){
        file = fopen(filename, "r");

        if(file == NULL){
            printf("Image file does not exist\n");
            return EXIT_FAILURE;
        }

        // TODO: Encode

    } else if (strcmp(argv[1], "r") == 0){

        // TODO: Recover
    } else {
        printf("Error: First argument must be either 'd' or 'r'");
        return EXIT_FAILURE;
    }

    /*  //TODO:
    free(bmFileHeader);
    free(bmCoreHeader);
    free(bmInfoHeader);
    fclose(imagen); 
    */

    fclose(file);
    closedir(dir);

    printf("Success!\n");
    return EXIT_SUCCESS;
}


const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

const int checkFileCount(DIR * dir){
    struct dirent * entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if(strcmp("bmp", get_filename_ext(entry->d_name)) != 0)
            return -1;
        if (entry->d_type == DT_REG) {
            count++;
        }
    }

    return count;   
}