#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include "shadowgen/shadowgen.h"
#include "bmpReader/bmp.h"
#include "steganography/steganography.h"

/*
 Parametros:

 - argv[1]: d o r
 - argv[2]: imagenSecreta
 - argv[3]: k
 - argv[4]: directorio

*/

#define LSB2 2
#define LSB4 4


const char *get_filename_ext(const char *filename);
const int checkFileCount(DIR * dir);
int hideSecret(DIR *dir, long originalImageSize, uint8_t **shadows, size_t shadowLen, int mode);

int main(int argc, char const *argv[]) {
    srand(time(0));

    const char * filename;
    DIR* dir;
    FILE* file; 
    int k, n = 0;

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
        printf("Directory must contain at least k = %d .bmp images (contains %d)\n", k, n);
        return EXIT_FAILURE;
    }
    closedir(dir);

    dir = opendir(argv[4]);

    if(strcmp(argv[1], "d") == 0){
        file = fopen(filename, "r");

        if(file == NULL){
            printf("Image file does not exist\n");
            return EXIT_FAILURE;
        }
        
        long size, width, heigth, shadowLen;
        readHeaderSetOffet(file, &width, &heigth, &size);
        uint8_t ** shadows = generateShadows(file, k, n, width, heigth, &shadowLen);

        if(k > 4) {
            // LSB2
            hideSecret(dir, size, shadows, shadowLen, LSB2);
        } else {
            // LSB4
            hideSecret(dir, size, shadows, shadowLen, LSB4);
        }

    } else if (strcmp(argv[1], "r") == 0){
        // TODO: Recover
        // TODO: Chequear que las imagenes sean del mismo tamanio entre si
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

int hideSecret(DIR *dir, long originalImageSize, uint8_t **shadows, size_t shadowLen, int mode) {
    struct dirent * entry;
    int j=0;
    printf("Hiding shadows...\n\n");
    while ((entry = readdir(dir)) != NULL) {
    char dirPath[50] = "bmpfiles/";
        if (entry->d_type == DT_REG) {
            printf("Hiding shadow %d in file %s\n", j+1, entry->d_name);
            
            FILE * participant = fopen(strcat(dirPath, entry->d_name), "r+");
            
            BITMAPFILEHEADER * bmFileHeader = ReadBMFileHeader(participant);

            
            if(checkImageSize(originalImageSize, bmFileHeader->bfSize)){
                printf("File %s has a diferent size\n",  entry->d_name);
                return EXIT_FAILURE;
            }

            size_t imageSize = bmFileHeader->bfSize - bmFileHeader->bfOffBits + 1;

            uint8_t * imageBuffer = malloc(sizeof(uint8_t) * imageSize);

            fseek(participant, bmFileHeader->bfOffBits, SEEK_SET);
            fread(imageBuffer, sizeof(uint8_t), imageSize, participant);

            if(mode == LSB2){
                lsb2Encode(imageBuffer, imageSize, 0, shadows[j], shadowLen);
            } else {
                lsb4Encode(imageBuffer, imageSize, 0, shadows[j], shadowLen);
            }

            fseek(participant, bmFileHeader->bfOffBits, SEEK_SET);
            fwrite(imageBuffer, sizeof(uint8_t), imageSize, participant);

            j++;
        }
    }     
    return EXIT_SUCCESS;        
}