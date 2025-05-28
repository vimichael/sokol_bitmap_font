#ifndef IMG_H
#define IMG_H

#include "sokol_gfx.h"
#include <stdio.h>

typedef enum {
  IMAGE_FORMAT_JPG,
  IMAGE_FORMAT_PNG,
} image_format_t;

typedef struct {
  int width, height;
  unsigned char *data;
  int num_channels;
} image_data_t;

void debug_img_data(FILE *f, image_data_t *desc);

image_data_t load_img_data_from_file(const char *name, image_format_t format);

// load image from image data
// does not free image data!!!
sg_image load_img_from_data(image_data_t img);

// load an image from a file. frees image data automatically,
// since it there is no way to access the data with this shortcut
// function
sg_image load_img_from_file(const char *name, image_format_t format);

void img_data_free(image_data_t *self);

#endif
