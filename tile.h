#ifndef TILE_H
#define TILE_H

#include <uspienv.h>
#include "game_screen.h"
#include "queue.h"

#define ROTATIONS 4
#define ROTATION_GRID_SIZE 4
#define BACKGROUND_TILE_TICKNESS 4

typedef struct
{
    int rotations[ROTATIONS][ROTATION_GRID_SIZE][ROTATION_GRID_SIZE];
    unsigned color;
    unsigned show_height;
    unsigned show_width;
} TileType;

typedef struct
{
    int type;
    int rotation;
    int x, y;
} Tile;

typedef struct
{
    unsigned type;
    unsigned rotation;
    unsigned i;
    unsigned j;
} BackgroundTile;

extern TileType TILE_TYPES[];
extern BackgroundTile BACKGROUND_TILES[];
extern size_t BACKGROUND_TILES_SIZE;

// Function declarations
void draw_tile(Tile tile, unsigned color);
void put_tile(Tile tile);
void remove_tile(Tile tile);
int (*get_rotation(Tile tile))[ROTATION_GRID_SIZE];
int (*get_next_rotation(Tile tile))[ROTATION_GRID_SIZE];
Tile create_tile();
int get_show_height(Tile tile);
int get_show_width(Tile tile);
int get_tile_color(Tile tile);
void screen_update_next(Tile tile);
void draw_statistics_tile(TileType tile_type, unsigned i);
void draw_statistics_tiles(void);
void draw_background_tile(BackgroundTile tile);
void draw_background_tiles(void);

#endif // TILE_H
