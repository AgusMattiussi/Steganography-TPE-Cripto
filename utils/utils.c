#include "utils.h"

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

const int checkFileCount(DIR * dir){
    struct dirent * entry;
    int count = 0;

    printf("Reading entries...\n");
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_type == DT_REG){
            if(strcmp("bmp", get_filename_ext(entry->d_name)) != 0 ){
                printf("file %d is not bmp\n", count + 1);
                return -1;
            }
            printf("Entry Name: %s\n", entry->d_name);
            count++;
        }
    }
    printf("\n =============================== \n");

    return count;   
}

const int checkImageSize(long size1, long size2){
    return size1 == size2? EXIT_SUCCESS : EXIT_FAILURE;
}

char * getFullPath(const char *dirName, int dirNameLen, char *entryName){
    char *fullPath = calloc(dirNameLen + strlen(entryName) + 2, sizeof(char));
    strcat(fullPath, dirName);
    if(fullPath[dirNameLen-1] == '/'){
        fullPath[dirNameLen-1] = '\0';
    }
    strcat(fullPath, "/");
    strcat(fullPath, entryName);
    return fullPath;
}