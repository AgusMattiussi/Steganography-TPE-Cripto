#include "shadowgen.h"

#define GROUP_SIZE 251
#define MAX_K 8

static void blockSubshadow(FILE * image, uint8_t ** vm, uint8_t ** vd, int k, int n, int blockNum);
static int generateB(uint8_t a, int r);
static int generateR();

void generateShadows(FILE * image, int k, int n, long width, long height) {

    printf("\nGenerating shadows...\n");
    printf("k: %d\n", k);
    printf("n: %d\n", n);
    printf("size: %ld (width) x %ld (height)\n ", width, height);

    // t = block count
    int t = (width*height) / (2*k - 2);
    printf("Block count (t): %d\n", t);

    // TODO: Modularizar
    uint8_t ** vm = calloc(t, sizeof(uint8_t *));
    uint8_t ** vd = calloc(t, sizeof(uint8_t *));
    for (int i = 0; i < t; i++)
    {
        vm[i] = calloc(n, sizeof(uint8_t));
        vd[i] = calloc(n, sizeof(uint8_t));
        if(vm[i] == NULL || vd[i] == NULL)
            printf("Null en i=%d\n", i);
    }
    

    for (int i = 0; i < t; i++) {
        //printf("\n===== Block %d/%d =====\n", i+1, t);
        // TODO: Dividir en bloques la imagen
        blockSubshadow(image, vm, vd, k, n, i);
    }
    printf("\nGenerados los bloques!!!\n");
    uint8_t shadows[n][2*t];
    printf("\n");
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < 2*t; i += 2) {
            //printf("j = %d ; i = %d\n", j, i);
            shadows[j][i] = vm[i/2][j];
            shadows[j][i+1] = vd[i/2][j];
        }
    }

    // TODO: Modularizar
    for (int i = 0; i < t; i++)
    {
        free(vm[i]);
        free(vd[i]);
    }
    free(vm);
    free(vd);   

    // Para que no tire warning por no usar variables
    printf("ignorar -> %hhx\n", shadows[0][0]);
}


/*
    Para recorrer cada bloque de la imagen (matriz) como si fuera unidimensional (array),
    llevo un iterador total 'iter' y recorro la imagen de la forma image[iter/width][iter%width]

*/

static void blockSubshadow(FILE * image, uint8_t ** vm, uint8_t ** vd, int k, int n, int blockNum) {
    size_t aux = 0;

    uint64_t resultF, resultG;
    uint8_t buffer[MAX_K] = {0};

    aux = fread(buffer, sizeof(uint8_t), k, image);
    if(aux < k){
        printf("Error: leyo %ld en lugar de %d(k)\n", aux, k);
    }

    /* if(blockNum == 0){
        printf("buffer[%d] = { ", k);
        for (size_t i = 0; i < 3; i++){
            printf("%hhx ", buffer[i]);
        }
        printf("}\n");
    } */
    

    /* for (int i = 0; i < k; i++){
        printf("%hhx ", buffer[i]);
    }
    printf("\n-->\n "); */

    for (int j = 1; j <= n; j++) {
        resultF = 0;
        for (int d = 0; d < k; d++) {
            //printf("resultF%d ", d);
            /* if(blockNum == 0 && j<=5){
                printf(" + %d * %d^%d\n", buffer[d], j, d);
            } */
            
            resultF += buffer[d] *  pow((double) j, (double) d);
            //printf("%d\n", resultF);
        }
        /* if(blockNum == 0){
            printf("(%d) %ld (=%d MOD 251)\n", blockNum, resultF, (uint8_t)(resultF % GROUP_SIZE));
        } */
        vm[blockNum][j-1] = resultF % GROUP_SIZE;
        //printf("vm[%d][%d] = %d\n\n", blockNum, j-1, resultF % GROUP_SIZE);
    }

    int r = generateR();
    //printf("r: %d\n", r);
    uint8_t b0 = generateB(buffer[0], r);
    uint8_t b1 = generateB(buffer[1], r);
    
    fread(buffer, sizeof(uint8_t), k-2, image);
    if(aux < k-2){
        printf("Error: leyo %ld en lugar de %d(k-2)\n", aux, k-2);
    }

    /* printf("%hhx %hhx ", b0, b1);
    for (int i = 0; i < k-2; i++){
        printf("%hhx ", buffer[i]);
    }
    printf("\n\n"); */


    for (int j = 1; j <= n; j++) {
        resultG = b0 + b1*j;
        for (int d = 2; d < k; d++) {
            resultG += buffer[d - 2] *  pow((double) j, (double) d);
        }
        vd[blockNum][j-1] = resultG % GROUP_SIZE;
    }

}

static int generateB(uint8_t a, int r) {
    uint8_t aux = a == 0 ? 1 : a;
    return -(r*aux) % GROUP_SIZE;
}

// [1, 250]
static int generateR() {
    return (rand() % (GROUP_SIZE-1)) + 1;
}



