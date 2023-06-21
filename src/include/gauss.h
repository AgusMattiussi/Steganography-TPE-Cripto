#ifndef _GAUSS_H
#define _GAUSS_H

#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

uint8_t * gauss(uint8_t * y, uint8_t * x, int dim);

void triangulate(uint8_t ** m, int dim);

#endif