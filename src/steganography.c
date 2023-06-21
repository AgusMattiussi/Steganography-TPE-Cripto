#include "include/steganography.h"

static void lsb2Encode(uint8_t *image, size_t imageSize, uint8_t *shadows, size_t shadowsSize);
static void lsb4Encode(uint8_t *image, size_t imageSize, uint8_t *shadows, size_t shadowsSize);
static void encoder(lsb_params_t params, uint8_t* image, size_t imageSize, uint8_t* shadows, size_t shadowsSize);

/* Encodes shadow bytes into image bytes according to LSB2 or LSB4 */
static void encoder(lsb_params_t params, uint8_t* image, size_t imageSize, uint8_t* shadows, size_t shadowsSize) {
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

/* Encodes according to LSB2 */
static void lsb2Encode(uint8_t *image, size_t imageSize, uint8_t *shadows, size_t shadowsSize) {
    encoder(LSB2, image, imageSize, shadows, shadowsSize);
}

/* Encodes according to LSB4 */
static void lsb4Encode(uint8_t *image, size_t imageSize, uint8_t *shadows, size_t shadowsSize) {
    encoder(LSB4, image, imageSize, shadows, shadowsSize);
}

int hideSecret(const char *dirName, FILE *file, int n, int k) {
    DIR *dir = opendir(dirName);
    if(dir == NULL){
        printf("Error: Directory does not exist\n");
        return EXIT_FAILURE;
    }

    long shadowLen;
    BMP * original = createBMP(file);
    uint8_t ** shadows = generateShadows(file, k, n, original->info->biSizeImage, &shadowLen);
    
    struct dirent * entry;
    int processed = 0;
    /* Hides a part of the secret in each entry */
    while ((entry = readdir(dir)) != NULL) {
        if (IS_FILE(entry->d_type)) {
            char * fullPath = getFullPath(dirName, entry->d_name);
            FILE * participant = fopen(fullPath, "r+");
            BMP * current = createBMP(participant);

            /* Verifies host image's size is equal to the original's size */
            size_t imageSize = current->info->biSizeImage;
            if(original->info->biSizeImage != imageSize){
                printf("Error: File %s has a different size\n",  entry->d_name);
                return EXIT_FAILURE;
            }

            /* Copies image bytes into buffer for easier modification */
            uint8_t * imageBuffer = getImageDataCopy(current);

            /* Encode according to k */
            if(k > LSB_MODE)
                lsb2Encode(imageBuffer, imageSize, shadows[processed], shadowLen);
            else 
                lsb4Encode(imageBuffer, imageSize, shadows[processed], shadowLen);
            
            /* Overwrite image (from offset) with encoded bytes */
            fwrite(imageBuffer, sizeof(uint8_t), imageSize, current->file);
            /* Write image processing order (x value) to bfReserved1 byte */
            modifyReservedBit(current, processed + 1);

            processed++;
            fclose(participant);
            free(fullPath);
            free(imageBuffer);
            freeBMP(current);
        }
    }
    closedir(dir);
    return EXIT_SUCCESS;
}