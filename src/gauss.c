#include "include/gauss.h"

static uint8_t positiveMod(int n);
static uint8_t * solve(uint8_t ** m, int dim);



static uint8_t modInverses[GROUP_MOD] = {
    0, 1, 126, 84, 63, 201, 42, 36, 157, 28, 226, 137, 21, 58, 18, 67, 204,
    192, 14, 185, 113, 12, 194, 131, 136, 241, 29, 93, 9, 26, 159, 81, 102,
    213, 96, 208, 7, 95, 218, 103, 182, 49, 6, 216, 97, 106, 191, 235, 68,
    41, 246, 64, 140, 90, 172, 178, 130, 229, 13, 234, 205, 107, 166, 4, 51,
    112, 232, 15, 48, 211, 104, 99, 129, 196, 173, 164, 109, 163, 177, 197,
    91, 31, 150, 124, 3, 189, 108, 176, 174, 110, 53, 80, 221, 27, 243, 37,
    34, 44, 146, 71, 123, 169, 32, 39, 70, 153, 45, 61, 86, 76, 89, 199, 65,
    20, 240, 227, 132, 118, 117, 135, 228, 195, 179, 100, 83, 249, 2, 168,
    151, 72, 56, 23, 116, 134, 133, 119, 24, 11, 231, 186, 52, 162, 175, 165,
    190, 206, 98, 181, 212, 219, 82, 128, 180, 105, 207, 217, 214, 8, 224,
    30, 171, 198, 141, 77, 75, 143, 62, 248, 127, 101, 220, 160, 54, 74, 88,
    142, 87, 78, 55, 122, 152, 147, 40, 203, 236, 19, 139, 200, 247, 85, 144,
    46, 17, 238, 22, 121, 73, 79, 161, 111, 187, 5, 210, 183, 16, 60, 145,
    154, 35, 245, 202, 69, 148, 33, 156, 244, 43, 155, 38, 149, 170, 92, 225,
    242, 158, 222, 10, 115, 120, 57, 239, 138, 66, 237, 59, 47, 184, 233,
    193, 230, 114, 25, 223, 94, 215, 209, 50, 188, 167, 125, 250
};

//static int once = 0;

void imprimirMatriz(uint8_t ** matriz, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim + 1; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

uint8_t * gauss(uint8_t * y, uint8_t * x, int dim){
    uint8_t ** gaussMatrix = allocateMatrix(dim, dim + 1);

    /* if(!once){
        printf("y: ");
        for (int i = 0; i < dim; i++){
            printf("%d ", y[i]);
        }
        printf("\n");

        printf("x: ");
        for (int i = 0; i < dim; i++){
            printf("%d ", x[i]);
        }
        printf("\n");
    } */

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            gaussMatrix[i][j] = positiveMod(pow(x[i], j));
        }
        gaussMatrix[i][dim] = positiveMod(y[i]);
    }

    /* if(!once){
        printf("\n\n");
        imprimirMatriz(gaussMatrix, dim);
        printf("\n\n");
    } */
    
    triangulate(gaussMatrix, dim);

    /* if(!once){
        printf("\n\n");
        imprimirMatriz(gaussMatrix, dim);
        printf("\n\n");
    } */
    
    

    uint8_t * solutions = solve(gaussMatrix, dim);
    freeMatrix(gaussMatrix, dim);
    
    //once++;
    return solutions;
}

void triangulate(uint8_t ** m, int dim){
    for (int i = 0; i < dim - 1; i++){
        /* if(!once){
        printf("i = %d\n", i);} */
        uint8_t inverse = modInverses[m[i][i]];

        /* if(!once){
        imprimirMatriz(m, dim);
        printf("Multiplico por %d (inverso de %d)...\n", modInverses[m[i][i]], inverse);
        } */
        for (size_t z = i; z < dim + 1; z++){
            /* if(!once)
                printf("%d * %d MOD %d = ", m[i][z], inverse, GROUP_MOD); */
            m[i][z] = (m[i][z] * inverse) % GROUP_MOD;
            /* if(!once)
                printf("%d\n", m[i][z]); */
        }
        /* if(!once){
        imprimirMatriz(m, dim);
        } */

        for (int j = i + 1; j < dim; j++){
            for (int k = i; k < dim + 1; k++){
                /* if(!once){
                    printf("i=%d, j=%d, k=%d\n", i, j, k);
                    printf("%d - %d = %d --> %d\n\n", m[j][k], m[i][k],  m[j][k]-m[i][k], positiveMod(m[j][k] - m[i][k]));
                } */
                int ratio = m[j][i];
                m[j][k] = positiveMod(m[j][k] - ratio * m[i][k]);
                /* if(!once){
                imprimirMatriz(m, dim);
                } */
            }
        }
    }

    /* if(!once){
        imprimirMatriz(m, dim);
    } */
}

static uint8_t * solve(uint8_t ** m, int dim){
    uint8_t * solutions = calloc(dim, sizeof(int));
    
    for (int i = dim-1; i >= 0; i--){
        int accum = m[i][dim];

        for (int j = dim-1; j > i; j--){
            accum -= m[i][j] * solutions[j];
        }

        solutions[i] = (positiveMod(accum) * modInverses[m[i][i]]) % GROUP_MOD;
        if(solutions[i] >= GROUP_MOD){
            printf("\n\nUna sol me dio %hhx (%d)\n\n", solutions[i], solutions[i]);
        }

        
        
    }

    /* printf("Gauss: Solutions = ");
    for (size_t x = 0; x < dim; x++)
        {
            printf("%hhx ", solutions[x]);
        }
    printf("\n"); */

   return solutions;
}


static uint8_t positiveMod(int n){
    int aux = n;
    while(aux < 0)
        aux += GROUP_MOD;
    return aux % GROUP_MOD;
}
