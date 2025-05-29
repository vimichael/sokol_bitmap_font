#ifndef IMG_H
#define IMG_H

#include "sokol_gfx.h"
#include <stdio.h>

typedef enum {
  IMAGE_FORMAT_JPG,
  IMAGE_FORMAT_PNG,
} image_format;

typedef struct {
  int width, height;
  unsigned char *data;
  int num_channels;
} image_data;

void debug_img_data(FILE *f, image_data *desc);

image_data load_img_data_from_file(const char *name, image_format format);

// load image from image data
// does not free image data!!!
sg_image load_img_from_data(image_data img);

// load an image from a file. frees image data automatically,
// since it there is no way to access the data with this shortcut
// function
sg_image load_img_from_file(const char *name, image_format format);

void img_data_free(image_data *self);

#endif
