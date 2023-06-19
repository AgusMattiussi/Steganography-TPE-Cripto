#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

const char *get_filename_ext(const char *filename);
const int checkFileCount(DIR * dir);
const int checkImageSize(long size1, long size2);

#endif
