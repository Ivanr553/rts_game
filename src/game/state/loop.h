#pragma once

#include "../../engine/engine.h"

// void game_loop_handle_ui(void);
void game_loop_update_entities(void);
void game_loop(void);
void handle_entity_class_updates(Entity *entity);

Entity *handle_move_to(Entity *entity);
Entity *update_entity_pos(Entity *entity);
Entity *handle_collisions(Entity *entity);
void animate_entity(Entity *entity);
