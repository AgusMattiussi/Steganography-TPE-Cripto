#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define GROUP_SIZE 251
#define MAX_K 8

void generateShadows(FILE * image, int k, int n, int width, int height);
static void blockSubshadow(FILE * image, uint8_t ** vm,
 uint8_t ** vd, int k, int n, int blockNum);
static int generateB(uint8_t a, int r);
static int generateR();

void generateShadows(FILE * image, int k, int n, int width, int height) {
    // t = block count
    int t = (width*height) / (2*k - 2);
    uint8_t ** vm = calloc(t * n, sizeof(uint8_t));
    uint8_t ** vd = calloc(t * n, sizeof(uint8_t));

    for (int i = 0; i < t; i++) {
        // TODO: Dividir en bloques la imagen
        blockSubshadow(image, vm, vd, k, n, i);
    }
    
    uint8_t shadows[n][2*t];
    
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < 2*t; i += 2) {
            shadows[j][i] = vm[i/2][j];
            shadows[j][i+1] = vd[i/2][j];
        }
    }

    // Para que no tire warning por no usar variables
    printf("%hhx", shadows[0][0]);
}


/*
    Para recorrer cada bloque de la imagen (matriz) como si fuera unidimensional (array),
    llevo un iterador total 'iter' y recorro la imagen de la forma image[iter/width][iter%width]

*/

static void blockSubshadow(FILE * image, uint8_t ** vm, uint8_t ** vd,
 int k, int n, int blockNum) {

    int resultF = 0;
    uint8_t buffer[MAX_K];

    fread(buffer, k, sizeof(uint8_t), image);
    for (int j = 1; j <= n; j++) {
        for (int d = 0; d <= k - 1; d++) {
            resultF += buffer[d] *  pow((double) j, (double) d);
        }
        vm[blockNum][j] = resultF % GROUP_SIZE;
    }

    int r = generateR();

    uint8_t b0 = generateB(buffer[0], r);
    uint8_t b1 = generateB(buffer[1], r);
    
    fread(buffer, k-2, sizeof(uint8_t), image);
    int resultG = 0;
    for (int j = 1; j <= n; j++) {
        resultG += b0 + b1*j;
        for (int d = 2; d <= k - 1; d++) {
            resultG += buffer[d - 2] *  pow((double) j, (double) d);
        }
        vd[blockNum][j] = resultG % GROUP_SIZE;
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

