#pragma once

#include "../game_entities_internal.h"
#include "../../../engine/engine.h"

typedef struct building_data
{
    vec2 size;
    float unit_radius;
    char *file_path;
    int sprite_sheet_size[2];
    short can_produce;
    short can_rally;
    int crystal_cost;
    int build_time;
    int pop_increase;
} Building_Data;

Game_Entity *create_building(float *pos, BUILDING_TYPE building_type);

/** Building Data */
Building_Data get_building_data_by_type(BUILDING_TYPE type);

/** Rally Point */
void update_rally_point(Entity *rally_point);
void create_rally_point(Entity *entity_to_bind_to);