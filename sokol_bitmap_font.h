#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H

#include "sokol_gfx.h"
#include "sokol_gp.h"
#include <stdlib.h>

typedef struct sbm_allocator {
  void *(*alloc)(size_t, void *);
  void *(*realloc)(void *, size_t, void *);
  void (*free)(void *, void *);
  void *ctx;
} sbm_allocator;

void *sbm_alloc(size_t n, void *ctx) {
  (void)ctx;
  return malloc(n);
}

void *sbm_realloc(void *mem, size_t n, void *ctx) {
  (void)ctx;
  return realloc(mem, n);
}

void sbm_free(void *mem, void *ctx) {
  (void)ctx;
  free(mem);
}

sbm_allocator sbm_default_allocator() {
  return (sbm_allocator){
      .alloc = sbm_alloc,
      .realloc = sbm_realloc,
      .free = sbm_free,
      .ctx = NULL,
  };
}

typedef struct sbm_string_slice {
  const char *items;
  size_t len;
} sbm_string_slice;

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
  sbm_allocator allocator;

  bitmap_desc opts;

  uint32_t img_width_chars;
  uint32_t img_height_chars;

  size_t *char_jump_tbl;
} bitmap_font;

extern bool bitmap_font_init(sbm_allocator allocator, bitmap_font *self,
                             bitmap_desc opts);
extern void bitmap_font_free(bitmap_font *self);

extern void bitmap_draw_char(bitmap_font *self, char c, sgp_rect r);
extern void bitmap_draw_line(bitmap_font *self, sbm_string_slice slice,
                             float gap, sgp_rect r);

extern void bitmap_draw_lines(bitmap_font *self, sbm_string_slice slice,
                              float gap_x, float gap_y, sgp_rect r);

#if defined(SOKOL_BITMAP_IMPL) && !defined(SOKOL_BITMAP_IMPL_DONE)
#define SOKOL_BITMAP_IMPL_DONE

size_t find_max(const char *chars, size_t num_chars) {
  if (num_chars == 0) {
    return 0;
  }

  size_t max_val = chars[0];
  for (size_t i = 0; i < num_chars; i++) {
    if (chars[i] > max_val) {
      max_val = chars[i];
    }
  }
  return max_val;
}

bool build_char_jump_tbl(size_t *tbl, const char *chars, size_t num_chars) {
  if (!tbl || !chars) {
    return false;
  }

  for (size_t i = 0; i < num_chars; i++) {
    tbl[(size_t)chars[i]] = i;
  }

  return true;
}

bool bitmap_font_init(sbm_allocator allocator, bitmap_font *self,
                      bitmap_desc opts) {
  if (!self) {
    return false;
  }

  self->allocator = allocator;
  self->opts = opts;
  self->img_width_chars = (opts.img_width_pixels + opts.char_padding_x_pixels) /
                          (opts.char_width_pixels + opts.char_padding_x_pixels);
  self->img_height_chars =
      (opts.img_height_pixels + opts.char_padding_y_pixels) /
      (opts.char_height_pixels + opts.char_padding_y_pixels);

  size_t max_val = find_max(opts.chars, opts.num_chars);
  size_t *tbl = allocator.alloc((max_val + 1) * sizeof(size_t), allocator.ctx);
  if (!tbl) {
    return false;
  }

  if (!build_char_jump_tbl(tbl, opts.chars, opts.num_chars)) {
    free(tbl);
    return false;
  }

  self->char_jump_tbl = tbl;

  return true;
}

void bitmap_font_free(bitmap_font *self) {
  if (self && self->char_jump_tbl) {
    self->allocator.free(self->char_jump_tbl, self->allocator.ctx);
    self->char_jump_tbl = NULL;
  }
}

void bitmap_draw_char(bitmap_font *self, char c, sgp_rect r) {
  if (!self) {
    return;
  }

  size_t index = self->char_jump_tbl[(size_t)c];

  uint32_t x_tiles = index % self->img_width_chars;
  uint32_t y_tiles = index / self->img_width_chars;
  uint32_t x_pixels = x_tiles * (self->opts.char_width_pixels +
                                 self->opts.char_padding_x_pixels);
  uint32_t y_pixels = y_tiles * (self->opts.char_width_pixels +
                                 self->opts.char_padding_x_pixels);

  sgp_draw_textured_rect(0, r,
                         (sgp_rect){
                             .x = x_pixels,
                             .y = y_pixels,
                             .w = self->opts.char_width_pixels,
                             .h = self->opts.char_height_pixels,
                         });
}

void bitmap_draw_line(bitmap_font *self, sbm_string_slice slice, float gap,
                      sgp_rect r) {
  for (size_t i = 0; i < slice.len; i++) {
    bitmap_draw_char(self, slice.items[i],
                     (sgp_rect){
                         .x = r.x + (i * (r.w + gap)),
                         .y = r.y,
                         .w = r.w,
                         .h = r.h,
                     });
  }
}

void bitmap_draw_lines(bitmap_font *self, sbm_string_slice slice, float gap_x,
                       float gap_y, sgp_rect r) {

  float x = r.x;
  float y = r.y;

  for (size_t i = 0; i < slice.len; i++) {
    switch (slice.items[i]) {
    case '\n':
      x = r.x;
      y += r.h + gap_y;
      continue;
    case '\t':
      x += (r.w + gap_x) * 2;
      continue;
    default:
      break;
    }

    bitmap_draw_char(self, slice.items[i],
                     (sgp_rect){
                         .x = x,
                         .y = y,
                         .w = r.w,
                         .h = r.h,
                     });

    x += r.w + gap_x;
  }
}

#endif

#endif
