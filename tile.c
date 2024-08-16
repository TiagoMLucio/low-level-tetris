#include "tile.h"
#include <uspienv/util.h>
#include "graphics.h"

#include "custom_types.h"
#include <stdint.h>
#include <stdlib.h>

TileType TILE_TYPES[] = {
    // I block
    {{{{1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0}},
      {{1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0}}},
     CYAN_PALETTE,
     1,
     4},
    // O block
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
    // T block
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
    // L block
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
    // J block
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
    // S block
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
    // Z block
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
    tile.x = 3; // Starting X position (middle of the frame)
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