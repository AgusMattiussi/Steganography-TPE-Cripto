#include "include/steganography.h"

typedef struct {
  uint8_t mask;
  uint8_t ioff;
  uint8_t doff;
} lsb_params_t;

static const lsb_params_t LSB4 = { 
    .mask = 0x0F, 
    .ioff = 4, 
    .doff = 4 
};

static const lsb_params_t LSB2 = {
    .mask = 0x03, 
    .ioff = 6, 
    .doff = 2
};

static void encoder(lsb_params_t params, uint8_t* image, size_t imageSize, size_t offset, uint8_t* shadows, size_t shadowsSize) {
    uint8_t mask = params.mask;
    int off = params.ioff;

    for (int i = 0, j = 0; i < imageSize && j < shadowsSize; i++) {
        uint8_t shadowBit = (shadows[j] >> off) & mask;

        image[i] &= ~mask;
        image[i] |= shadowBit;


        off -= params.doff;
        if (off < 0) {
            off += 8;
            j++;
        }
    }
}

void lsb2Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize) {
    encoder(LSB2, image, imageSize, offset, shadows, shadowsSize);
}

void lsb4Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize) {
    encoder(LSB4, image, imageSize, offset, shadows, shadowsSize);
}

int hideSecret(const char *dirName, FILE *file, int n, int k) {
    DIR *dir = opendir(dirName);
    if(dir == NULL){
        printf("Error: Directory does not exist\n");
        return EXIT_FAILURE;
    }

    long width, heigth, shadowLen;
    readHeaderSetOffset(file, &width, &heigth);
    uint8_t ** shadows = generateShadows(file, k, n, width, heigth, &shadowLen);
    
    struct dirent * entry;
    int j=0;

    int dirNameLen = strlen(dirName);
    char *fullPath;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {

            fullPath = getFullPath(dirName, dirNameLen, entry->d_name);
            
            FILE * participant = fopen(fullPath, "r+");
            
            BITMAPFILEHEADER * bmFileHeader = ReadBMFileHeader(participant);

            size_t imageSize = bmFileHeader->bfSize - bmFileHeader->bfOffBits;
            
            if(checkImageSize(width*heigth, imageSize)){
                printf("Error: File %s has a different size\n",  entry->d_name);
                return EXIT_FAILURE;
            }

            uint8_t * imageBuffer = malloc(sizeof(uint8_t) * imageSize);

            fseek(participant, bmFileHeader->bfOffBits, SEEK_SET);
            fread(imageBuffer, sizeof(uint8_t), imageSize, participant);

            if(k > LSB_MODE){
                lsb2Encode(imageBuffer, imageSize, 0, shadows[j], shadowLen);
            } else {
                lsb4Encode(imageBuffer, imageSize, 0, shadows[j], shadowLen);
            }

            fseek(participant, bmFileHeader->bfOffBits, SEEK_SET);
            fwrite(imageBuffer, sizeof(uint8_t), imageSize, participant);

            modifyReservedBit(participant, j+1);

            j++;
        }
    }
    closedir(dir);
    return EXIT_SUCCESS;
}