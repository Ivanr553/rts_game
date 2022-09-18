#pragma once

#include "../../engine/engine.h"

#include "game_entities_internal.h"

Game_Entity *create_worker(vec3 pos, char *file_path);
void add_worker_animations(Entity *entity);
void update_worker(Entity *entity);
void handle_selection(Game_Entity *worker);