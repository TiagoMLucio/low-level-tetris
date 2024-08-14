#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <uspienv.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

// Draw a single pixel on the screen
void fb_pixel(int x, int y, unsigned color);

// Draw a line either horizontally or vertically
void draw_line(int x, int y, int length, unsigned color, int thickness, boolean vertical);

// Draw a rectangle with a specified thickness
void draw_rect(int x, int y, int width, int height, int thickness, unsigned color);

// Draw a single square of size BLOCK_SIZE
void draw_square(int pos_x, int pos_y, unsigned size, unsigned color);

#endif // GRAPHICS_H