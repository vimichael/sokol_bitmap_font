#include "img.h"
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"
#include "sokol_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define SOKOL_BITMAP_IMPL
#include "sokol_bitmap_font.h"

void hex_to_rgb(int hex, float *target) {
  (target)[0] = ((hex >> 24) & 0xFF) / 255.0; // A
  (target)[1] = ((hex >> 16) & 0xFF) / 255.0; // R
  (target)[2] = ((hex >> 8) & 0xFF) / 255.0;  // G
  (target)[3] = (hex & 0xFF) / 255.0;         // B
}

static struct {
  sg_pass_action action;
  bitmap_font font;
} state;

void init() {
  sg_setup(&(sg_desc){
      .environment = sglue_environment(),
      .logger.func = slog_func,
  });

  sgp_setup(&(sgp_desc){0});

  float clear_color[4];
  hex_to_rgb(0x202020FF, clear_color);

  state.action =
      (sg_pass_action){.colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                                     .clear_value = (sg_color){
                                         .r = clear_color[0],
                                         .g = clear_color[1],
                                         .b = clear_color[2],
                                         .a = 1.0f,
                                     }}};

  image_data img_data =
      load_img_data_from_file("../assets/font.png", IMAGE_FORMAT_PNG);
  sg_image img = load_img_from_data(img_data);
  stbi_image_free(img_data.data);

  const char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123"
                      "456789.,\"'?! _*#$%&()+-/:;<=>[\\]^`{|}/";
  size_t nchars = strlen(chars);

  if (!bitmap_font_init(sbm_default_allocator(), &state.font,
                        (bitmap_desc){
                            .img = img,
                            .img_width_pixels = img_data.width,
                            .img_height_pixels = img_data.height,
                            .chars = chars,
                            .num_chars = nchars,
                            .char_padding_x_pixels = 1,
                            .char_padding_y_pixels = 0,
                            .char_width_pixels = 3,
                            .char_height_pixels = 5,
                        })) {
    printf("failed to initialize bitmap font\n");
    exit(1);
  }
}

void frame(void) {
  sg_begin_pass(&(sg_pass){
      .action = state.action,
      .swapchain = sglue_swapchain(),
  });

  int width = sapp_width();
  int height = sapp_height();

  sgp_begin(width, height);
  sgp_viewport(0, 0, width, height);

  sgp_set_image(0, state.font.opts.img);
  sgp_set_blend_mode(SGP_BLENDMODE_ADD);

  const char *content = "BITMAP FONT IN C";
  size_t nchars = strlen(content);
  bitmap_draw_lines(&state.font,
                    (sbm_string_slice){
                        .items = content,
                        .len = nchars,
                    },
                    25.0f, 25.0f,
                    (sgp_rect){
                        .x = 100.0f,
                        .y = 100.0f,
                        .w = 75.0f,
                        .h = 125.0f,
                    });

  sgp_flush();
  sgp_end();

  sg_end_pass();

  sg_commit();
}

void event(const sapp_event *ev) {
  switch (ev->type) {
  case SAPP_EVENTTYPE_KEY_DOWN:
    switch (ev->key_code) {
    case SAPP_KEYCODE_Q:
      sapp_request_quit();
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

void cleanup() {
  bitmap_font_free(&state.font);
  sgp_shutdown();
  sg_shutdown();
}

sapp_desc sokol_main(int argc, char **argv) {
  return (sapp_desc){
      .init_cb = init,
      .frame_cb = frame,
      .event_cb = event,
      .cleanup_cb = cleanup,
      .width = 640,
      .height = 480,
      .logger.func = slog_func,
  };
}
