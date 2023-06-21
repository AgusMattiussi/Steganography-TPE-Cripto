/*
 * Read bitmap image information
 * source: https://gist.github.com/takatoh/9d141ee9a7cc8a1f4a5e#file-bmp-c
 */

#include "include/bmp.h"

static BMPInfo *ReadBMInfoHeader(FILE *fp);
static fileHeader *ReadBMFileHeader(FILE *fp);
static void modifyFileReservedBit(FILE * image, unsigned short value);
static unsigned short ReadLE2(FILE *fp);
static unsigned int ReadLE4(FILE *fp);

BMP * createBMP(FILE * image){
    BMP * new = malloc(sizeof(BMP));

    if(new != NULL){
        new->fileHeader = ReadBMFileHeader(image);
        new->info = ReadBMInfoHeader(image);
        new->file = image;

        setToOffset(new);
    }

    return new;
}

void freeBMP(BMP * bmp){
    free(bmp->fileHeader);
    free(bmp->info);
    free(bmp);
}

/*
 * Read bitmap file header
 */
static fileHeader * ReadBMFileHeader(FILE *fp) {
    fileHeader *header;
    char           filetype[3] = {'\0', '\0', '\0'};
    unsigned int   filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long  offset;

    /* File type (2 bytes) */
    fread(&filetype, 1, 2, fp);

    /* File size (4 bytes) */
    filesize = (unsigned int) ReadLE4(fp);

    /* Reserved 1 (2 bytes) */
    fread(&reserved1, 2, 1, fp);

    /* Reserved 2 (2 bytes) */
    fread(&reserved2, 2, 1, fp);

    /* Offset (4 bytes) */
    offset = (unsigned long) ReadLE4(fp);

    header = (fileHeader *) malloc(sizeof(fileHeader));
    strcpy(header->bfType, filetype);
    header->bfSize      = filesize;
    header->bfReserved1 = reserved1;
    header->bfReserved2 = reserved2;
    header->bfOffBits   = offset;

    return header;
}


/* Read bitmap info header (Windows bitmap) */
static BMPInfo * ReadBMInfoHeader(FILE *fp){
    BMPInfo *header;
    unsigned int   headersize;
    int            width;
    int            height;
    unsigned short planes;
    unsigned short bitcount;
    unsigned int   compression;
    unsigned int   size_image;
    int            x_pix_per_meter;
    int            y_pix_per_meter;
    unsigned int   clr_used;
    unsigned int   clr_important;

    /* Header size (4 bytes) */
    headersize = (unsigned int) ReadLE4(fp);

    /* Width (4 bytes) */
    width = (int) ReadLE4(fp);

    /* Height (4 bytes) */
    height = (int) ReadLE4(fp);

    /* Planes (2 bytes) */
    planes = (unsigned short) ReadLE2(fp);

    /* Bit Count (2 bytes) */
    bitcount = (unsigned short) ReadLE2(fp);

    /* Compression (4 bytes) */
    compression = (unsigned int) ReadLE4(fp);

    /* Size image (4 bytes) */
    size_image = (unsigned int) ReadLE4(fp);

    /* X pix per meter (4 bytes) */
    x_pix_per_meter = (int) ReadLE4(fp);

    /* Y pix per meter (4 bytes) */
    y_pix_per_meter = (int) ReadLE4(fp);

    /* Color used (4 bytes) */
    clr_used = (unsigned int) ReadLE4(fp);

    /* Color important (4 bytes) */
    clr_important = (unsigned int) ReadLE4(fp);

    header = (BMPInfo *) malloc(sizeof(BMPInfo));
    header->biSize         = headersize;
    header->biWidth        = width;
    header->biHeight       = height;
    header->biPlanes       = planes;
    header->biBitCount     = bitcount;
    header->biCompression  = compression;
    header->biSizeImage    = size_image;
    header->biXPixPerMeter = x_pix_per_meter;
    header->biYPixPerMeter = y_pix_per_meter;
    header->biClrUsed      = clr_used;
    header->biClrImportant = clr_important;

    return header;
}

void setToOffset(BMP * bmp){
    fseek(bmp->file, bmp->fileHeader->bfOffBits, SEEK_SET);
}

void modifyReservedBit(BMP * bmp, unsigned short value){
    modifyFileReservedBit(bmp->file, value);
}

static void modifyFileReservedBit(FILE * image, unsigned short value){
    long currentOffset = ftell(image);
    fseek(image, BF_RESERVED_1_POS, SEEK_SET);
    fwrite(&value, sizeof(unsigned short), 1, image);
    fseek(image, currentOffset, SEEK_SET);
}

void copyHeader(uint8_t * dest, BMP * bmp){
    long currentOffset = ftell(bmp->file);
    fseek(bmp->file, 0, SEEK_SET);
    fread(dest, sizeof(uint8_t), bmp->fileHeader->bfOffBits, bmp->file);
    fseek(bmp->file, currentOffset, SEEK_SET);
}

uint8_t * getImageDataCopy(BMP * bmp){
    uint8_t * copy = malloc(sizeof(uint8_t) * bmp->info->biSizeImage);
    long currentOffset = ftell(bmp->file);

    fseek(bmp->file, bmp->fileHeader->bfOffBits, SEEK_SET);
    fread(copy, sizeof(uint8_t), bmp->info->biSizeImage, bmp->file);
    fseek(bmp->file, currentOffset, SEEK_SET);

    return copy;
}

void printBmpInfo(BMP * bmp){ 
    printf("File type          = %s\n", bmp->fileHeader->bfType);
    printf("File size          = %d bytes\n", bmp->fileHeader->bfSize);
    printf("Reserved bit 1     = %d bytes\n", bmp->fileHeader->bfReserved1);
    printf("Reserved bit 2     = %d bytes\n", bmp->fileHeader->bfReserved2);
    printf("Data offset        = %ld bytes\n", bmp->fileHeader->bfOffBits);

    printf("Info header size   = %d bytes\n", bmp->info->biSize);
    printf("Width              = %ld pixels\n", bmp->info->biWidth);
    printf("Height             = %ld pixels\n", bmp->info->biHeight);
    printf("Planes             = %d\n", bmp->info->biPlanes);
    printf("Bit count          = %d bits/pixel\n", bmp->info->biBitCount);
    printf("Compression        = %d\n", bmp->info->biCompression);
    printf("Size image         = %d bytes\n", bmp->info->biSizeImage);
    printf("X pixels per meter = %ld\n", bmp->info->biXPixPerMeter);
    printf("Y pixels per meter = %ld\n", bmp->info->biYPixPerMeter);
    printf("Color used         = %ld colors\n", bmp->info->biClrUsed);
}

/*
 * Read 2 bytes in little endian
 */
static unsigned short ReadLE2(FILE *fp) {
    unsigned char buf[2];
    unsigned short result = 0;
    int i;

    fread(buf, 1, 2, fp);
    for (i = 1; i >= 0; i--) {
        result = (result << 8) | (unsigned short) buf[i];
    }

    return result;
}

/*
 * Read 4 bytes in little endian
 */
static unsigned int ReadLE4(FILE *fp) {
    unsigned char buf[4];
    unsigned int result = 0;
    int i;

    fread(buf, 1, 4, fp);
    for (i = 3; i >= 0; i--) {
        result = (result << 8) | (unsigned int) buf[i];
    }

    return result;
}