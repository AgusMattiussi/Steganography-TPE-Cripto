#ifndef _STEGANOGRAPHY_H
#define _STEGANOGRAPHY_H

#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include "bmp.h"
#include "utils.h"
#include "shadowgen.h"

#define LSB_MODE 4

void lsb2Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize);

void lsb4Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize);

int hideSecret(const char *dirName, FILE *file, int n, int k);

#endif
