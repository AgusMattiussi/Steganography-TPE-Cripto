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
        
        /*
        image[i] &= ~mask;
        image[i] += (shadows[j] & 0xF0) >> params.doff;
        shadows[j] <<= params.doff;

        if(shadows[j] == 0)
            j++;
        */
    }
}

void lsb2Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize) {
    encoder(LSB2, image, imageSize, offset, shadows, shadowsSize);
}

void lsb4Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize) {
    encoder(LSB4, image, imageSize, offset, shadows, shadowsSize);
}

//TODO: Chequear cuando este lista la reconstruccion
int hideSecret(const char *dirName, FILE *file, int n, int k) {
    DIR *dir = opendir(dirName);
    if(dir == NULL){
        printf("Directory does not exist\n");
        return EXIT_FAILURE;
    }

    long width, heigth, shadowLen;
    readHeaderSetOffset(file, &width, &heigth);
    uint8_t ** shadows = generateShadows(file, k, n, width, heigth, &shadowLen);
    
    struct dirent * entry;
    int j=0;

    int dirNameLen = strlen(dirName);
    char *fullPath;

    printf("Hiding shadows...\n\n");
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            printf("Hiding shadow %d in file %s\n", j+1, entry->d_name);

            fullPath = getFullPath(dirName, dirNameLen, entry->d_name);
            
            FILE * participant = fopen(fullPath, "r+");
            
            BITMAPFILEHEADER * bmFileHeader = ReadBMFileHeader(participant);

            size_t imageSize = bmFileHeader->bfSize - bmFileHeader->bfOffBits;
            
            if(checkImageSize(width*heigth, imageSize)){
                printf("File %s has a different size\n",  entry->d_name);
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

/*
void testPrinter(char *image, int size){
    for (int i = 0; i < size; i++) {
        // printf("%hhX ", image[i]);
        print_binary_wrapper(image[i]);
    }
}


int main() {
    uint8_t secret[4] = {0xFF, 0x34, 0x12, 0x23 };
    uint8_t original[8] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F };
    uint8_t answer[8] = {0x1F, 0x1F, 0x13, 0x14, 0x11, 0x12, 0x12, 0x13 };

    lsb4Encode(original, 8, 0, secret, 4);

    printf("\n============== LSB4 ================\n");
    testPrinter(original, 8);
    printf("\n============= Answer ===============\n");    
    testPrinter(answer, 8);
    printf("\n====================================\n\n\n");  

    uint8_t secret2[1] = {0b11011010};
    uint8_t original2[4] = {0b01110111, 0b01110111, 0b01110110, 0b01110100};
    uint8_t offuscated[4] = {0b01110111, 0b01110101, 0b01110110, 0b01110110};

    lsb2Encode(original2, 4, 0, secret2, 1);

    printf("============== LSB2 ================\n");
    testPrinter(original2, 4);
    printf("\n============= Answer ===============\n");    
    testPrinter(offuscated, 4);
    printf("\n====================================\n\n"); 
}
*/
