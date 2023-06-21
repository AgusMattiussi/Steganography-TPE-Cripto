#include "include/shadowgen.h"

static int blockSubshadow(FILE * image, uint8_t ** vm, uint8_t ** vd, int k, int n, int blockNum);
static uint8_t generateB(uint8_t a, int r);
static int generateR();

uint8_t ** generateShadows(FILE * image, int k, int n, long imageSize, long * shadowLen) {
    int t = imageSize/(2*k - 2);
    *shadowLen = 2*t;

    uint8_t ** vm = allocateMatrix(t, n);
    if(vm == NULL){
        return NULL;
    }
    
    uint8_t ** vd = allocateMatrix(t, n);
    if(vd == NULL){
        freeMatrix(vm, t);
        return NULL;
    }

    for (int i = 0; i < t; i++) {
        if(blockSubshadow(image, vm, vd, k, n, i) == EXIT_FAILURE){
            freeMatrix(vm, t);
            freeMatrix(vd, t);
            return NULL;
        }
    }

    uint8_t ** shadows = allocateMatrix(n, *shadowLen);
    if(shadows != NULL){
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < *shadowLen; i += 2) {
                shadows[j][i] = vm[i/2][j];
                shadows[j][i+1] = vd[i/2][j];
            }
        }
    }
    freeMatrix(vm, t);
    freeMatrix(vd, t); 

    return shadows;
}

void freeShadows(uint8_t ** shadows, long shadowLen){
    freeMatrix(shadows, shadowLen);
}

/* Computes sub-shadows v_ij = {vm[i][j], vd[i][j]} */
static int blockSubshadow(FILE * image, uint8_t ** vm, uint8_t ** vd, int k, int n, int blockNum) {
    uint64_t resultF, resultG;
    uint8_t buffer[MAX_K] = {0};

    if(fread(buffer, sizeof(uint8_t), k, image) < k){
        printf("Error: Could not completely read file\n");
        return EXIT_FAILURE;
    }
    
    /* Evaluates the a_i pixels polynomial for each j and stores it in vm */
    for (int j = 1; j <= n; j++) {
        resultF = 0;
        for (int d = 0; d < k; d++) {       
            resultF += buffer[d] *  pow((double) j, (double) d);
        }
        vm[blockNum][j-1] = resultF % GROUP_MOD;
    }

    /* Generate b_i0 and b_i1 */
    int r = generateR();
    uint8_t b0 = generateB(buffer[0], r);
    uint8_t b1 = generateB(buffer[1], r);
    
    if(fread(buffer, sizeof(uint8_t), k-2, image) < k-2){
        printf("Error: Could not completely read file\n");
        return EXIT_FAILURE;
    }

    /* Evaluates the b_i pixels polynomial for each j and stores it in vd */
    for (int j = 1; j <= n; j++) {
        resultG = b0 + b1*j;
        for (int d = 2; d < k; d++) {
            resultG += buffer[d - 2] *  pow((double) j, (double) d);
        }
        vd[blockNum][j-1] = resultG % GROUP_MOD;
    }
    return EXIT_SUCCESS;
}

/* Used for generating b_0 and b_1 for each block */
static uint8_t generateB(uint8_t a, int r) {
    uint8_t aux = NON_ZERO(a);
    return positiveMod(-(r*aux));
}

/* Generates a random integer r in the interval [1, 250]*/
static int generateR() {
    return positiveMod(rand() % (GROUP_MOD-1)) + 1;
}



