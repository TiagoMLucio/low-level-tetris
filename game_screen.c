#include <uspienv.h>
#include <uspios.h>

#include "game_screen.h"

static const char FromGameScreen[] = "game_screen";

void draw_game_block(blocks i, blocks j, unsigned size, unsigned color)
{
    if (i >= GAME_HEIGHT || j >= GAME_WIDTH)
        LogWrite(FromGameScreen, LogError, "draw called outside of game_screen: (i, j) = (%d, %d)", i, j);
    draw_square(block_position_x(j), block_position_y(i), size, color);
}

void draw_block(unsigned x, unsigned y, unsigned size, unsigned color)
{
    draw_frame(x, y, size, size, 1, 0);
    draw_square(x + 1, y + 1, size - 2, color);
}

void draw_grid_block(blocks i, blocks j, unsigned size, unsigned color)
{
    if (i >= GAME_GRID_HEIGHT || j >= GAME_GRID_WIDTH)
        LogWrite(FromGameScreen, LOG_ERROR, "draw called outside of grid: (i, j) = (%d, %d)", i, j);
    draw_block(block_position_x(GAME_GRID_J) + j * size, block_position_y(GAME_GRID_I) + i * size, size, color);
}

void delete_line(unsigned i)
{
    for (unsigned j = 0; j < GAME_GRID_WIDTH; j++)
        draw_grid_block(i, j, BLOCK_SIZE, 0);
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
    draw_frame(block_position_x(j), block_position_y(i), width * BLOCK_SIZE, height * BLOCK_SIZE, FRAME_THICKNESS_PX, BG_MAIN_PALETTE);
    draw_frame(block_position_x(j), block_position_y(i), width * BLOCK_SIZE, height * BLOCK_SIZE, BLACK_FRAME_THICKNESS_PX, BLACK_COLOR);
}

void draw_inner_frame(blocks i, blocks j, blocks width, blocks height, boolean thick)
{
    unsigned thickness = thick ? FRAME_THICKNESS_PX : SMALL_FRAME_THICKNESS_PX;
    draw_frame(block_position_x(j) + FRAME_THICKNESS_PX, block_position_y(i) + FRAME_THICKNESS_PX, width * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, height * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, thickness, BG_SECONDARY_PALETTE);
    draw_frame(block_position_x(j) + FRAME_THICKNESS_PX, block_position_y(i) + FRAME_THICKNESS_PX, width * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, height * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, BLACK_FRAME_THICKNESS_PX, BLACK_COLOR);
}

void write_title(blocks i, blocks j, unsigned diff_x, unsigned diff_y, char *str, unsigned size, unsigned scale, unsigned color)
{
    unsigned pos_y = block_position_y(i) - CHAR_HEIGHT / 2 + BLOCK_SIZE / 2;
    write_string(block_position_x(j) + diff_x, pos_y + diff_y, str, size, scale, color);
}

void draw_block_rect(blocks i, blocks j, blocks width, blocks height, unsigned color)
{
    draw_rect(block_position_x(j), block_position_y(i), width * BLOCK_SIZE, height * BLOCK_SIZE, color);
}

void draw_background(void)
{
    draw_block_rect(0, 0, GAME_WIDTH, GAME_HEIGHT, BG_MAIN_PALETTE);

    draw_block_rect(GAME_FRAME_I, GAME_FRAME_J, GAME_FRAME_WIDTH, GAME_FRAME_HEIGHT, BLACK_COLOR);
    draw_block_rect(TYPE_FRAME_I, TYPE_FRAME_J, TYPE_FRAME_WIDTH, TYPE_FRAME_HEIGHT, BLACK_COLOR);
    draw_block_rect(STATISTICS_FRAME_I, STATISTICS_FRAME_J, STATISTICS_FRAME_WIDTH, STATISTICS_FRAME_HEIGHT, BLACK_COLOR);
    draw_block_rect(LINES_FRAME_I, LINES_FRAME_J, LINES_FRAME_WIDTH, LINES_FRAME_HEIGHT, BLACK_COLOR);
    draw_block_rect(SCORE_FRAME_I, SCORE_FRAME_J, SCORE_FRAME_WIDTH, SCORE_FRAME_HEIGHT, BLACK_COLOR);
    draw_block_rect(NEXT_FRAME_I, NEXT_FRAME_J, NEXT_FRAME_WIDTH, NEXT_FRAME_HEIGHT, BLACK_COLOR);
    draw_block_rect(LEVEL_FRAME_I, LEVEL_FRAME_J, LEVEL_FRAME_WIDTH, LEVEL_FRAME_HEIGHT, BLACK_COLOR);
}

