#include <uspienv.h>
#include <uspios.h>

#include "frame.h"

static const char FromFrame[] = "frame";

void draw_grid_block(int j, int i, unsigned color)
{
    if ((i < 0 || i >= FRAME_HEIGHT) || (j < 0 || j >= FRAME_WIDTH))
        LogWrite(FromFrame, LOG_ERROR, "draw called outside of grid: (i, j) = (%d, %d)", i, j);
    draw_square(GRID_POS_X + j * BLOCK_SIZE, GRID_POS_Y + i * BLOCK_SIZE, BLOCK_SIZE, color);
}

void delete_line(int i) {
    for (int j = 0; j < FRAME_WIDTH; j++) 
        draw_grid_block(j, i, 0);
}

void draw_grid(void)
{
    for (int i = 0; i < FRAME_HEIGHT; i++)
    {
        for (int j = 0; j < FRAME_WIDTH; j++)
        {
            fb_pixel(GRID_POS_X + j * BLOCK_SIZE, GRID_POS_Y + i * BLOCK_SIZE, NORMAL_COLOR);
            fb_pixel(GRID_POS_X + j * BLOCK_SIZE + BLOCK_SIZE - 1, GRID_POS_Y + i * BLOCK_SIZE, NORMAL_COLOR);
            fb_pixel(GRID_POS_X + j * BLOCK_SIZE, GRID_POS_Y + i * BLOCK_SIZE + BLOCK_SIZE - 1, NORMAL_COLOR);
            fb_pixel(GRID_POS_X + j * BLOCK_SIZE + BLOCK_SIZE - 1, GRID_POS_Y + i * BLOCK_SIZE + BLOCK_SIZE - 1, NORMAL_COLOR);
        }
    }
}
void draw_frame(void)
{
    draw_rect(FRAME_POS_X, FRAME_POS_Y, FRAME_WIDTH_PX, FRAME_HEIGHT_PX, FRAME_THICKNESS_PX, NORMAL_COLOR);
}
