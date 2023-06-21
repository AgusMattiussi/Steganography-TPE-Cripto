#ifndef _RECONSTRUCT_H
#define _RECONSTRUCT_H

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bmp.h"
#include "utils.h"
#include "gauss.h"
#include "steganography.h"

/* Reconstruct image in 'outputName' file using images in 'sourceDirName'*/
void reconstruct(char * outputName, char * sourceDirName, int k);

#endif