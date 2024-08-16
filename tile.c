#include "tile.h"
#include <uspienv/util.h>
#include "graphics.h"

#include "custom_types.h"
#include <stdint.h>
#include <stdlib.h>

TileType TILE_TYPES[] = {
    // I block 0
    {{{{1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0}},
      {{1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 0, 1, 0},
       {0, 0, 1, 0},
       {0, 0, 1, 0},
       {0, 0, 1, 0}}},
     CYAN_PALETTE,
     1,
     4},
    // O block 1
    {{{{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}}},
     YELLOW_PALETTE,
     2,
     2},
    // T block 2
    {{{{0, 1, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 1, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}}},
     PURPLE_PALETTE,
     2,
     3},
    // L block 3
    {{{{0, 0, 1, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 1, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}}},
     ORANGE_PALETTE,
     2,
     3},
    // J block 4
    {{{{1, 0, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 1, 0},
       {0, 0, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {0, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0}}},
     GREEN_PALETTE,
     2,
     3},
    // S block 5
    {{{{0, 1, 1, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 1, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}}},
     BLUE_PALETTE,
     2,
     3},
    // Z block 6
    {{{{1, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}}},
     RED_PALETTE,
     2,
     3}};

BackgroundTile BACKGROUND_TILES[] = {
    // primeira linha
    {6, 1, -1, 0},
    {0, 1, -3, 1},
    {0, 1, -2, 2},
    {0, 1, -3, 3},
    {2, 0, 0, 4},
    {6, 0, 0, 6},
    {0, 1, -3, 7},
    {6, 1, -1, 9},
    {0, 1, -3, 10},
    {0, 0, 0, 12},
    {0, 1, -3, 15},
    {2, 0, -1, 17},
    {3, 1, -2, 20},
    {1, 0, -1, 22},
    {4, 0, -1, 24},
    {0, 1, -3, 26},
    {4, 3, -2, 28},
    {2, 3, -1, 30},

    // segunda linha
    {3, 1, 2, 0},
    {4, 1, 1, 1},
    {0, 1, 1, 9},
    {0, 0, 5, 0},
    {0, 0, 6, -2},
    {3, 0, 5, 2},
    {1, 0, 5, 5},
    {4, 0, 5, 7},
    {4, 2, 5, 8},

    // primeira coluna
    {3, 3, 7, -1},
    {0, 0, 8, -3},
    {0, 0, 9, -3},
    {0, 0, 10, -3},
    {0, 1, 11, -1},
    {1, 0, 15, -1},
    {0, 0, 17, -3},
    {0, 0, 18, -3},
    {3, 0, 19, -2},
    {3, 3, 21, -1},
    {0, 1, 24, -1},

    // ultima linha
    {5, 1, 26, 1},
    {2, 2, 26, 2},
    {0, 1, 27, 3},
    {3, 2, 26, 5},
    {0, 1, 27, 5},
    {6, 1, 26, 7},
    {0, 1, 27, 8},
    {6, 0, 26, 9},
    {3, 3, 26, 11},
    {0, 1, 26, 12},
    {4, 1, 26, 14},
    {0, 1, 27, 14},
    {3, 2, 26, 16},
    {0, 1, 27, 16},
    {0, 0, 26, 19},
    {1, 0, 27, 18},
    {4, 1, 27, 20},
    {0, 1, 27, 21},

    // ultima coluna
    {0, 1, 2, 30},
    {4, 0, 6, 31},
    {4, 3, 8, 30},
    {4, 2, 10, 27},
    {0, 0, 10, 23},
    {0, 0, 11, 31},
    {0, 0, 12, 31},
    {6, 1, 11, 29},
    {5, 0, 13, 29},
    {1, 0, 14, 31},
    {4, 1, 15, 29},
    {3, 3, 16, 30},
    {0, 1, 17, 29},
    {0, 0, 19, 31},
    {2, 3, 20, 30},

    // inferior direito
    {2, 1, 22, 23},
    {3, 2, 22, 28},
    {2, 3, 22, 26},
    {2, 2, 22, 24},
    {0, 0, 23, 30},
    {6, 1, 23, 28},
    {5, 0, 24, 29},
    {3, 0, 25, 29},
    {0, 0, 27, 29},
    {0, 1, 26, 27},
    {2, 3, 25, 26},
    {4, 2, 24, 24},
    {1, 0, 25, 24},
    {3, 1, 25, 23},
    {3, 3, 27, 25}};

size_t BACKGROUND_TILES_SIZE = sizeof(BACKGROUND_TILES) / sizeof(BackgroundTile);

void draw_tile(Tile tile, unsigned color)
{
    TileType type = TILE_TYPES[tile.type];
    int(*rotation)[ROTATION_GRID_SIZE] = type.rotations[tile.rotation];

    for (int i = 0; i < ROTATION_GRID_SIZE; i++)
        for (int j = 0; j < ROTATION_GRID_SIZE; j++)
            if (rotation[i][j])
                draw_grid_block(tile.y + i, tile.x + j, BLOCK_SIZE, color);
}

void put_tile(Tile tile)
{
    draw_tile(tile, TILE_TYPES[tile.type].color);
}

void remove_tile(Tile tile)
{
    draw_tile(tile, 0);
}

int (*get_rotation(Tile tile))[ROTATION_GRID_SIZE]
{
    return TILE_TYPES[tile.type].rotations[tile.rotation];
}

int (*get_next_rotation(Tile tile))[ROTATION_GRID_SIZE]
{
    return TILE_TYPES[tile.type].rotations[(tile.rotation + 1) % ROTATION_GRID_SIZE];
}

Tile create_tile()
{
    Tile tile;
    tile.type = rand() % 7; // Random tile among the three defined ones
    tile.rotation = 0;
    tile.x = 4; // Starting X position (middle of the frame)
    tile.y = 0; // Starting Y position (top of the frame)

    return tile;
}

int get_show_height(Tile tile)
{
    return TILE_TYPES[tile.type].show_height;
};

int get_show_width(Tile tile)
{
    return TILE_TYPES[tile.type].show_width;
};

int get_tile_color(Tile tile)
{
    return TILE_TYPES[tile.type].color;
}

void screen_update_next(Tile tile)
{
    screen_clear_next();
    unsigned height = get_show_height(tile);
    unsigned width = get_show_width(tile);
    int(*rotation)[ROTATION_GRID_SIZE] = get_rotation(tile);
    unsigned pos_y = NEXT_FRAME_I + 4;
    unsigned pos_x = NEXT_FRAME_J + 1 + 2;

    for (unsigned i = 0; i < height; i++)
        for (unsigned j = 0; j < width; j++)
            if (rotation[i][j])
                draw_block(GAME_POS_X + (pos_x + j) * BLOCK_SIZE - width * BLOCK_SIZE / 2, GAME_POS_Y + (pos_y + i) * BLOCK_SIZE - BLOCK_SIZE / 2, BLOCK_SIZE, get_tile_color(tile));
}

void draw_statistics_tile(TileType tile_type, unsigned i)
{
    unsigned height = tile_type.show_height;
    unsigned width = tile_type.show_width;
    int(*rotation)[ROTATION_GRID_SIZE] = tile_type.rotations[0];
    unsigned pos_y = STATISTICS_FRAME_I + 4 + i * 2;
    unsigned pos_x = STATISTICS_FRAME_J + 4;

    for (unsigned i = 0; i < height; i++)
        for (unsigned j = 0; j < width; j++)
            if (rotation[i][j])
                draw_block(GAME_POS_X + pos_x * BLOCK_SIZE - width * BLOCK_SIZE / 2 + j * LITTLE_BLOCK_SIZE, GAME_POS_Y + pos_y * BLOCK_SIZE - height / 2 - BLOCK_SIZE / 2 + i * LITTLE_BLOCK_SIZE, LITTLE_BLOCK_SIZE, tile_type.color);
}

void draw_statistics_tiles(void)
{
    for (size_t i = 0; i < 7; i++)
        draw_statistics_tile(TILE_TYPES[i], i);
}

void draw_background_tile(BackgroundTile tile)
{
    int pos_x = GAME_POS_X + tile.j * BLOCK_SIZE;
    int pos_y = GAME_POS_Y + tile.i * BLOCK_SIZE;
    TileType tile_type = TILE_TYPES[tile.type];
    int(*rotation)[ROTATION_GRID_SIZE] = tile_type.rotations[tile.rotation];

    for (int k = 0; k < ROTATION_GRID_SIZE; k++)
    {
        for (int l = 0; l < ROTATION_GRID_SIZE; l++)
        {
            if (tile.i + k >= 0 && tile.i + k < GAME_HEIGHT && tile.j + l >= 0 && tile.j + l < GAME_WIDTH && rotation[k][l])
            {
                if (k == 0 || rotation[k - 1][l] == 0)
                    draw_line(pos_x + l * BLOCK_SIZE, pos_y + k * BLOCK_SIZE, BLOCK_SIZE, CYAN_PALETTE, BACKGROUND_TILE_TICKNESS, FALSE);
                if (l == 0 || rotation[k][l - 1] == 0)
                    draw_line(pos_x + l * BLOCK_SIZE, pos_y + k * BLOCK_SIZE, BLOCK_SIZE, CYAN_PALETTE, BACKGROUND_TILE_TICKNESS, TRUE);
                if (k == 3 || rotation[k + 1][l] == 0)
                    draw_line(pos_x + l * BLOCK_SIZE, pos_y + k * BLOCK_SIZE + BLOCK_SIZE - BACKGROUND_TILE_TICKNESS, BLOCK_SIZE, BLACK_COLOR, BACKGROUND_TILE_TICKNESS, FALSE);
                if (l == 3 || rotation[k][l + 1] == 0)
                    draw_line(pos_x + l * BLOCK_SIZE + BLOCK_SIZE - BACKGROUND_TILE_TICKNESS, pos_y + k * BLOCK_SIZE, BLOCK_SIZE, BLACK_COLOR, BACKGROUND_TILE_TICKNESS, TRUE);
            }
        }
    }
}

void draw_background_tiles(void)
{
    for (int i = 0; i < BACKGROUND_TILES_SIZE; i++)
        draw_background_tile(BACKGROUND_TILES[i]);
}