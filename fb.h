#ifndef __FB_H__
#define __FB_H__

#include <stdbool.h>
#include <stdint.h>

/*
 * Configuração desejada
 */
#define DEPTH 16
// #define DEPTH  24
#define WIDTH 800
#define HEIGHT 600

bool init_fb(void);
void fb_pixel(unsigned x, unsigned y, unsigned c);
uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b);

#endif
