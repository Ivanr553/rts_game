#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "input.h"

typedef struct file
{
    char *data;
    size_t len;
    bool is_valid;
} File;

typedef struct image_file
{
    int width;
    int height;
    int num_color_channels;
    unsigned char *bytes;
} Image_File;

File io_file_read(const char *path);
int io_file_write(void *buffer, size_t size, const char *path);
