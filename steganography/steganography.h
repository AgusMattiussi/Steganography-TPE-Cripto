#ifndef _STEGANOGRAPHY_H
#define _STEGANOGRAPHY_H

#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include "../bmp/bmp.h"
#include "../utils/utils.h"

#define LSB_MODE 4

void lsb2Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize);

void lsb4Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize);

int hideSecret(const char *dirName, long originalImageSize, uint8_t **shadows, size_t shadowLen, int mode);

#endif
