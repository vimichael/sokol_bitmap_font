#include "sokol_bitmap_font.h"
#include <stdio.h>
#include <stdlib.h>

// size_t find_max(const char *chars, size_t num_chars) {
//   if (num_chars == 0) {
//     return 0;
//   }
//
//   size_t max_val = chars[0];
//   for (size_t i = 0; i < num_chars; i++) {
//     if (chars[i] > max_val) {
//       max_val = chars[i];
//     }
//   }
//   return max_val;
// }
//
// bool build_char_jump_tbl(size_t *tbl, const char *chars, size_t num_chars) {
//   if (!tbl || !chars) {
//     return false;
//   }
//
//   for (size_t i = 0; i < num_chars; i++) {
//     tbl[(size_t)chars[i]] = i;
//   }
//
//   return true;
// }
//
// bool bitmap_font_init(bitmap_font *self, bitmap_desc opts) {
//   if (!self) {
//     return false;
//   }
//
//   self->opts = opts;
//   self->img_width_chars = opts.img_width_pixels /
//                           (opts.char_width_pixels +
//                           opts.char_padding_x_pixels);
//   self->img_height_chars =
//       opts.img_height_pixels /
//       (opts.char_height_pixels + opts.char_padding_y_pixels);
//
//   size_t max_val = find_max(opts.chars, opts.num_chars);
//   size_t *tbl = malloc((max_val + 1) * sizeof(size_t));
//   if (!tbl) {
//     return false;
//   }
//
//   if (!build_char_jump_tbl(tbl, opts.chars, opts.num_chars)) {
//     free(tbl);
//     return false;
//   }
//
//   self->char_jump_tbl = tbl;
//
//   return true;
// }
//
// void bitmap_font_free(bitmap_font *self) {
//   if (self && self->char_jump_tbl) {
//     free(self->char_jump_tbl);
//     self->char_jump_tbl = NULL;
//   }
// }
//
// void bitmap_draw_char(bitmap_font *self, char c, sgp_rect r) {
//   if (!self) {
//     return;
//   }
//
//   size_t index = self->char_jump_tbl[(size_t)c];
//
//   uint32_t x_tiles = index % self->img_width_chars;
//   uint32_t y_tiles = index / self->img_width_chars;
//   uint32_t x_pixels = x_tiles * (self->opts.char_width_pixels +
//                                  self->opts.char_padding_x_pixels);
//   uint32_t y_pixels = y_tiles * (self->opts.char_width_pixels +
//                                  self->opts.char_padding_x_pixels);
//
//   sgp_draw_textured_rect(0, r,
//                          (sgp_rect){
//                              .x = x_pixels,
//                              .y = y_pixels,
//                              .w = self->opts.char_width_pixels,
//                              .h = self->opts.char_height_pixels,
//                          });
// }
//
// void bitmap_draw_line(bitmap_font *self, const char *content, size_t
// num_chars,
//                       float gap, sgp_rect r) {
//   for (size_t i = 0; i < num_chars; i++) {
//     bitmap_draw_char(self, content[i],
//                      (sgp_rect){
//                          .x = r.x + (i * (r.w + gap)),
//                          .y = r.y,
//                          .w = r.w,
//                          .h = r.h,
//                      });
//   }
// }
//
// void bitmap_draw_lines(bitmap_font *self, const char *content, size_t
// num_chars,
//                        float gap_x, float gap_y, sgp_rect r) {
//
//   float x = r.x;
//   float y = r.y;
//
//   for (size_t i = 0; i < num_chars; i++) {
//     switch (content[i]) {
//     case '\n':
//       x = r.x;
//       y += r.h + gap_y;
//       continue;
//     case '\t':
//       x += (r.w + gap_x) * 2;
//       continue;
//     default:
//       break;
//     }
//
//     bitmap_draw_char(self, content[i],
//                      (sgp_rect){
//                          .x = x,
//                          .y = y,
//                          .w = r.w,
//                          .h = r.h,
//                      });
//
//     x += r.w + gap_x;
//   }
// }
