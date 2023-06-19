#include "steganography.h"

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
    uint8_t off = params.ioff;

    for (int i = offset, j = 0; i < imageSize && j < shadowsSize; i++) {
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

//TODO: Chequear cuando este lista la reconstruccion
int hideSecret(const char *dirName, long originalImageSize, uint8_t **shadows, size_t shadowLen, int k) {
    DIR *dir = opendir(dirName);
    if(dir == NULL){
        printf("Directory does not exist\n");
        return EXIT_FAILURE;
    }
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
            
            if(checkImageSize(originalImageSize, bmFileHeader->bfSize)){
                printf("File %s has a diferent size\n",  entry->d_name);
                return EXIT_FAILURE;
            }

            size_t imageSize = bmFileHeader->bfSize - bmFileHeader->bfOffBits + 1;

            uint8_t * imageBuffer = malloc(sizeof(uint8_t) * imageSize);

            fseek(participant, bmFileHeader->bfOffBits, SEEK_SET);
            fread(imageBuffer, sizeof(uint8_t), imageSize, participant);

            if(k > 4){
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
        printf("%hhX ", image[i]);
    }
}


int main() {
    char image[16] = {0x00};
    char shadows[3] = {0xFF};
    char image2[16] = {0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    char secret[1] = {0b11011010};
    char original[4] = {0b01110111, 0b01110111, 0b01110110, 0b01110100};
    char offuscated[4] = {0b01110111, 0b01110101, 0b01110110, 0b01110110};

    lsb4Encode(image, 16, 0, shadows, 3);
    lsb2Encode(original, 4, 0, secret, 1);

    printf("============== LSB2 ================\n");
    testPrinter(original, 4);
    printf("\n============= Answer ===============\n");    
    testPrinter(offuscated, 4);
    printf("\n====================================\n\n");    

    printf("\n============== LSB4 ================\n");
    testPrinter(image, 16);
    printf("\n============= Answer ===============\n");    
    testPrinter(image2, 16);
    printf("\n====================================\n");  
}
*/