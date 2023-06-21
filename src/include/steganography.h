#ifndef _STEGANOGRAPHY_H
#define _STEGANOGRAPHY_H

#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include "bmp.h"
#include "utils.h"
#include "shadowgen.h"

#define LSB_MODE 4

typedef struct {
  uint8_t mask;
  uint8_t ioff;
  uint8_t doff;
} lsb_params_t;

static const lsb_params_t LSB4 = { 
    .mask = 0x0F, 
    .ioff = 4, 
    .doff = 4 
};

static const lsb_params_t LSB2 = {
    .mask = 0x03, 
    .ioff = 6, 
    .doff = 2
};

void lsb2Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize);

void lsb4Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize);

int hideSecret(const char *dirName, FILE *file, int n, int k);

#endif
