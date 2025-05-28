#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H

#include "sokol_gfx.h"
#include "sokol_gp.h"

typedef struct bitmap_desc {
  sg_image img;

  uint32_t img_width_pixels;
  uint32_t img_height_pixels;
  uint32_t char_width_pixels;
  uint32_t char_height_pixels;
  uint32_t char_padding_x_pixels;
  uint32_t char_padding_y_pixels;

  const char *chars;
  size_t num_chars;
} bitmap_desc;

typedef struct bitmap_font {
  bitmap_desc opts;

  uint32_t img_width_chars;
  uint32_t img_height_chars;

  size_t *char_jump_tbl;
} bitmap_font;

bool bitmap_font_init(bitmap_font *self, bitmap_desc opts);
void bitmap_font_free(bitmap_font *self);

void bitmap_draw_char(bitmap_font *self, char c, sgp_rect r);
void bitmap_draw_line(bitmap_font *self, const char *content, size_t num_chars,
                      float gap, sgp_rect r);

void bitmap_draw_lines(bitmap_font *self, const char *content, size_t num_chars,
                       float gap_x, float gap_y, sgp_rect r);

#endif
