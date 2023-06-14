#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define P 251

void generateShadows(uint8_t ** image, int k, int n, int width, int height);
static void blockSubshadow(uint8_t ** image, uint8_t ** vm,
 uint8_t ** vd, int k, int n, int blockNum, int * iter, int width);
static int generateB(uint8_t a, int r);
static int generateR();

void generateShadows(uint8_t ** image, int k, int n, int width, int height) {
    // t = block count
    int t = (width*height) / (2*k - 2);
    uint8_t ** vm = calloc(t * n, sizeof(uint8_t));
    uint8_t ** vd = calloc(t * n, sizeof(uint8_t));
    int iter = 0;

    for (int i = 0; i < t; i++) {
        // TODO: Dividir en bloques la imagen
        blockSubshadow(image, vm, vd, k, n, i, &iter, width);
    }
    
    uint8_t shadows[n][2*t];
    
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < 2*t; i += 2) {
            shadows[j][i] = vm[i/2][j];
            shadows[j][i+1] = vd[i/2][j];
        }
    }

    // Para que no tire warning por no usar variables
    printf("%d %d", shadows[0][0], iter);
}


/*
    Para recorrer cada bloque de la imagen (matriz) como si fuera unidimensional (array),
    llevo un iterador total 'iter' y recorro la imagen de la forma image[iter/width][iter%width]

*/

static void blockSubshadow(uint8_t ** image, uint8_t ** vm, uint8_t ** vd,
 int k, int n, int blockNum, int * iter, int width) {

    int resultF = 0;
    for (int j = 1; j <= n; j++) {
        for (int d = 0; d <= k - 1; d++) {
            resultF += (image[*iter / width][*iter % width] % P) *  pow((double) j, (double) d);
            (*iter)++;
        }
        vm[blockNum][j] = resultF;
    }

    int r = generateR();
    uint8_t a0 = image[*iter / width][*iter % width];
    uint8_t a1 = image[*iter / width][(*iter + 1) % width];
    uint8_t b0 = generateB(a0, r);
    uint8_t b1 = generateB(a1, r);
    
    int resultG = 0;
    for (int j = 1; j <= n; j++) {
        resultG += b0 + b1*j;
        for (int d = 2; d <= k - 1; d++) {
            resultG += (image[*iter / width][(d + *iter) % width] % P) *  pow((double) j, (double) d);
            vd[blockNum][j] = resultG;
            (*iter)++;
        }
        vd[blockNum][j] = resultG;
    }

}

static int generateB(uint8_t a, int r) {
    uint8_t aux = a == 0 ? 1 : a;
    return -(r*aux) % P;
}

// [1, 250]
static int generateR() {
    return (rand() % (P-1)) + 1;
}

