#pragma once

#include <linmath.h>

#include "../../generic/generic.h"

#define MAP_TILE_OFFSET_FOR_CAMERA 8
#define MAP_TILE_SIZE 1

typedef struct game_map
{
    int width;
    int height;
    int *tile_set;
    int tile_set_len;
    int x_len;
    int y_len;
    float start_x;
    float start_y;
} Game_Map;

void init_game_map(Game_Map *game_map, char *file_path);
void create_map_from_tiles(void);
void snap_to_map_grid(float *pos, float *size);
void generate_resources(Array *resource_positions);