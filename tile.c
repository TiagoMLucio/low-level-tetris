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
     CYAN_PALETTE},
    // O blockf
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
     YELLOW_PALETTE},
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
     PURPLE_PALETTE},
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
     ORANGE_PALETTE},
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
     GREEN_PALETTE},
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
     BLUE_PALETTE},
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
     RED_PALETTE}};

void draw_tile(Tile tile, unsigned color)
{
    TileType type = TILE_TYPES[tile.type];
    int(*rotation)[ROTATION_GRID_SIZE] = type.rotations[tile.rotation];

    for (int i = 0; i < ROTATION_GRID_SIZE; i++)
    {
        for (int j = 0; j < ROTATION_GRID_SIZE; j++)
        {
            if (rotation[i][j])
            {
                draw_grid_block(tile.x + j, tile.y + i, color);
            }
        }
    }
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

Tile create_tile(void)
{
    Tile current_tile;
    current_tile.type = rand() % 7; // Random tile among the three defined ones
    current_tile.rotation = rand() % ROTATION_GRID_SIZE;
    current_tile.x = 3; // Starting X position (middle of the frame)
    current_tile.y = 0; // Starting Y position (top of the frame)

    return current_tile;
}
