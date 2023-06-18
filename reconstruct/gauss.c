#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define GROUP_MOD 251

static uint8_t ** allocateMatrix(int rows, int cols);
static void freeMatrix(uint8_t ** m, long rows);

int main(int argc, char const *argv[]){
    
    return 0;
}


void gauss(uint8_t * y, uint8_t * x, int dim){
    
    uint8_t ** gaussMatrix = allocateMatrix(dim, dim + 1);

    for (size_t i = 0; i < dim; i++){
        for (size_t j = 0; i < dim; i++){
            gaussMatrix[i][j] = ((uint8_t) pow(x[i], j)) % GROUP_MOD;
        }
        gaussMatrix[i][dim] = y[i];
    }
    
}

static uint8_t ** allocateMatrix(int rows, int cols){
    uint8_t ** m = calloc(rows, sizeof(uint8_t *));
    for (int i = 0; i < rows; i++){
        m[i] = calloc(cols, sizeof(uint8_t));
    }
    return m;
}

static void freeMatrix(uint8_t ** m, long rows){
    for (int i = 0; i < rows; i++){
        free(m[i]);
    }
    free(m);
}