# sokol_bitmap_font

A small bitmap font renderer for sokol.

```c

#define SOKOL_BITMAP_IMPL
#include "sokol_bitmap_font.h"

void init() {
  sg_setup(&(sg_desc){
      .environment = sglue_environment(),
      .logger.func = slog_func,
  });

  // setup...


  // load image
  image_data img_data =
      load_img_data_from_file("../assets/font.png", IMAGE_FORMAT_PNG);
  sg_image img = load_img_from_data(img_data);
  stbi_image_free(img_data.data);

  // list of chars in order
  const char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123"
                      "456789.,\"'?! _*#$%&()+-/:;<=>[\\]^`{|}/";
  size_t nchars = strlen(chars);

  // initialize font
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
  // rendering...

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
```

## dependencies

- sokol
- sokol_gp

## installation

copy `sokol_bitmap_font.h` into your project.

Put this in one `.c` file:

```c

#define SOKOL_BITMAP_IMPL
#include "sokol_bitmap_font.h"
```

## running test

```bash
git clone --recursive https://github.com/vimichael/sokol_bitmap_font.git
cd sokol_bitmap_font
mkdir -p build
cd build
cmake ..
make skbmf_test
./skbmf_test
```

## license

[MIT](./LICENSE)
