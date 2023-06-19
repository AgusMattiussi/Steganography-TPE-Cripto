#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "include/bmp.h"
#include "include/utils.h"
#include "include/gauss.h"

static int checkRi(uint8_t ai0, uint8_t ai1, uint8_t bi0, uint8_t bi1);

void reconstruct(char * outputName, char * sourceDirName, int k);
void recoverShadow(FILE * participant, int k, uint8_t * shadow, long shadowLen);

/* int main(int argc, char *argv[]){
    printf("%s\n\n", argv[1]);
    reconstruct("", argv[1], 3);
    return 0;
} */

static uint8_t modInverses[GROUP_MOD] = {
    0, 1, 126, 84, 63, 201, 42, 36, 157, 28, 226, 137, 21, 58, 18, 67, 204,
    192, 14, 185, 113, 12, 194, 131, 136, 241, 29, 93, 9, 26, 159, 81, 102,
    213, 96, 208, 7, 95, 218, 103, 182, 49, 6, 216, 97, 106, 191, 235, 68,
    41, 246, 64, 140, 90, 172, 178, 130, 229, 13, 234, 205, 107, 166, 4, 51,
    112, 232, 15, 48, 211, 104, 99, 129, 196, 173, 164, 109, 163, 177, 197,
    91, 31, 150, 124, 3, 189, 108, 176, 174, 110, 53, 80, 221, 27, 243, 37,
    34, 44, 146, 71, 123, 169, 32, 39, 70, 153, 45, 61, 86, 76, 89, 199, 65,
    20, 240, 227, 132, 118, 117, 135, 228, 195, 179, 100, 83, 249, 2, 168,
    151, 72, 56, 23, 116, 134, 133, 119, 24, 11, 231, 186, 52, 162, 175, 165,
    190, 206, 98, 181, 212, 219, 82, 128, 180, 105, 207, 217, 214, 8, 224,
    30, 171, 198, 141, 77, 75, 143, 62, 248, 127, 101, 220, 160, 54, 74, 88,
    142, 87, 78, 55, 122, 152, 147, 40, 203, 236, 19, 139, 200, 247, 85, 144,
    46, 17, 238, 22, 121, 73, 79, 161, 111, 187, 5, 210, 183, 16, 60, 145,
    154, 35, 245, 202, 69, 148, 33, 156, 244, 43, 155, 38, 149, 170, 92, 225,
    242, 158, 222, 10, 115, 120, 57, 239, 138, 66, 237, 59, 47, 184, 233,
    193, 230, 114, 25, 223, 94, 215, 209, 50, 188, 167, 125, 250
};


void reconstruct(char * outputName, char * sourceDirName, int k){
    DIR* dir = opendir(sourceDirName);
    if(dir == NULL){
        printf("malardo\n");
        return;
    }
    struct dirent * entry;

    int dirNameLen = strlen(sourceDirName);

    
    char * fullPath;
    FILE * participant;
    int processed = 0;

    long width, height, t, shadowLen;
    uint8_t ** shadows = NULL;
    uint8_t * preimages = malloc(sizeof(uint8_t) * k);

    while(processed < k && ((entry = readdir(dir)) != NULL)){
        
        if (entry->d_type == DT_REG) {

            fullPath = getFullPath(sourceDirName, dirNameLen, entry->d_name);
            
            participant = fopen(fullPath, "r");
            if(participant == NULL){
                printf("Malardo 2\n");
                return;
            }
            
            if(shadows == NULL){
                readHeaderSetOffset(participant, &width, &height);
                t = (width*height) / (2*k - 2);
                shadowLen = 2*t;
                shadows = allocateMatrix(k, shadowLen);
            }

            BITMAPFILEHEADER * participantHeader = ReadBMFileHeader(participant);
            preimages[processed] = participantHeader->bfReserved1;
            recoverShadow(participant, k, shadows[processed], shadowLen);
            processed++;
            fclose(participant);
        }
        
    }
    if (processed < k){
        // TODO: Salida con error
    }

    uint8_t ** vm = allocateMatrix(t, k);
    uint8_t ** vd = allocateMatrix(t, k);
    // TODO: A chequear
    for (int j = 0; j < k; j++) {
        for (int i = 0; i < shadowLen; i += 2) {
            vm[i/2][j] = shadows[j][i];
            vd[i/2][j] = shadows[j][i+1];
        }
    }

    for (int i = 0; i < t; i++){
        uint8_t * a_i = gauss(vm[i], preimages, k);
        uint8_t * b_i = gauss(vd[i], preimages, k);

        if(checkRi(a_i[0], a_i[1], b_i[0], b_i[1]) == 0){
            //TODO: Manejar error
            printf("CHEATING DETECTED!!\n");
            exit(1);
        }


    }
    
    
    closedir(dir);
    freeMatrix(shadows, shadowLen);
}

void recoverShadow(FILE * participant, int k, uint8_t * shadow, long shadowLen){

    // Para lsb2, leo de a 4
    // Para lsb4, leo de a 2
    uint8_t lsbValue, lsbMask, bytesToRead;

    if(k <= 4){
        lsbValue = 4;
        lsbMask = 0x0F; // 0000 1111
        bytesToRead = 2; 
    } else {
        lsbValue = 2;
        lsbMask = 0x03; // 0000 0011
        bytesToRead = 4;
    }

    uint8_t * buf = calloc(bytesToRead, sizeof(uint8_t));
    for (long i = 0; i < shadowLen; i++){
        uint8_t shadowByte = 0x00;

        if(fread(buf, sizeof(uint8_t), bytesToRead, participant) < 8/lsbValue){
            // TODO: Manejar error
            printf("Me falto leer \n");
            return;
        }

        for (int j = 0; j < bytesToRead; j++){
            shadowByte <<= lsbValue;
            shadowByte += buf[j] & lsbMask;
        }
        
        shadow[i] = shadowByte;
    }
}

static int checkRi(uint8_t ai0, uint8_t ai1, uint8_t bi0, uint8_t bi1){
    int ri0 = -bi0 * modInverses[ai0];
    int ri1 = -bi1 * modInverses[ai1];

    return ri0 == ri1;
}

