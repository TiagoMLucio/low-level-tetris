#include <uspienv.h>
#include <uspios.h>

#include "game_screen.h"

static const char FromGameScreen[] = "game_screen";

void draw_game_block(unsigned i, unsigned j, unsigned color)
{
    if (i >= GAME_HEIGHT || j >= GAME_WIDTH)
        LogWrite(FromGameScreen, LogError, "draw called outside of game_screen: (i, j) = (%d, %d)", i, j);
    draw_square(GAME_POS_X + j * BLOCK_SIZE, GAME_POS_Y + i * BLOCK_SIZE, BLOCK_SIZE, color);
}

void draw_grid_block(unsigned i, unsigned j, unsigned color)
{
    if (i >= GAME_GRID_HEIGHT || j >= GAME_GRID_WIDTH)
        LogWrite(FromGameScreen, LOG_ERROR, "draw called outside of grid: (i, j) = (%d, %d)", i, j);
    draw_rect(block_position_x(GAME_GRID_J) + j * BLOCK_SIZE, block_position_y(GAME_GRID_J) + j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, 1, 0);
    draw_square(block_position_x(GAME_GRID_J) + j * BLOCK_SIZE + 1, block_position_y(GAME_GRID_I) + i * BLOCK_SIZE + 1, BLOCK_SIZE - 2, color);
}

void delete_line(unsigned i)
{
    for (unsigned j = 0; j < GAME_GRID_WIDTH; j++)
        draw_grid_block(i, j, 0);
}

void draw_grid(void)
{
    for (unsigned i = 0; i < GAME_HEIGHT; i++)
    {
        for (unsigned j = 0; j < GAME_WIDTH; j++)
        {
            fb_pixel(GAME_POS_X + j * BLOCK_SIZE, GAME_POS_Y + i * BLOCK_SIZE, NORMAL_COLOR);
            fb_pixel(GAME_POS_X + j * BLOCK_SIZE + BLOCK_SIZE - 1, GAME_POS_Y + i * BLOCK_SIZE, NORMAL_COLOR);
            fb_pixel(GAME_POS_X + j * BLOCK_SIZE, GAME_POS_Y + i * BLOCK_SIZE + BLOCK_SIZE - 1, NORMAL_COLOR);
            fb_pixel(GAME_POS_X + j * BLOCK_SIZE + BLOCK_SIZE - 1, GAME_POS_Y + i * BLOCK_SIZE + BLOCK_SIZE - 1, NORMAL_COLOR);
        }
    }
}

void draw_outer_frame(blocks i, blocks j, blocks width, blocks height)
{
    draw_rect(block_position_x(j), block_position_y(i), width * BLOCK_SIZE, height * BLOCK_SIZE, FRAME_THICKNESS_PX, BG_MAIN_PALETTE);
}

void draw_inner_frame(blocks i, blocks j, blocks width, blocks height, boolean thick)
{
    unsigned thickness = thick ? FRAME_THICKNESS_PX : SMALL_FRAME_THICKNESS_PX;
    draw_rect(block_position_x(j) + FRAME_THICKNESS_PX, block_position_y(i) + FRAME_THICKNESS_PX, width * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, height * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, thickness, BG_SECONDARY_PALETTE);
}

void write_title(blocks i, blocks j, unsigned diff_x, unsigned diff_y, char *str, unsigned size, unsigned scale)
{
    unsigned pos_y = block_position_y(i) - CHAR_HEIGHT / 2 + BLOCK_SIZE / 2;
    write_string(block_position_x(j) + diff_x, pos_y + diff_y, str, size, scale);
}

void setup_type_frame(void)
{
    draw_outer_frame(TYPE_FRAME_I, TYPE_FRAME_J, TYPE_FRAME_WIDTH, TYPE_FRAME_HEIGHT);
    draw_inner_frame(TYPE_FRAME_I, TYPE_FRAME_J, TYPE_FRAME_WIDTH, TYPE_FRAME_HEIGHT, FALSE);
    write_title(TYPE_FRAME_I + 1, TYPE_FRAME_J + 1, BLOCK_SIZE / 2, 0, TYPE_TITLE, TYPE_TITLE_SIZE, FONT_SCALE);
}

void setup_lines_frame(void)
{
    draw_outer_frame(LINES_FRAME_I, LINES_FRAME_J, LINES_FRAME_WIDTH, LINES_FRAME_HEIGHT);
    draw_inner_frame(LINES_FRAME_I, LINES_FRAME_J, LINES_FRAME_WIDTH, LINES_FRAME_HEIGHT, FALSE);
    write_title(LINES_FRAME_I + 1, LINES_FRAME_J + 2, 0, 0, LINES_TITLE, LINES_TITLE_SIZE, FONT_SCALE);
    screen_update_lines(0);
}

