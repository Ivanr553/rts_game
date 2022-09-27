#pragma once

#include "../../../engine/engine.h"

#include "../game_entities_internal.h"

typedef struct unit_data
{
    int crystal_cost;
    int build_time;
    int health;
    int armor;
    int energy;
    vec2 size;
    float unit_radius;
    WEAPON_TYPE weapon_1;
    WEAPON_TYPE weapon_2;
    char *file_path;
} Unit_Data;

/** Worker */
Game_Entity *create_worker(vec3 pos, GAME_ENTITY_TYPE unit_type);
void add_worker_animations(Entity *entity);
void update_worker(Entity *entity);

/** Resource */
Game_Entity *create_resource(RESOURCE type, vec3 pos);

/** Spike */
Game_Entity *create_unit(vec3 pos, GAME_ENTITY_TYPE unit_type);
