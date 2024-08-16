#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <uspienv.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

#define CHAR_BASE_HEIGHT 16
#define SPACE_BASE_HEIGHT 14
#define CHAR_HEIGHT (FONT_SCALE * CHAR_BASE_HEIGHT)

void fb_pixel(int x, int y, unsigned color);

void draw_line(int x, int y, int length, unsigned color, int thickness, boolean vertical);

void draw_frame(int x, int y, int width, int height, int thickness, unsigned color);

void draw_rect(int x, int y, int width, int height, unsigned color);

void draw_square(int pos_x, int pos_y, unsigned size, unsigned color);

void write_string(unsigned x, unsigned y, const void *str, unsigned size, unsigned scale, unsigned color);

void display_char(unsigned *x, unsigned y, char c, unsigned scale, unsigned color);

#endif // GRAPHICS_H