void int_to_string(unsigned num, char *str, int size)
{
    for (int i = 0; i < size; i++)
        str[i] = '0';
    str[size] = '\0';

    int i = size - 1;
    for (int i = size - 1; num > 0 && i >= 0; num /= 10, i--)
    str[i] = (num % 10) + '0';
}

void screen_update_lines(unsigned lines)
{
    write_title(LINES_FRAME_I + 1, LINES_FRAME_J + 7, TEXT_GAP, 0, "   ", 3, FONT_SCALE);

    char lines_str[3];
    int_to_string(lines, &lines_str, 3);

    write_title(LINES_FRAME_I + 1, LINES_FRAME_J + 7, TEXT_GAP, 0, lines_str, 3, FONT_SCALE);
}

void setup_score_frame(void)
{
    draw_outer_frame(SCORE_FRAME_I, SCORE_FRAME_J, SCORE_FRAME_WIDTH, SCORE_FRAME_HEIGHT);
    draw_inner_frame(SCORE_FRAME_I, SCORE_FRAME_J, SCORE_FRAME_WIDTH, SCORE_FRAME_HEIGHT, FALSE);
    write_title(SCORE_FRAME_I + 2, SCORE_FRAME_J + 1, 0, 0, SCORE_TITLE_TOP, SCORE_TITLE_TOP_SIZE, FONT_SCALE);
    write_title(SCORE_FRAME_I + 6, SCORE_FRAME_J + 1, 0, -BLOCK_SIZE / 2, SCORE_TITLE_SCORE, SCORE_TITLE_SCORE_SIZE, FONT_SCALE);
    screen_update_score(0);
}

void screen_update_score(unsigned score) {
    write_title(SCORE_FRAME_I + 7, SCORE_FRAME_J + 1, 0, 0, "      ", 6, FONT_SCALE);

    char score_str[6];
    int_to_string(score, &score_str, 6);

    write_title(SCORE_FRAME_I + 7, SCORE_FRAME_J + 1, 0, 0, score_str, 6, FONT_SCALE);
}


void setup_next_frame(void)
{
    draw_outer_frame(NEXT_FRAME_I, NEXT_FRAME_J, NEXT_FRAME_WIDTH, NEXT_FRAME_HEIGHT);
    draw_inner_frame(NEXT_FRAME_I, NEXT_FRAME_J, NEXT_FRAME_WIDTH, NEXT_FRAME_HEIGHT, TRUE);
    write_title(NEXT_FRAME_I + 2, NEXT_FRAME_J + 1, 6, -BLOCK_SIZE / 2, NEXT_TITLE, NEXT_TITLE_SIZE, FONT_SCALE);
}

void setup_level_frame(void)
{
    draw_outer_frame(LEVEL_FRAME_I, LEVEL_FRAME_J, LEVEL_FRAME_WIDTH, LEVEL_FRAME_HEIGHT);
    draw_inner_frame(LEVEL_FRAME_I, LEVEL_FRAME_J, LEVEL_FRAME_WIDTH, LEVEL_FRAME_HEIGHT, FALSE);
    write_title(LEVEL_FRAME_I + 1, LEVEL_FRAME_J + 1, BLOCK_SIZE / 2, 0, LEVEL_TITLE, LEVEL_TITLE_SIZE, FONT_SCALE);
}

void setup_statistics_frame(void)
{
    draw_outer_frame(STATISTICS_FRAME_I, STATISTICS_FRAME_J, STATISTICS_FRAME_WIDTH, STATISTICS_FRAME_HEIGHT);
    draw_inner_frame(STATISTICS_FRAME_I, STATISTICS_FRAME_J, STATISTICS_FRAME_WIDTH, STATISTICS_FRAME_HEIGHT, FALSE);
    write_title(STATISTICS_FRAME_I + 1, STATISTICS_FRAME_J + 1, -6, 0, STATISTICS_TITLE, STATISTICS_TITLE_SIZE, FONT_SCALE);
}

void setup_game_frame(void)
{
    draw_outer_frame(GAME_FRAME_I, GAME_FRAME_J, GAME_FRAME_WIDTH, GAME_FRAME_HEIGHT);
    draw_inner_frame(GAME_FRAME_I, GAME_FRAME_J, GAME_FRAME_WIDTH, GAME_FRAME_HEIGHT, TRUE);
}
