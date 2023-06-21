#include "include/gauss.h"

static uint8_t * solve(uint8_t ** m, int dim);
static void triangulate(uint8_t ** m, int dim);

uint8_t * gauss(uint8_t * y, uint8_t * x, int dim){
    /* Allocates space for the extended matrix */
    uint8_t ** gaussMatrix = allocateMatrix(dim, dim + 1);

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            gaussMatrix[i][j] = positiveMod(pow(x[i], j));
        }
        gaussMatrix[i][dim] = positiveMod(y[i]);
    }
    
    triangulate(gaussMatrix, dim);
    uint8_t * solutions = solve(gaussMatrix, dim);

    freeMatrix(gaussMatrix, dim);
    return solutions;
}

/* Applies elimination to create an upper triangular matrix */
static void triangulate(uint8_t ** m, int dim){
    for (int i = 0; i < dim - 1; i++){
        uint8_t inverse = MOD_INVERSE(m[i][i]);

        /* Multiplies current row by the modular inverse of its first coefficient,
           so it becomes 1 */
        for (size_t z = i; z < dim + 1; z++){
            m[i][z] = (m[i][z] * inverse) % GROUP_MOD;
        }

        /* Substracts a multiple of the correspondig upper row to each lower row */
        for (int j = i + 1; j < dim; j++){
            int ratio = m[j][i];
            for (int k = i; k < dim + 1; k++){
                m[j][k] = positiveMod(m[j][k] - ratio * m[i][k]);
            }
        }
    }
}

/* Solves an upper triangular matrix equation system */
static uint8_t * solve(uint8_t ** m, int dim){
    uint8_t * solutions = calloc(dim, sizeof(int));
    
    for (int i = dim-1; i >= 0; i--){
        /* y value for this row */
        int accum = m[i][dim];

        /* Use known solutions to partially solve current equation*/
        for (int j = dim-1; j > i; j--){
            accum -= m[i][j] * solutions[j];
        }

        /* Finds current coeficcient */
        solutions[i] = (positiveMod(accum) * MOD_INVERSE(m[i][i])) % GROUP_MOD;
    }

   return solutions;
}
