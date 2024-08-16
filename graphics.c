#include "graphics.h"
#include <uspienv/util.h>

void fb_pixel(int x, int y, unsigned color)
{
    ScreenDeviceSetPixel(USPiEnvGetScreen(), x, y, color);
}

void draw_line(int x, int y, int length, unsigned color, int thickness, boolean vertical)
{
    for (int i = 0; i < thickness; i++)
        for (int j = 0; j < length; j++)
            fb_pixel(x + (vertical ? i : j), y + (vertical ? j : i), color);
}

void draw_rect(int x, int y, int width, int height, int thickness, unsigned color)
{
    draw_line(x, y, height, color, thickness, 1);                     // left
    draw_line(x + width - thickness, y, height, color, thickness, 1); // right
    draw_line(x, y, width, color, thickness, 0);                      // top
    draw_line(x, y + height - thickness, width, color, thickness, 0); // bottom
}

void draw_square(int x, int y, unsigned size, unsigned color)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            fb_pixel(x + i, y + j, color);
}

void write_string(unsigned x, unsigned y, const void *str, unsigned size, unsigned scale, unsigned color)
{
    const char *pChar = (const char *)str;

    while (size--)
    {
        char c = *pChar++;
        display_char(&x, y, c, scale, color);
    }
}

void display_char(unsigned *pos_x, unsigned pos_y, char c, unsigned scale, unsigned color)
{
    TScreenDevice *pThis = USPiEnvGetScreen();
    for (unsigned y = 0; y < (c != ' ' ? CHAR_BASE_HEIGHT : SPACE_BASE_HEIGHT); y++)
        for (unsigned x = 0; x < CharGeneratorGetCharWidth(&pThis->m_CharGen); x++)
            for (int i = 0; i < scale; i++)
                for (int j = 0; j < scale; j++)
                    if (c == ' ' || CharGeneratorGetPixel(&pThis->m_CharGen, c, x, y))
                        ScreenDeviceSetPixel(pThis, *pos_x + scale * x + j, pos_y + scale * y + i, c != ' ' ? color : 0);

    *pos_x += scale * CharGeneratorGetCharWidth(&pThis->m_CharGen);
}