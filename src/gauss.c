#include "include/gauss.h"

static uint8_t * solve(uint8_t ** m, int dim);


uint8_t * gauss(uint8_t * y, uint8_t * x, int dim){
    /* Reserva espacio para la matriz extendida*/
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

void triangulate(uint8_t ** m, int dim){
    for (int i = 0; i < dim - 1; i++){
        uint8_t inverse = MOD_INVERSE(m[i][i]);

        /* Multiplica esta fila por su inverso multiplicativo para
            que el primer coeficiente sea 1 */
        for (size_t z = i; z < dim + 1; z++){
            m[i][z] = (m[i][z] * inverse) % GROUP_MOD;
        }

        /* A cada fila, le resta un multiplo de la fila actual para anular
            su primer coeficiente */
        for (int j = i + 1; j < dim; j++){
            for (int k = i; k < dim + 1; k++){
                int ratio = m[j][i];
                m[j][k] = positiveMod(m[j][k] - ratio * m[i][k]);
            }
        }
    }
}

static uint8_t * solve(uint8_t ** m, int dim){
    uint8_t * solutions = calloc(dim, sizeof(int));
    
    for (int i = dim-1; i >= 0; i--){
        /* Resultado de esta fila*/
        int accum = m[i][dim];

        /* Usa las incognitas encontradas para despejar la ecuacion */
        for (int j = dim-1; j > i; j--){
            accum -= m[i][j] * solutions[j];
        }

        /* Despeja una de las incognitas */
        solutions[i] = (positiveMod(accum) * MOD_INVERSE(m[i][i])) % GROUP_MOD;
    }

   return solutions;
}
