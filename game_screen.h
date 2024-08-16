#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <uspienv.h>
#include "graphics.h"
#include "tile.h"

#define FONT_SCALE 3

#define BLOCK_SIZE 28
#define LITTLE_BLOCK_SIZE 24
#define GAME_WIDTH 32
#define GAME_HEIGHT 28
#define GAME_DISPLACEMENT_X 8 * BLOCK_SIZE
#define GAME_POS_X (SCREEN_WIDTH / 2 - GAME_WIDTH * BLOCK_SIZE / 2 + GAME_DISPLACEMENT_X)
#define GAME_POS_Y (SCREEN_HEIGHT / 2 - GAME_HEIGHT * BLOCK_SIZE / 2)

#define block_position_x(j) (GAME_POS_X + j * BLOCK_SIZE)
#define block_position_y(i) (GAME_POS_Y + i * BLOCK_SIZE)

#define FRAME_THICKNESS_PX BLOCK_SIZE / 2
#define SMALL_FRAME_THICKNESS_PX 4
#define BLACK_FRAME_THICKNESS_PX 2
#define TEXT_GAP 4

#define RESTART_BOX_WIDTH 14
#define RESTART_BOX_HEIGHT 7
#define RESTART_BOX_POS_X 0
#define RESTART_BOX_POS_Y 4 * BLOCK_SIZE
#define RESTART_BOX_MSG1 "Press 'r'"
#define RESTART_BOX_MSG1_SIZE 9
#define RESTART_BOX_MSG2 "to restart..."
#define RESTART_BOX_MSG2_SIZE 13

#define GAME_FRAME_I 4
#define GAME_FRAME_J 11
#define GAME_FRAME_WIDTH 12
#define GAME_FRAME_HEIGHT 22

#define GAME_GRID_I 5
#define GAME_GRID_J 12
#define GAME_GRID_WIDTH (GAME_FRAME_WIDTH - 2)
#define GAME_GRID_HEIGHT (GAME_FRAME_HEIGHT - 2)

#define TYPE_FRAME_I 2
#define TYPE_FRAME_J 2
#define TYPE_FRAME_WIDTH 8
#define TYPE_FRAME_HEIGHT 3
#define TYPE_TITLE "A-TYPE"
#define TYPE_TITLE_SIZE 6

#define STATISTICS_FRAME_I 7
#define STATISTICS_FRAME_J 1
#define STATISTICS_FRAME_WIDTH 10
#define STATISTICS_FRAME_HEIGHT 19
#define STATISTICS_TITLE "STATISTICS"
#define STATISTICS_TITLE_SIZE 10

#define LINES_FRAME_I 1
#define LINES_FRAME_J 11
#define LINES_FRAME_WIDTH 12
#define LINES_FRAME_HEIGHT 3
#define LINES_TITLE "LINES-"
#define LINES_TITLE_SIZE 9

#define SCORE_FRAME_I 1
#define SCORE_FRAME_J 23
#define SCORE_FRAME_WIDTH 8
#define SCORE_FRAME_HEIGHT 9
#define SCORE_TITLE_TOP "TOP"
#define SCORE_TITLE_TOP_SIZE 3
#define SCORE_TITLE_SCORE "SCORE"
#define SCORE_TITLE_SCORE_SIZE 5

#define NEXT_FRAME_I 11
#define NEXT_FRAME_J 23
#define NEXT_FRAME_WIDTH 6
#define NEXT_FRAME_HEIGHT 7
#define NEXT_TITLE "NEXT"
#define NEXT_TITLE_SIZE 4

#define LEVEL_FRAME_I 18
#define LEVEL_FRAME_J 23
#define LEVEL_FRAME_WIDTH 7
#define LEVEL_FRAME_HEIGHT 4
#define LEVEL_TITLE "LEVEL"
#define LEVEL_TITLE_SIZE 5

typedef unsigned blocks;

typedef int Statistics[7];

void draw_game_block(blocks i, blocks j, unsigned size, unsigned color);

void draw_block_rect(blocks i, blocks j, blocks width, blocks height, unsigned color);

void draw_block(unsigned x, unsigned y, unsigned size, unsigned color);

void draw_grid_block(blocks i, blocks j, unsigned size, unsigned color);

void delete_line(unsigned i);

void draw_grid(void);

void draw_outer_frame(blocks i, blocks j, blocks width, blocks height);

void draw_inner_frame(blocks i, blocks j, blocks width, blocks height, boolean thick);

void write_title(blocks i, blocks j, unsigned diff_x, unsigned diff_y, char *str, unsigned size, unsigned scale, unsigned color);

void draw_background(void);

void reset_game_screen(void);

void setup_type_frame(void);

void setup_lines_frame(void);

void setup_score_frame(void);

void setup_next_frame(void);

void setup_level_frame(void);

void screen_update_stats(Statistics stats);

void setup_statistics_frame(void);

void setup_game_frame(void);

void screen_update_lines(unsigned lines);

void screen_update_score(unsigned score);

void screen_update_top(unsigned top);

void screen_update_level(unsigned level);

void screen_clear_next(void);

void display_reset_msg(void);

void clear_reset_msg(void);

#endif // GAME_SCREEN_H