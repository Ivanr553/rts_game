#pragma once

#include "../../engine/engine.h"

void game_loop_update_entities(void);
void game_loop(void);
short handle_entity_class_updates(Entity *entity);

Entity *handle_move_to(Entity *entity);
Entity *update_entity_pos(Entity *entity);
void animate_entity(Entity *entity);
