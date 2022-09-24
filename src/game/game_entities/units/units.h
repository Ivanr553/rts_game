#pragma once

#include "../../../engine/engine.h"

#include "../game_entities_internal.h"

/** Worker */
Game_Entity *create_worker(vec3 pos, GAME_ENTITY_TYPE unit_type);
void add_worker_animations(Entity *entity);
void update_worker(Entity *entity);

/** Resource */
Game_Entity *create_resource(RESOURCE type, vec3 pos);
