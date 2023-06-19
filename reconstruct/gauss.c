#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define GROUP_MOD 251

static uint8_t ** allocateMatrix(int rows, int cols);
//static void freeMatrix(uint8_t ** m, long rows);
static uint8_t positiveMod(uint8_t n);

static uint8_t modInverses[GROUP_MOD];
static int initialized = 0;
//TODO: Crear array estatico de inversos
/* static uint8_t modInverse[GROUP_MOD]{

} */

void calculateModInverses();
void triangulate(uint8_t ** m, int dim);

int main(int argc, char const *argv[]){
    
    return 0;
}


void gauss(uint8_t * y, uint8_t * x, int dim){
    if(!initialized){
        calculateModInverses();
        initialized = 1;
    }


    uint8_t ** gaussMatrix = allocateMatrix(dim, dim + 1);

    for (size_t i = 0; i < dim; i++){
        for (size_t j = 0; i < dim; i++){
            gaussMatrix[i][j] = positiveMod(((uint8_t) pow(x[i], j)));
        }
        gaussMatrix[i][dim] = positiveMod(y[i]);
    }
    
    triangulate(gaussMatrix, dim);
}

void triangulate(uint8_t ** m, int dim){
    for (size_t i = 0; i < dim - 1; i++){

        uint8_t inverse = modInverses[m[i][i]];
        for (size_t z = i; z < dim + 1; z++){
            m[i][z] = (m[i][z] * inverse) % GROUP_MOD;
        }

        for (size_t j = i + 1; j < dim; j++){
            for (size_t k = i; k < dim + 1; k++){
                m[j][k] -= m[i][k];
                m[j][k] = positiveMod(m[j][k]);
            }
        }
    }
    
}

void calculateModInverses() {
    for (uint8_t i = 1; i < GROUP_MOD; i++) {
        for (uint8_t j = 1; j < GROUP_MOD; j++) {
            if ((i * j) % GROUP_MOD == 1) {
                modInverses[i] = j;
                break;
            }
        }
    }
}

static uint8_t positiveMod(uint8_t n){
    uint8_t aux = n;
    while(aux < 0)
        aux += GROUP_MOD;
    return aux % GROUP_MOD;
}

static uint8_t ** allocateMatrix(int rows, int cols){
    uint8_t ** m = calloc(rows, sizeof(uint8_t *));
    for (int i = 0; i < rows; i++){
        m[i] = calloc(cols, sizeof(uint8_t));
    }
    return m;
}

/* static void freeMatrix(uint8_t ** m, long rows){
    for (int i = 0; i < rows; i++){
        free(m[i]);
    }
    free(m);
} */

