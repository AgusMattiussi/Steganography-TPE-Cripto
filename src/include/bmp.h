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

/* Bitmap info header (Windows) */
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

/* Creates a BMP structure from an image file */
BMP * createBMP(FILE * image);

/* Frees resources in a BMP structure */
void freeBMP(BMP * bmp);

/* Modifies bfReserved1 byte */
void modifyReservedBit(BMP * bmp, unsigned short value);

/* Prints a bmp's file information */
void printBmpInfo(BMP * bmp);

/* Sets a file's offset to the value where the image bytes start */
void setToOffset(BMP * bmp);

/* Copies the bytes from a bmp file's header to '*dest' */
void copyHeader(uint8_t * dest, BMP * bmp);

/* Returns a copy of the bytes of a bmp file's image */
uint8_t * getImageDataCopy(BMP * bmp);

#endif