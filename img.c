#include "img.h"

#include "stb_image.h"

static int format_channels[] = {
    [IMAGE_FORMAT_JPG] = 4,
    [IMAGE_FORMAT_PNG] = 4,
};

void debug_img_data(FILE *f, image_data_t *desc) {
  fprintf(f, "width: %d :: height: %d :: num_channels: %d\n", desc->width,
          desc->height, desc->num_channels);
}

image_data_t load_img_data_from_file(const char *name, image_format_t format) {
  int desired_channels = format_channels[format];

  int w, h, chn;
  unsigned char *data = stbi_load(name, &w, &h, &chn, desired_channels);
  if (!data) {
    fprintf(stdout, "failed to load image: %s\n", name);
    return (image_data_t){0};
  }

  return (image_data_t){
      .data = data,
      .width = w,
      .height = h,
      .num_channels = desired_channels,
  };
}

sg_image load_img_from_data(image_data_t desc) {
  return sg_make_image(&(sg_image_desc){
      .data.subimage[0][0] = {.ptr = desc.data,
                              .size = (size_t)(desc.width * desc.height *
                                               desc.num_channels)},
      .width = desc.width,
      .height = desc.height,
      .pixel_format = SG_PIXELFORMAT_RGBA8,
  });
}

sg_image load_img_from_file(const char *name, image_format_t format) {
  image_data_t desc = load_img_data_from_file(name, format);
  // give back default image if image is not loaded
  if (!desc.data) {
    sg_image def = {0};
    return def;
  }
  sg_image img = load_img_from_data(desc);
  stbi_image_free(desc.data);
  return img;
}

void img_data_free(image_data_t *self) { stbi_image_free(self->data); }
