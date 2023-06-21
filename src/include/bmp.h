/*
 *
 * Reading bitmap file informations.
 *
 */

/*
 * Bitmap file header
 * source: https://gist.github.com/takatoh/9d141ee9a7cc8a1f4a5e#file-bmp-c
 */
#ifndef _BMP_H
#define _BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BF_RESERVED_1_POS 6

typedef struct fileHeader {
    char           bfType[3];    /* 2 bytes + null char */
    unsigned int   bfSize;       /* 4 bytes */
    unsigned short bfReserved1;  /* 2 bytes */
    unsigned short bfReserved2;  /* 2 bytes */
    unsigned long  bfOffBits;    /* 4 bytes */
} fileHeader;

/*
 * Bitmap info header (Windows)
 */
typedef struct BMPInfo {
    unsigned int   biSize;          /* 4 bytes */
    long           biWidth;         /* 4 bytes */
    long           biHeight;        /* 4 bytes */
    unsigned short biPlanes;        /* 2 bytes */
    unsigned short biBitCount;      /* 2 bytes */
    unsigned int   biCompression;   /* 4 bytes */
    unsigned int   biSizeImage;     /* 4 bytes */
    long           biXPixPerMeter;  /* 4 bytes */
    long           biYPixPerMeter;  /* 4 bytes */
    unsigned long  biClrUsed;       /* 4 bytes */
    unsigned long  biClrImportant;  /* 4 bytes */
} BMPInfo;

typedef struct BMP {
    fileHeader * fileHeader;
    BMPInfo * info;
    FILE * file;
} BMP;

BMP * createBMP(FILE * image);

void freeBMP(BMP * bmp);

void modifyReservedBit(BMP * bmp, unsigned short value);

void printBmpInfo(BMP * bmp);

void setFileToBMPOffset(FILE * image, BMP * bmp);

void setToOffset(BMP * bmp);

void copyHeader(uint8_t * dest, BMP * bmp);

uint8_t * getImageDataCopy(BMP * bmp);

#endif