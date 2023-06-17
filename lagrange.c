#include <stdarg.h>

#define K 4

/*
falta definir xp = punto de interpolacion
*/
void lagrange(int sh1[], int sh2[], int sh3[], int sh4[]) {
    int x[K], y[K], p, yp, xp = 0;

    x[0] = sh1[0];
    x[1] = sh2[0];
    x[2] = sh3[0];
    x[3] = sh4[0];

    y[0] = sh1[1];
    y[1] = sh2[1];
    y[2] = sh3[1];
    y[3] = sh4[1];

    for(int i = 1; i <= K;i++) {
        p = 1;
		for(int j = 1;j <= K;j++) {
			if(i != j) {
			    p *= (xp - x[j])/(x[i] - x[j]);
			}
		  }
		yp += p * y[i];
	}

}