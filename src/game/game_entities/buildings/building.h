#pragma once

#include "../game_entities_internal.h"
#include "../../../engine/engine.h"

typedef struct building_data
{
    vec2 size;
    float unit_radius;
} Building_Data;

Game_Entity *create_building(float *pos, BUILDING_TYPE building_type);

/** Building Data */
Building_Data get_building_data_by_type(BUILDING_TYPE type);