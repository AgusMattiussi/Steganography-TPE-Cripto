#ifndef _SHADOWGEN_H
#define _SHADOWGEN_H

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../utils/utils.h"

uint8_t ** generateShadows(FILE * image, int k, int n, long width, long height, long * shadowLen);
void freeShadows(uint8_t ** shadows, long shadowLen);

#endif