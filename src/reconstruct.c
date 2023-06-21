#include "include/reconstruct.h"

static int recoverShadow(FILE * participant, int k, uint8_t * shadow, long shadowLen);
static int checkRi(uint8_t ai0, uint8_t ai1, uint8_t bi0, uint8_t bi1);
static void recoverV(uint8_t ** vm, uint8_t ** vd, uint8_t ** shadows, int k, int shadowLen);
static void initializeOutputHeader(BMP * firstBmp);
static void initializeShadows(long imageSize, int k);


static uint8_t ** shadows = NULL;       // Shadows matrix
static long shadowLen = 0;              // Length of each shadow in **shadows
static uint8_t * outputHeader = NULL;   // Output file header
static int offset = 0;                  // Output file offset
static long t = 0;                      // Block count


int reconstruct(char * outputName, char * sourceDirName, int k){
    DIR * dir = opendir(sourceDirName);
    if(dir == NULL){
        printf("Error: Directory does not exist\n");
        return EXIT_FAILURE;
    }

    struct dirent * entry;
    int processed = 0;
    uint8_t * preimages = malloc(sizeof(uint8_t) * k);

    /* Processes k files from source directory */
    while(processed < k && ((entry = readdir(dir)) != NULL)){
        FILE * participant;
        BMP * current;
        char * fullPath;
        
        if (IS_FILE(entry->d_type)) {
            fullPath = getFullPath(sourceDirName, entry->d_name);
            
            participant = fopen(fullPath, "r");
            if(participant == NULL){
                printf("Error: Could not open participant in %s\n", fullPath);
                return EXIT_FAILURE;
            }

            current = createBMP(participant);

            if(outputHeader == NULL)
                initializeOutputHeader(current);
            
            if(shadows == NULL)
                initializeShadows(current->info->biSizeImage, k);
            
            /* Stores current x's value (from bfReserved1 byte) */
            preimages[processed] = current->fileHeader->bfReserved1;
            /* Recovers shadow for this participant image and store it in **shadows */
            if(recoverShadow(participant, k, shadows[processed], shadowLen) == EXIT_FAILURE){
                printf("Error: Could not recover shadow from %s\n", fullPath);
                return EXIT_FAILURE;
            }
            
            processed++;
            free(fullPath);
            fclose(participant);
            freeBMP(current);
        }
        
    }
    if (processed < k){
        printf("Error: Directory must contain at least %d (k) images\n", k);
        return EXIT_FAILURE;
    }    

    /* Stores each v_ij = {m_ij, d_ij} in vm and vd respectively */
    uint8_t ** vm = allocateMatrix(t, k); 
    uint8_t ** vd = allocateMatrix(t, k);
    recoverV(vm, vd, shadows, k, shadowLen);
    
    /* Creates (or truncate) output file */
    FILE * outputFile = fopen(outputName, "w+");
    /* Writes header to output file */
    fwrite(outputHeader, sizeof(uint8_t), offset, outputFile);

    /* Recovers each a_i and b_i pixel from vm and vd respectively, using 
        Gauss' elimination method */
    for (int i = 0; i < t; i++){
        uint8_t * a_i = gauss(vm[i], preimages, k);
        uint8_t * b_i = gauss(vd[i], preimages, k);

        /* Checks for cheating */
        if(checkRi(a_i[0], a_i[1], b_i[0], b_i[1]) == 0){
            printf("Error: Cheating detected\n", i);
            return EXIT_FAILURE;
        }
        
        /* If not cheating, writes pixels to output file */
        fwrite(a_i, sizeof(uint8_t), k, outputFile);
        fwrite(b_i + 2, sizeof(uint8_t), k-2, outputFile);
    }

    fclose(outputFile);
    closedir(dir);
    free(preimages);
    free(outputHeader);
    freeMatrix(shadows, k);

    return EXIT_SUCCESS;
}


/* Copies header from existing bmp file */
static void initializeOutputHeader(BMP * firstBmp){
    offset = firstBmp->fileHeader->bfOffBits;
    outputHeader = malloc(sizeof(uint8_t) * offset);
    copyHeader(outputHeader, firstBmp);
}

/* Initializes **shadows and calculates block count */
static void initializeShadows(long imageSize, int k){
    t = imageSize / (2*k - 2);
    shadowLen = 2*t;

    shadows = allocateMatrix(k, shadowLen);
}

/* Stores each v_ij = {m_ij, d_ij} in vm and vd respectively, extracting them from **shadows
    Remember shadow S_j = v_1j || v_2j || ... || v_tj */
static void recoverV(uint8_t ** vm, uint8_t ** vd, uint8_t ** shadows, int k, int shadowLen){
    for (int j = 0; j < k; j++) {
        for (int i = 0; i < shadowLen; i += 2) {
            vm[i/2][j] = shadows[j][i];
            vd[i/2][j] = shadows[j][i+1];
        }
    }
}

/* Recovers shadow bytes stored in each file and stores them in *shadow */
static int recoverShadow(FILE * participant, int k, uint8_t * shadow, long shadowLen){
    lsb_params_t lsb = k <= 4 ? LSB4 : LSB2;

    uint8_t buf[lsb.bytesToRead]; // Stores bytes from the image needed to reconstruct shadow
    for (long i = 0; i < shadowLen; i++){
        uint8_t shadowByte = 0x00;

        if(fread(buf, sizeof(uint8_t), lsb.bytesToRead, participant) < lsb.bytesToRead){
            printf("Error: Could not completely read file\n");
            return EXIT_FAILURE;
        }

        /* Recovers shadow byte shifting image bytes */
        for (int j = 0; j < lsb.bytesToRead; j++){
            shadowByte <<= lsb.doff;
            shadowByte += buf[j] & lsb.mask;
        }
        
        shadow[i] = shadowByte;
    }
    return EXIT_SUCCESS;
}

/* Checks if there is an r_i that satisfies a_i0 * r_i + b_i0 = 0
   and a_i1 * r_i + b_i1 = 0 */
static int checkRi(uint8_t ai0, uint8_t ai1, uint8_t bi0, uint8_t bi1){
    int ri0 = -bi0 * MOD_INVERSE(NON_ZERO(ai0));
    int ri1 = -bi1 * MOD_INVERSE(NON_ZERO(ai1));

    return positiveMod(ri0) == positiveMod(ri1);

}

