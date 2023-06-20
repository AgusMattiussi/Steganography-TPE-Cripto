#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#define GROUP_MOD 251

const char *get_filename_ext(const char *filename);
const int checkFileCount(DIR * dir);
const int checkImageSize(long size1, long size2);
char * getFullPath(const char *dirName, int dirNameLen, char *entryName);
uint8_t ** allocateMatrix(int rows, int cols);
void freeMatrix(uint8_t ** m, long rows);
void print_binary(unsigned int number);
void print_binary_wrapper(unsigned int number);
uint8_t positiveMod(int n);

#endif
