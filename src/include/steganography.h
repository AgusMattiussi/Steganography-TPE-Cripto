#ifndef _STEGANOGRAPHY_H
#define _STEGANOGRAPHY_H

#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include "bmp.h"
#include "utils.h"
#include "shadowgen.h"

#define LSB_MODE 4

/* Stores parameters for LSB2 or LSB4 */
typedef struct {
  uint8_t mask;       
  uint8_t ioff;
  uint8_t doff;        // Shadow bits' hidden in each image byte
  uint8_t bytesToRead; // Number of bytes needed to recover each shadow byte
} lsb_params_t;

static const lsb_params_t LSB4 = { 
    .mask = 0x0F, // 0000 1111
    .ioff = 4, 
    .doff = 4,
    .bytesToRead = 2
};

static const lsb_params_t LSB2 = {
    .mask = 0x03, // 0000 0011
    .ioff = 6, 
    .doff = 2,
    .bytesToRead = 4
};

/* Distributes the secret '*file' on the 'n' files in directory '*dirname' */
int hideSecret(const char *dirName, FILE *file, int n, int k);

#endif
