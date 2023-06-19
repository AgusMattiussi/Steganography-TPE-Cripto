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

typedef struct tagBITMAPFILEHEADER {
    char           bfType[3];    /* 2 bytes + null char */
    unsigned int   bfSize;       /* 4 bytes */
    unsigned short bfReserved1;  /* 2 bytes */
    unsigned short bfReserved2;  /* 2 bytes */
    unsigned long  bfOffBits;    /* 4 bytes */
} BITMAPFILEHEADER;

/*
 * Bitmap info header (Windows)
 */
typedef struct tagBITMAPINFOHEADER {
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
} BITMAPINFOHEADER;

/*
 * Bitmap core header (OS/2)
 */
typedef struct tagBITMAPCOREHEADER {
    unsigned int   bcSize;      /* 4 bytes */
    short          bcWidth;     /* 2 bytes */
    short          bcHeight;    /* 2 bytes */
    unsigned short bcPlanes;    /* 2 bytes */
    unsigned short bcBitCount;  /* 2 bytes */
} BITMAPCOREHEADER;


int readHeaderSetOffetWithSize(FILE *image, long *width, long *heigth, long *size);
int readHeaderSetOffet(FILE *image, long *width, long *height);
BITMAPFILEHEADER *ReadBMFileHeader(FILE *fp);
void modifyReservedBit(FILE *image, unsigned short value);
void printBmpInfo(FILE * image);
int getDimensions(FILE * image, long *width, long *height);

#endif