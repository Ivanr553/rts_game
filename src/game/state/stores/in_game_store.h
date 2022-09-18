#pragma once

#include "../../../engine/engine.h"
#include "../../models/control_group/control_group.h"

void create_control_groups(void);
void initialize_in_game_store(void);
void select_units(Array *entity_id_array);
short handle_right_click_unit_interaction(void);