void setup_type_frame(void)
{
    draw_outer_frame(TYPE_FRAME_I, TYPE_FRAME_J, TYPE_FRAME_WIDTH, TYPE_FRAME_HEIGHT);
    draw_inner_frame(TYPE_FRAME_I, TYPE_FRAME_J, TYPE_FRAME_WIDTH, TYPE_FRAME_HEIGHT, FALSE);
    write_title(TYPE_FRAME_I + 1, TYPE_FRAME_J + 1, BLOCK_SIZE / 2, 0, TYPE_TITLE, TYPE_TITLE_SIZE, FONT_SCALE, NORMAL_COLOR);
}

void setup_lines_frame(void)
{
    draw_outer_frame(LINES_FRAME_I, LINES_FRAME_J, LINES_FRAME_WIDTH, LINES_FRAME_HEIGHT);
    draw_inner_frame(LINES_FRAME_I, LINES_FRAME_J, LINES_FRAME_WIDTH, LINES_FRAME_HEIGHT, FALSE);
    write_title(LINES_FRAME_I + 1, LINES_FRAME_J + 2, 0, 0, LINES_TITLE, LINES_TITLE_SIZE, FONT_SCALE, NORMAL_COLOR);
    screen_update_lines(0);
}

void int_to_string(unsigned num, char *str, int size)
{
    for (int i = 0; i < size; i++)
        str[i] = '0';

    for (int i = size - 1; num > 0 && i >= 0; num /= 10, i--)
        str[i] = (num % 10) + '0';
}

void screen_update_lines(unsigned lines)
{
    write_title(LINES_FRAME_I + 1, LINES_FRAME_J + 7, TEXT_GAP, 0, "   ", 3, FONT_SCALE, NORMAL_COLOR);

    char lines_str[3];
    int_to_string(lines, lines_str, 3);

    write_title(LINES_FRAME_I + 1, LINES_FRAME_J + 7, TEXT_GAP, 0, lines_str, 3, FONT_SCALE, NORMAL_COLOR);
}

void setup_score_frame(void)
{
    draw_outer_frame(SCORE_FRAME_I, SCORE_FRAME_J, SCORE_FRAME_WIDTH, SCORE_FRAME_HEIGHT);
    draw_inner_frame(SCORE_FRAME_I, SCORE_FRAME_J, SCORE_FRAME_WIDTH, SCORE_FRAME_HEIGHT, FALSE);
    write_title(SCORE_FRAME_I + 2, SCORE_FRAME_J + 1, 0, -BLOCK_SIZE / 2, SCORE_TITLE_TOP, SCORE_TITLE_TOP_SIZE, FONT_SCALE, NORMAL_COLOR);
    write_title(SCORE_FRAME_I + 6, SCORE_FRAME_J + 1, 0, -BLOCK_SIZE / 2, SCORE_TITLE_SCORE, SCORE_TITLE_SCORE_SIZE, FONT_SCALE, NORMAL_COLOR);
    screen_update_top(0);
    screen_update_score(0);
}

void screen_update_top(unsigned score)
{
    write_title(SCORE_FRAME_I + 3, SCORE_FRAME_J + 1, 0, 0, "      ", 6, FONT_SCALE, NORMAL_COLOR);

    char score_str[6];
    int_to_string(score, score_str, 6);

    write_title(SCORE_FRAME_I + 3, SCORE_FRAME_J + 1, 0, 0, score_str, 6, FONT_SCALE, NORMAL_COLOR);
}

