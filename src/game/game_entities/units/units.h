#pragma once

#include "../../../engine/engine.h"

#include "../game_entities_internal.h"

/** Worker */
Game_Entity *create_worker(vec3 pos, UNIT_TYPE unit_type);
void add_worker_animations(Entity *entity);
void update_worker(Entity *entity);
void handle_selection(Game_Entity *worker);

/** Resource */
Game_Entity *create_resource(RESOURCE type, vec3 pos);
