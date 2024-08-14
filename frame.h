#ifndef FRAME_H
#define FRAME_H

#include "graphics.h"

#define BLOCK_SIZE 16

#define FRAME_WIDTH 10
#define FRAME_HEIGHT 20

#define FRAME_WIDTH_PX (FRAME_WIDTH * BLOCK_SIZE)
#define FRAME_HEIGHT_PX (FRAME_HEIGHT * BLOCK_SIZE)
#define FRAME_THICKNESS_PX (BLOCK_SIZE / 2)

#define FRAME_POS_X (SCREEN_WIDTH * 3 / 4 - FRAME_WIDTH_PX / 2 - FRAME_THICKNESS_PX)
#define FRAME_POS_Y (SCREEN_HEIGHT / 2 - FRAME_HEIGHT_PX / 2 - FRAME_THICKNESS_PX / 2 - FRAME_THICKNESS_PX)
#define GRID_POS_X (FRAME_POS_X + FRAME_THICKNESS_PX)
#define GRID_POS_Y (FRAME_POS_Y + FRAME_THICKNESS_PX)

// Draw the game frame
void draw_frame(void);

// Draw a block in the grid
void draw_grid_block(int j, int i, unsigned color);

// Delete a hole line of the grid
void delete_line(int i);

// Draw the entire grid (used for debugging)
void draw_grid(void);

#endif // FRAME_H