void screen_update_score(unsigned score)
{
    write_title(SCORE_FRAME_I + 7, SCORE_FRAME_J + 1, 0, 0, "      ", 6, FONT_SCALE, NORMAL_COLOR);

    char score_str[6];
    int_to_string(score, score_str, 6);

    write_title(SCORE_FRAME_I + 7, SCORE_FRAME_J + 1, 0, 0, score_str, 6, FONT_SCALE, NORMAL_COLOR);
}

void setup_next_frame(void)
{
    draw_outer_frame(NEXT_FRAME_I, NEXT_FRAME_J, NEXT_FRAME_WIDTH, NEXT_FRAME_HEIGHT);
    draw_inner_frame(NEXT_FRAME_I, NEXT_FRAME_J, NEXT_FRAME_WIDTH, NEXT_FRAME_HEIGHT, TRUE);
    write_title(NEXT_FRAME_I + 2, NEXT_FRAME_J + 1, 6, -BLOCK_SIZE / 2, NEXT_TITLE, NEXT_TITLE_SIZE, FONT_SCALE, NORMAL_COLOR);
}

void screen_clear_next(void)
{
    for (unsigned i = 0; i < 3; i++)
        for (unsigned j = 0; j < 4; j++)
            draw_game_block(NEXT_FRAME_I + 3 + i, NEXT_FRAME_J + 1 + j, BLOCK_SIZE, 0);
}

void setup_level_frame(void)
{
    draw_outer_frame(LEVEL_FRAME_I, LEVEL_FRAME_J, LEVEL_FRAME_WIDTH, LEVEL_FRAME_HEIGHT);
    draw_inner_frame(LEVEL_FRAME_I, LEVEL_FRAME_J, LEVEL_FRAME_WIDTH, LEVEL_FRAME_HEIGHT, FALSE);
    write_title(LEVEL_FRAME_I + 1, LEVEL_FRAME_J + 1, BLOCK_SIZE / 2, -4, LEVEL_TITLE, LEVEL_TITLE_SIZE, FONT_SCALE, NORMAL_COLOR);
    screen_update_level(1);
}

void screen_update_level(unsigned level)
{
    write_title(LEVEL_FRAME_I + 2, LEVEL_FRAME_J + 3, 0, 6, "  ", 2, FONT_SCALE, NORMAL_COLOR);

    char level_str[2];
    int_to_string(level, level_str, 2);

    write_title(LEVEL_FRAME_I + 2, LEVEL_FRAME_J + 3, 0, 6, level_str, 2, FONT_SCALE, NORMAL_COLOR);
}

void screen_update_stats(Statistics stats)
{
    int pos_x = STATISTICS_FRAME_I + 4;
    int pos_y = STATISTICS_FRAME_J + 4 + 2;
    int diff_y = -BLOCK_SIZE / 2;

    for (int i = 0; i < 7; i++)
        write_title(pos_x + i * 2, pos_y, 0, diff_y, "   ", 3, FONT_SCALE, 0);

    char stats_str[7][3];

    for (int i = 0; i < 7; i++)
    {
        int_to_string(stats[i], stats_str[i], 3);
        write_title(pos_x + i * 2, pos_y, 0, diff_y, stats_str[i], 3, FONT_SCALE, RED_PALETTE);
    }
}

void setup_statistics_frame(void)
{
    draw_outer_frame(STATISTICS_FRAME_I, STATISTICS_FRAME_J, STATISTICS_FRAME_WIDTH, STATISTICS_FRAME_HEIGHT);
    draw_inner_frame(STATISTICS_FRAME_I, STATISTICS_FRAME_J, STATISTICS_FRAME_WIDTH, STATISTICS_FRAME_HEIGHT, FALSE);
    write_title(STATISTICS_FRAME_I + 1, STATISTICS_FRAME_J + 1, -6, 0, STATISTICS_TITLE, STATISTICS_TITLE_SIZE, FONT_SCALE, NORMAL_COLOR);
    screen_update_stats((Statistics){0, 0, 0, 0, 0, 0, 0});
}

