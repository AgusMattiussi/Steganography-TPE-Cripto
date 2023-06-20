#include "include/reconstruct.h"

static void recoverShadow(FILE * participant, int k, uint8_t * shadow, long shadowLen);
static int checkRi(uint8_t ai0, uint8_t ai1, uint8_t bi0, uint8_t bi1);

void reconstruct(char * outputName, char * sourceDirName, int k){
    long width, height, t, shadowLen;
    struct dirent * entry;
    char * fullPath;
    FILE * participant;
    uint8_t ** shadows = NULL;
    uint8_t * outputHeader = NULL;
    int offset = 0;


    DIR * dir = opendir(sourceDirName);
    if(dir == NULL){
        printf("Error: Directory does not exist\n");
        return;
    }

    int processed = 0;
    uint8_t * preimages = malloc(sizeof(uint8_t) * k);
    while(processed < k && ((entry = readdir(dir)) != NULL)){
        BMP * current;
        
        if (entry->d_type == DT_REG) {

            fullPath = getFullPath(sourceDirName, entry->d_name);
            
            participant = fopen(fullPath, "r");
            if(participant == NULL){
                printf("Error opening participant in %s\n", fullPath);
                return;
            }

            current = createBMP(participant);

            if(outputHeader == NULL){
                offset = current->fileHeader->bfOffBits;
                outputHeader = malloc(sizeof(uint8_t) * offset);
                copyHeader(outputHeader, current);
            }

            if(shadows == NULL){
                width = current->info->biWidth;
                height = current->info->biHeight;

                t = (width*height) / (2*k - 2);

                shadowLen = 2*t;
                shadows = allocateMatrix(k, shadowLen);
            }

            setToOffset(current);

            preimages[processed] = current->fileHeader->bfReserved1;
            recoverShadow(participant, k, shadows[processed], shadowLen);
            processed++;

            free(fullPath);
            fclose(participant);
            freeBMP(current);
        }
        
    }
    if (processed < k){
        printf("Error: Directory must contain at least %d (k) images\n", k);
        return;
    }    

    uint8_t ** vm = allocateMatrix(t, k);
    uint8_t ** vd = allocateMatrix(t, k);

    for (int j = 0; j < k; j++) {
        for (int i = 0; i < shadowLen; i += 2) {
            vm[i/2][j] = shadows[j][i];
            vd[i/2][j] = shadows[j][i+1];
        }
    }

    FILE * outputFile = fopen(outputName, "w+");
    fwrite(outputHeader, sizeof(uint8_t), offset, outputFile);

    for (int i = 0; i < t; i++){
        uint8_t * a_i = gauss(vm[i], preimages, k);
        uint8_t * b_i = gauss(vd[i], preimages, k);

        //TODO: Check cheating
        
        fwrite(a_i, sizeof(uint8_t), k, outputFile);
        fwrite(&(b_i[2]), sizeof(uint8_t), k-2, outputFile);

        free(a_i);
        free(b_i);
    }

    fclose(outputFile);
    closedir(dir);
    free(outputHeader);
    freeMatrix(shadows, k);
}

static void recoverShadow(FILE * participant, int k, uint8_t * shadow, long shadowLen){
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

        if(fread(buf, sizeof(uint8_t), bytesToRead, participant) < bytesToRead){
            // TODO: Manejar error
            printf("Could not completely read file\n");
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
    int ri0 = -bi0 * MOD_INVERSE(ai0);
    int ri1 = -bi1 * MOD_INVERSE(ai1);

    return positiveMod(ri0) == positiveMod(ri1);
}

