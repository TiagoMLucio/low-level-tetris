
#include <stdio.h>
#include "fb.h"
#include "cpu.h"

extern void delay(int mili);

const int BLOCK_SIZE = 16;

const int frame_width = 10 * BLOCK_SIZE;
const int frame_height = 20 * BLOCK_SIZE;
const int frame_thickness = 4;

const int frame_pos_x = WIDTH / 2 - frame_width / 2 - frame_thickness;
const int frame_pos_y = HEIGHT / 2 - frame_height / 2 - frame_thickness;

void draw_line(int x, int y, int length, unsigned color, int thickness, bool vertical)
{
   for (int i = 0; i < thickness; i++)
      for (int j = 0; j < length; j++)
         fb_pixel(x + (!vertical ? j : 0), y + (vertical ? j : 0), color);
}

void draw_rect(int x, int y, int width, int height, int thickness, unsigned color) {
   draw_line(x, y, height + 2 * thickness, color, thickness, true);
   draw_line(x + width + 2 * thickness, y, height + 2 * thickness, color, thickness, true);
   draw_line(x, y, width + 2 * thickness, color, thickness, false);
   draw_line(x, y + height + 2 * thickness, width + 2 * thickness, color, thickness, false);
}

void draw_frame()
{
   unsigned white = pack_color(255, 255, 255);
   draw_rect(frame_pos_x, frame_pos_y, frame_width, frame_height, frame_thickness, white);
}

void draw_square(int pos_x, int pos_y, unsigned color)
{
   for (int i = 0; i < BLOCK_SIZE; i++)
      for (int j = 0; j < BLOCK_SIZE; j++)
         fb_pixel(pos_x + i, pos_y + j, color);
}

void draw_block(int pos_x, int pos_y, unsigned color)
{
   draw_square(pos_x, pos_y, color);
   draw_square(pos_x - BLOCK_SIZE, pos_y + BLOCK_SIZE, color);
   draw_square(pos_x, pos_y + BLOCK_SIZE, color);
   draw_square(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, color);
}

void main(void)
{
   enable_cache();
   bool ok = init_fb();
   // seria sábio tratar o valor retornado por init_fb()...

   draw_frame();

   unsigned cyan = pack_color(0, 255, 255);
   unsigned black = pack_color(0, 0, 0);

   int block_pos_x = frame_pos_x + frame_width / 2;
   int block_pos_y = frame_pos_y;

   delay(10000000);

   for (;;)
   {
      draw_block(block_pos_x, block_pos_y, cyan);
      delay(1000000);
      if (block_pos_y + 2 * BLOCK_SIZE >= frame_pos_y + frame_thickness + frame_height)
         for (;;)
            ;
      draw_block(block_pos_x, block_pos_y, black);
      block_pos_y += BLOCK_SIZE;
   }
}
