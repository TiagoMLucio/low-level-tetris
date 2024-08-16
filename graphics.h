#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <uspienv.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

#define CHAR_BASE_HEIGHT 16
#define SPACE_BASE_HEIGHT 14
#define CHAR_HEIGHT (FONT_SCALE * CHAR_BASE_HEIGHT)

// Draw a single pixel on the screen
void fb_pixel(int x, int y, unsigned color);

// Draw a line either horizontally or vertically
void draw_line(int x, int y, int length, unsigned color, int thickness, boolean vertical);

// Draw a rectangle with a specified thickness
void draw_rect(int x, int y, int width, int height, int thickness, unsigned color);

// Draw a single square of size BLOCK_SIZE
void draw_square(int pos_x, int pos_y, unsigned size, unsigned color);

void write_string(unsigned x, unsigned y, const void *str, unsigned size, unsigned scale, unsigned color);

void display_char(unsigned *x, unsigned y, char c, unsigned scale, unsigned color);

#endif // GRAPHICS_H