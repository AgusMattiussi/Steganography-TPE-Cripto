#include "include/utils.h"

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

const int checkFileCount(DIR * dir){
    struct dirent * entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_type == DT_REG){
            if(strcmp("bmp", get_filename_ext(entry->d_name)) != 0 ){
                return -1;
            }
            count++;
        }
    }

    return count;   
}

const int checkImageSize(long size1, long size2){
    return size1 == size2? EXIT_SUCCESS : EXIT_FAILURE;
}

char * getFullPath(const char *dirName, char *entryName){
    int dirNameLen = strlen(dirName);

    char *fullPath = calloc(dirNameLen + strlen(entryName) + 2, sizeof(char));
    
    strcat(fullPath, dirName);
    if(fullPath[dirNameLen-1] != '/'){
        strcat(fullPath, "/");
    }
    strcat(fullPath, entryName);
    
    return fullPath;
}

uint8_t ** allocateMatrix(int rows, int cols){
    uint8_t ** m = calloc(rows, sizeof(uint8_t *));
    for (int i = 0; i < rows; i++){
        m[i] = calloc(cols, sizeof(uint8_t));
    }
    return m;
}

void freeMatrix(uint8_t ** m, long rows){
    for (int i = 0; i < rows; i++){
        free(m[i]);
    }
    free(m);
}

void print_binary(unsigned int number){
    if (number >> 1) {
        print_binary(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}

void print_binary_wrapper(unsigned int number){
    print_binary(number);
    printf(" ");
}


uint8_t positiveMod(int n){
    // Multiplicar por -(GROUP_MOD - 1) es equivalente a multiplicar por 1
    int aux = n;
    if(aux < 0)
        aux *= -(GROUP_MOD-1); //-250
    return aux % GROUP_MOD;
}

void printMatrix(uint8_t ** matriz, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}