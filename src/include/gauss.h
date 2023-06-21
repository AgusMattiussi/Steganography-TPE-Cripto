#ifndef _GAUSS_H
#define _GAUSS_H

#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "utils.h"

/* Solves a system of 'dim' equations using Gauss' elimination method */
uint8_t * gauss(uint8_t * y, uint8_t * x, int dim);

#endif