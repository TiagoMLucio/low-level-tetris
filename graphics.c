#include "graphics.h"
#include <uspienv/util.h>

void fb_pixel(int x, int y, unsigned color) {
    ScreenDeviceSetPixel(USPiEnvGetScreen(), x, y, color);
}

void draw_line(int x, int y, int length, unsigned color, int thickness, boolean vertical) {
    for (int i = 0; i < thickness; i++)
        for (int j = 0; j < length; j++)
            fb_pixel(x + (vertical ? i : j), y + (vertical ? j : i), color);
}

void draw_rect(int x, int y, int width, int height, int thickness, unsigned color) {
    draw_line(x, y, height + 2 * thickness, color, thickness, 1);                     // left
    draw_line(x + width + thickness, y, height + 2 * thickness, color, thickness, 1); // right
    draw_line(x, y, width + 2 * thickness, color, thickness, 0);                      // top
    draw_line(x, y + height + thickness, width + 2 * thickness, color, thickness, 0); // bottom
}

void draw_square(int pos_x, int pos_y, unsigned size, unsigned color) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            fb_pixel(pos_x + i, pos_y + j, color);
}


