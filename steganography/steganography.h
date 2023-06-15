#ifndef _STEGANOGRAPHY_H
#define _STEGANOGRAPHY_H

#include <stdint.h>
#include <stdio.h>

void lsb2Encode(char *image, size_t imageSize, size_t offset, char *shadows, size_t shadowsSize);

void lsb4Encode(char *image, size_t imageSize, size_t offset, char *shadows, size_t shadowsSize);

#endif