void setup_game_frame(void)
{
    draw_outer_frame(GAME_FRAME_I, GAME_FRAME_J, GAME_FRAME_WIDTH, GAME_FRAME_HEIGHT);
    draw_inner_frame(GAME_FRAME_I, GAME_FRAME_J, GAME_FRAME_WIDTH, GAME_FRAME_HEIGHT, TRUE);
}

void reset_game_screen(void)
{
    screen_update_level(1);
    screen_update_lines(0);
    screen_update_score(0);
    clear_reset_msg();
    screen_erase_qrcode();
}

void display_reset_msg(void)
{
    clear_reset_msg();
    draw_frame(RESTART_BOX_POS_X, RESTART_BOX_POS_Y, RESTART_BOX_WIDTH * BLOCK_SIZE, RESTART_BOX_HEIGHT * BLOCK_SIZE, FRAME_THICKNESS_PX, BG_MAIN_PALETTE);
    draw_frame(RESTART_BOX_POS_X, RESTART_BOX_POS_Y, RESTART_BOX_WIDTH * BLOCK_SIZE, RESTART_BOX_HEIGHT * BLOCK_SIZE, BLACK_FRAME_THICKNESS_PX, BLACK_COLOR);
    draw_frame(RESTART_BOX_POS_X + FRAME_THICKNESS_PX, RESTART_BOX_POS_Y + FRAME_THICKNESS_PX, RESTART_BOX_WIDTH * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, RESTART_BOX_HEIGHT * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, FRAME_THICKNESS_PX, BG_SECONDARY_PALETTE);
    draw_frame(RESTART_BOX_POS_X + FRAME_THICKNESS_PX, RESTART_BOX_POS_Y + FRAME_THICKNESS_PX, RESTART_BOX_WIDTH * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, RESTART_BOX_HEIGHT * BLOCK_SIZE - 2 * FRAME_THICKNESS_PX, BLACK_FRAME_THICKNESS_PX, BLACK_COLOR);

    unsigned pos_y = RESTART_BOX_POS_Y + 2 * BLOCK_SIZE - CHAR_HEIGHT / 2 + BLOCK_SIZE / 2;

    write_string(RESTART_BOX_POS_X + BLOCK_SIZE + TEXT_GAP, pos_y, RESTART_BOX_MSG1, RESTART_BOX_MSG1_SIZE, FONT_SCALE, NORMAL_COLOR);
    write_string(RESTART_BOX_POS_X + BLOCK_SIZE + TEXT_GAP, pos_y + 2 * BLOCK_SIZE, RESTART_BOX_MSG2, RESTART_BOX_MSG2_SIZE, FONT_SCALE, NORMAL_COLOR);

    screen_draw_qrcode();
}

void clear_reset_msg(void)
{
    draw_square(RESTART_BOX_POS_X, RESTART_BOX_POS_Y, RESTART_BOX_WIDTH * BLOCK_SIZE, 0);
}

int QR_CODE[QR_CODE_SIZE][QR_CODE_SIZE] = {{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
                                           {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
                                           {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
                                           {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
                                           {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
                                           {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                                           {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                                           {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                           {1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0},
                                           {0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0},
                                           {1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0},
                                           {0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                                           {1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1},
                                           {0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1},
                                           {1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1},
                                           {0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                                           {1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                                           {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
                                           {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1},
                                           {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1},
                                           {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
                                           {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0},
                                           {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1},
                                           {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1},
                                           {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1}};

void paint_qrcode(unsigned color)
{
    for (int i = 0; i < QR_CODE_SIZE; i++)
        for (int j = 0; j < QR_CODE_SIZE; j++)
            if (QR_CODE[i][j])
                draw_square(QR_CODE_POS_X + j * QR_CODE_BLOCK_SIZE, QR_CODE_POS_Y + i * QR_CODE_BLOCK_SIZE, QR_CODE_BLOCK_SIZE, color);
}

void screen_erase_qrcode(void)
{
    paint_qrcode(BLACK_COLOR);
}

void screen_draw_qrcode(void)
{
    paint_qrcode(NORMAL_COLOR);
}